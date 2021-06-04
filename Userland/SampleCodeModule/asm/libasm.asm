GLOBAL read
GLOBAL write
GLOBAL scClear
GLOBAL DateTime
GLOBAL getMem
GLOBAL getRegs
GLOBAL changeScreen
GLOBAL opCodeExc

; read - lee de un fd determinado
; IN- RDI - FD
;    RSI - Buffer
;    RDX - size
read:
    push rbp
    mov rbp,rsp;

    push rdi
    push rsi
    push rdx
    push rcx

    mov rcx ,rdx ;len en rdx
    mov rdx, rsi  ;buff en rdx
    mov rsi,rdi   ; fd en rsi
    mov rdi,0    ; id int
    int 80h

    pop rcx
    pop rdx
    pop rsi
    pop rdi
    

    leave
    ret


; write - escribe en un fd determinado
;IN- RDI - FD
;    RSI - Buffer
;    RDX - size
write:
    push rbp
    mov rbp,rsp;

    push rdi
    push rsi
    push rdx
    push rcx

    mov rcx ,rdx ;len en rdx
    mov rdx, rsi  ;buff en rdx
    mov rsi,rdi   ; fd en rsi
    mov rdi,1     ; id int
    int 80h

    pop rcx
    pop rdx
    pop rsi
    pop rdi

    leave
    ret

;scClear - Limpia la pantalla
scClear:
    push rbp
    mov rbp,rsp

    push rdi
    push rsi

    mov rdi, 2
    int 80h
    
    pop rsi
    pop rdi
    leave
    ret

;DateTime - obtiene info del tiempo
;IN - RDI selector de data
DateTime:

    push rbp
    mov rbp,rsp

    push rdi
    push rsi

    mov rsi,rdi
    mov rdi, 5
    int 80h

    pop rsi
    pop rdi

    leave
    ret

;getMem - recibe el valor de memoria en un vecctor
;IN- RDI - buffer
;    RSI - address
;    RDX - bytes
getMem:
    push rbp
    mov rbp,rsp;

    push rdi
    push rsi
    push rdx
    push rcx

    mov rcx ,rdx ;bytes en rdx
    mov rdx, rsi  ;address en rdx
    mov rsi, rdi   ; buff en rsi
    mov rdi, 3     ; id int
    int 80h

    pop rcx
    pop rdx
    pop rsi
    pop rdi

    leave
    ret
    
;getRegs - abtiene el valor de los 15 registros
;IN - RDI buffer
getRegs:

    push rbp
    mov rbp,rsp

    push rdi
    push rsi

    mov rsi,rdi
    mov rdi, 4
    int 80h

    pop rsi
    pop rdi
    
    leave
    ret

changeScreen:
    push rbp
    mov rbp,rsp

    push rdi
    push rsi

    mov rsi,rdi
    mov rdi, 6
    int 80h

    pop rsi
    pop rdi

    leave
    ret

opCodeExc:
    UD2
    ret