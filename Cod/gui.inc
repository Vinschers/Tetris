;//////////////////////////////////////////////////////////////////////////////////
;feito por:
;18188 Guilherme Salim de Barros
;18178 Felipe Scherer Vicentin
;18343 Lorenna Joanna Leal Nunes 
;18190 Maria Eduarda Elias Rocha
;para o prof Sergio na matéria 'linguagem de montagem'
;todos direitos reservados -- 2019  ©
;//////////////////////////////////////////////////////////////////////////////////

;procedimento que desenha a janela 
desenharTela proc hWin:DWORD, hdc:DWORD
    LOCAL corDeFundo:BYTE
    LOCAL corDeFundoRect:BYTE
    mov corDeFundo, 0
    mov corDeFundoRect, 255

    cmp tema, 2
    jne temaClaro

    mov corDeFundoRect, 33

    temaClaro:
    cmp tema, 0
    jne temaEscuro

    add corDeFundo, 220

    temaEscuro:
    add corDeFundo, 20

    mov rect.top, 0
    mov rect.left, 0
    mov rect.bottom, 700
    mov rect.right, 650
    RGB corDeFundo, corDeFundo, corDeFundo
    invoke CreateSolidBrush, eax
    invoke FillRect, hdc, OFFSET rect, eax

    ;configuração das posições
	mov rect.top, 10
	mov rect.bottom, 652
	mov rect.left, 10
	mov rect.right, 332

	RGB corDeFundoRect, corDeFundoRect, corDeFundoRect
    invoke CreateSolidBrush, eax
    invoke FillRect, hdc, OFFSET rect, eax

	invoke  lstrlen, ADDR pecaSeguradaTxt
	invoke  TextOut, hdc, 400, 20, ADDR pecaSeguradaTxt, eax

	mov rect.top, 50
	mov rect.bottom, 210
	mov rect.left, 390
	mov rect.right, 582
	RGB corDeFundoRect, corDeFundoRect, corDeFundoRect
    invoke CreateSolidBrush, eax
    invoke FillRect, hdc, OFFSET rect, eax

	invoke  lstrlen, ADDR proxPecaTxt
	invoke  TextOut, hdc, 400, 250, ADDR proxPecaTxt, eax

	mov rect.top, 280
	mov rect.bottom, 450
	mov rect.left, 390
	mov rect.right, 582
	RGB corDeFundoRect, corDeFundoRect, corDeFundoRect
    invoke CreateSolidBrush, eax
    invoke FillRect, hdc, OFFSET rect, eax

    xor eax, eax
    mov al, nivel


    invoke wsprintf, OFFSET buffer, OFFSET formatacaoNivel, eax
    invoke GetClientRect, hWin, ADDR rect
    
    
    mov rect.left, 400
    mov rect.top, 470
    mov rect.right, 650
    mov rect.bottom, 500
    invoke DrawText, hdc, ADDR buffer, -1, ADDR rect, DT_SINGLELINE

    mov eax, pontuacao


    invoke wsprintf, OFFSET buffer, OFFSET formatacaoPontuacao, eax
    invoke GetClientRect, hWin, ADDR rect
    
    
    mov rect.left, 400
    mov rect.top, 530
    mov rect.right, 650
    mov rect.bottom, 550
    invoke DrawText, hdc, ADDR buffer, -1, ADDR rect, DT_SINGLELINE

	ret
desenharTela endp

;P + Tcol + Tlin*16
desenharTetromino proc hWin:DWORD, hDC:DWORD, pTetromino:DWORD, cor:BYTE
    mov edx, pTetromino
    mov edx, (TETROMINO ptr[edx]).mat
    ;limpa os registradores
    xor eax, eax
    xor ecx, ecx
    mov al, (MATRIZ ptr[edx]).largura
    mov cl, (MATRIZ ptr[edx]).altura
    mul ecx

    mov edx, pTetromino
    mov edx, (TETROMINO ptr[edx]).mat
    mov edx, (MATRIZ ptr[edx]).ponteiro
    ;limpas os registradores
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

    xor ecx,ecx; limpa registrador
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

    xor ebx, ebx; limpa registrador
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
    ; limpa registradores
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

