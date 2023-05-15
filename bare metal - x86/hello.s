bits 16        ; Tell the assembler that this is 16-bit code
global start   ; Make our "start" label public for the linker

start:
  mov ah, 0x00 ; Set Video Mode to:
  mov al, 0x03 ; Text, 80x25 characters
  int 0x10
  mov si, message
  mov cx, message_len
  call print_string
  call done
               ; Your code can start here

print_string:
  mov ah, 0x0E ; Teletype output
  .loop:
    lodsb ; load byte at address si into al, the char for teletype output, inc si
    int 0x10 ; Interrupt call
    loop .loop
  ret

done:
  jmp done     ; When we're done, loop indefinitely

message:
  db `Hello World\r\n`      ; "message" is the address at the start of the
                            ; character buffer

message_len equ $ - message ; "message_len" is a constant representing the
                            ; length of the character buffer
