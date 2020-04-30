include cabecalho.inc
include data.inc

.code

start:
    
include janela.inc

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

        invoke montarTetrimino, OFFSET bloco, ROXO

        invoke CreateEvent,NULL,FALSE,FALSE,NULL
        mov    hEventStart,eax

        mov ecx, OFFSET ThreadProcDescer
        invoke CreateThread, NULL, NULL, ecx, ADDR mapa, NORMAL_PRIORITY_CLASS, ADDR ThreadDescer
        mov hThread, eax


    .elseif uMsg == WM_PAINT
        mov desenhandoTetrimino, 1
        invoke BeginPaint, hWin, ADDR ps
        mov hdc, eax
        
        cmp telaDesenhada, 1
        je cont

        invoke desenharTela, hdc
        mov telaDesenhada, 1

        cont:
        mov al, bloco.posicao
        cmp al, 200
        jb desenhar

        invoke refazerTetrimino, OFFSET bloco, LARANJA

        desenhar:
        invoke tetriminoPaint, hWin, hdc, OFFSET bloco
        
        invoke EndPaint, hWin, ADDR ps
        mov desenhandoTetrimino, 0


    .elseif uMsg == WM_DESCER
        invoke InvalidateRect,hWnd, NULL, FALSE

    .elseif uMsg == WM_DESTROY
        invoke PostQuitMessage,NULL
        invoke destruirTetrimino, OFFSET bloco
        return 0

    .endif

    invoke DefWindowProc,hWin,uMsg,wParam,lParam

    ret

WndProc endp

include matriz.inc
include tetrimino.inc
include gui.inc

end start