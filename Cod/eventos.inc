WndProc proc hWin   :DWORD,
             uMsg   :DWORD,
             wParam :DWORD,
             lParam :DWORD

    LOCAL hdc:HDC
    LOCAL ps:PAINTSTRUCT

    invoke GetProcessHeap
    mov hHeap, eax
    .if uMsg == WM_CREATE
        invoke MessageBox, hWin, addr EXPLICACAO_TEXTO, addr EXPLICACAO_TITULO, MB_OK ; logo no inicio do jogo uma caixa com informações sobre as teclas irá aparecer
        
        invoke montarTetromino, OFFSET bloco, CIANO
        invoke montarTetromino, OFFSET fantasma, GHOST

        invoke CreateEvent,NULL,FALSE,FALSE,NULL
        mov    hEventStart,eax
        mov ecx, OFFSET ThreadProcDescer
        invoke CreateThread, NULL, NULL, ecx, ADDR mapa, NORMAL_PRIORITY_CLASS, ADDR ThreadDescer
        mov hThread, eax
        
        invoke comecarJogo, hWin        ;aqui o jogo se inicia

    .elseif uMsg == WM_COMMAND
        mov eax, wParam
        .if ax == 100
            showmsg ADDR txt
        .endif
    
    .elseif uMsg == WM_KEYDOWN
        .if wParam == 40        ; se a tecla apretada for seta para baixo, enquanto está estiver apertada a velocidade da queda muda
            mov velocidade, 50
        .elseif wParam == 32    ; caso seja o espaço a peça cairá completamente
            pintar PP_CAIR_TUDO
        .endif

    .elseif uMsg == WM_KEYUP
        .if wParam == VK_UP     ; seta para cima significa girar a peça no sentido horário
            pintar PP_ROTACIONAR
        .elseif wParam == 39    ; a peça anda para a direita quando a seta da direita é apertada
            pintar PP_MOVER_DIREITA
        .elseif wParam == 37    ; a peça anda para a esquerda quando a seta da esquerda é apertada
            pintar PP_MOVER_ESQUERDA
        .elseif wParam == 40    ; caso a seta para baixo seja solta a velocidade volta ao normal
            mov velocidade, 500
        .elseif wParam == 72    ; caso o H seja pressionado a peça será trocada pela prox ou se já tiver outra guardada será trocada por essa
            pintar PP_REFAZER
        .elseif wParam == 84    ; caso o T seja pressionado o tema do tetris será alterado
            pintar PP_MUDAR_TEMA
        .endif

    .elseif uMsg == WM_PAINT ;  LOLO COMENTE
        cmp desenhandoTetromino, 1
        je fim
        cmp perdeu, 1
        je fim

        mov desenhandoTetromino, 1
        invoke BeginPaint, hWin, ADDR ps
        mov hdc, eax

        invoke paint, hWin, hdc, OFFSET bloco, OFFSET mapa, OFFSET fantasma, OFFSET vetorPecas, OFFSET pecaAtual

        mov al, PP_DESENHAR
        mov paintParam, al

        invoke EndPaint, hWin, ADDR ps
        mov desenhandoTetromino, 0

        fim:

    .elseif uMsg == WM_DESCER

    .elseif uMsg == WM_DESTROY
        invoke PostQuitMessage,NULL
        invoke destruirTetromino, OFFSET bloco
        invoke destruirMatriz, OFFSET mapa, 0
        return 0
    .endif

    invoke DefWindowProc,hWin,uMsg,wParam,lParam

    ret

WndProc endp

comecarJogo proc hWin:DWORD
    invoke PlaySound, OFFSET CAMINHO_MUSICA,NULL,SND_ASYNC or SND_LOOP ; a música de fundo começa a tocar

    mov mapa.altura, 26     ; defininmos a altura do mapa
    mov mapa.largura, 16    ; definimos a largura do mapa
    invoke copiarPonteiroMatriz, OFFSET vetMapa, OFFSET mapa
    invoke randomizarVetor, OFFSET vetorPecas   ; randomizamos o vetor
    mov pecaAtual, 0    ; definimos a peça atual como a do primeiro tipo no vetor já randomizado
    invoke refazerTetrominoVetor, OFFSET bloco, OFFSET vetorPecas, pecaAtual
    mov desenhandoTetromino, 0
    mov perdeu, 0
    ret
comecarJogo endp