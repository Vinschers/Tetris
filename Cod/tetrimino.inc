montarTetrimino proc pTetrimino:DWORD, tipo:BYTE
    call criarMatriz
    mov edx, pTetrimino
    mov (TETRIMINO ptr[edx]).mat, eax
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