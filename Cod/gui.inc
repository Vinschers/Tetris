desenharTela proc hdc:DWORD
	mov rect.top, 10
	mov rect.bottom, 651
	mov rect.left, 10
	mov rect.right, 332

	invoke  Rectangle, hdc, rect.left, rect.top, rect.right, rect.bottom  

	invoke  lstrlen, ADDR pecaSeguradaTxt
	invoke  TextOut, hdc, 400, 20, ADDR pecaSeguradaTxt, eax

	mov rect.top, 50
	mov rect.bottom, 200
	mov rect.left, 390
	mov rect.right, 580 
	invoke  Rectangle, hdc, rect.left, rect.top, rect.right, rect.bottom  


	invoke  lstrlen, ADDR proxPecaTxt
	invoke  TextOut, hdc, 400, 250, ADDR proxPecaTxt, eax

	mov rect.top, 280
	mov rect.bottom, 450
	mov rect.left, 390
	mov rect.right, 580
	invoke  Rectangle, hdc, rect.left, rect.top, rect.right, rect.bottom

	invoke  lstrlen, ADDR pontuacaoTxt
	invoke  TextOut, hdc, 400, 480, ADDR pontuacaoTxt, eax

	ret
desenharTela endp

;P + Tcol + Tlin*16
desenharTetrimino proc hWin:DWORD, hDC:DWORD, pTetrimino:DWORD, cor:BYTE
    mov edx, pTetrimino
    mov edx, (TETRIMINO ptr[edx]).mat

    xor eax, eax
    xor ecx, ecx
    mov al, (MATRIZ ptr[edx]).largura
    mov cl, (MATRIZ ptr[edx]).altura
    mul ecx

    mov edx, pTetrimino
    mov edx, (TETRIMINO ptr[edx]).mat
    mov edx, (MATRIZ ptr[edx]).ponteiro

    xor ebx, ebx
    xor ecx, ecx

    forT:
    cmp ebx, eax
    je fim

    mov cl, byte ptr[edx]
    cmp cl, 0
    je cont

    push eax
    push ebx
    push ecx
    push edx

    xor eax, eax
    mov al, bl ;al tem o contador
    xor ebx, ebx

    mov edx, pTetrimino
    mov edx, (TETRIMINO ptr[edx]).mat
    mov bl, (MATRIZ ptr[edx]).largura  ;bl tem a largura

    xor edx, edx

    div ebx   ;al tem a linha

    mov esi, edx

    mov ebx, 10
    mul ebx

    mov edx, esi

    add al, dl

    xor ecx,ecx
    mov edx, pTetrimino
    mov ecx, (TETRIMINO ptr[edx]).posicao
    add eax, ecx

    mov cl, cor
    
    invoke desenharBloco, hWin, hDC, cl, al

    pop edx
    pop ecx
    pop ebx
    pop eax

    cont:
    inc ebx
    inc edx
    jmp forT

    fim:
    ret
desenharTetrimino endp

ThreadProcDescer PROC USES ecx Param:DWORD
    invoke WaitForSingleObject,hEventStart, 3000
        .IF eax == WAIT_TIMEOUT
            pintar PP_DESCER
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

    xor ebx, ebx
    invoke getPixel, posicao
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

apagarTetrimino proc hWin:DWORD, hdc:DWORD, pTetrimino:DWORD
	invoke desenharTetrimino, hWin, hdc, pTetrimino, NADA
    ret
apagarTetrimino endp


tetriminoPaint proc hWin:DWORD, hdc:DWORD, pTetrimino:DWORD	
    invoke apagarTetrimino, hWin, hdc, pTetrimino
    ;invoke desenharTetrimino, hWin, hdc, pTetrimino, NADA
	mov edx, pTetrimino
	add (TETRIMINO ptr[edx]).posicao, 10
	
	mov cl, (TETRIMINO ptr[edx]).tipo
	invoke desenharTetrimino, hWin, hdc, pTetrimino, cl

	fim:
	ret
tetriminoPaint endp

;ax -> coluna
;resto -> linha
getPixel proc pos:BYTE
    xor edx, edx
    xor eax, eax
    mov al, pos
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