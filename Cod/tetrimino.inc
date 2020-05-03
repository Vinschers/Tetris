montarTetrimino proc pTetrimino:DWORD, tipo:BYTE
    call criarMatriz
    mov edx, pTetrimino
    mov (TETRIMINO ptr[edx]).mat, eax
    mov (TETRIMINO ptr[edx]).posicao, 54
    mov (TETRIMINO ptr[edx]).rotacao, 0

    cmp tipo, 6
    jne pecaNormal

    mov (MATRIZ ptr[eax]).largura, 4
    mov (MATRIZ ptr[eax]).altura, 4
    jmp fim

    pecaNormal:
    mov (MATRIZ ptr[eax]).largura, 3
    mov (MATRIZ ptr[eax]).altura, 3

    fim:
    xor eax, eax
    mov al, tipo

    mov edx, pTetrimino

    mov (TETRIMINO ptr[edx]).tipo, al

    mov cl, 9
    mul cl
    mov ecx, OFFSET pecas
    add ecx, eax

    mov eax, pTetrimino

    mov edx, (TETRIMINO ptr[eax]).mat

    invoke copiarPonteiroMatriz, ecx, edx

    ret
montarTetrimino endp

destruirTetrimino proc pTetrimino:DWORD
    mov eax, pTetrimino
    invoke destruirMatriz, (TETRIMINO ptr[eax]).mat, 1
    mov eax, pTetrimino
    invoke GlobalFree, eax
    ret
destruirTetrimino endp

refazerTetrimino proc pTetrimino:DWORD, tipo:BYTE
    mov edx, pTetrimino
    mov eax, (TETRIMINO ptr[edx]).mat
    mov (TETRIMINO ptr[edx]).posicao, 54
    mov (TETRIMINO ptr[edx]).rotacao, 0

    cmp tipo, 6
    jne pecaNormal

    mov (MATRIZ ptr[eax]).largura, 4
    mov (MATRIZ ptr[eax]).altura, 4
    jmp fim

    pecaNormal:
    mov (MATRIZ ptr[eax]).largura, 3
    mov (MATRIZ ptr[eax]).altura, 3

    fim:
    xor eax, eax
    mov al, tipo

    mov edx, pTetrimino

    mov (TETRIMINO ptr[edx]).tipo, al

    mov cl, 9
    mul cl
    mov ecx, OFFSET pecas
    add ecx, eax

    mov eax, pTetrimino
    mov edx, (TETRIMINO ptr[eax]).mat

    invoke copiarPonteiroMatriz, ecx, edx

    ret
refazerTetrimino endp


copiarTetrimino proc ponteiro:DWORD
    LOCAL ptrNovo: DWORD

    invoke GlobalAlloc, 0, sizeof TETRIMINO
    mov edx, ponteiro
    mov cl, (TETRIMINO ptr[edx]).tipo

    mov ptrNovo, eax
    mov edx, eax
    invoke montarTetrimino, edx, cl

    mov eax, ptrNovo
    mov edx, ponteiro

    mov ebx, (TETRIMINO ptr[edx]).posicao
    mov (TETRIMINO ptr[eax]).posicao, ebx
    mov bl, (TETRIMINO ptr[edx]).rotacao
    mov (TETRIMINO ptr[eax]).rotacao, bl

    mov eax, (TETRIMINO ptr[edx]).mat
    mov bl, (MATRIZ ptr[eax]).largura
    mov cl, (MATRIZ ptr[eax]).altura

    xor eax, eax
    mov al, cl
    xor ecx, ecx
    mov cl, bl

    mul ecx
    xor esi, esi   ;contador

    mov edx, ponteiro
    mov edx, (TETRIMINO ptr[edx]).mat
    mov edx, (MATRIZ ptr[edx]).ponteiro

    mov edi, ptrNovo
    mov edi, (TETRIMINO ptr[edi]).mat
    mov edi, (MATRIZ ptr[edi]).ponteiro

    forCopia:
        cmp esi, eax
        je fim

        mov bl, byte ptr [edx]
        mov byte ptr [edi], bl

        inc esi
        inc edx
        inc edi

        jmp forCopia

    fim:
    return ptrNovo

copiarTetrimino  endp

