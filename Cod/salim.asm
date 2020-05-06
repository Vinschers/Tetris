      .486                                    ; create 32 bit code
      .model flat, stdcall                    ; 32 bit memory model
      option casemap :none                    ; case sensitive
 
      include \masm32\include\windows.inc
      include \masm32\macros\macros.asm

      include \masm32\include\masm32.inc
      include \masm32\include\user32.inc
      include \masm32\include\kernel32.inc
      include \masm32\include\gdi32.inc
      include \masm32\include\msimg32.inc
      include \masm32\include\dnsapi.inc

      includelib \masm32\lib\masm32.lib
      includelib \masm32\lib\user32.lib
      includelib \masm32\lib\kernel32.lib
      includelib \masm32\lib\gdi32.lib
      includelib \masm32\lib\msimg32.lib
      includelib \masm32\lib\dnsapi.lib

      verificarTipoNoVetor PROTO :DWORD, :BYTE
    
.data?

.data

    vetor db 8,8,8,8,8,8,8     ; vetor com 7 posições
    texto  db 0,0               ; variável usada para printar na tela

.code
start: 

    call printar
    exit

    printar proc

        call randomizarVetor
        xor ecx, ecx
        forzinLindao:
            mov ebx, OFFSET vetor
            mov dl, byte ptr [ebx + ecx]
            mov texto, dl
            add texto, 48
            push ecx
            print OFFSET texto
            pop ecx

            inc ecx
            cmp ecx, 7
            jne forzinLindao

        ret

    printar endp
    

    randomizarVetor proc 
        mov esi, OFFSET vetor 
        xor ecx, ecx    ; contador do for menor
        for1:

            cmp ecx, 7
            je final

            push esi 
            push ecx
            call randomizar
            pop ecx
            pop esi
            

            push esi
            push ecx
            push eax
            invoke verificarTipoNoVetor, esi, al
            xor edx, edx
            mov dl, al
            pop eax
            pop ecx
            pop esi
            
            cmp edx, 1
            je for1

            mov byte ptr [esi + ecx], al
            inc ecx
            jmp for1
            
        final:
            ret

    randomizarVetor endp 


    verificarTipoNoVetor proc vet:DWORD, tipo:BYTE
        mov ebx, vet
        xor ecx, ecx
        xor eax, eax
        for1:
            cmp cl, 7
            je final

            mov ch, tipo
            cmp byte ptr[ebx], ch
            jne nExiste

            mov eax, 1
            jmp final

            nExiste:
                inc ecx
                inc ebx
                jmp for1

        final:    
            ret

    verificarTipoNoVetor endp 


    randomizar proc
       
        invoke Dns_GetRandomXid, 0          ; Argument not used -> AX = random WORD
        mov ecx, 7
        xor edx, edx
        div ecx
        mov eax, edx
        ret
       
    randomizar endp


end start



