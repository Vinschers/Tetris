montarTetrimino proc pTetrimino:DWORD, tipo:BYTE
    call criarMatriz
    mov edx, pTetrimino
    mov (TETRIMINO ptr[edx]).mat, eax
    mov (TETRIMINO ptr[edx]).posicao, 183

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
    invoke GlobalFree, (MATRIZ ptr[(TETRIMINO ptr[eax]).mat]).ponteiro
    mov eax, pTetrimino
    invoke GlobalFree, (TETRIMINO ptr[eax]).mat
    mov eax, pTetrimino
    invoke GlobalFree, eax
    ret
destruirTetrimino endp

refazerTetrimino proc pTetrimino:DWORD, tipo:BYTE
    mov edx, pTetrimino
    mov eax, (TETRIMINO ptr[edx]).mat
    mov (TETRIMINO ptr[edx]).posicao, 3

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
    mov cl, (TETRIMINO ptr[ponteiro]).tipo

    mov ptrNovo, eax
    mov edx, eax
    invoke montarTetrimino, edx, cl

    mov eax, ptrNovo
    mov edx, ponteiro

    mov ebx, (TETRIMINO ptr[edx]).posicao
    mov (TETRIMINO ptr[eax]).posicao, ebx

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

    push esi
    push edi

    mov eax, edx
    xor edx, edx
    mov ecx, 10
    div ecx
    add eax, 3
    shl eax, 4
    add eax, edx
    add eax, 3

    mov edx, eax
    xor eax, eax
    xor ecx, ecx

    pop edi
    pop esi

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
        add edx, 13
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

    push esi
    push edi

    mov eax, edx
    xor edx, edx
    mov ecx, 10
    div ecx
    add eax, 3
    shl eax, 4
    add eax, edx
    add eax, 3
    mov edx, eax
    xor eax, eax
    xor ecx, ecx

    pop edi
    pop esi

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

            add eax, ebx

            mov cl, byte ptr[edi + eax]
            cmp cl, 1
            jne continuar

            mov cl, valor
            mov byte ptr[esi + edx], cl

            continuar:
            pop ecx

            xor dh, dh

            inc dl
            inc ch
            jmp forColuna
        fimForColuna:
        xor ch, ch
        add dl, 13
        inc cl
        jmp forLinha
    fim:
    ret
colocarMatrizLogica endp

verificarColisao proc mat:DWORD
    xor eax, eax
    xor ecx, ecx

    mov esi, mat
    mov al, (MATRIZ ptr[esi]).altura
    mov cl, (MATRIZ ptr[esi]).largura
    mul ecx
    mov ecx, eax
    mov esi, (MATRIZ ptr[esi]).ponteiro

    xor ebx, ebx
    xor eax, eax

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
    mov cl, (TETRIMINO ptr[src]).tipo

    mov eax, dest
    mov edx, src

    mov ebx, (TETRIMINO ptr[edx]).posicao
    mov (TETRIMINO ptr[eax]).posicao, ebx

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