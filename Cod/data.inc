bitmap equ 111
CREF_TRANSPARENT  EQU 0FF00FFh

WinMain                     PROTO   :DWORD, :DWORD, :DWORD, :DWORD
WndProc                     PROTO   :DWORD, :DWORD, :DWORD, :DWORD
TopXY                       PROTO   :DWORD, :DWORD
desenharBloco               PROTO   :DWORD, :DWORD, :BYTE,  :WORD
montarTetromino             PROTO   :DWORD, :BYTE
desenharTetromino           PROTO   :DWORD, :DWORD, :DWORD, :BYTE
strMatriz                   PROTO   :DWORD, :DWORD
refazerTetromino            PROTO   :DWORD, :DWORD, :BYTE
destruirTetromino           PROTO   :DWORD
rotacionarMatriz            PROTO   :DWORD
desenharTela                PROTO   :DWORD
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
paint                       PROTO   :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :BYTE
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
verificarTipoNoVetor        PROTO   :DWORD, :BYTE



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




.data   ; area de dados já inicializados.
    szDisplayName   db "Tetris", 0 
    proxPecaTxt     db "Proxima Peca", 0
    pecaSeguradaTxt db "Peca Segurada", 0
    pontuacaoTxt    db "Pontuacao", 0
    perdeuTxt       db "Perdeu!", 10, "Deseja jogar novamente?",0
    CommandLine     dd 0  ; parametros passados pela linha de comando (ponteiro)
    hWnd            dd 0  ; Handle principal do programa no windows
    hInstance       dd 0  ; instancia do programa
    hHeap           dd 0
    hbmp            dd 0 ; handler 
    paintParam      db 0
    perdeu          db 0
    pecaGuardada    db 7 
    jaGuardou       db 0 ; variavel de controle 

    telaDesenhada   db 0
    desenhandoTetromino  db 0

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

    ThreadDescer  dd 0
	ExitCode 	  dd 0
	hThread 	  dd 0
	hEventStart   dd 0
    hBmp          dd 0
    velocidade    dd 500
    vetorPecas    db 8,8,8,8,8,8,8
    pecaAtual     db 0

.const
    WM_DESCER equ WM_USER+100h
    AZUL     db 0
    LARANJA  db 1
    AMARELO  db 2
    VERDE    db 3
    ROXO     db 4
    VERMELHO db 5
    CIANO    db 6
    NADA     db 7
    GHOST    db 8
    PP_DESENHAR       db 0
    PP_DESCER         db 1
    PP_ROTACIONAR     db 2
    PP_MOVER_DIREITA  db 3
    PP_MOVER_ESQUERDA db 4
    PP_REFAZER        db 5
    PP_CAIR_TUDO      db 6