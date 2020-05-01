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
            pintar PP_ROTACIONAR
        .elseif wParam == 39
            pintar PP_MOVER_DIREITA
        .elseif wParam == 37
            pintar PP_MOVER_ESQUERDA
        .endif

    .elseif uMsg == WM_PAINT
        invoke BeginPaint, hWin, ADDR ps
        mov hdc, eax

        ;copiar bloco

        mov al, paintParam
        .if al == PP_DESENHAR
            invoke desenharTela, hdc

        .elseif al == PP_DESCER
            invoke desenharTetrimino, hWin, hdc, OFFSET bloco, NADA
            add bloco.posicao, 10

        .elseif al == PP_ROTACIONAR
            invoke desenharTetrimino, hWin, hdc, OFFSET bloco, NADA
            invoke rotacionarMatriz, bloco.mat

        .elseif al == PP_MOVER_DIREITA
            invoke desenharTetrimino, hWin, hdc, OFFSET bloco, NADA
            inc bloco.posicao

        .elseif al == PP_MOVER_ESQUERDA
            invoke desenharTetrimino, hWin, hdc, OFFSET bloco, NADA
            dec bloco.posicao

        .endif

        ;verificar se colidiu
        ;se sim, pegar a copia, colocar no atual e excluir a copia
        ;se nao, excluir a copia

        invoke desenharTetrimino, hWin, hdc, OFFSET bloco, bloco.tipo

        mov al, PP_DESENHAR
        mov paintParam, al

        invoke EndPaint, hWin, ADDR ps


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