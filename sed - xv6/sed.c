#include "user.h"
#define NULL (void *)0

/*
The goal is to write a function that ends up going through
the whole file, looking for parts of the file that match the
old string, then replacing each one with the new
*/

typedef struct {
    char *base;
    unsigned int size;
    unsigned int used;
} ByteBuffer;

char *sed(char *string, char *old[], char*new[], int used);
int readIntoBuffer(ByteBuffer* buf, int fd);
ByteBuffer allocBuffer(unsigned int size);
int memcmp(char *first, char *second, int size);
void reallocBuffer(ByteBuffer* buf, int newSize);

int main( int argc, char *argv[] )  {
    int fd = 0;
    int size = 128;
    char *first[size];
    char *second[size];
    if (argc == 0){ exit();}
    //if argc == 3, cat is used as input
    else if (argc == 3) {
        fd = 0; //no fd
        *first = argv[1]+1;
        *second = argv[2]+1;
    }
    //if argc == 4 | sed -Old -New input.txt
    else if (argc == 4) {
        fd = open(argv[3], O_RDWR);
        *first = argv[1]+1;
        *second = argv[2]+1;
    }
    ByteBuffer buf = allocBuffer(20);
    int res = readIntoBuffer(&buf, fd);
    //Reallocate more size if buffer is too small
    while(res>0){
        if(buf.size == buf.used){
            reallocBuffer(&buf, 2*buf.size);
        }
        res = readIntoBuffer(&buf, fd);
    }

    printf(1, sed(buf.base, first, second, buf.used));

    exit();
};

//Return 1 if the strings equal each other, 0 if not.
int memcmp(char* dummy, char* sec, int size_old){
    for (int counter = 0; counter < size_old; counter++) {
        if (*dummy != sec[counter]){
            return 0;
        }
        dummy++;
    }
    return 1;
}

char *sed(char *str, char *old[], char *new[], int used){
    int size_old = strlen(*old);
    int size_new = strlen(*new);
    int size_str = strlen(str);
    int i = 0; //counter 1
    int j = 0; //counter 2
    int num_occurences = 0; //number of times old string is replaced with new
    //Indexing won't work properly unless i have these two variables to hold the old and new strings. not sure why
    char* sec = *old;
    char* sec2 = *new;

    for (char* cursor = str; cursor < str+used; ++cursor){
        if (*cursor == sec[0]){
            char*dummy = cursor;
            if (memcmp(dummy, sec, size_old) == 1){
                num_occurences+=1;
            }
        }
    }
    //allocating for result string
    char *res = malloc(2*(size_str + num_occurences*(size_new) - num_occurences*(size_old)));
    
    //go through and replace string whenever detected
    for (char* cursor = str; cursor < str+used; ++cursor) {
        if (*cursor == sec[0]){
            char* dummy = cursor;
            if (memcmp(dummy, sec, size_old) == 1){
                for (int count = 0; count < size_new; count++){
                    res[i+j] = sec2[count];
                    j++;
                }
                cursor += size_old-1;
            }
            else {
                res[i+j] = *cursor;
                i++;
            }
        } else {
            res[i+j] = *cursor;
            i++;
        }
    }

    return res;
}

//allocating ByteBuffer struct
ByteBuffer allocBuffer(unsigned int size) {
    ByteBuffer buf;
    buf.base = malloc(size);
    buf.size = size;
    buf.used = 0;
    return buf;
}
// read file into buffer
int readIntoBuffer(ByteBuffer* buf, int fd){
    int result = read(fd, buf->base + buf->used, buf->size - buf->used);
    if(result >= 0){
        buf -> used += result;
    }
    return result;
}

void reallocBuffer(ByteBuffer* buf, int newSize){
    buf->base = realloc(buf->base, newSize);
    buf->size = newSize;
}
