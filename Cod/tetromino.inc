;//////////////////////////////////////////////////////////////////////////////////
;feito por:
;18188 Guilherme Salim de Barros
;18178 Felipe Scherer Vicentin
;18343 Lorenna Joanna Leal Nunes 
;18190 Maria Eduarda Elias Rocha
;para o prof Sergio na matéria 'linguagem de montagem'
;todos direitos reservados -- 2020  ©
;//////////////////////////////////////////////////////////////////////////////////

;Inicia o tetromino com valores padrão
montarTetromino proc pTetromino:DWORD, tipo:BYTE    ;ponteiro para o tetromino e o tipo dele
    call criarMatriz                                ;eax tem o ponteiro para a matriz vazia
    mov edx, pTetromino
    mov (TETROMINO ptr[edx]).mat, eax               ;a matriz do tetromino é a matriz criada
    mov (TETROMINO ptr[edx]).posicao, 54            ;posição referente ao 0,0 visual na matriz lógica
    mov (TETROMINO ptr[edx]).rotacao, 0

    cmp tipo, 6
    jne pecaNormal                                  ;as peças 0 a 5 têm matrizes 3x3 enquanto a peça 6 tem a matriz 4x4

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

    mov (TETROMINO ptr[edx]).tipo, al               ;tipo do tetromino é atribuido

    mov cl, 9
    mul cl
    mov ecx, OFFSET pecas
    add ecx, eax

    mov eax, pTetromino

    mov edx, (TETROMINO ptr[eax]).mat

    invoke copiarPonteiroMatriz, ecx, edx           ;copia-se o vetor do modelo para a matriz do tetromino, para evitar mudanças no modelo geral

    ret
montarTetromino endp

;Destroi o ponteiro de um tetromino
destruirTetromino proc pTetromino:DWORD
    mov eax, pTetromino
    invoke destruirMatriz, (TETROMINO ptr[eax]).mat, 1          ;destroi a matriz do tetromino
    mov eax, pTetromino
    invoke GlobalFree, eax                                      ;destroi a si próprio
    ret
destruirTetromino endp

;refaz o tetromino baseado em um tipo
refazerTetrominoTipo proc pTetromino:DWORD, tipo:BYTE

    invoke refazerTetromino, pTetromino, tipo
    ret

refazerTetrominoTipo endp

;refaz o tetromino baseado no vetor de tipos e a posição atual
refazerTetrominoVetor proc pTetromino:DWORD, vetor:DWORD, pos:BYTE

    mov eax, vetor
    xor edx, edx
    mov dl, pos
    mov al, byte ptr[eax + edx]

    invoke refazerTetromino, pTetromino, al
    ret

refazerTetrominoVetor endp 

;Reinicia um tetromino já iniciado com outro tipo
refazerTetromino proc pTetromino:DWORD, tipo:BYTE
    ; é quase igual ao montarTetromino, mas a matriz não é recriada
    
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

;retorna um ponteiro para um tetromino identico ao passado
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

    mov edx, ponteiro
    mov edx, (TETROMINO ptr[edx]).mat
    mov edx, (MATRIZ ptr[edx]).ponteiro

    mov edi, ptrNovo
    mov edi, (TETROMINO ptr[edi]).mat

    invoke copiarPonteiroMatriz, edx, edi

    return ptrNovo

copiarTetromino  endp

;adiciona os valores da matriz lógica à matriz do tetromino na posição respectiva
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

    forLinha: ;percorre a matriz do tetromino
        cmp cl, tetroAltura
        je fim

        forColuna:
            cmp ch, tetroLargura
            je fimForColuna

            push ecx
            push edx

            ;calcula onde a posição atual da matriz do tetromino estaria na matriz lógica do mapa
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

            mov cl, byte ptr[esi + edx] ;cl tem o valor da posição atual da matriz do tetromino
            
            add byte ptr[edi + eax], cl ;cl é adicionado na posição respectiva na matriz lógica

            ;por exemplo, se na posição atual da matriz do tetromino há o número 1 e na posição atual da matriz lógica há o número 1,
            ;a matriz lógica terá na sua respectiva posição atual o valor 1+1, ou seja, 2

            pop ecx

            inc edx
            inc ch
            jmp forColuna
        fimForColuna:
        xor ch, ch
        xor eax, eax
        mov al, mapaLargura
        add edx, eax
        xor eax, eax
        mov al, tetroLargura
        sub edx, eax
        inc cl
        jmp forLinha
    fim:
    ret
