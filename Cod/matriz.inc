;//////////////////////////////////////////////////////////////////////////////////
;feito por:
;18188 Guilherme Salim de Barros
;18178 Felipe Scherer Vicentin
;18343 Lorenna Joanna Leal Nunes 
;18190 Maria Eduarda Elias Rocha
;para o prof Sergio na matéria 'linguagem de montagem'
;todos direitos reservados -- 2020  ©
;//////////////////////////////////////////////////////////////////////////////////

criarMatriz proc
    invoke GlobalAlloc, 0, sizeof MATRIZ ;aloca a matriz na memóra
    mov (MATRIZ ptr[eax]).ponteiro, 0 ;zera todos os atributos da struct
    mov (MATRIZ ptr[eax]).altura, 0
    mov (MATRIZ ptr[eax]).largura, 0
    ret
criarMatriz endp

;converte a matriz numa string de caracteres dinamicamente alocada
;procedimento usado apenas para debug
strMatriz proc hWin:DWORD, pMat:DWORD
    mov edx, pMat
    xor ecx, ecx
    xor eax, eax
    mov al, (MATRIZ ptr[edx]).altura
    mov cl, (MATRIZ ptr[edx]).largura
    inc cl
    mul ecx
    mov ecx, 2
    mul ecx

    inc eax
    
    invoke HeapAlloc, hHeap, 8, eax

    push eax
    mov edx, pMat
    mov esi, (MATRIZ ptr[edx]).ponteiro

    xor cx,cx
    xor ebx, ebx
    forLinha:
        cmp cl, (MATRIZ ptr[edx]).altura ; linha
        je fim
        forColuna:
            cmp ch, (MATRIZ ptr[edx]).largura ; coluna
            je fimForL
            mov bl, byte ptr[esi]
            add bl, 48

            mov byte ptr[eax], bl
            inc eax
            mov byte ptr[eax], 32
            inc eax

            inc ch
            inc esi
            jmp forColuna
        fimForL:
            mov byte ptr[eax], 13
            inc eax
            mov byte ptr[eax], 10
            inc eax
            inc cl
            xor ch,ch

            jmp forLinha
        fim:
            mov byte ptr[eax], 0
            pop eax
            showmsg eax
            invoke HeapFree, hHeap, NULL, eax
            ret
strMatriz endp

;rotaciona a matriz em 90° para a direita
rotacionarMatriz proc USES edx pMat:DWORD
    mov edx, pMat
    xor ecx, ecx
    xor eax, eax
    xor ebx, ebx
    mov al, (MATRIZ ptr[edx]).altura
    mov cl, (MATRIZ ptr[edx]).largura
    mul ecx
    
    invoke HeapAlloc, hHeap, 8, eax ;um vetor de tamanho igual à matriz a ser rotacionada é alocado
    mov edx, pMat
    mov esi, eax
    mov eax, (MATRIZ ptr[edx]).ponteiro
    xor ecx,ecx

    ;nesse for, a lógica de rotação de matriz é aplicada e a matriz rotacionada é a que foi dinamicamente alocada
    forLinha:
        cmp cl, (MATRIZ ptr[edx]).altura ; linha
        je fimFors
        forColuna:
            cmp ch, (MATRIZ ptr[edx]).largura ; coluna
            je fimForL
            mov bl, byte ptr[eax]   

            push eax
            push ecx

            xor eax, eax
            mov al, ch
            xor ch, ch
            mov cl, (MATRIZ ptr[edx]).altura
            mul ecx

            pop ecx

            mov edx, pMat
            add al, (MATRIZ ptr[edx]).altura
            dec al
            sub al, cl

            mov byte ptr[esi + eax], bl
            pop eax

            inc ch
            inc eax
            jmp forColuna

        fimForL:
            inc cl
            xor ch,ch
            jmp forLinha
    fimFors:
        mov edx, pMat
        cmp (MATRIZ ptr[edx]).ponteiro, 0
        je fimRotacionarMatriz

        push esi
        invoke HeapFree, hHeap, NULL, (MATRIZ ptr[edx]).ponteiro ;o ponteiro antigo é liberado e substituido pelo novo vetor alocado, que tem a matriz rotacionada
        pop esi
    fimRotacionarMatriz:
        mov edx, pMat
        mov (MATRIZ ptr[edx]).ponteiro, esi
        ret
rotacionarMatriz endp

;copia o vetor da matriz src para dest
copiarPonteiroMatriz proc src:DWORD, dest:DWORD
    mov ecx, dest

    ;tamanho do vetor é calculado com base nas dimensões
    xor eax, eax
    xor ebx, ebx
    mov al, (MATRIZ ptr[ecx]).largura
    mov bl, (MATRIZ ptr[ecx]).altura
    mul ebx

    mov ecx, dest
    mov edx, src

    push eax

    ;se o destino não tiver uma matriz, uma nova matriz é alocada
    xor ebx, ebx
    mov ecx, (MATRIZ ptr[ecx]).ponteiro

    cmp ecx, 0
    je alocar

    ;se dest já for uma matriz, ela é liberada e depois alocada de novo

    push eax
    invoke GlobalFree, ecx
    pop eax

    alocar:
    invoke GlobalAlloc, 0, eax

    mov ecx, dest
    mov (MATRIZ ptr[ecx]).ponteiro, eax
    mov ecx, eax
    mov edx, src

    pop eax

    ;o vetor de src é percorrido e copiado para dest
    forM:
    cmp ebx, eax
    je fim

    push eax

    mov al, byte ptr[edx]
    mov byte ptr[ecx], al

    pop eax

    inc ebx
    inc ecx
    inc edx

    jmp forM

    fim:
    ret
copiarPonteiroMatriz endp

;aloca nova matriz que é copia de src
copiarMatriz proc src:DWORD
    call criarMatriz
    push eax
    invoke atribuirMatriz, eax, src
    pop eax
    ret
copiarMatriz endp

;copia a matriz src para dest
atribuirMatriz proc dest:DWORD, src:DWORD
    mov esi, dest
    mov edi, src

    ;copia as dimensões
    mov cl, (MATRIZ ptr[edi]).largura
    mov (MATRIZ ptr[esi]).largura, cl
    mov cl, (MATRIZ ptr[edi]).altura
    mov (MATRIZ ptr[esi]).altura, cl
    mov ebx, (MATRIZ ptr[edi]).ponteiro
    ;copia o vetor
    invoke copiarPonteiroMatriz, ebx, dest

    ret
atribuirMatriz endp

;dealoca a matriz da memória e libera os ponteiros do vetor
destruirMatriz proc pMat:DWORD, excluirSiMesma:BYTE
    mov eax, pMat
    mov edx, (MATRIZ ptr[eax]).ponteiro
    invoke GlobalFree, edx
    cmp excluirSiMesma, 1
    jne fim

    mov eax, pMat
    invoke GlobalFree, eax

    fim:
    ret
destruirMatriz endp