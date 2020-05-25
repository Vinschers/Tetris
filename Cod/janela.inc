invoke GetModuleHandle, NULL ; provides the instance handle
mov hInstance, eax

invoke LoadBitmap,hInstance, bitmap
mov hBmp, eax

invoke GetCommandLine        ; provides the command line address
mov CommandLine, eax

invoke WinMain,hInstance,NULL,CommandLine,SW_SHOWDEFAULT

invoke ExitProcess,eax       ; cleanup & return to operating system

WinMain proc hInst     :DWORD,
             hPrevInst :DWORD,
             CmdLine   :DWORD,
             CmdShow   :DWORD

    ;====================
    ; Put LOCALs on stack
    ;====================

    LOCAL wc   :WNDCLASSEX
    LOCAL msg  :MSG

    LOCAL Wwd  :DWORD
    LOCAL Wht  :DWORD
    LOCAL Wtx  :DWORD
    LOCAL Wty  :DWORD

    szText szClassName,"Basica_Class"

    ;==================================================
    ; Fill WNDCLASSEX structure with required variables
    ;==================================================

    mov wc.cbSize,         sizeof WNDCLASSEX
    mov wc.style,          CS_HREDRAW or CS_VREDRAW \
                           or CS_BYTEALIGNWINDOW
    mov wc.lpfnWndProc,    offset WndProc      ; address of WndProc
    mov wc.cbClsExtra,     NULL
    mov wc.cbWndExtra,     NULL
    m2m wc.hInstance,      hInst               ; instance handle
    mov wc.hbrBackground,  COLOR_BTNFACE+1     ; system color
    mov wc.lpszMenuName,   NULL
    mov wc.lpszClassName,  offset szClassName  ; window class name
    invoke LoadIcon,hInst,500                  ; icon ID   ; resource icon
    mov wc.hIcon,          eax
    invoke LoadCursor,NULL,IDC_ARROW           ; system cursor
    mov wc.hCursor,        eax
    mov wc.hIconSm,        0

    invoke RegisterClassEx, ADDR wc     ; register the window class

    ;================================
    ; Centre window at following size
    ;================================

    mov Wwd, 630
    mov Wht, 700

    invoke GetSystemMetrics,SM_CXSCREEN ; get screen width in pixels
    invoke TopXY,Wwd,eax
    mov Wtx, eax

    invoke GetSystemMetrics,SM_CYSCREEN ; get screen height in pixels
    invoke TopXY,Wht,eax
    mov Wty, eax

    ; ==================================
    ; Create the main application window
    ; ==================================
    invoke CreateWindowEx,WS_EX_OVERLAPPEDWINDOW,
                          ADDR szClassName,
                          ADDR szDisplayName,
                          WS_OVERLAPPEDWINDOW,
                          Wtx,Wty,Wwd,Wht,
                          NULL,NULL,
                          hInst,NULL
    
    mov   hWnd,eax  ; copy return value into handle DWORD

    invoke ShowWindow,hWnd,SW_SHOWNORMAL      ; display the window
    invoke UpdateWindow,hWnd                  ; update the display

    ;===================================
    ; Loop until PostQuitMessage is sent
    ;===================================

StartLoop:
    invoke GetMessage,ADDR msg,NULL,0,0         ; get each message
    cmp eax, 0                                  ; exit if GetMessage()
    je ExitLoop                                 ; returns zero
    invoke TranslateMessage, ADDR msg           ; translate it
    invoke DispatchMessage,  ADDR msg           ; send it to message proc
    jmp StartLoop

ExitLoop:

    return msg.wParam

WinMain endp

TopXY proc wDim:DWORD, sDim:DWORD
    shr sDim, 1      ; divide screen dimension by 2
    shr wDim, 1      ; divide window dimension by 2
    mov eax, wDim    ; copy window dimension into eax
    sub sDim, eax    ; sub half win dimension from half screen dimension

    return sDim

TopXY endp


; SALIM SUA PARTE

randomizarVetor proc vet:DWORD
    mov esi, vet    ; atribuímos o vetor ao registrador esi
    xor ecx, ecx    ; limpamos o registrador ecx 

    ; agora completamos todas as posições do vetor com o número 8
    completarVetor:
        mov byte ptr[esi+ecx], 8
        inc ecx

        cmp ecx, 7
        jne completarVetor

    xor ecx, ecx    ; usaremos ecx como contador do for abaixo

    ; neste for colocaremos valores não aleatórios em cada posição do vetor
    for1:
        cmp ecx, 7  ; caso ecx, o contador, seja igual a 7,
        je final    ; vamos para o final do método

        ; chamamos o método que retorna um número aleatório no intervalo de 0 a 6
        push esi    
        push ecx
        call randomizar
        pop ecx
        pop esi

        ; verificamos se o valor gerado já existe no vetor
        push esi
        push ecx
        push eax
        invoke verificarTipoNoVetor, esi, al
        xor edx, edx
        mov dl, al  ; salvamos o valor de el em dl para que não seja perdido
        pop eax
        pop ecx
        pop esi
        
        cmp edx, 1  ; caso edx seja igual a 1, significa que o tipo já existe no vetor
        je for1     ; então retornamos ao começo do for1

        mov byte ptr [esi + ecx], al    ; atribuimos o valor aleatório à posição atual do vetor
        inc ecx     ; incrementamos o contador ecx 
        jmp for1    ; voltamos ao começo do for1
        
    final:
        ret 

randomizarVetor endp 

; procedure que verifica que o tipo passado como parâmetro já existe no vetor
verificarTipoNoVetor proc vet:DWORD, tipo:BYTE
; vet é o vetor com os tipos de peças e tipo é o que queremos verificar

    ; atribuímos o vetor ao registrador ebx
    mov ebx, vet

    ; limpamos os registradores a serem usados
    xor ecx, ecx
    xor eax, eax

    ; loop para verificação da existência do tipo
    for1:
        cmp cl, 7   ; caso cl, o contador, seja igual a 7,
        je final    ; vamos para o final do método

        mov ch, tipo        
        cmp byte ptr[ebx], ch   ; caso o tipo da posição atual do vetor seja igual ao passado como parâmetro
        jne nExiste             ; quer dizer que não achamos ainda

        mov eax, 1   ; atribuimos 1 a eax, porque o tipo já existe no vetor
        jmp final    ; e vamos para o final do método

        nExiste:
            inc ecx     ; incrementamos o contador
            inc ebx     ; avançamos uma posição no vetor
            jmp for1    ; retornamos ao começo do for1

    final:    
        ret

verificarTipoNoVetor endp 


; procedure que gera um número inteiro aleatório dentro de um intervalo de 0 a 6
randomizar proc     
    
    invoke Dns_GetRandomXid, 0          
    mov ecx, 7
    xor edx, edx
    div ecx
    mov eax, edx
    ret
    
randomizar endp