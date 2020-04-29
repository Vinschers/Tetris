include cabecalho.inc
bitmap equ 111 ; definição do bitmap
CREF_TRANSPARENT  EQU 0FF00FFh
; quaquer procedimento que for escrito 
; deve ter o seu prototipo descrito aqui.
; com em C 

WinMain PROTO :DWORD,:DWORD,:DWORD,:DWORD
WndProc PROTO :DWORD,:DWORD,:DWORD,:DWORD
TopXY PROTO   :DWORD,:DWORD
desenharBloco PROTO :DWORD, :DWORD, :BYTE, :BYTE


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
    vetMapa         db  200 dup(0)
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
        mov bloco.mat.ponteiro, OFFSET vet
        mov bloco.mat.altura, 3
        mov bloco.mat.largura, 3

        mov mapa.ponteiro, OFFSET vetMapa
        mov mapa.altura, 20
        mov mapa.largura, 10

        invoke CreateEvent,NULL,FALSE,FALSE,NULL
        mov    hEventStart,eax

        mov ecx, OFFSET ThreadProcDescer
        invoke CreateThread, NULL, NULL, ecx, ADDR mapa, NORMAL_PRIORITY_CLASS, ADDR ThreadDescer
        mov hThread, eax


    .elseif uMsg == WM_PAINT
        invoke BeginPaint, hWin, ADDR ps
        mov hdc, eax
        include gui.inc
        ;mov ecx, hdc
        ;push ecx
        ;mov ecx, hWin
        ;push ecx
        ;call desenharTetrimino  
        invoke desenharBloco, hWin, hdc, AZUL, bloco.posicao
        invoke EndPaint, hWin, ADDR ps


    .elseif uMsg == WM_DESCER
        add bloco.posicao, 10
        invoke InvalidateRect,hWnd,NULL,TRUE

    .elseif uMsg == WM_DESTROY
        invoke PostQuitMessage,NULL
        return 0

    .endif

    invoke DefWindowProc,hWin,uMsg,wParam,lParam

    ret

WndProc endp

include matriz.inc
include tetrimino.inc

end start