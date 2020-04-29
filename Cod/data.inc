matriz struct
    ponteiro    DWORD   ?
    altura      BYTE    ?
    largura     BYTE    ?
matriz ends

tetrimino struct
    tipo    BYTE    ?
    posicao BYTE    ?
    mat     matriz  <>
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
    txt             dd 100,0
    mapa            matriz <>
    vet             db  1,0,0,1,1,1,0,0,0
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
    VERDE    db 0
    ROXO     db 1
    AZUL     db 2
    LARANJA  db 3
    AMARELO  db 4
    PRETO    db 5
    VERMELHO db 6