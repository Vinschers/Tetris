;//////////////////////////////////////////////////////////////////////////////////
;feito por:
;18188 Guilherme Salim de Barros
;18178 Felipe Scherer Vicentin
;18343 Lorenna Joanna Leal Nunes 
;18190 Maria Eduarda Elias Rocha
;para o prof Sergio na matéria 'linguagem de montagem'
;todos direitos reservados -- 2020  ©
;//////////////////////////////////////////////////////////////////////////////////

bitmap equ 111
CREF_TRANSPARENT  EQU 0FF00FFh

WinMain                     PROTO   :DWORD, :DWORD, :DWORD, :DWORD
WndProc                     PROTO   :DWORD, :DWORD, :DWORD, :DWORD
TopXY                       PROTO   :DWORD, :DWORD
desenharBloco               PROTO   :DWORD, :DWORD, :BYTE,  :WORD
montarTetromino             PROTO   :DWORD, :BYTE
desenharTetromino           PROTO   :DWORD, :DWORD, :DWORD, :BYTE
strMatriz                   PROTO   :DWORD, :DWORD
refazerTetromino            PROTO   :DWORD, :BYTE
refazerTetrominoTipo        PROTO   :DWORD, :BYTE
refazerTetrominoVetor       PROTO   :DWORD, :DWORD, :BYTE
destruirTetromino           PROTO   :DWORD
rotacionarMatriz            PROTO   :DWORD
desenharTela                PROTO   :DWORD, :DWORD
tetrominoPaint              PROTO   :DWORD, :DWORD, :DWORD
copiarPonteiroMatriz        PROTO   :DWORD, :DWORD
getPixel                    PROTO   :WORD
apagarTetromino             PROTO   :DWORD, :DWORD, :DWORD
copiarTetromino             PROTO   :DWORD
adicionarMatrizLogica       PROTO   :DWORD, :DWORD
colocarMatrizLogica         PROTO   :DWORD, :DWORD, :BYTE
verificarColisao            PROTO   :DWORD
atribuirTetromino           PROTO   :DWORD, :DWORD
copiarMatriz                PROTO   :DWORD
atribuirMatriz              PROTO   :DWORD, :DWORD
destruirMatriz              PROTO   :DWORD, :BYTE
testesRotacao               PROTO   :DWORD, :DWORD
testarRotacao               PROTO   :DWORD, :DWORD, :BYTE, :BYTE
paint                       PROTO   :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
atualizarFantasma           PROTO   :DWORD, :DWORD, :DWORD
descerTudo                  PROTO   :DWORD, :DWORD
trabalharComLinhas          PROTO   :DWORD, :DWORD, :DWORD
acharLinhas                 PROTO   :DWORD, :DWORD
descerMatriz                PROTO   :DWORD, :DWORD, :DWORD, :WORD
limparLinha                 PROTO   :DWORD, :DWORD, :DWORD, :WORD
perder                      PROTO   :DWORD, :DWORD
comecarJogo                 PROTO   :DWORD
desenharBlocoGuardado       PROTO   :DWORD, :DWORD, :BYTE,  :WORD
desenharTetrominoGuardado   PROTO   :DWORD, :DWORD, :DWORD, :BYTE
getPixelGuardado            PROTO   :WORD
desenharBlocoProx           PROTO   :DWORD, :DWORD, :BYTE,  :WORD
desenharTetrominoProx       PROTO   :DWORD, :DWORD, :BYTE
getPixelProx                PROTO   :WORD
verificarTipoNoVetor        PROTO   :DWORD, :BYTE
randomizarVetor             PROTO   :DWORD
printNum                    PROTO   :DWORD, :DWORD, :DWORD, :DWORD
printDig                    PROTO   :DWORD, :DWORD
getExp                      PROTO   :DWORD



MATRIZ struct
    ponteiro    DWORD   ?
    altura      BYTE    ?
    largura     BYTE    ?
MATRIZ ends

TETROMINO struct
    tipo    BYTE    ?
    posicao DWORD   ?
    rotacao BYTE    ?
    mat     DWORD   ?
TETROMINO ends


.data?
    hitpoint POINT <>
    rect     RECT  <>
    rectParaApagar RECT <>
    posx  dd ?
    posy  dd ?
    hButton HWND ?	

    ;variaveis para produzir os efeitos sonoros
    open_dwCallback     dd ?
    open_wDeviceID     dd ?
    open_lpstrDeviceType  dd ?
    open_lpstrElementName  dd ?
    open_lpstrAlias     dd ?
    generic_dwCallback   dd ?
    play_dwCallback     dd ?
    play_dwFrom       dd ?
    play_dwTo        dd ?




.data   ; area de dados já inicializados.
    szDisplayName   db "Tetris", 0 
    proxPecaTxt     db "Proxima Peca", 0
    pecaSeguradaTxt db "Peca Segurada", 0
    pontuacaoTxt    db "Pontuacao: ", 0
    nivelTxt        db "Nivel: ", 0
    perdeuTxt       db "Perdeu!", 10, "Deseja jogar novamente?",0
    temaTxt         db "Mudar o tema? O jogo vai reiniciar",0

    CommandLine         dd 0  ; parametros passados pela linha de comando (ponteiro)
    hWnd                dd 0  ; Handle principal do programa no windows
    hInstance           dd 0  ; instancia do programa
    hHeap               dd 0
    hbmp                dd 0 ; handler 
    paintParam          db 0
    perdeu              db 0
    pecaGuardada        db 7 
    jaGuardou           db 0 ; variavel de controle 
    nivel               db 0,0 ;
    linhas              db 0 ;
    linhasAux           db 0
    pontuacao           dd 0,0 ;
    posAtual            dd 500
    telaDesenhada       db 0
    desenhandoTetromino db 0


    ;variaveis para debbugar o programa no desenvolvimento
    txt             dd 97,0
    txtb            db 0,0
    txtw            word 0,0

    
    mapa            MATRIZ <>
    ;todos 9 menos o ultimo (16)
    pecas               db  1,0,0,1,1,1,0,0,0 ;azul
                        db  0,0,1,1,1,1,0,0,0 ;laranja
                        db  1,1,0,1,1,0,0,0,0 ;amarelo
                        db  0,1,1,1,1,0,0,0,0 ;verde
                        db  0,1,0,1,1,1,0,0,0 ;roxo
                        db  1,1,0,0,1,1,0,0,0 ;vermelho
                        db  0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0 ;ciano

    vetMapa         db  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                    db  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                    db  0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0
                    db  0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0
                    db  0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0
                    db  0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0
                    db  0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0
                    db  0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0
                    db  0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0
                    db  0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0
                    db  0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0
                    db  0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0
                    db  0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0
                    db  0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0
                    db  0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0
                    db  0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0
                    db  0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0
                    db  0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0
                    db  0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0
                    db  0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0
                    db  0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0
                    db  0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0
                    db  0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0
                    db  0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0
                    db  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                    db  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                    
    bloco           TETROMINO <>
    fantasma        TETROMINO <>

    ThreadDescer        dd 0
	ExitCode 	        dd 0
	hThread 	        dd 0
	hEventStart         dd 0
    hBmp                dd 0
    velocidade          dd 500
    vetorPecas          db 0,1,2,3,4,5,6
    pecaAtual           db 0
    formatacaoNivel     db "Nivel: %d", 0
    formatacaoPontuacao db "Pontuacao: %d", 0
    buffer              db 20 dup(0)
    tema                db 0
    FontName            db "script",0

.const
    WM_DESCER         equ WM_USER+100h
    ;constantes que definem a cor das peças
    AZUL              db 0
    LARANJA           db 1
    AMARELO           db 2
    VERDE             db 3
    ROXO              db 4
    VERMELHO          db 5
    CIANO             db 6
    NADA              db 7
    GHOST             db 8

    ;constantes para saber qual parte do paint deverá ser feito
    PP_DESENHAR       db 0
    PP_DESCER         db 1
    PP_ROTACIONAR     db 2
    PP_MOVER_DIREITA  db 3
    PP_MOVER_ESQUERDA db 4
    PP_REFAZER        db 5
    PP_CAIR_TUDO      db 6
    PP_MUDAR_TEMA     db 7

    BUTTON_ID         equ 100
    EXPLICACAO_TITULO db "CONTROLES",0
    EXPLICACAO_TEXTO  db "-->      Mover para a direita",10
                      db "<--      Mover para a esquerda",10
                      db "v        Aumentar velocidade de descida",10
                      db "^        Rotacionar tetromino",10
                      db "[Espaco] Descer tetromino",10
                      db "[H]      Segurar tetromino",10
                      db "[T]      Mudar o tema do jogo",0
    

    ;caminhos dos efeitos sonoros e música do jogo
    CAMINHO_MUSICA    db "tetris_theme.wav", 0
    CAMINHO_LINHA     db "clear.wav", 0
    CAMINHO_GAME      db "gameover.wav", 0