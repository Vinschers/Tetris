    .486                                    ; create 32 bit code
    .model flat, stdcall                    ; 32 bit memory model
    option casemap :none                    ; case sensitive
 
     include \masm32\include\windows.inc
      include \masm32\macros\macros.asm

      include \masm32\include\masm32.inc
      include \masm32\include\user32.inc
      include \masm32\include\kernel32.inc
      include \masm32\include\gdi32.inc
      include \masm32\include\msimg32.inc

      includelib \masm32\lib\masm32.lib
      includelib \masm32\lib\user32.lib
      includelib \masm32\lib\kernel32.lib
      includelib \masm32\lib\gdi32.lib
      includelib \masm32\lib\msimg32.lib
    
.data?

.data



texto  db 0,0     ; variável usada para printar na tela


.code
start: 

    


    randomize:
        invoke  GetTickCount
        invoke  nseed, eax
        invoke  nrandom, 6  ; gera um número aleatório entre 0 e 6



end start



