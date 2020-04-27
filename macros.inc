
; definição de MACROS a serem utilizadas no programa para facilitar a vida

; 1. szText
; A macro to insert TEXT into the code section for convenient and 
; more intuitive coding of functions that use byte data as text.

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

; 2. m2m
; There is no mnemonic to copy from one memory location to another,
; this macro saves repeated coding of this process and is easier to
; read in complex code.

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

; 3. return
; Every procedure MUST have a "ret" to return the instruction
; pointer EIP back to the next instruction after the call that
; branched to it. This macro puts a return value in eax and
; makes the "ret" instruction on one line. It is mainly used
; for clear coding in complex conditionals in large branching
; code such as the WndProc procedure.

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
        string titulo,"Debug"
        invoke MessageBox,hWin,msg,ADDR titulo,MB_OK
    ENDM