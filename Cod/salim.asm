    .486                                    ; create 32 bit code
    .model flat, stdcall                    ; 32 bit memory model
    option casemap :none                    ; case sensitive
 
    include \masm32\include\windows.inc     ; always first
    include \masm32\macros\macros.asm       ; MASM support macros
    include \masm32\include\masm32.inc
    include \masm32\include\gdi32.inc
    include \masm32\include\user32.inc
    include \masm32\include\kernel32.inc
    
    includelib \masm32\lib\masm32.lib
    includelib \masm32\lib\gdi32.lib
    includelib \masm32\lib\user32.lib
    includelib \masm32\lib\kernel32.lib
    
.data?

.data



texto  db 0,0     ; variável usada para printar na tela


.code                       ; Tell MASM where the code starts
start: 

    randomizando:
        call random

        cmp dl, 6
        jg menorQue7

        jmp randomizando

    menorQue7:
        add edx, 48
        print edx
        ret


  
    random proc

        mov  ax, dx
        xor  dx, dx
        mov  cx, 10    
        div  cx       ; here dx contains the remainder of the division - from 0 to 9
        add  dl, '0'  ; to ascii from '0' to '9'

    random endp


    # fazer vetor com números já usados 

end start



