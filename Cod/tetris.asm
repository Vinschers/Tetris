include cabecalho.inc
bitmap equ 111 ; definição do bitmap
; quaquer procedimento que for escrito 
; deve ter o seu prototipo descrito aqui.
; com em C 

WinMain PROTO :DWORD,:DWORD,:DWORD,:DWORD
WndProc PROTO :DWORD,:DWORD,:DWORD,:DWORD
TopXY PROTO   :DWORD,:DWORD


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

    ThreadDescer 	  dd 0
	ExitCode 	  dd 0
	hThread 	  dd 0
	hEventStart   dd 0

.const
    WM_DESCER equ WM_USER+100h

.code   ; parte do codigo

start:  ; o programa  deve ser escrito entre start e end start
        ; inclusive procedimentos que vão ser escritos e utilizados.
    
    include janela.inc

; #########################################################################

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

        mov eax, OFFSET ThreadProcDescer
        invoke CreateThread,NULL,NULL,eax,\
                            NULL, NORMAL_PRIORITY_CLASS,\
                            ADDR ThreadDescer
                hThread, eax



    ;; lparam da mensagem traz as posições x e y do mouse
    .elseif uMsg == WM_LBUTTONDOWN

        mov     eax, lParam
        and     eax, 0FFFFh
        mov     hitpoint.x, eax
        mov     posx, eax
        mov     eax, lParam
        shr     eax, 16  ; desloca o registrador eax de 16 bits para a direita ->
        mov     hitpoint.y, eax
        mov     posy, eax
        mov     MouseClick, TRUE
        invoke  InvalidateRect, hWin, NULL, TRUE ; 


    .elseif uMsg == WM_PAINT
        invoke BeginPaint, hWin, ADDR ps
        include gui.inc 
        mov ecx, HDC
        push ecx
        mov ecx, hWin
        push ecx
        call desenharTetrimino  
        invoke EndPaint, hWin, ADDR ps
    .elseif uMsg == WM_DESCER
        add bloco.posicao, 10
    .endif
    .elseif uMsg == WM_DESTROY
        invoke PostQuitMessage,NULL
        return 0 
    .endif

    invoke DefWindowProc,hWin,uMsg,wParam,lParam
    ret

WndProc endp

; ########################################################################

TopXY proc wDim:DWORD, sDim:DWORD
    shr sDim, 1      ; divide screen dimension by 2
    shr wDim, 1      ; divide window dimension by 2
    mov eax, wDim    ; copy window dimension into eax
    sub sDim, eax    ; sub half win dimension from half screen dimension

    return sDim

TopXY endp

desenharTetrimino proc hWin:DWORD, hDC:DWORD
    LOCAL hOld:DWORD
    LOCAL memDC :DWORD

    invoke CreateCompatibleDC,hDC
    mov memDC, eax

    invoke SelectObject,memDC,bitmap
    mov hOld, eax 

    xor eax, eax 
    xor ebx, ebx
    mov al, bloco.posicao
    push eax
    call getPixel 
    mov bx, ax ; ebx coluna e eax linha
    shr eax, 16
    invoke TransparentBlt,hDC,eax,ebx,32,32,memDC,0,0,224,32,CREF_TRANSPARENT
    invoke SelectObject,hDC,hOld
    invoke DeleteDC,memDC
    return 0
desenharTetrimino endp

ThreadProcDescer PROC USES ecx Param:DWORD
    ;bitmap
    invoke WaitForSingleObject,hEventStart,100
        .IF eax == WAIT_TIMEOUT
            invoke PostMessage,hWnd,WM_DESCER,NULL,NULL
            jmp ThreadProcDescer
        .ENDIF

        jmp ThreadProcDescer
    ret
ThreadProcDescer ENDP

; ########################################################################

include matriz.inc

end start