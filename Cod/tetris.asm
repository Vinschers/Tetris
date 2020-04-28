include cabecalho.inc

; quaquer procedimento que for escrito 
; deve ter o seu prototipo descrito aqui.
; com em C 

WinMain PROTO :DWORD,:DWORD,:DWORD,:DWORD
WndProc PROTO :DWORD,:DWORD,:DWORD,:DWORD
TopXY PROTO   :DWORD,:DWORD


matriz struct
    ponteiro    DWORD   ?
    altura      BYTE    ?
    largura     BYTE    ?
matriz ends


.data?
    hitpoint POINT <>
    posx  dd ?
    posy  dd ?
    


.data   ; area de dados já inicializados.
    szDisplayName   db "Tetris",0 
    CommandLine     dd 0  ; parametros passados pela linha de comando (ponteiro)
    hWnd            dd 0  ; Handle principal do programa no windows
    hInstance       dd 0  ; instancia do programa
    hHeap           dd 0

    MouseClick      db 0 ; 0 = no click yet
    txt             dd 100,0
    mat             matriz <>
    vet             db 0,1,0,1,1,1,0,0,0

.code   ; parte do codigo

start:  ; o programa  deve ser escrito entre start e end start
        ; inclusive procedimentos que vão ser escritos e utilizados.
    
    invoke GetModuleHandle, NULL ; provides the instance handle
    mov hInstance, eax

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

    mov Wwd, 500
    mov Wht, 350

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

; #########################################################################

WndProc proc hWin   :DWORD,
             uMsg   :DWORD,
             wParam :DWORD,
             lParam :DWORD

    LOCAL hdc:HDC
    LOCAL ps:PAINTSTRUCT

    invoke GetProcessHeap
    mov hHeap, eax
; -------------------------------------------------------------------------
; Message are sent by the operating system to an application through the
; WndProc proc. Each message can have additional values associated with it
; in the two parameters, wParam & lParam. The range of additional data that
; can be passed to an application is determined by the message.
; -------------------------------------------------------------------------

    .if uMsg == WM_CREATE
        mov mat.ponteiro, OFFSET vet
        mov mat.altura, 3
        mov mat.largura, 3
    ; --------------------------------------------------------------------
    ; This message is sent to WndProc during the CreateWindowEx function
    ; call and is processed before it returns. This is used as a position
    ; to start other items such as controls. IMPORTANT, the handle for the
    ; CreateWindowEx call in the WinMain does not yet exist so the HANDLE
    ; passed to the WndProc [ hWin ] must be used here for any controls
    ; or child windows.
    ; --------------------------------------------------------------------

    ;; lparam da mensagem traz as posições x e y do mouse
    .elseif uMsg == WM_LBUTTONDOWN

        mov     eax, lParam
        and     eax, 0FFFFh
        mov     hitpoint.x, eax
        mov     posx, eax
        mov     eax, lParam
        shr     eax, 16  ; desloca o registrador eax de 16 bits para a direita ->
        mov     hitpoint.y, eax
        mov     posy, eax
        mov     MouseClick, TRUE
        invoke  InvalidateRect, hWin, NULL, TRUE ; 


    .elseif uMsg == WM_PAINT
        invoke BeginPaint, hWin, ADDR ps
        mov    hdc, eax
        .if MouseClick
            invoke  lstrlen, ADDR szDisplayName
            invoke  TextOut, hdc, posx, posy, ADDR szDisplayName, eax
        .endif
        
        szText MSG3,"Assembler, Pure & Simple"
        invoke  lstrlen, ADDR MSG3
        invoke  TextOut, hdc, 40, 40, ADDR MSG3, eax
        
        push OFFSET mat
        mov ecx, hWin
        push ecx
        call strMatriz

        showmsg eax

        push OFFSET mat
        call rotacionarMatriz

        push OFFSET mat
        mov ecx, hWin
        push ecx
        call strMatriz

        showmsg eax

        invoke EndPaint, hWin, ADDR ps
    .elseif uMsg == WM_DESTROY
    ; ----------------------------------------------------------------
    ; This message MUST be processed to cleanly exit the application.
    ; Calling the PostQuitMessage() function makes the GetMessage()
    ; function in the WinMain() main loop return ZERO which exits the
    ; application correctly. If this message is not processed properly
    ; the window disappears but the code is left in memory.
    ; ----------------------------------------------------------------
        invoke PostQuitMessage,NULL
        return 0 
    .endif

    invoke DefWindowProc,hWin,uMsg,wParam,lParam
    ; --------------------------------------------------------------------
    ; Default window processing is done by the operating system for any
    ; message that is not processed by the application in the WndProc
    ; procedure. If the application requires other than default processing
    ; it executes the code when the message is trapped and returns ZERO
    ; to exit the WndProc procedure before the default window processing
    ; occurs with the call to DefWindowProc().
    ; --------------------------------------------------------------------
    ret

WndProc endp

; ########################################################################

TopXY proc wDim:DWORD, sDim:DWORD

    ; ----------------------------------------------------
    ; This procedure calculates the top X & Y co-ordinates
    ; for the CreateWindowEx call in the WinMain procedure
    ; ----------------------------------------------------

    shr sDim, 1      ; divide screen dimension by 2
    shr wDim, 1      ; divide window dimension by 2
    mov eax, wDim    ; copy window dimension into eax
    sub sDim, eax    ; sub half win dimension from half screen dimension

    return sDim

TopXY endp

; ########################################################################

include matriz.inc

end start