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

    .elseif uMsg == WM_KEYUP
        .if wParam == VK_UP
            mov al, PP_ROTACIONAR
        .elseif wParam == 39
            mov al, PP_MOVER_DIREITA
        .elseif wParam == 37
            mov al, PP_MOVER_ESQUERDA
        .endif
        mov paintParam, al
        invoke InvalidateRect,hWnd, NULL, FALSE

    .elseif uMsg == WM_PAINT
       
        mov desenhandoTetrimino, 1
        invoke BeginPaint, hWin, ADDR ps
        mov hdc, eax
        
        cmp telaDesenhada, 1
        je cont

        invoke desenharTela, hdc
        mov telaDesenhada, 1

        cont:
        xor eax,eax
        mov eax, bloco.posicao
        cmp eax, 200
        jl desenhar

        invoke refazerTetrimino, OFFSET bloco, LARANJA

        desenhar:
            mov al, paintParam
            .if al == PP_DESENHAR
                invoke desenharTetrimino, hWin, hdc, OFFSET bloco, bloco.tipo

            .elseif al == PP_DESCER
                invoke desenharTetrimino, hWin, hdc, OFFSET bloco, NADA
                add bloco.posicao, 10
                invoke desenharTetrimino, hWin, hdc, OFFSET bloco, bloco.tipo

            .elseif al == PP_ROTACIONAR
                invoke desenharTetrimino, hWin, hdc, OFFSET bloco, NADA
                invoke rotacionarMatriz, bloco.mat
                invoke desenharTetrimino, hWin, hdc, OFFSET bloco, bloco.tipo

            .elseif al == PP_MOVER_DIREITA
                invoke desenharTetrimino, hWin, hdc, OFFSET bloco, NADA
                inc bloco.posicao
                invoke desenharTetrimino, hWin, hdc, OFFSET bloco, bloco.tipo

            .elseif al == PP_MOVER_ESQUERDA
                invoke desenharTetrimino, hWin, hdc, OFFSET bloco, NADA
                dec bloco.posicao
                invoke desenharTetrimino, hWin, hdc, OFFSET bloco, bloco.tipo
            .endif
            invoke EndPaint, hWin, ADDR ps
            mov desenhandoTetrimino, 0


    .elseif uMsg == WM_DESCER

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