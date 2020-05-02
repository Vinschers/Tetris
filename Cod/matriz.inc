criarMatriz proc
    invoke GlobalAlloc, 0, sizeof MATRIZ
    mov (MATRIZ ptr[eax]).ponteiro, 0
    mov (MATRIZ ptr[eax]).altura, 0
    mov (MATRIZ ptr[eax]).largura, 0
    ret
criarMatriz endp

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

rotacionarMatriz proc USES edx pMat:DWORD
    mov edx, pMat
    xor ecx, ecx
    xor eax, eax
    xor ebx, ebx
    mov al, (MATRIZ ptr[edx]).altura
    mov cl, (MATRIZ ptr[edx]).largura
    mul ecx
    
    invoke HeapAlloc, hHeap, 8, eax
    mov edx, pMat
    mov esi, eax
    mov eax, (MATRIZ ptr[edx]).ponteiro
    xor ecx,ecx

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
        invoke HeapFree, hHeap, NULL, (MATRIZ ptr[edx]).ponteiro
        pop esi
    fimRotacionarMatriz:
        mov edx, pMat
        mov (MATRIZ ptr[edx]).ponteiro, esi
        ret
rotacionarMatriz endp

copiarPonteiroMatriz proc src:DWORD, dest:DWORD
    mov ecx, dest

    xor eax, eax
    xor ebx, ebx
    mov al, (MATRIZ ptr[ecx]).largura
    mov bl, (MATRIZ ptr[ecx]).altura
    mul ebx

    mov ecx, dest
    mov edx, src

    push eax

    xor ebx, ebx
    mov ecx, (MATRIZ ptr[ecx]).ponteiro

    cmp ecx, 0
    je alocar

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

copiarMatriz proc src:DWORD
    call criarMatriz
    push eax
    invoke atribuirMatriz, eax, src
    pop eax
    ret
copiarMatriz endp

atribuirMatriz proc dest:DWORD, src:DWORD
    LOCAL teste:DWORD
    mov esi, dest
    mov edi, src

    mov cl, (MATRIZ ptr[edi]).largura
    mov (MATRIZ ptr[esi]).largura, cl
    mov cl, (MATRIZ ptr[edi]).altura
    mov (MATRIZ ptr[esi]).altura, cl
    mov ebx, (MATRIZ ptr[edi]).ponteiro
    invoke copiarPonteiroMatriz, ebx, dest

    ret
atribuirMatriz endp

destruirMatriz proc pMat:DWORD, excluirSiMesma:BYTE
    mov eax, pMat
    mov edx, (MATRIZ ptr[eax]).ponteiro
    ;invoke GlobalFree, edx
    ;cmp excluirSiMesma, 1
    ;jne fim

    ;mov eax, pMat
    ;invoke GlobalFree, eax

    fim:
    ret
destruirMatriz endp


COMMENT @
rodarDireitaPeca proc p:DWORD
    LOCAL matAux[16]:BYTE
    mov eax, p
    lea esi, matAux
    xor ecx,ecx
    forLinha:
        cmp cl, 4 ; linha
        je fim
        forColuna:
            cmp ch, 4 ; coluna
            je fimForL
            mov bl, byte ptr[eax]
            xor edx, edx
            mov dl, ch
            add dl, ch
            add dl, ch
            add dl, ch
            add dl, 3
            sub dl, cl
            mov byte ptr[esi + edx], bl
            cont:
                inc ch
                inc eax
                jmp forColuna
        fimForL:
            inc cl
            xor ch,ch
            jmp forLinha
        fim:
            lea eax, matAux
            mov ebx, p
            xor ecx,ecx
            forCopia:
                cmp cl, 16
                je retorno
                xor edx, edx
                mov dl, byte ptr[eax + ecx]
                mov byte ptr[ebx + ecx], dl
                inc cl
                jmp forCopia
            retorno:
                ret
rodarDireitaPeca endp

printarMatriz proc hWin:DWORD, p:DWORD
    mov eax, p
    xor cx,cx
    xor ebx, ebx
    forLinha:
        cmp cl, 3 ; linha
        je fim
        forColuna:
            cmp ch, 3 ; coluna
            je fimForL
            mov bl, byte ptr[eax]
            push eax
            push ebx
            push ecx
            showmsg ebx
            pop ecx
            pop ebx
            pop eax
            cont:
                inc ch
                inc eax
                jmp forColuna
        fimForL:
            push eax
            push ebx
            push ecx
            print chr$(13,10)
            pop ecx
            pop ebx
            pop eax
            inc cl
            xor ch,ch

            jmp forLinha
        fim:
            ret
printarMatriz endp


teste proc hWin:DWORD, pMat:DWORD
    string limpo, "Heap freed"
    mov edx, pMat
    cmp (matriz ptr[edx]).ponteiro, 0
    je cont

    invoke HeapFree, hHeap, NULL, (matriz ptr[edx]).ponteiro
    showmsg ADDR limpo


    cont:
    mov edx, pMat
    xor ecx, ecx
    xor eax, eax
    mov al, (matriz ptr[edx]).altura
    mov cl, (matriz ptr[edx]).largura
    mul ecx
    
    invoke HeapAlloc, hHeap, 8, eax
    
    mov edx, pMat
    mov (matriz ptr[edx]).ponteiro, eax

    mov byte ptr[eax], 'a'
    inc eax
    mov byte ptr[eax], 0

    ret
teste endp

@