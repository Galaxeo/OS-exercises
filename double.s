bits 16        ; Tell the assembler that this is 16-bit code
global start   ; Make our "start" label public for the linker

start:
  mov ah, 0x00 ; Set Video Mode to:
  mov al, 0x03 ; Text, 80x25 characters
  int 0x10

  begin: ; Beginning of program
    ;Enter a number:
    call print_newl
    mov si, enter
    mov cx, enter_len
    call print_string


    ;Read string into buffer
    call read_string
    push cx

    mov si, buffer

    call atoi ; Converts buffer string to integer stored in ax
    push ax ; save ax for now because ax is used to print strings
    mov si, doubled
    mov cx, doubled_len
    call print_string
    pop ax ; now we need to double the integer

  mov bx, 2
  mul bx

  ; Pop cx to loop and store the needed # of digits
  pop cx
  ; Using di to count # of digits we are storing
  xor di, di

  loop1: ; Getting remainder by dividing by 10 and pushing it into a stack of ax
    mov bx, 10
    div bx
    mov bx, ax ; store quotient in bx
    mov al, dl
    add al, 0x30
    push ax
    xor ax, ax
    xor dx, dx
    mov ax, bx
    add di, 1
    loop loop1

  mov cx, di ; move our di counter into cx

  loop2: ; Popping out the stack of ax to print out the digits in the correct order
    ; Making sure we don't print leading 0s
    cmp cx, di
    pop ax
    je check

    cont: ; Printing the chars
      mov ah, 0x0E

      ;If the char being printed isn't a digit, don't print
      cmp al, 48
      jl begin
      cmp al, 57
      jg begin

      int 0x10
    last:
      loop loop2
  jmp begin

done:
  jmp done

check:
  sub al, 0x30
  cmp al, 0
  je last
  add al, 0x30
  jmp cont

;print_string and read_string imported from name.s, look there for comments

print_string:
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
    int 0x16
    stosb

    mov ah, 0x0E
    int 0x10

    cmp al, `\r`
    jne .loop

  mov al, `\n`
  int 0x10

  sub di, buffer
  mov cx, di

  ret

atoi:
  xor ax, ax ; integer stored in ax
  xor dx, dx

  .loop:
    ;Modified lodsb to be able to use different register
    xor bx, bx
    mov bl, [si] ; obtain value
    inc si ; increment for next character
    cmp bl, `\r` ; check to see if done
    je ret1 ; if done then return
    
    cmp bl, 48 ; if ASCII is out of bounds return error (negative value)
    jl error

    cmp bl, 57 ; if ASCII is out of bounds return error (negative value)
    jg error

    sub bl, 48 ; convert value to ASCII
    mov dl, 10 ; prep value for multiplication
    mul dl ; move current digits 1 to left
    add ax, bx ; add current digit to ax
    jmp .loop
ret1:
  ret
error:
  mov ax, -1
  ret

enter:
  db `Enter a number: `
enter_len equ $ - enter

print_newl:
  mov ah, 0x0E
  mov al, `\r`
  int 0x10
  mov al, `\n`
  int 0x10
  ret

doubled:
  db `Doubled it: `      ; "message" is the address at the start of the
                            ; character buffer

doubled_len equ $ - doubled ; "message_len" is a constant representing the
                            ; length of the character buffer

buffer:
