montarTetromino proc pTetromino:DWORD, tipo:BYTE
    call criarMatriz
    mov edx, pTetromino
    mov (TETROMINO ptr[edx]).mat, eax
    mov (TETROMINO ptr[edx]).posicao, 54
    mov (TETROMINO ptr[edx]).rotacao, 0

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

    mov edx, pTetromino

    mov (TETROMINO ptr[edx]).tipo, al

    mov cl, 9
    mul cl
    mov ecx, OFFSET pecas
    add ecx, eax

    mov eax, pTetromino

    mov edx, (TETROMINO ptr[eax]).mat

    invoke copiarPonteiroMatriz, ecx, edx

    ret
montarTetromino endp

destruirTetromino proc pTetromino:DWORD
    mov eax, pTetromino
    invoke destruirMatriz, (TETROMINO ptr[eax]).mat, 1
    mov eax, pTetromino
    invoke GlobalFree, eax
    ret
destruirTetromino endp

refazerTetromino proc pTetromino:DWORD, tipo:BYTE
    mov edx, pTetromino
    mov eax, (TETROMINO ptr[edx]).mat
    mov (TETROMINO ptr[edx]).posicao, 54
    mov (TETROMINO ptr[edx]).rotacao, 0

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

    mov edx, pTetromino

    mov (TETROMINO ptr[edx]).tipo, al

    mov cl, 9
    mul cl
    mov ecx, OFFSET pecas
    add ecx, eax

    mov eax, pTetromino
    mov edx, (TETROMINO ptr[eax]).mat

    invoke copiarPonteiroMatriz, ecx, edx

    ret
refazerTetromino endp


copiarTetromino proc ponteiro:DWORD
    LOCAL ptrNovo: DWORD

    invoke GlobalAlloc, 0, sizeof TETROMINO
    mov edx, ponteiro
    mov cl, (TETROMINO ptr[edx]).tipo

    mov ptrNovo, eax
    mov edx, eax
    invoke montarTetromino, edx, cl

    mov eax, ptrNovo
    mov edx, ponteiro

    mov ebx, (TETROMINO ptr[edx]).posicao
    mov (TETROMINO ptr[eax]).posicao, ebx
    mov bl, (TETROMINO ptr[edx]).rotacao
    mov (TETROMINO ptr[eax]).rotacao, bl

    mov eax, (TETROMINO ptr[edx]).mat
    mov bl, (MATRIZ ptr[eax]).largura
    mov cl, (MATRIZ ptr[eax]).altura

    xor eax, eax
    mov al, cl
    xor ecx, ecx
    mov cl, bl

    mul ecx
    xor esi, esi   ;contador

    mov edx, ponteiro
    mov edx, (TETROMINO ptr[edx]).mat
    mov edx, (MATRIZ ptr[edx]).ponteiro

    mov edi, ptrNovo
    mov edi, (TETROMINO ptr[edi]).mat
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

copiarTetromino  endp

adicionarMatrizLogica proc pTetromino:DWORD, pMapa:DWORD
    LOCAL mapaLargura:BYTE
    LOCAL mapaAltura:BYTE
    LOCAL tetroLargura:BYTE
    LOCAL tetroAltura:BYTE

    mov esi, pMapa
    mov cl, (MATRIZ ptr[esi]).altura
    mov mapaAltura, cl
    mov cl, (MATRIZ ptr[esi]).largura
    mov mapaLargura, cl
    mov esi, (MATRIZ ptr[esi]).ponteiro

    mov edi, pTetromino
    xor edx, edx
    mov edx, (TETROMINO ptr[edi]).posicao
    mov edi, (TETROMINO ptr[edi]).mat
    mov cl, (MATRIZ ptr[edi]).altura
    mov tetroAltura, cl
    mov cl, (MATRIZ ptr[edi]).largura
    mov tetroLargura, cl
    mov edi, (MATRIZ ptr[edi]).ponteiro


    xor eax, eax
    xor ecx, ecx

    forLinha:
        cmp cl, tetroAltura
        je fim

        forColuna:
            cmp ch, tetroLargura
            je fimForColuna

            push ecx
            push edx

            xor eax, eax
            xor ebx, ebx
            mov al, cl
            mov bl, ch
            xor ch, ch
            mov cl, tetroLargura
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
        mov al, tetroLargura
        cbw
        cwde
        sub edx, eax
        inc cl
        jmp forLinha
    fim:
    ret
adicionarMatrizLogica endp

colocarMatrizLogica proc pTetromino:DWORD, pMapa:DWORD, valor:BYTE
    LOCAL mapaLargura:BYTE
    LOCAL mapaAltura:BYTE
    LOCAL tetroLargura:BYTE
    LOCAL tetroAltura:BYTE

    mov esi, pMapa
    mov cl, (MATRIZ ptr[esi]).altura
    mov mapaAltura, cl
    mov cl, (MATRIZ ptr[esi]).largura
    mov mapaLargura, cl
    mov esi, (MATRIZ ptr[esi]).ponteiro

    mov edi, pTetromino
    xor edx, edx
    mov edx, (TETROMINO ptr[edi]).posicao
    mov edi, (TETROMINO ptr[edi]).mat
    mov cl, (MATRIZ ptr[edi]).altura
    mov tetroAltura, cl
    mov cl, (MATRIZ ptr[edi]).largura
    mov tetroLargura, cl
    mov edi, (MATRIZ ptr[edi]).ponteiro

    xor ecx, ecx

    forLinha:
        cmp cl, tetroAltura
        je fim

        forColuna:
            cmp ch, tetroLargura
            je fimForColuna

            push ecx
            push edx

            xor eax, eax
            xor ebx, ebx
            mov al, cl
            mov bl, ch
            xor ch, ch
            mov cl, tetroLargura
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
        mov al, tetroLargura
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

atribuirTetromino proc dest:DWORD, src:DWORD
    mov eax, dest
    mov edx, src

    mov ebx, (TETROMINO ptr[edx]).posicao
    mov (TETROMINO ptr[eax]).posicao, ebx
    mov cl, (TETROMINO ptr[edx]).tipo
    mov (TETROMINO ptr[eax]).tipo, cl
    mov cl, (TETROMINO ptr[edx]).rotacao
    mov (TETROMINO ptr[eax]).rotacao, cl

    mov eax, (TETROMINO ptr[edx]).mat
    mov bl, (MATRIZ ptr[eax]).largura
    mov cl, (MATRIZ ptr[eax]).altura

    xor eax, eax
    mov al, cl
    xor ecx, ecx
    mov cl, bl

    mul ecx
    xor esi, esi   ;contador

    mov edx, src
    mov edx, (TETROMINO ptr[edx]).mat
    mov edx, (MATRIZ ptr[edx]).ponteiro

    mov edi, dest
    mov edi, (TETROMINO ptr[edi]).mat
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
atribuirTetromino endp

testesRotacao proc pTetromino:DWORD, pMapa:DWORD
    LOCAL tipo:BYTE
    LOCAL rot:BYTE

    mov eax, pTetromino
    mov cl, (TETROMINO ptr[eax]).tipo
    mov tipo, cl
    mov cl, (TETROMINO ptr[eax]).rotacao
    mov rot, cl

    .if tipo == 6
        .if rot == 0
            invoke testarRotacao, pTetromino, pMapa, 1, 0
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetromino, pMapa, -2, 0
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetromino, pMapa, 1, 2
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetromino, pMapa, -2, -1
            .if eax == 1
                jmp fim
            .endif

        .elseif rot == 1
            invoke testarRotacao, pTetromino, pMapa, -2, 0
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetromino, pMapa, 1, 0
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetromino, pMapa, -2, 1
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetromino, pMapa, 1, -2
            .if eax == 1
                jmp fim
            .endif
            
        .elseif rot == 2
            invoke testarRotacao, pTetromino, pMapa, -1, 0
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetromino, pMapa, 2, 0
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetromino, pMapa, -1, -2
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetromino, pMapa, 2, 1
            .if eax == 1
                jmp fim
            .endif
            
        .elseif rot == 3
            invoke testarRotacao, pTetromino, pMapa, 2, 0
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetromino, pMapa, -1, 0
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetromino, pMapa, 2, -1
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetromino, pMapa, -1, 2
            .if eax == 1
                jmp fim
            .endif
            
        .endif

    .else
        .if rot == 0
            invoke testarRotacao, pTetromino, pMapa, -1, 0
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetromino, pMapa, -1, 1
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetromino, pMapa, 0, -2
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetromino, pMapa, -1, -2
            .if eax == 1
                jmp fim
            .endif

        .elseif rot == 1
            invoke testarRotacao, pTetromino, pMapa, -1, 0
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetromino, pMapa, -1, -1
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetromino, pMapa, 0, 2
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetromino, pMapa, -1, 2
            .if eax == 1
                jmp fim
            .endif
            
        .elseif rot == 2
            invoke testarRotacao, pTetromino, pMapa, 1, 0
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetromino, pMapa, 1, 1
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetromino, pMapa, 0, -2
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetromino, pMapa, 1, -2
            .if eax == 1
                jmp fim
            .endif
            
        .elseif rot == 3
            invoke testarRotacao, pTetromino, pMapa, 1, 0
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetromino, pMapa, 1, -1
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetromino, pMapa, 0, 2
            .if eax == 1
                jmp fim
            .endif

            invoke testarRotacao, pTetromino, pMapa, 1, 2
            .if eax == 1
                jmp fim
            .endif
            
        .endif
    .endif
    xor eax, eax
    fim:
    ret
testesRotacao endp

testarRotacao proc pTetromino:DWORD, pMapa:DWORD, x:BYTE, y:BYTE
    LOCAL copiaT:DWORD
    LOCAL result:DWORD
    invoke copiarTetromino, pTetromino
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
    add (TETROMINO ptr[edi]).posicao, eax

    invoke adicionarMatrizLogica, edi, esi
    mov edi, copiaT
    invoke verificarColisao, (TETROMINO ptr[edi]).mat

    mov result, eax

    .if result == 0
        mov edi, copiaT
        mov ebx, (TETROMINO ptr[edi]).posicao
        mov edi, pTetromino
        mov (TETROMINO ptr[edi]).posicao, ebx
    .endif

    invoke destruirTetromino, copiaT

    .if result == 0
        mov result, 1
    .elseif result == 1
        mov result, 0
    .endif

    mov eax, result
    ret
testarRotacao endp

atualizarFantasma proc pFant:DWORD, pTetro:DWORD, pMapa:DWORD
    invoke atribuirTetromino, pFant, pTetro
    mov esi, pFant
    mov cl, GHOST
    mov (TETROMINO ptr[esi]).tipo, cl

    invoke descerTudo, pFant, pMapa

    ret
atualizarFantasma endp

descerTudo proc pTetro:DWORD, pMapa:DWORD
    LOCAL copiaT:DWORD
    invoke GlobalAlloc, 0, sizeof TETROMINO
    mov copiaT, eax
    invoke montarTetromino, copiaT, NADA
    invoke atribuirTetromino, copiaT, pTetro

    xor eax, eax
    
    forDescer:
        cmp eax, 1
        je fim

        mov edi, pTetro
        mov esi, copiaT

        mov ecx, (TETROMINO ptr[esi]).posicao
        mov (TETROMINO ptr[edi]).posicao, ecx

        mov edi, (TETROMINO ptr[edi]).mat
        mov esi, (TETROMINO ptr[esi]).mat

        invoke atribuirMatriz, esi, edi
        mov esi, copiaT

        add (TETROMINO ptr[esi]).posicao, 16
        invoke adicionarMatrizLogica, esi, pMapa

        mov esi, copiaT

        mov edi, (TETROMINO ptr[esi]).mat
        invoke verificarColisao, edi

        jmp forDescer

    fim:
    invoke destruirTetromino, copiaT
    ret
descerTudo endp