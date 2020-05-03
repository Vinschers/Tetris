desenharTela proc hdc:DWORD
	mov rect.top, 10
	mov rect.bottom, 652
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

    mov ebx, 16
    mul ebx

    mov edx, esi

    add ax, dx

    xor ecx,ecx
    mov edx, pTetrimino
    mov ecx, (TETRIMINO ptr[edx]).posicao
    add eax, ecx

    mov cl, cor
    
    invoke desenharBloco, hWin, hDC, cl, ax

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
    invoke WaitForSingleObject,hEventStart, velocidade
        .IF eax == WAIT_TIMEOUT
            pintar PP_DESCER
            jmp ThreadProcDescer
        .ENDIF

        jmp ThreadProcDescer
    ret
ThreadProcDescer ENDP

desenharBloco proc hWin:DWORD, hDC:DWORD, cor:BYTE, posicao:WORD

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
getPixel proc pos:WORD
    xor edx, edx
    xor eax, eax
    mov ax, pos
    mov ecx, 16
    div ecx
    sub eax, 3
    sub edx, 3

    shl eax, 5
    shl edx, 5
    add eax, 11
    add edx, 11
    shl eax, 16
    mov ax, dx
    ret
getPixel endp

paint proc hWin:DWORD, hdc:DWORD, pTetromino:DWORD, pMapa:DWORD
	LOCAL copiaTetrimino:DWORD
	LOCAL copiaMatriz:DWORD
	LOCAL colidiu:BYTE

	invoke copiarTetrimino, pTetromino
    mov copiaTetrimino, eax
    
    invoke colocarMatrizLogica, pTetromino, pMapa, 0

    mov al, paintParam
    .if al == PP_DESENHAR
        invoke desenharTela, hdc

    .elseif al == PP_DESCER
        invoke desenharTetrimino, hWin, hdc, pTetromino, NADA
        mov esi, pTetromino
        add (TETRIMINO ptr[esi]).posicao, 16
        mov desenhandoTetrimino, 0

    .elseif al == PP_ROTACIONAR
        invoke desenharTetrimino, hWin, hdc, pTetromino, NADA
        mov esi, pTetromino
        invoke rotacionarMatriz, (TETRIMINO ptr[esi]).mat
        mov esi, pTetromino
        add (TETRIMINO ptr[esi]).rotacao, 1
        mov cl, (TETRIMINO ptr[esi]).rotacao
        .if cl == 4
        	mov esi, pTetromino
            mov (TETRIMINO ptr[esi]).rotacao, 0
        .endif

    .elseif al == PP_MOVER_DIREITA
        invoke desenharTetrimino, hWin, hdc, pTetromino, NADA
        mov esi, pTetromino
        inc (TETRIMINO ptr[esi]).posicao

    .elseif al == PP_MOVER_ESQUERDA
        invoke desenharTetrimino, hWin, hdc, pTetromino, NADA
        mov esi, pTetromino
        dec (TETRIMINO ptr[esi]).posicao

    .elseif al == PP_REFAZER
        invoke refazerTetrimino, pTetromino, CIANO

    .endif

    mov esi, pTetromino
    mov edi, (TETRIMINO ptr[esi]).mat
    invoke copiarMatriz, edi
    mov copiaMatriz, eax

    invoke adicionarMatrizLogica, pTetromino, pMapa
    mov esi, pTetromino
    mov edi, (TETRIMINO ptr[esi]).mat
    invoke verificarColisao, edi

    mov colidiu, al

    mov esi, pTetromino
    mov edi, (TETRIMINO ptr[esi]).mat

    invoke atribuirMatriz, edi, copiaMatriz
    invoke destruirMatriz, copiaMatriz, 1

    mov al, paintParam
    .if colidiu == 1
        .if al == PP_ROTACIONAR
            invoke testesRotacao, pTetromino, pMapa
            .if eax == 0
                invoke atribuirTetrimino, pTetromino, copiaTetrimino
            .endif
        .else
            invoke atribuirTetrimino, pTetromino, copiaTetrimino
        .endif
    .endif

    invoke destruirTetrimino, copiaTetrimino
    invoke colocarMatrizLogica, pTetromino, pMapa, 1
    mov esi, pTetromino
    mov cl, (TETRIMINO ptr[esi]).tipo
    invoke desenharTetrimino, hWin, hdc, pTetromino, cl

    mov al, paintParam
    .if colidiu == 1
        .if al == PP_DESCER
        	mov esi, pTetromino
        	mov ecx, (TETRIMINO ptr[esi]).posicao
            .if ecx < 10
                call perder
            .else
                mov velocidade, 500
                invoke refazerTetrimino, pTetromino, AZUL
                mov esi, pTetromino
			    mov cl, (TETRIMINO ptr[esi]).tipo
			    invoke desenharTetrimino, hWin, hdc, pTetromino, cl
            .endif
        .endif
    .endif

	ret
paint endp

esperarDesenho proc
    comparar:
    cmp desenhandoTetrimino, 1
    je comparar
    ret
esperarDesenho endp

perder proc
    ;string str, "Perdeu!\nDeseja jogar novamente?"
    ret
perder endp