adicionarMatrizLogica endp

;coloca o valor passado na matriz lógica na posição do tetromino
colocarMatrizLogica proc pTetromino:DWORD, pMapa:DWORD, valor:BYTE
    ;é basicamente igual ao procedimento adicionarMatrizLogica, mas ao invés de adicionar os valores, os valores da matriz do tetromino são atribuidos a suas respectivas
    ;posições na matriz lógica
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

;verifica se há algum 2 na matriz do tetromino. Se sim, houve colisão
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

;copia o tetromino src para o tetromino dest. Ambos devem estar montados
atribuirTetromino proc dest:DWORD, src:DWORD
    mov eax, dest
    mov edx, src

    mov ebx, (TETROMINO ptr[edx]).posicao
    mov (TETROMINO ptr[eax]).posicao, ebx
    mov cl, (TETROMINO ptr[edx]).tipo
    mov (TETROMINO ptr[eax]).tipo, cl
    mov cl, (TETROMINO ptr[edx]).rotacao
    mov (TETROMINO ptr[eax]).rotacao, cl

    mov edi, (TETROMINO ptr[edx]).mat
    mov esi, (TETROMINO ptr[eax]).mat

    invoke atribuirMatriz, esi, edi
    ret
atribuirTetromino endp

;seguindo a lógica oficial do tetris, presente em https://tetris.fandom.com/wiki/SRS, quando uma rotação de tetromino resulta em colisão
;os seguintes testes são feitos. Se algum deles resultar em uma rotação possível, a rotação é executada. Se não, ela é barrada
;Esses testes consistem em mudanças na posição do tetromino no eixo X e Y
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

;testa a rotação para uma posição X e Y, verificando colisão
testarRotacao proc pTetromino:DWORD, pMapa:DWORD, x:BYTE, y:BYTE
    LOCAL copiaT:DWORD
    LOCAL result:DWORD
    invoke copiarTetromino, pTetromino
    mov copiaT, eax

    mov esi, pMapa

    ;mudanças no eixo x e y são aplicadas em copiaT para verificar colisao
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

    .if result == 0 ;se não houve colisão, tetromino recebe copiaT
        mov edi, copiaT
        mov ebx, (TETROMINO ptr[edi]).posicao
        mov edi, pTetromino
        mov (TETROMINO ptr[edi]).posicao, ebx
    .endif

    invoke destruirTetromino, copiaT

    .if result == 0 ;se não houve colisão, o resultado do procedimento é positivo
        mov result, 1
    .elseif result == 1
        mov result, 0
    .endif

    mov eax, result
    ret
testarRotacao endp

;atualiza a posição da peça fantasma
atualizarFantasma proc pFant:DWORD, pTetro:DWORD, pMapa:DWORD
    invoke atribuirTetromino, pFant, pTetro
    mov esi, pFant
    mov cl, GHOST
    add cl, tema
    mov (TETROMINO ptr[esi]).tipo, cl

    invoke descerTudo, pFant, pMapa

    ret
atualizarFantasma endp

;atualiza a posição do tetromino e o coloca na posição mais baixa possível
descerTudo proc pTetro:DWORD, pMapa:DWORD
    LOCAL copiaT:DWORD
    invoke GlobalAlloc, 0, sizeof TETROMINO
    mov copiaT, eax
    invoke montarTetromino, copiaT, NADA
    invoke atribuirTetromino, copiaT, pTetro

    xor eax, eax
    
    forDescer: ;desce o tetromino uma posição até colidir. copiaT é uma copia do tetromino atual e estará sempre uma posição abaixo do tetromino original.
               ;se copiaT colidir com algo, significa que abaixo do tetromino há um obstáculo e, portanto, ele deve parar de descer
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