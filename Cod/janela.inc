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


;ax -> coluna
;resto -> linha
getPixel proc pos:DWORD
    xor edx, edx
    mov eax, pos
    mov ecx, 10
    div ecx
    shl eax, 5
    shl edx, 5
    add eax, 11
    add edx, 11
    shl eax, 16
    mov ax, dx
    ret
getPixel endp