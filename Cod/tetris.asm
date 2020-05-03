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
    LOCAL copiaTetrimino:DWORD
    LOCAL copiaMatriz:DWORD
    LOCAL colidiu:BYTE

    invoke GetProcessHeap
    mov hHeap, eax
    .if uMsg == WM_CREATE
        mov mapa.ponteiro, OFFSET vetMapa
        mov mapa.altura, 26
        mov mapa.largura, 16

        invoke montarTetrimino, OFFSET bloco, LARANJA

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
        cmp desenhandoTetrimino, 1
        je fim

        mov desenhandoTetrimino, 1
        invoke BeginPaint, hWin, ADDR ps
        mov hdc, eax

        invoke copiarTetrimino, OFFSET bloco
        mov copiaTetrimino, eax
        
        invoke colocarMatrizLogica, OFFSET bloco, OFFSET mapa, 0

        mov al, paintParam
        .if al == PP_DESENHAR
            invoke desenharTela, hdc

        .elseif al == PP_DESCER
            invoke desenharTetrimino, hWin, hdc, OFFSET bloco, NADA
            add bloco.posicao, 16
            mov desenhandoTetrimino, 0

        .elseif al == PP_ROTACIONAR
            invoke desenharTetrimino, hWin, hdc, OFFSET bloco, NADA
            invoke rotacionarMatriz, bloco.mat
            add bloco.rotacao, 1
            .if bloco.rotacao == 4
                mov bloco.rotacao, 0
            .endif

        .elseif al == PP_MOVER_DIREITA
            invoke desenharTetrimino, hWin, hdc, OFFSET bloco, NADA
            inc bloco.posicao

        .elseif al == PP_MOVER_ESQUERDA
            invoke desenharTetrimino, hWin, hdc, OFFSET bloco, NADA
            dec bloco.posicao

        .elseif al == PP_REFAZER
            invoke refazerTetrimino, OFFSET bloco, CIANO

        .endif

        invoke copiarMatriz, bloco.mat
        mov copiaMatriz, eax

        invoke adicionarMatrizLogica, OFFSET bloco, OFFSET mapa
        invoke verificarColisao, bloco.mat

        mov colidiu, al

        invoke atribuirMatriz, bloco.mat, copiaMatriz
        invoke destruirMatriz, copiaMatriz, 1

        mov al, paintParam
        .if colidiu == 1
            .if al == PP_ROTACIONAR
                invoke testesRotacao, OFFSET bloco, OFFSET mapa
                .if eax == 0
                    invoke atribuirTetrimino, OFFSET bloco, copiaTetrimino
                .endif
            .else
                invoke atribuirTetrimino, OFFSET bloco, copiaTetrimino
            .endif
        .endif

        invoke destruirTetrimino, copiaTetrimino
        invoke colocarMatrizLogica, OFFSET bloco, OFFSET mapa, 1
        invoke desenharTetrimino, hWin, hdc, OFFSET bloco, bloco.tipo

        mov al, paintParam
        .if colidiu == 1
            .if al == PP_DESCER
                .if bloco.posicao < 10
                    call perder
                .else
                    mov velocidade, 500
                    invoke refazerTetrimino, OFFSET bloco, AZUL
                    invoke desenharTetrimino, hWin, hdc, OFFSET bloco, bloco.tipo
                .endif
            .endif
        .endif

        mov al, PP_DESENHAR
        mov paintParam, al

        invoke EndPaint, hWin, ADDR ps

        mov desenhandoTetrimino, 0

        fim:

    .elseif uMsg == WM_DESCER

    .elseif uMsg == WM_DESTROY
        invoke PostQuitMessage,NULL
        invoke destruirTetrimino, OFFSET bloco
        return 0


    .endif

    invoke DefWindowProc,hWin,uMsg,wParam,lParam

    ret

WndProc endp

esperarDesenho proc
    comparar:
    cmp desenhandoTetrimino, 1
    je comparar
    ret
esperarDesenho endp

perder proc
    string str, "Perdeu!\nDeseja jogar novamente?"
perder endp

include matriz.inc
include tetrimino.inc
include gui.inc

end start