adicionarMatrizLogica proc pTetrimino:DWORD, pMapa:DWORD
    LOCAL mapaLargura:BYTE
    LOCAL mapaAltura:BYTE
    LOCAL tetriLargura:BYTE
    LOCAL tetriAltura:BYTE

    mov esi, pMapa
    mov cl, (MATRIZ ptr[esi]).altura
    mov mapaAltura, cl
    mov cl, (MATRIZ ptr[esi]).largura
    mov mapaLargura, cl
    mov esi, (MATRIZ ptr[esi]).ponteiro

    mov edi, pTetrimino
    xor edx, edx
    mov edx, (TETRIMINO ptr[edi]).posicao
    mov edi, (TETRIMINO ptr[edi]).mat
    mov cl, (MATRIZ ptr[edi]).altura
    mov tetriAltura, cl
    mov cl, (MATRIZ ptr[edi]).largura
    mov tetriLargura, cl
    mov edi, (MATRIZ ptr[edi]).ponteiro


    xor eax, eax
    xor ecx, ecx

    forLinha:
        cmp cl, tetriAltura
        je fim

        forColuna:
            cmp ch, tetriLargura
            je fimForColuna

            push ecx
            push edx

            xor eax, eax
            xor ebx, ebx
            mov al, cl
            mov bl, ch
            xor ch, ch
            mov cl, tetriLargura
            mul ecx

            pop edx
            xor ecx, ecx

            add eax, ebx

            mov cl, byte ptr[esi + edx]
            
            add byte ptr[edi + eax], cl

            pop ecx

            inc edx
            inc ch
            jmp forColuna
        fimForColuna:
        xor ch, ch
        xor eax, eax
        mov al, mapaLargura
        cbw
        cwde
        add edx, eax
        xor eax, eax
        mov al, tetriLargura
        cbw
        cwde
        sub edx, eax
        inc cl
        jmp forLinha
    fim:
    ret
adicionarMatrizLogica endp

colocarMatrizLogica proc pTetrimino:DWORD, pMapa:DWORD, valor:BYTE
    LOCAL mapaLargura:BYTE
    LOCAL mapaAltura:BYTE
    LOCAL tetriLargura:BYTE
    LOCAL tetriAltura:BYTE

    mov esi, pMapa
    mov cl, (MATRIZ ptr[esi]).altura
    mov mapaAltura, cl
    mov cl, (MATRIZ ptr[esi]).largura
    mov mapaLargura, cl
    mov esi, (MATRIZ ptr[esi]).ponteiro

    mov edi, pTetrimino
    xor edx, edx
    mov edx, (TETRIMINO ptr[edi]).posicao
    mov edi, (TETRIMINO ptr[edi]).mat
    mov cl, (MATRIZ ptr[edi]).altura
    mov tetriAltura, cl
    mov cl, (MATRIZ ptr[edi]).largura
    mov tetriLargura, cl
    mov edi, (MATRIZ ptr[edi]).ponteiro

    xor ecx, ecx

    forLinha:
        cmp cl, tetriAltura
        je fim

        forColuna:
            cmp ch, tetriLargura
            je fimForColuna

            push ecx
            push edx

            xor eax, eax
            xor ebx, ebx
            mov al, cl
            mov bl, ch
            xor ch, ch
            mov cl, tetriLargura
            mul ecx

            add eax, ebx

            pop edx

            xor ecx, ecx
            mov cl, byte ptr[edi + eax]
            cmp cl, 1
            jne continuar

            mov cl, valor
            mov byte ptr[esi + edx], cl

            continuar:
            pop ecx

            inc edx
            inc ch
            jmp forColuna
        fimForColuna:
        xor ch, ch
        xor eax, eax
        mov al, mapaLargura
        cbw
        cwde
        add edx, eax
        xor eax, eax
        mov al, tetriLargura
        cbw
        cwde
        sub edx, eax
        inc cl
        jmp forLinha
    fim:
    ret
colocarMatrizLogica endp

verificarColisao proc mat:DWORD
    xor eax, eax
    xor ecx, ecx

    mov ebx, mat
    mov al, (MATRIZ ptr[ebx]).altura
    mov cl, (MATRIZ ptr[ebx]).largura
    mul ecx
    mov ecx, eax
    mov ebx, mat
    mov esi, (MATRIZ ptr[ebx]).ponteiro

    xor ebx, ebx
    xor eax, eax
    xor edx, edx

    forColisao:
    cmp bl, cl
    je fim

    mov dl, byte ptr[esi + ebx]

    cmp dl, 1
    jbe contForColisao

    mov eax, 1
    jmp fim

    contForColisao:
    inc bl
    jmp forColisao

    fim:
    ret
verificarColisao endp

atribuirTetrimino proc dest:DWORD, src:DWORD
    mov eax, dest
    mov edx, src

    mov ebx, (TETRIMINO ptr[edx]).posicao
    mov (TETRIMINO ptr[eax]).posicao, ebx
    mov cl, (TETRIMINO ptr[edx]).tipo
    mov (TETRIMINO ptr[eax]).tipo, cl
    mov cl, (TETRIMINO ptr[edx]).rotacao
    mov (TETRIMINO ptr[eax]).rotacao, cl

    mov eax, (TETRIMINO ptr[edx]).mat
    mov bl, (MATRIZ ptr[eax]).largura
    mov cl, (MATRIZ ptr[eax]).altura

    xor eax, eax
    mov al, cl
    xor ecx, ecx
    mov cl, bl

    mul ecx
    xor esi, esi   ;contador

    mov edx, src
    mov edx, (TETRIMINO ptr[edx]).mat
    mov edx, (MATRIZ ptr[edx]).ponteiro

    mov edi, dest
    mov edi, (TETRIMINO ptr[edi]).mat
    mov edi, (MATRIZ ptr[edi]).ponteiro

    forCopia:
        cmp esi, eax
        je fim

        mov bl, byte ptr [edx]
        mov byte ptr [edi], bl

        inc esi
        inc edx
        inc edi

        jmp forCopia

    fim:
    ret