paint proc hWin:DWORD, hdc:DWORD, pTetromino:DWORD, pMapa:DWORD, pFantasma:DWORD, vetPecas:DWORD, atual:DWORD
	LOCAL copiaTetromino:DWORD
	LOCAL copiaMatriz:DWORD
	LOCAL colidiu:BYTE
    LOCAL corDeFundoRect:BYTE
    mov corDeFundoRect, 255

    cmp tema, 2
    jne temaClaro

    mov corDeFundoRect, 33

    temaClaro:
	invoke copiarTetromino, pTetromino
    mov copiaTetromino, eax
    
    invoke colocarMatrizLogica, pTetromino, pMapa, 0

    mov al, paintParam
    mov dl, pecaGuardada
    mov cl, jaGuardou

    .if al == PP_DESENHAR
        invoke desenharTela, hWin ,hdc
        mov cl, NADA
        add cl, tema
        invoke desenharTetromino, hWin, hdc, pFantasma, cl
        invoke atualizarFantasma, pFantasma, pTetromino, pMapa
        mov esi, pFantasma
        mov cl, (TETROMINO ptr[esi]).tipo
        invoke desenharTetromino, hWin, hdc, pFantasma, cl
        jmp desenharProximaPeca

    .elseif al == PP_DESCER
        mov cl, NADA
        add cl, tema
        invoke desenharTetromino, hWin, hdc, pTetromino, cl
        mov esi, pTetromino
        add (TETROMINO ptr[esi]).posicao, 16

    .elseif al == PP_ROTACIONAR
        mov cl, NADA
        add cl, tema
        invoke desenharTetromino, hWin, hdc, pTetromino, cl
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
        mov cl, NADA
        add cl, tema
        invoke desenharTetromino, hWin, hdc, pTetromino, cl
        mov esi, pTetromino
        inc (TETROMINO ptr[esi]).posicao

    .elseif al == PP_MOVER_ESQUERDA
        mov cl, NADA
        add cl, tema
        invoke desenharTetromino, hWin, hdc, pTetromino, cl
        mov esi, pTetromino
        dec (TETROMINO ptr[esi]).posicao

    .elseif al == PP_REFAZER
        cmp cl, 0
        jne fimRefazer 
        
        push ecx
        push edx
        mov rect.top, 50
	    mov rect.bottom, 210
	    mov rect.left, 390
	    mov rect.right, 582
	    RGB corDeFundoRect, corDeFundoRect, corDeFundoRect
        invoke CreateSolidBrush, eax
        invoke FillRect, hdc, OFFSET rect, eax
        pop edx
        pop ecx

        mov jaGuardou, 1 ; há peça guardada 
        push edx
        mov cl, NADA
        add cl, tema
        invoke desenharTetromino, hWin, hdc, pTetromino, cl
        mov cl, NADA
        add cl, tema
        invoke desenharTetromino, hWin, hdc, pFantasma, cl
        mov esi, pTetromino
        mov bl, (TETROMINO ptr[esi]).tipo ; bl recebeu o tipo da peça que será guardada 
        mov pecaGuardada, bl
        invoke desenharTetrominoGuardado, hWin, hdc, pTetromino, bl
        mov esi, pTetromino
        pop edx
        cmp dl, 7
        jne jaTemGuardada
        jmp proximaPeca

        jaTemGuardada:
            invoke refazerTetrominoTipo, pTetromino, dl
        fimRefazer:
    .elseif al == PP_CAIR_TUDO
        mov cl, NADA
        add cl, tema
        invoke desenharTetromino, hWin, hdc, pTetromino, cl
        invoke descerTudo, pTetromino, pMapa
        mov colidiu, 1
        jmp atualizarVariaveis

    .elseif al == PP_MUDAR_TEMA
        mov perdeu, 1
        invoke PlaySound,NULL,NULL,SND_ASYNC
        invoke MessageBox,hWin, addr temaTxt, NULL, MB_YESNO

        .if eax == IDYES
            .if tema == 0
                mov tema, 2
            .else
                mov tema, 0
            .endif
            invoke comecarJogo, hWin
            invoke desenharTela, hWin,hdc
            mov edx, pTetromino
            mov cl, (TETROMINO ptr[edx]).tipo
            invoke desenharTetromino, hWin, hdc, pTetromino, cl
            invoke atualizarFantasma, pFantasma, pTetromino, pMapa
            mov esi, pFantasma
            mov cl, (TETROMINO ptr[esi]).tipo
            invoke desenharTetromino, hWin, hdc, pFantasma, cl
        .else
            mov perdeu, 0
            invoke PlaySound, OFFSET caminhoMusica,NULL,SND_ASYNC or SND_LOOP
        .endif

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

    mov cl, NADA
    add cl, tema
    invoke desenharTetromino, hWin, hdc, pFantasma, cl
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
                invoke desenharTela, hWin,hdc
                invoke atualizarFantasma, pFantasma, pTetromino, pMapa
                jmp desenharProximaPeca
            .else
                invoke trabalharComLinhas, hWin, hdc, pMapa
                mov velocidade, 500
                proximaPeca:
                mov ebx, atual
                inc byte ptr [ebx]
                mov cl, byte ptr [ebx]
                .if cl == 7   ; caso o registrador cl seja igual a 7, significa que chegamos ao fim do vetor de peças
                    mov ebx, atual
                    mov byte ptr [ebx], 0
                .endif 
                mov cl, byte ptr [ebx]
                invoke refazerTetrominoVetor, pTetromino, vetPecas, cl
                invoke atualizarFantasma, pFantasma, pTetromino, pMapa
                pontuacaoPecaCaida:
                    push eax
                    push ebx
                    push ecx
                    push edx

                    xor ebx,ebx
                    mov bl, nivel
                    inc ebx
                    mov eax, 100
                    mul ebx

                    add pontuacao, eax
                    push eax
                    push ebx
                    push ecx
                    push edx

                    mov eax, pontuacao


                    invoke wsprintf, OFFSET buffer, OFFSET formatacaoPontuacao, eax
                    invoke GetClientRect, hWin, ADDR rect
                    
                    
                    mov rect.left, 400
                    mov rect.top, 530
                    mov rect.right, 650
                    mov rect.bottom, 550
                    invoke DrawText, hdc, ADDR buffer, -1, ADDR rect, DT_SINGLELINE

                    pop edx
                    pop ecx
                    pop ebx
                    pop eax
                    pop edx
                    pop ecx
                    pop ebx
                    pop eax

                desenharProximaPeca:
                mov ebx, atual
                xor ecx, ecx; limpa registrador
                mov cl, byte ptr [ebx]
                inc cl 

                cmp cl, 7 
                jne atualizarProx

                invoke randomizarVetor, vetPecas
                mov cl, 0

                atualizarProx:
                mov edx, vetPecas
                mov cl, byte ptr[edx + ecx]
                invoke desenharTetrominoProx, hWin, hdc, cl
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
    invoke PlaySound,NULL,NULL,SND_ASYNC
    invoke MessageBox,hWin, addr perdeuTxt, NULL, MB_YESNO

    .if eax == IDYES
        invoke comecarJogo, hWin
    .else
        invoke ExitProcess,0
    .endif
    ret
