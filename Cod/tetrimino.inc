desenharTetrimino proc hWin:DWORD, hDC:DWORD
    LOCAL hOld:DWORD
    LOCAL memDC :DWORD

    invoke CreateCompatibleDC,hDC
    mov memDC, eax

    invoke SelectObject,memDC,hBmp
    mov hOld, eax 

    xor eax, eax 
    xor ebx, ebx
    mov al, bloco.posicao
    push eax
    call getPixel
    mov bx, ax ; ebx coluna e eax linha
    shr eax, 16

    invoke TransparentBlt, hDC, ebx, eax, 224, 32, memDC,0,0,224,32,0
    invoke SelectObject,hDC,hOld
    invoke DeleteDC,memDC
    ret
desenharTetrimino endp

ThreadProcDescer PROC USES ecx Param:DWORD
    invoke WaitForSingleObject,hEventStart, 500
        .IF eax == WAIT_TIMEOUT
            invoke PostMessage,hWnd,WM_DESCER,NULL,NULL
            jmp ThreadProcDescer
        .ENDIF

        jmp ThreadProcDescer
    ret
ThreadProcDescer ENDP

desenharBloco proc hWin:DWORD, hDC:DWORD, cor:BYTE, posicao:BYTE

    LOCAL hOld:DWORD
    LOCAL memDC :DWORD

    invoke CreateCompatibleDC,hDC

    mov memDC, eax
    invoke SelectObject,memDC,hBmp  ; selecionei o novo bitmap
    mov hOld, eax

    xor eax, eax 
    xor ebx, ebx
    mov al, bloco.posicao
    push eax
    call getPixel
    mov bx, ax ; ebx coluna e eax linha
    shr eax, 16

    mov ecx, eax
    mov edx, ebx

    xor eax, eax     ; limpamos eax
    xor ebx, ebx     ; limpar ebx
    mov ebx, 32 
    mov al, cor     ; atribuímos o valor do tipo ao registrador de 8 bits, o al
    push edx
    mul ebx          ; multiplicamos o valor armazenado em al por 32
    pop edx
    mov ebx, eax     ; salvamos o valor de al em tipo, que será usado para o ponto de início do corte no eixo X

    INVOKE  TransparentBlt,hDC,edx,ecx,32,32,memDC,ebx,0,32,32,CREF_TRANSPARENT    ; cortamos a imagem  

    invoke SelectObject,hDC,hOld
    invoke DeleteDC,memDC

    return 0

desenharBloco endp

;ax -> coluna
;resto -> linha
getPixel proc pos:DWORD
    xor edx, edx
    mov eax, pos
    mov ecx, 10
    div ecx
    shl eax, 5
    shl edx, 5
    add eax, 11
    add edx, 11
    shl eax, 16
    mov ax, dx
    ret
getPixel endp