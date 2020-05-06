desenharTela proc hdc:DWORD
	mov rect.top, 10
	mov rect.bottom, 652
	mov rect.left, 10
	mov rect.right, 332

	invoke  Rectangle, hdc, rect.left, rect.top, rect.right, rect.bottom  

	invoke  lstrlen, ADDR pecaSeguradaTxt
	invoke  TextOut, hdc, 400, 20, ADDR pecaSeguradaTxt, eax

	mov rect.top, 50
	mov rect.bottom, 210
	mov rect.left, 390
	mov rect.right, 582
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
desenharTetromino proc hWin:DWORD, hDC:DWORD, pTetromino:DWORD, cor:BYTE
    mov edx, pTetromino
    mov edx, (TETROMINO ptr[edx]).mat

    xor eax, eax
    xor ecx, ecx
    mov al, (MATRIZ ptr[edx]).largura
    mov cl, (MATRIZ ptr[edx]).altura
    mul ecx

    mov edx, pTetromino
    mov edx, (TETROMINO ptr[edx]).mat
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

    mov edx, pTetromino
    mov edx, (TETROMINO ptr[edx]).mat
    mov bl, (MATRIZ ptr[edx]).largura  ;bl tem a largura

    xor edx, edx

    div ebx   ;al tem a linha

    mov esi, edx

    mov ebx, 16
    mul ebx

    mov edx, esi

    add ax, dx

    xor ecx,ecx
    mov edx, pTetromino
    mov ecx, (TETROMINO ptr[edx]).posicao
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
desenharTetromino endp

ThreadProcDescer PROC USES ecx Param:DWORD
    invoke WaitForSingleObject,hEventStart, velocidade
        .IF eax == WAIT_TIMEOUT
            pintar PP_DESCER
            jmp ThreadProcDescer
        .ENDIF
        .if perdeu != 1
            jmp ThreadProcDescer
        .endif
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

apagarTetromino proc hWin:DWORD, hdc:DWORD, pTetromino:DWORD
	invoke desenharTetromino, hWin, hdc, pTetromino, NADA
    ret
apagarTetromino endp

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