perder endp

trabalharComLinhas proc hWin:DWORD, hdc:DWORD, pMapa:DWORD
    LOCAL retanguloTextoPontuacao:RECT
    invoke acharLinhas, hWin ,pMapa
    mov esi, pMapa
    mov esi, (MATRIZ ptr[esi]).ponteiro
    xor ecx, ecx; limpa registrador
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
        inc linhasAux 

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
        xor ecx, ecx; limpa registrador
        mov cl, linhasAux
        cmp cl, 0
        je retornar
        .if cl == 1
            mov eax, 40
        .elseif cl == 2
            mov eax, 100
        .elseif cl == 3
            mov eax, 300
        .elseif cl == 4
            mov eax, 1200
        .endif

        xor ebx, ebx; limpa registrador
        mov bl, nivel

        inc bl 
        mul ebx
        add pontuacao, eax
        
        push eax
        push ebx
        push ecx
        push edx

        mov eax, pontuacao


        invoke wsprintf, OFFSET buffer, OFFSET formatacaoPontuacao, eax
        invoke GetClientRect, hWin, ADDR retanguloTextoPontuacao
        
        
        mov retanguloTextoPontuacao.left, 400
        mov retanguloTextoPontuacao.top, 530
        mov retanguloTextoPontuacao.right, 650
        mov retanguloTextoPontuacao.bottom, 550
        invoke DrawText, hdc, ADDR buffer, -1, ADDR retanguloTextoPontuacao, DT_SINGLELINE

        pop edx
        pop ecx
        pop ebx
        pop eax

        dec bl
        mov linhasAux, 0
        add linhas, cl

        xor ecx, ecx ; limpa registrador
        mov cl, linhas
        cmp ecx, 10
        jl retornar

        inc bl
        mov nivel, bl
        push eax
        push ebx
        push ecx
        push edx
        xor eax, eax
        mov al, nivel


        invoke wsprintf, OFFSET buffer, OFFSET formatacaoNivel, eax
        invoke GetClientRect, hWin, ADDR retanguloTextoPontuacao
        
        
        mov retanguloTextoPontuacao.left, 400
        mov retanguloTextoPontuacao.top, 470
        mov retanguloTextoPontuacao.right, 650
        mov retanguloTextoPontuacao.bottom, 500
        invoke DrawText, hdc, ADDR buffer, -1, ADDR retanguloTextoPontuacao, DT_SINGLELINE

        pop edx
        pop ecx
        pop ebx
        pop eax

        mov linhas, 0

        mov edx, velocidade
        sub edx, 60
        mov velocidade, edx
        
    retornar:
        ret
trabalharComLinhas endp

limparLinha proc hWin:DWORD, hdc:DWORD, pMapa:DWORD, indice:WORD 
    xor edx, edx; limpa registrador
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

        mov cl, NADA
        add cl, tema
        invoke desenharBloco, hWin, hdc, cl, dx
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
    xor ecx, ecx; limpa registrador
    mov cx, indice
    sub ecx, 16
    ; limpa registradores
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
    xor ecx, ecx; limpa registrador
    mov ecx, 51
    ; limpa registradores
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
                xor edx, edx; limpa registrador
                mov dl,  byte ptr[esi + ecx]
                cmp dl, 0
                je voltarAbruptamente
                inc ecx
                jmp forPercorreLinha
                voltarAbruptamente:
                    mov ecx, ebx
                    add ecx, 6
                    xor edx,edx; limpa registrador
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
;procedimento que desenha na matriz o tetromino que foi guardado pelo usuário
desenharTetrominoGuardado proc hWin:DWORD, hDC:DWORD, pTetromino:DWORD, cor:BYTE
    mov edx, pTetromino
    mov edx, (TETROMINO ptr[edx]).mat
    ;limpa registradores
    xor eax, eax
    xor ecx, ecx
    mov al, (MATRIZ ptr[edx]).largura
    mov cl, (MATRIZ ptr[edx]).altura
    mul ecx ; eax tem o valor de posições da matriz do tetronimo, é usado para percorrer

    mov edx, pTetromino
    mov edx, (TETROMINO ptr[edx]).mat
    mov edx, (MATRIZ ptr[edx]).ponteiro
    ;limpa registradores
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

    xor eax, eax; limpa registrador
    mov al, bl ;al tem o contador
    xor ebx, ebx; limpa registrador

    mov edx, pTetromino
    mov edx, (TETROMINO ptr[edx]).mat
    mov bl, (MATRIZ ptr[edx]).largura  ;bl tem a largura

    xor edx, edx; limpa registrador

    div ebx   ;al tem a linha

    mov esi, edx

    mov ebx, 6
    mul ebx

    mov edx, esi

    add ax, dx

    xor ecx,ecx; limpa registrador
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
    xor ebx, ebx; limpa registrador
    invoke getPixelGuardado, posicao
    mov bx, ax ; ebx coluna e eax linha
    shr eax, 16

    mov ecx, eax
    mov edx, ebx
    ; limpa registradores
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
    ; limpa registradores
    xor edx, edx
    xor eax, eax
    mov ax, pos
    mov ecx, 6
    div ecx

    shl eax, 5
    shl edx, 5
    add eax, 51
    add edx, 451
    shl eax, 16
    mov ax, dx
    
    ret

