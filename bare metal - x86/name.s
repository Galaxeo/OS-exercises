bits 16        ; Tell the assembler that this is 16-bit code
global start   ; Make our "start" label public for the linker

start:
  mov ah, 0x00 ; Set Video Mode to:
  mov al, 0x03 ; Text, 80x25 characters
  int 0x10

  .loop:
    ; "Enter your name: "
    mov si, enter
    mov cx, enter_len
    call print_string
    
    call read_string ; Read into buffer
    push cx ; Remember # of bytes we have to read

    ; "Hello, "
    mov si, hello
    mov cx, hello_len
    call print_string

    ; Print name from buffer
    mov si, buffer
    pop cx
    call print_string
    call print_newl

    jmp .loop

print_string: ; look at hello.s for comments here
  mov ah, 0x0E
  .loop:
    lodsb
    int 0x10
    loop .loop
  ret

read_string:
  mov di, buffer
  .loop:
    xor ah, ah
    int 0x16 ; Interrupt call (ah = 0 is key press function code)
    stosb ; Store the byte from interrupt call into address di, increment di

    mov ah, 0x0E ; print the char out so people can see what they are typing
    int 0x10

    cmp al, `\r` ; Read until enter
    jne .loop

  mov al, `\n`
  int 0x10

  sub di, buffer+1 ; remember # of bytes that we have to print from buffer later
  mov cx, di

  ret

enter:
  db `Enter your name: `
enter_len equ $ - enter

print_newl:
  mov ah, 0x0E
  mov al, `\r`
  int 0x10
  mov al, `\n`
  int 0x10
  ret

hello:
  db `Hello, `      ; "message" is the address at the start of the
                            ; character buffer

hello_len equ $ - hello ; "message_len" is a constant representing the
                            ; length of the character buffer
buffer:
