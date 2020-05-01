bitmap equ 111
CREF_TRANSPARENT  EQU 0FF00FFh

WinMain                     PROTO   :DWORD, :DWORD, :DWORD, :DWORD
WndProc                     PROTO   :DWORD, :DWORD, :DWORD, :DWORD
TopXY                       PROTO   :DWORD, :DWORD
desenharBloco               PROTO   :DWORD, :DWORD, :BYTE,  :BYTE
montarTetrimino             PROTO   :DWORD, :BYTE
desenharTetrimino           PROTO   :DWORD, :DWORD, :DWORD, :BYTE
strMatriz                   PROTO   :DWORD, :DWORD
refazerTetrimino            PROTO   :DWORD, :BYTE
destruirTetrimino           PROTO   :DWORD
rotacionarMatriz            PROTO   :DWORD
desenharTela                PROTO   :DWORD
tetriminoPaint              PROTO   :DWORD, :DWORD, :DWORD
copiarPonteiroMatriz        PROTO   :DWORD, :DWORD
getPixel                    PROTO   :BYTE
apagarTetrimino             PROTO   :DWORD, :DWORD, :DWORD


MATRIZ struct
    ponteiro    DWORD   ?
    altura      BYTE    ?
    largura     BYTE    ?
MATRIZ ends

TETRIMINO struct
    tipo    BYTE    ?
    posicao DWORD    ?
    mat     DWORD   ?
TETRIMINO ends


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
    CommandLine     dd 0  ; parametros passados pela linha de comando (ponteiro)
    hWnd            dd 0  ; Handle principal do programa no windows
    hInstance       dd 0  ; instancia do programa
    hHeap           dd 0
    hbmp            dd 0 ; handler 
    paintParam      db 0

    telaDesenhada   db 0
    desenhandoTetrimino  db 0

    txt             dd 97,0
    txtb            db 0,0
    mapa            MATRIZ <>
    ;todos 9 menos o ultimo (16)
    pecas               db  1,0,0,1,1,1,0,0,0 ;azul
                        db  0,0,1,1,1,1,0,0,0 ;laranja
                        db  1,1,0,1,1,0,0,0,0 ;amarelo
                        db  0,1,1,1,1,0,0,0,0 ;verde
                        db  0,1,0,1,1,1,0,0,0 ;roxo
                        db  1,1,0,0,1,1,0,0,0 ;vermelho
                        db  0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0 ;ciano

    vetMapa         db  0,0,0,0,0,0,0,0,0,0,0,0
                    db  0,0,0,0,0,0,0,0,0,0,0,0
                    db  0,0,0,0,0,0,0,0,0,0,1,1
                    db  1,1,1,1,1,1,1,1,1,1,0,0
                    db  0,0,1,0,0,0,0,0,0,0,0,0
                    db  0,1,0,0,0,0,1,0,0,0,0,0
                    db  0,0,0,0,0,1,0,0,0,0,1,0
                    db  0,0,0,0,0,0,0,0,0,1,0,0
                    db  0,0,1,0,0,0,0,0,0,0,0,0
                    db  0,1,0,0,0,0,1,0,0,0,0,0
                    db  0,0,0,0,0,1,0,0,0,0,1,0
                    db  0,0,0,0,0,0,0,0,0,1,0,0
                    db  0,0,1,0,0,0,0,0,0,0,0,0
                    db  0,1,0,0,0,0,1,0,0,0,0,0
                    db  0,0,0,0,0,1,0,0,0,0,1,0
                    db  0,0,0,0,0,0,0,0,0,1,0,0
                    db  0,0,1,0,0,0,0,0,0,0,0,0
                    db  0,1,0,0,0,0,1,0,0,0,0,0
                    db  0,0,0,0,0,1,0,0,0,0,1,0
                    db  0,0,0,0,0,0,0,0,0,1,0,0
                    db  0,0,1,0,0,0,0,0,0,0,0,0
                    db  0,1,0,0,0,0,1,0,0,0,0,0
                    db  0,0,0,0,0,1,0,0,0,0,1,0
                    db  0,0,0,0,0,0,0,0,0,1,0,0
                    db  0,0,1,0,0,0,0,0,0,0,0,0
                    db  0,1,0,0,0,0,1,0,0,0,0,0
                    db  0,0,0,0,0,1,0,0,0,0,1,0
                    db  0,0,0,0,0,0,0,0,0,1,0,0
                    db  0,0,1,0,0,0,0,0,0,0,0,0
                    db  0,1,0,0,0,0,1,0,0,0,0,0
                    db  0,0,0,0,0,1,0,0,0,0,1,1
                    db  1,1,1,1,1,1,1,1,1,1,0,0
                    db  0,0,0,0,0,0,0,0,0,0,0,0
                    db  0,0,0,0,0,0,0,0,0,0,0,0
                    db  0,0,0,0,0,0,0,0
    bloco           TETRIMINO <>

    ThreadDescer  dd 0
	ExitCode 	  dd 0
	hThread 	  dd 0
	hEventStart   dd 0
    hBmp          dd 0

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
    PP_DESENHAR       db 0
    PP_DESCER         db 1
    PP_ROTACIONAR     db 2
    PP_MOVER_DIREITA  db 3
    PP_MOVER_ESQUERDA db 4