getPixelGuardado endp

;P + Tcol + Tlin*6
desenharTetrominoProx proc hWin:DWORD, hDC:DWORD, cor:BYTE
    LOCAL pTetromino:DWORD
    LOCAL corDeFundoRect:BYTE
    mov corDeFundoRect, 255

    cmp tema, 2
    jne temaClaro

    mov corDeFundoRect, 33

    temaClaro:
    invoke GlobalAlloc, 0, sizeof TETROMINO
    mov pTetromino, eax

    mov rect.top, 280
	mov rect.bottom, 450
	mov rect.left, 390
	mov rect.right, 582
	RGB corDeFundoRect, corDeFundoRect, corDeFundoRect
    invoke CreateSolidBrush, eax
    invoke FillRect, hDC, OFFSET rect, eax



    invoke montarTetromino, pTetromino, cor
    mov edx, pTetromino
    mov edx, (TETROMINO ptr[edx]).mat
    ; limpa registradores
    xor eax, eax
    xor ecx, ecx
    mov al, (MATRIZ ptr[edx]).largura
    mov cl, (MATRIZ ptr[edx]).altura
    mul ecx ; eax tem o valor de posições da matriz do tetromino, é usado para percorrer

    mov edx, pTetromino
    mov edx, (TETROMINO ptr[edx]).mat
    mov edx, (MATRIZ ptr[edx]).ponteiro
    ; limpa registradores
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

    xor eax, eax; limpa registrador
    mov al, bl ;al tem o contador
    xor ebx, ebx; limpa registrador

    mov edx, pTetromino
    mov edx, (TETROMINO ptr[edx]).mat
    mov bl, (MATRIZ ptr[edx]).largura  ;bl tem a largura

    xor edx, edx; limpa registrador

    div ebx   ;al tem a linha

    mov esi, edx

    mov ebx, 6
    mul ebx

    mov edx, esi

    add ax, dx

    xor ecx,ecx; limpa registrador
    mov edx, pTetromino

    mov cl, cor
    
    invoke desenharBlocoProx, hWin, hDC, cl, ax

    pop edx
    pop ecx
    pop ebx
    pop eax

    cont:
    inc ebx
    inc edx
    jmp forT

    fim:
    invoke destruirTetromino, pTetromino
    ret
desenharTetrominoProx endp

desenharBlocoProx proc hWin:DWORD, hDC:DWORD, cor:BYTE, posicao:WORD

    LOCAL hOld:DWORD
    LOCAL memDC :DWORD

    invoke CreateCompatibleDC,hDC

    mov memDC, eax
    invoke SelectObject,memDC,hBmp  ; selecionei o novo bitmap
    mov hOld, eax
    xor ebx, ebx; limpa registrador
    invoke getPixelProx, posicao
    mov bx, ax ; ebx coluna e eax linha
    shr eax, 16

    mov ecx, eax
    mov edx, ebx
    ; limpa registrador
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

desenharBlocoProx endp

getPixelProx proc pos:WORD
    ; limpa registradores
    xor edx, edx
    xor eax, eax
    mov ax, pos
    mov ecx, 6
    div ecx

    shl eax, 5
    shl edx, 5
    add eax, 321
    add edx, 451
    shl eax, 16
    mov ax, dx
    
    ret

getPixelProx endp