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