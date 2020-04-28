criarMatriz proc
    invoke HeapAlloc, hHeap, 8, sizeof matriz
    (matriz ptr[eax]).ponteiro = 0
    (matriz ptr[eax]).altura = 0
    (matriz ptr[eax]).largura = 0
    ret
criarMatriz endp

strMatriz proc hWin:DWORD, pMat:DWORD
    mov edx, pMat
    xor ecx, ecx
    xor eax, eax
    mov al, (matriz ptr[edx]).altura
    mov cl, (matriz ptr[edx]).largura
    inc cl
    mul ecx
    mov ecx, 2
    mul ecx

    inc eax
    
    invoke HeapAlloc, hHeap, 8, eax

    push eax
    mov edx, pMat
    mov esi, (matriz ptr[edx]).ponteiro

    xor cx,cx
    xor ebx, ebx
    forLinha:
        cmp cl, (matriz ptr[edx]).altura ; linha
        je fim
        forColuna:
            cmp ch, (matriz ptr[edx]).largura ; coluna
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
            ret
strMatriz endp

rotacionarMatriz proc pMat:DWORD
    mov edx, pMat
    xor ecx, ecx
    xor eax, eax
    xor ebx, ebx
    mov al, (matriz ptr[edx]).altura
    mov cl, (matriz ptr[edx]).largura
    mul ecx
    
    invoke HeapAlloc, hHeap, 8, eax
    mov edx, pMat
    mov esi, eax
    mov eax, (matriz ptr[edx]).ponteiro
    xor ecx,ecx

    forLinha:
        cmp cl, (matriz ptr[edx]).altura ; linha
        je fimFors
        forColuna:
            cmp ch, (matriz ptr[edx]).largura ; coluna
            je fimForL
            mov bl, byte ptr[eax]   

            push eax
            push ecx

            xor eax, eax
            mov al, ch
            xor ch, ch
            mov cl, (matriz ptr[edx]).altura
            mul ecx

            pop ecx

            mov edx, pMat
            add al, (matriz ptr[edx]).altura
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
        cmp (matriz ptr[edx]).ponteiro, 0
        je fimRotacionarMatriz

        push esi
        invoke HeapFree, hHeap, NULL, (matriz ptr[edx]).ponteiro
        pop esi
    fimRotacionarMatriz:
        mov edx, pMat
        mov (matriz ptr[edx]).ponteiro, esi
        ret
rotacionarMatriz endp


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