paint proc hWin:DWORD, hdc:DWORD, pTetromino:DWORD, pMapa:DWORD, pFantasma:DWORD
	LOCAL copiaTetromino:DWORD
	LOCAL copiaMatriz:DWORD
	LOCAL colidiu:BYTE

	invoke copiarTetromino, pTetromino
    mov copiaTetromino, eax
    
    invoke colocarMatrizLogica, pTetromino, pMapa, 0

    mov al, paintParam
    mov dl, pecaGuardada
    mov cl, jaGuardou

    .if al == PP_DESENHAR
        invoke desenharTela, hdc

    .elseif al == PP_DESCER
        invoke desenharTetromino, hWin, hdc, pTetromino, NADA
        mov esi, pTetromino
        add (TETROMINO ptr[esi]).posicao, 16

    .elseif al == PP_ROTACIONAR
        invoke desenharTetromino, hWin, hdc, pTetromino, NADA
        mov esi, pTetromino
        invoke rotacionarMatriz, (TETROMINO ptr[esi]).mat
        mov esi, pTetromino
        add (TETROMINO ptr[esi]).rotacao, 1
        mov cl, (TETROMINO ptr[esi]).rotacao
        .if cl == 4
        	mov esi, pTetromino
            mov (TETROMINO ptr[esi]).rotacao, 0
        .endif

    .elseif al == PP_MOVER_DIREITA
        invoke desenharTetromino, hWin, hdc, pTetromino, NADA
        mov esi, pTetromino
        inc (TETROMINO ptr[esi]).posicao

    .elseif al == PP_MOVER_ESQUERDA
        invoke desenharTetromino, hWin, hdc, pTetromino, NADA
        mov esi, pTetromino
        dec (TETROMINO ptr[esi]).posicao

    .elseif al == PP_REFAZER
        cmp cl, 0
        jne fimRefazer 
        
        push ecx
        push edx
        mov rect.top, 50
        mov rect.bottom, 200
        mov rect.left, 390
        mov rect.right, 580 
        invoke  Rectangle, hdc, rect.left, rect.top, rect.right, rect.bottom  
        pop edx
        pop ecx

        mov jaGuardou, 1 ; há peça guardada 
        push edx
        invoke desenharTetromino, hWin, hdc, pTetromino, NADA
        mov esi, pTetromino
        mov bl, (TETROMINO ptr[esi]).tipo ; bl recebeu o tipo da peça que será guardada 
        mov pecaGuardada, bl
        invoke desenharTetrominoGuardado, hWin, hdc, pTetromino, bl
        mov esi, pTetromino
        pop edx
        cmp dl, 7
        jne jaTemGuardada
        invoke refazerTetromino, pTetromino, CIANO
        jmp fimRefazer

        jaTemGuardada:
            invoke refazerTetromino, pTetromino, dl
        fimRefazer:
    .elseif al == PP_CAIR_TUDO
        invoke desenharTetromino, hWin, hdc, pTetromino, NADA
        invoke descerTudo, pTetromino, pMapa
        mov colidiu, 1
        jmp atualizarVariaveis

    .endif

    mov esi, pTetromino
    mov edi, (TETROMINO ptr[esi]).mat
    invoke copiarMatriz, edi
    mov copiaMatriz, eax

    invoke adicionarMatrizLogica, pTetromino, pMapa
    mov esi, pTetromino
    mov edi, (TETROMINO ptr[esi]).mat
    invoke verificarColisao, edi

    mov colidiu, al

    mov esi, pTetromino
    mov edi, (TETROMINO ptr[esi]).mat

    invoke atribuirMatriz, edi, copiaMatriz
    invoke destruirMatriz, copiaMatriz, 1

    mov al, paintParam
    .if colidiu == 1
        .if al == PP_ROTACIONAR
            invoke testesRotacao, pTetromino, pMapa
            .if eax == 0
                invoke atribuirTetromino, pTetromino, copiaTetromino
            .endif
        .else
            invoke atribuirTetromino, pTetromino, copiaTetromino
        .endif
    .endif

    atualizarVariaveis:

    invoke destruirTetromino, copiaTetromino

    invoke desenharTetromino, hWin, hdc, pFantasma, NADA
    invoke atualizarFantasma, pFantasma, pTetromino, pMapa
    mov esi, pFantasma
    mov cl, (TETROMINO ptr[esi]).tipo
    invoke desenharTetromino, hWin, hdc, pFantasma, cl

    invoke colocarMatrizLogica, pTetromino, pMapa, 1
    mov esi, pTetromino
    mov cl, (TETROMINO ptr[esi]).tipo
    invoke desenharTetromino, hWin, hdc, pTetromino, cl

    mov al, paintParam
    .if colidiu == 1
        .if al == PP_DESCER || al == PP_CAIR_TUDO
            mov jaGuardou, 0
        	mov esi, pTetromino
        	mov ecx, (TETROMINO ptr[esi]).posicao
            .if ecx < 67
                mov perdeu, 1
                invoke perder, hWin, hdc
                invoke desenharTela, hdc
                mov edx, pTetromino
                mov cl, (TETROMINO ptr[edx]).tipo
                invoke desenharTetromino, hWin, hdc, pTetromino, cl
                invoke atualizarFantasma, pFantasma, pTetromino, pMapa
                mov esi, pFantasma
                mov cl, (TETROMINO ptr[esi]).tipo
                invoke desenharTetromino, hWin, hdc, pFantasma, cl
            .else
                invoke trabalharComLinhas, hWin, hdc, pMapa
                mov velocidade, 500
                invoke refazerTetromino, pTetromino, ROXO
                invoke atualizarFantasma, pFantasma, pTetromino, pMapa
                mov esi, pFantasma
                mov cl, (TETROMINO ptr[esi]).tipo
                invoke desenharTetromino, hWin, hdc, pFantasma, cl
                mov esi, pTetromino
			    mov cl, (TETROMINO ptr[esi]).tipo
			    invoke desenharTetromino, hWin, hdc, pTetromino, cl
            .endif
        .endif
    .endif
	ret
paint endp

esperarDesenho proc
    comparar:
    cmp desenhandoTetromino, 1
    je comparar
    ret
esperarDesenho endp

perder proc hWin:DWORD, hdc:DWORD
    invoke MessageBox,hWin, addr perdeuTxt, NULL, MB_YESNO

    .if eax == IDYES
        invoke comecarJogo, hWin
    .else
        invoke ExitProcess,0
    .endif
    ret
perder endp

trabalharComLinhas proc hWin:DWORD, hdc:DWORD, pMapa:DWORD
    invoke acharLinhas, hWin ,pMapa
    mov esi, pMapa
    mov esi, (MATRIZ ptr[esi]).ponteiro
    xor ecx, ecx
    forAchados:
        cmp ecx, 8
        je fimAchados
        cmp word ptr[eax], 0
        je fimAchados

        push esi
        push ecx
        push eax

        mov dx, word ptr[eax]
        invoke limparLinha, hWin, hdc, pMapa, dx

        pop eax
        push eax

        mov dx, word ptr[eax]
        invoke descerMatriz, hWin, hdc, pMapa, dx

        pop eax
        pop ecx
        pop esi

        add ecx, 2
        add eax, 2
        jmp forAchados

    fimAchados:
        ret
trabalharComLinhas endp

limparLinha proc hWin:DWORD, hdc:DWORD, pMapa:DWORD, indice:WORD 
    xor edx, edx
    add dx, indice

    mov ecx, edx
    add ecx, 10

    forLimpar:
        cmp edx, ecx
        je fimForLimpar

        push ecx
        push edx

        mov ecx, pMapa
        mov ecx, (MATRIZ ptr[ecx]).ponteiro
        mov byte ptr[edx + ecx], 0

        invoke desenharBloco, hWin, hdc, NADA, dx
        invoke Sleep, 75

        pop edx
        pop ecx

        inc edx
        jmp forLimpar
    fimForLimpar:
        ret
limparLinha endp