atribuirTetrimino endp

testesRotacao proc pTetrimino:DWORD, pMapa:DWORD
    LOCAL tipo:BYTE
    LOCAL rot:BYTE

    mov eax, pTetrimino
    mov cl, (TETRIMINO ptr[eax]).tipo
    mov tipo, cl
    mov cl, (TETRIMINO ptr[eax]).rotacao
    mov rot, cl

    .if tipo == 6
        .if rot == 0
            invoke testarRotacao, pTetrimino, pMapa, 1, 0
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetrimino, pMapa, -2, 0
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetrimino, pMapa, 1, 2
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetrimino, pMapa, -2, -1
            .if eax == 1
                jmp fim
            .endif

        .elseif rot == 1
            invoke testarRotacao, pTetrimino, pMapa, -2, 0
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetrimino, pMapa, 1, 0
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetrimino, pMapa, -2, 1
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetrimino, pMapa, 1, -2
            .if eax == 1
                jmp fim
            .endif
            
        .elseif rot == 2
            invoke testarRotacao, pTetrimino, pMapa, -1, 0
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetrimino, pMapa, 2, 0
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetrimino, pMapa, -1, -2
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetrimino, pMapa, 2, 1
            .if eax == 1
                jmp fim
            .endif
            
        .elseif rot == 3
            invoke testarRotacao, pTetrimino, pMapa, 2, 0
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetrimino, pMapa, -1, 0
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetrimino, pMapa, 2, -1
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetrimino, pMapa, -1, 2
            .if eax == 1
                jmp fim
            .endif
            
        .endif

    .else
        .if rot == 0
            invoke testarRotacao, pTetrimino, pMapa, -1, 0
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetrimino, pMapa, -1, 1
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetrimino, pMapa, 0, -2
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetrimino, pMapa, -1, -2
            .if eax == 1
                jmp fim
            .endif

        .elseif rot == 1
            invoke testarRotacao, pTetrimino, pMapa, -1, 0
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetrimino, pMapa, -1, -1
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetrimino, pMapa, 0, 2
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetrimino, pMapa, -1, 2
            .if eax == 1
                jmp fim
            .endif
            
        .elseif rot == 2
            invoke testarRotacao, pTetrimino, pMapa, 1, 0
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetrimino, pMapa, 1, 1
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetrimino, pMapa, 0, -2
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetrimino, pMapa, 1, -2
            .if eax == 1
                jmp fim
            .endif
            
        .elseif rot == 3
            invoke testarRotacao, pTetrimino, pMapa, 1, 0
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetrimino, pMapa, 1, -1
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetrimino, pMapa, 0, 2
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetrimino, pMapa, 1, 2
            .if eax == 1
                jmp fim
            .endif
            
        .endif
    .endif
    xor eax, eax
    fim:
    ret
testesRotacao endp

testarRotacao proc pTetrimino:DWORD, pMapa:DWORD, x:BYTE, y:BYTE
    LOCAL copiaT:DWORD
    LOCAL result:DWORD
    invoke copiarTetrimino, pTetrimino
    mov copiaT, eax

    mov esi, pMapa

    xor eax, eax
    xor ecx, ecx
    mov al, y
    mov cl, (MATRIZ ptr[esi]).largura
    mul cl

    xor ecx, ecx
    mov cl, x
    push eax
    xor eax, eax
    mov al, cl
    cbw
    mov cx, ax
    pop eax
    add ax, cx
    cwde

    mov edi, copiaT
    add (TETRIMINO ptr[edi]).posicao, eax

    invoke adicionarMatrizLogica, edi, esi
    mov edi, copiaT
    invoke verificarColisao, (TETRIMINO ptr[edi]).mat

    mov result, eax

    .if result == 0
        mov edi, copiaT
        mov ebx, (TETRIMINO ptr[edi]).posicao
        mov edi, pTetrimino
        mov (TETRIMINO ptr[edi]).posicao, ebx
    .endif

    invoke destruirTetrimino, copiaT

    .if result == 0
        mov result, 1
    .elseif result == 1
        mov result, 0
    .endif

    mov eax, result
    ret
testarRotacao endp