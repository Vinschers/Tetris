printar MACRO var
    push eax
    push ebx
    push ecx
    push edx
    print OFFSET var
    pop edx
    pop ecx
    pop ebx
    pop eax
ENDM

return MACRO arg
    mov eax, arg
    ret
ENDM

showmsg MACRO msg
    push eax
    push ebx
    push ecx
    push edx
    push esi
    invoke MessageBox,hWin,msg, NULL, MB_OK
    pop esi
    pop edx
    pop ecx
    pop ebx
    pop eax
ENDM

pintar MACRO param
    call esperarDesenho
    push eax
    mov al, param
    mov paintParam, al
    pop eax
    invoke InvalidateRect, hWnd, NULL, FALSE
ENDM