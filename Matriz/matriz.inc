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

printarMatriz proc p:DWORD
    mov eax, p
    xor cx,cx
    xor ebx, ebx
    forLinha:
        cmp cl, 4 ; linha
        je fim
        forColuna:
            cmp ch, 4 ; coluna
            je fimForL
            mov bl, byte ptr[eax]
            push eax
            push ebx
            push ecx
            print str$(ebx)
            print " "
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