WndProc proc hWin   :DWORD,
             uMsg   :DWORD,
             wParam :DWORD,
             lParam :DWORD

    LOCAL hdc:HDC
    LOCAL ps:PAINTSTRUCT

    invoke GetProcessHeap
    mov hHeap, eax
    .if uMsg == WM_CREATE
        mov mapa.ponteiro, OFFSET vetMapa
        mov mapa.altura, 26
        mov mapa.largura, 16

        invoke montarTetromino, OFFSET bloco, LARANJA

        invoke CreateEvent,NULL,FALSE,FALSE,NULL
        mov    hEventStart,eax

        mov ecx, OFFSET ThreadProcDescer
        invoke CreateThread, NULL, NULL, ecx, ADDR mapa, NORMAL_PRIORITY_CLASS, ADDR ThreadDescer
        mov hThread, eax
    
    .elseif uMsg == WM_KEYDOWN
        .if wParam == 40
            mov velocidade, 50
        .elseif wParam == 32
            mov velocidade, 1
        .endif

    .elseif uMsg == WM_KEYUP
        .if wParam == VK_UP
            pintar PP_ROTACIONAR
        .elseif wParam == 39
            pintar PP_MOVER_DIREITA
        .elseif wParam == 37
            pintar PP_MOVER_ESQUERDA
        .elseif wParam == 40
            mov velocidade, 500
        .elseif wParam == 72
            pintar PP_REFAZER
        .endif

    .elseif uMsg == WM_PAINT
        cmp desenhandoTetromino, 1
        je fim

        mov desenhandoTetromino, 1
        invoke BeginPaint, hWin, ADDR ps
        mov hdc, eax

        invoke paint, hWin, hdc, OFFSET bloco, OFFSET mapa

        mov al, PP_DESENHAR
        mov paintParam, al

        invoke EndPaint, hWin, ADDR ps
        mov desenhandoTetromino, 0

        fim:

    .elseif uMsg == WM_DESCER

    .elseif uMsg == WM_DESTROY
        invoke PostQuitMessage,NULL
        invoke destruirTetromino, OFFSET bloco
        return 0


    .endif

    invoke DefWindowProc,hWin,uMsg,wParam,lParam

    ret

WndProc endp