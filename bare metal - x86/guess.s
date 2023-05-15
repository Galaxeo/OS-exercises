bits 16        ; Tell the assembler that this is 16-bit code
global start   ; Make our "start" label public for the linker

start:
  mov ah, 0x00 ; Set Video Mode to:
  mov al, 0x03 ; Text, 80x25 characters
  int 0x10
               ; Your code can start here
  .loop:
    call generate

  .game:
    call print_newl
    ; "Guess a number: "
    mov si, guess
    mov cx, guess_len
    call print_string

    call read_num

    call check
    jnz .wr
    jz .cr

  .cr:
    call print_newl
    mov si, correct
    mov cx, correct_len
    call print_string
    jmp .loop ; start new game if correct

  .wr:
    call print_newl
    mov si, wrong
    mov cx, wrong_len
    call print_string
    jmp .game ; try again

check:
  cmp bh, al ; Compare input to digit from generate func
  ret
  
generate: ; Probing CMOS clock
  xor ax, ax
  xor bx, bx
  ; Read the value of the CMOS register into al
  out 0x70, al
  in al, 0x71

  ; We only care about last digit
  mov bh, 0x0A
  div bh
  mov bh, al ; Have to move digit to bh because al is used later for input
  ret

print_string: ; Look at hello.s for comments
  mov ah, 0x0E
  .loop:
    lodsb
    int 0x10
    loop .loop
  ret

read_num: ; changed from read_string to read only one number, look at name.s for read_string comments
  mov di, buffer
  xor ah, ah
  int 0x16
  stosb

  mov ah, 0x0E
  int 0x10

  sub al, 48 ; sub 48 to get int val from ascii

  ret

print_newl:
  mov ah, 0x0E
  mov al, `\r`
  int 0x10
  mov al, `\n`
  int 0x10
  ret

guess:
  db `Guess a number: `
guess_len equ $ - guess

wrong:
  db `Wrong`
wrong_len equ $ - wrong

correct:
  db `Correct`
correct_len equ $ - correct

here:
  db `here`
here_len equ $ - here

buffer:
