WinMain PROTO :DWORD,:DWORD,:DWORD,:DWORD
WndProc PROTO :DWORD,:DWORD,:DWORD,:DWORD
TopXY PROTO   :DWORD,:DWORD
desenharBloco PROTO :DWORD, :DWORD, :BYTE, :BYTE
montarTetrimino PROTO :DWORD, :BYTE
desenharTetrimino PROTO :DWORD, :DWORD, :DWORD
strMatriz   PROTO :DWORD, :DWORD


matriz struct
    ponteiro    DWORD   ?
    altura      BYTE    ?
    largura     BYTE    ?
matriz ends

tetrimino struct
    tipo    BYTE    ?
    posicao BYTE    ?
    mat     DWORD   ?
tetrimino ends


.data?
    hitpoint POINT <>
    rect     RECT  <>
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

    MouseClick      db 0 ; 0 = no click yet
    txt             dd 97,0
    txtb            db 97,0
    mapa            matriz <>
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
    bloco           tetrimino <>

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