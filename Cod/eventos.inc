;//////////////////////////////////////////////////////////////////////////////////
;feito por:
;18188 Guilherme Salim de Barros
;18178 Felipe Scherer Vicentin
;18343 Lorenna Joanna Leal Nunes 
;18190 Maria Eduarda Elias Rocha
;para o prof Sergio na matéria 'linguagem de montagem'
;todos direitos reservados -- 2020  ©
;//////////////////////////////////////////////////////////////////////////////////
;manipular mensagens do sistema operacional identificadas na estrutura de Message

; função para manipular as mensagens do sistema operacional identificadas na estrutura de Message
WndProc proc hWin   :DWORD,
             uMsg   :DWORD,
             wParam :DWORD,
             lParam :DWORD

    LOCAL hdc:HDC
    LOCAL ps:PAINTSTRUCT

    invoke GetProcessHeap ; obtém um identificador para o heap padrão para o processo de chamada
    mov hHeap, eax
    .if uMsg == WM_CREATE
        invoke MessageBox, hWin, addr EXPLICACAO_TEXTO, addr EXPLICACAO_TITULO, MB_OK ; logo no inicio do jogo uma caixa com informações sobre as teclas irá aparecer
        
        ; cria o primeiro tetromino e o sua sombra
        invoke montarTetromino, OFFSET bloco, CIANO
        invoke montarTetromino, OFFSET fantasma, GHOST

        invoke CreateEvent,NULL,FALSE,FALSE,NULL ; cria e programa o novo evento
        mov    hEventStart,eax
        mov ecx, OFFSET ThreadProcDescer
        invoke CreateThread, NULL, NULL, ecx, ADDR mapa, NORMAL_PRIORITY_CLASS, ADDR ThreadDescer
        mov hThread, eax
        
        invoke comecarJogo, hWin ; jogo se inicia

    .elseif uMsg == WM_COMMAND
        mov eax, wParam
        .if ax == 100 ; verifica se 
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

    .elseif uMsg == WM_PAINT 
        cmp desenhandoTetromino, 1 ; verifica se o tetromino está desenhado
        je fim  
        cmp perdeu, 1 ; verifica se o jogo acabou
        je fim

        mov desenhandoTetromino, 1 ; está desenhando o tetromino 
        invoke BeginPaint, hWin, ADDR ps ; começa a função paint
        mov hdc, eax ; guarda o retorno da função BeginPaint
        invoke paint, hWin, hdc, OFFSET bloco, OFFSET mapa, OFFSET fantasma, OFFSET vetorPecas, OFFSET pecaAtual; desenha o tetromino
        
        ;atualiza a tela com o tetromino que foi desenhado
        mov al, PP_DESENHAR  
        mov paintParam, al
       
        invoke EndPaint, hWin, ADDR ps ; termina a função paint
        mov desenhandoTetromino, 0 ; já desenhou o tetromino

        fim:

    .elseif uMsg == WM_DESCER

    .elseif uMsg == WM_DESTROY
        ;deleta a janela
        invoke PostQuitMessage,NULL
        invoke destruirTetromino, OFFSET bloco
        invoke destruirMatriz, OFFSET mapa, 0
        return 0
    .endif

    invoke DefWindowProc,hWin,uMsg,wParam,lParam ; garante que todas as mensagens sejam processadas

    ret

WndProc endp ; fin da função WndProc

; função para inciar o jogo
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
comecarJogo endp ; fim da função comecarJogo