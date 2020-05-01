string MACRO Name, Text:VARARG
    LOCAL lbl
    jmp lbl
    Name db Text,0
    lbl:
ENDM

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

atrc MACRO M1, M2
    push eax
    mov al, M2
    mov M1, al
    pop eax
ENDM

atrs MACRO M1, M2
    push eax
    mov ax, M2
    mov M1, ax
    pop eax
ENDM

atr32 MACRO M1, M2
    push M2
    pop M1
ENDM

return MACRO arg
    mov eax, arg
    ret
ENDM


char MACRO Nome, Valor:VARARG
    LOCAL lbl
    jmp lbl
    Nome byte Valor
    lbl:
ENDM

sInt MACRO Nome, Valor:VARARG
    LOCAL lbl
    jmp lbl
    Nome word Valor
    lbl:
ENDM

int32 MACRO Nome, Valor:VARARG
    LOCAL lbl
    jmp lbl
    Nome dword Valor
    lbl:
ENDM

float MACRO Nome, Valor:VARARG
    LOCAL lbl
    jmp lbl
    Nome Real4 Valor
    lbl:
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
    push eax
    mov al, param
    mov paintParam, al
    pop eax
    invoke InvalidateRect, hWnd, NULL, FALSE
ENDM