descerMatriz proc hWin:DWORD, hdc:DWORD, pMapa:DWORD, indice:WORD
   
    mov edx, pMapa
    mov esi, (MATRIZ ptr[edx]).ponteiro
    xor ecx, ecx
    mov cx, indice
    sub ecx, 16
    xor ebx,ebx
    xor edx,edx
    forLinhas:
        cmp ecx, 51
        jle fimForLinhas
        mov ebx, ecx
        add ebx, 10
        forPercorreLinha:
            ;invoke strMatriz, hWin, pMapa 
            cmp ecx, ebx
            je fimForPercorreLinha
            ;sub ecx, 16
            mov dl, byte ptr[esi + ecx]
            add ecx, 16
            mov byte ptr[esi + ecx], dl
            sub ecx, 16
            inc ecx
            jmp forPercorreLinha
        fimForPercorreLinha:  
            sub ecx, 26
            jmp forLinhas

    fimForLinhas:
        mov ecx, 51
        mov ebx, 61
        forParaLimparPrimeira:
            cmp ecx, ebx
            je fimForParaLimparPrimeira
            mov byte ptr[esi + ecx], 0
            inc ecx
            jmp forParaLimparPrimeira
        fimForParaLimparPrimeira:
            mov rectParaApagar.top, 11
            mov rectParaApagar.left, 11
            invoke getPixel, indice
            mov bx, ax ; ebx coluna e eax linha
            shr eax, 16
            mov rectParaApagar.bottom, eax
            mov ecx, rectParaApagar.bottom
            sub ecx, 11
            push ecx
            mov rectParaApagar.right,ebx
            invoke GetWindowRect, hWnd, OFFSET rectParaApagar
            pop ecx
            invoke BitBlt, hdc, 11, 43, 320, ecx, hdc, 11, 11, SRCCOPY
            ret
    


descerMatriz endp



acharLinhas proc hWin:DWORD, pMapa:DWORD
    invoke GlobalAlloc, 0, 8
    push eax
    mov dword ptr[eax], 0
    add eax, 4
    mov dword ptr[eax], 0
    sub eax, 4
    mov edx, pMapa
    mov esi, (MATRIZ ptr[edx]).ponteiro
    xor ecx, ecx
    mov ecx, 51
    xor ebx,ebx
    xor edx,edx
    forLinhas:
        cmp edx, 0
        je continuarForLinhas
        mov word ptr[eax], dx
        add eax, 2
        continuarForLinhas:
            cmp ecx, 365
            jge fimForLinhas
            mov ebx, ecx
            add ebx, 10
            forPercorreLinha:
                cmp ecx, ebx
                je achouLinha
                xor edx, edx
                mov dl,  byte ptr[esi + ecx]
                cmp dl, 0
                je voltarAbruptamente
                inc ecx
                jmp forPercorreLinha
                voltarAbruptamente:
                    mov ecx, ebx
                    add ecx, 6
                    xor edx,edx
                    jmp forLinhas
                achouLinha:
                    mov edx, ecx
                    sub edx, 10
                    add ecx, 6
                    jmp forLinhas
    fimForLinhas:
        pop eax
        ret
acharLinhas endp


;P + Tcol + Tlin*6
desenharTetrominoGuardado proc hWin:DWORD, hDC:DWORD, pTetromino:DWORD, cor:BYTE
    mov edx, pTetromino
    mov edx, (TETROMINO ptr[edx]).mat

    xor eax, eax
    xor ecx, ecx
    mov al, (MATRIZ ptr[edx]).largura
    mov cl, (MATRIZ ptr[edx]).altura
    mul ecx ; eax tem o valor de posições da matriz do tetronimo, é usado para percorrer

    mov edx, pTetromino
    mov edx, (TETROMINO ptr[edx]).mat
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

    mov edx, pTetromino
    mov edx, (TETROMINO ptr[edx]).mat
    mov bl, (MATRIZ ptr[edx]).largura  ;bl tem a largura

    xor edx, edx

    div ebx   ;al tem a linha

    mov esi, edx

    mov ebx, 6
    mul ebx

    mov edx, esi

    add ax, dx

    xor ecx,ecx
    mov edx, pTetromino

    mov cl, cor
    
    invoke desenharBlocoGuardado, hWin, hDC, cl, ax

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
desenharTetrominoGuardado endp

desenharBlocoGuardado proc hWin:DWORD, hDC:DWORD, cor:BYTE, posicao:WORD

    LOCAL hOld:DWORD
    LOCAL memDC :DWORD

    invoke CreateCompatibleDC,hDC

    mov memDC, eax
    invoke SelectObject,memDC,hBmp  ; selecionei o novo bitmap
    mov hOld, eax
    xor ebx, ebx
    invoke getPixelGuardado, posicao
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

desenharBlocoGuardado endp

getPixelGuardado proc pos:WORD

    xor edx, edx
    xor eax, eax
    mov ax, pos
    mov ecx, 6
    div ecx

    shl eax, 5
    shl edx, 5
    add eax, 91
    add edx, 451
    shl eax, 16
    mov ax, dx
    
    ret
getPixelGuardado endp