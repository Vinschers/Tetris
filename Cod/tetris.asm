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

    invoke GetProcessHeap
    mov hHeap, eax

    .if uMsg == WM_CREATE
        mov bloco.mat.ponteiro, OFFSET vet
        mov bloco.mat.altura, 3
        mov bloco.mat.largura, 3

        mov mapa.ponteiro, OFFSET vetMapa
        mov mapa.altura, 26
        mov mapa.largura, 16

        push OFFSET mapa
        push hWin
        call strMatriz

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