// init: The initial user-level program
#include "bcrypt.h"
#include "user.h"


// The maximum number characters allowed for passwords
// not including the NULL terminator.
#define MAX_PASSLEN 72

char* argv[] = {"sh", 0};


// Implementation of itoa in order to turn random ints into strings
char* itoa(int num, char* str)
{
    int i = 0;
    int isNegative = 0;
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
    if (num < 0) {
        isNegative = 1;
        num = -num;
    }
    while (num != 0) {
        int rem = num % 10;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / 10;
    }

    if (isNegative)
        str[i++] = '-';
    str[i] = '\0';

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }
 
    return str;
}

// See if one and two are the same string
int check(char* one, char* two){
  for (int i = 0; i < 72; ++i){
    if (one[i] != two[i]){
      return 0;
    }
    else if ((one[i] == 0) & (two[i] == 0)){
      return 1;
    }
  }
  return 1;
}

// Enter password & confirm in order to set password
int setPassword(char pass[72]){
  char one[72];
  char two[72];
  printf(1, "Enter password: ");

  gets(one, 72);
  
  printf(1, "Re-enter to confirm: ");
  gets(two, 72);

  if (check(one, two)){
    printf(1, "Password succesfully set. You may now use it to log in.\n");
    for (int i = 0; i < 72; i ++){
      pass[i] = one[i];
    }
    return 1;
  } else {
    printf(1, "Passwords do not match. Try again.\n");
    return 0;
  }
}

int login(const unsigned char* salt, const unsigned char* hash){
  char pass[72];
  printf(1, "Enter password: ");
  gets(pass, 72);
  return bcrypt_checkpass(pass, salt, hash);
}

int main(void) {
  int pid, wpid;
  printf(1, "init: HERE\n");

  if(open("console", O_RDWR) < 0) {
    mknod("console", 1, 1);
    open("console", O_RDWR);
  }
  dup(0); // stdout
  dup(0); // stderr

  char temp[32];
  char pass[72];
  unsigned char saltstr[128];

  // Generate the random integers to fill out the salt value
  for (int i = 0; i < 4; i++){
    char* one = itoa(random(), temp);
    for (int j = 0; j < 32; j++){
      saltstr[i*32+j] = one[j];
    }
  }

  for(;;) {
    // Check to see if we have a password
    int hsh = open("hash.txt", O_RDWR);
    int slt = open("salt.txt", O_RDWR);

    // If file descriptors < 0, we have no password so create one
    if (hsh < 0){
      printf(1, "No password set. Please choose one. \n");
      while (setPassword(pass) == 0){
        continue;
      }
      unsigned char* hash = bcrypt(pass, saltstr);
      // Write the hash and the salt to files
      hsh = open("hash.txt", O_CREATE | O_RDWR);
      write(hsh, hash, 72);
      slt = open("salt.txt", O_CREATE | O_RDWR);
      write(slt, saltstr, 128);
    }

    // Login, first read the hash and salt into local variables
    unsigned char* comphash;
    unsigned char* compsalt;
    hsh = open("hash.txt", O_RDONLY);
    slt = open("salt.txt", O_RDONLY);
    comphash = malloc(72);
    compsalt = malloc(128);
    read(hsh, comphash, 72);
    read(slt, compsalt, 128);

    // Login process
    while (1){
      if (login(compsalt, comphash) == 0){
        printf(1, "Password correct, logging you in.");
        printf(1, "\n");
        break;
      } else{
        printf(1, "Password incorrect. Try again.");
        printf(1, "\n");
      }
    }

    printf(1, "init: starting sh\n");
    pid = fork();
    if(pid < 0) {
      printf(1, "init: fork failed\n");
      exit();
    }
    if(pid == 0) {
      exec("sh", argv);
      printf(1, "init: exec sh failed\n");
      exit();
    }
    while((wpid = wait()) >= 0 && wpid != pid)
      printf(1, "zombie!\n");
  }
}
