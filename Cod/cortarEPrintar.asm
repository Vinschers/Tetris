
    ; ##########################################################################################################
    ; #                                         Cabeçalho                                                      #
    ; ##########################################################################################################   

	include cabecalho.inc
    
	b2			equ		111         ; código para link da imagem, no caso, o SPRITE
    WinMain      PROTO :DWORD,:DWORD,:DWORD,:DWORD
    WndProc      PROTO :DWORD,:DWORD,:DWORD,:DWORD
    TopXY        PROTO :DWORD,:DWORD
    FillBuffer   PROTO :DWORD,:DWORD,:BYTE
    Paint_Proc   PROTO :DWORD,:DWORD

    ; possíveis variáveis para novos locais X e Y 
    ;newX db 300                
    ;newY db 100

    CREF_TRANSPARENT  EQU 0FF00FFh  ; transparência da imagem

    .data?
        hitpoint POINT <>
        rect     RECT  <>
        posx  dd ?
        posy  dd ?

    .data 

        tipo dd 3                       ; variável para definição do tipo, ou seja, cor da peça
        szDisplayName   db "Tetris", 0 
        proxPecaTxt     db "Proxima Peca", 0
        pecaSeguradaTxt db "Peca Segurada", 0
        pontuacaoTxt    db "Pontuacao", 0
        CommandLine     dd 0  ; parametros passados pela linha de comando (ponteiro)
        hWnd            dd 0  ; Handle principal do programa no windows
        hInstance       dd 0  ; instancia do programa
        hHeap           dd 0
        hBmp            dd 0 ; handler 

        MouseClick      db 0 ; 0 = no click yet
        txt             dd 100,0
        vet             db 0,1,0,1,1,1,0,0,0
	

    ; ######################################################################
    ; #          Tabela de cores e seus respectivos números:               #
    ; ######################################################################
    ; # 0         1       2       3           4           5       6        #
    ; # verde     roxo    azul    laranja     amarelo     preto   vermelho #
    ; ######################################################################

    ; MEU BEBE



    ; ##########################################################################################################
    ; #                                           Código                                                       #
    ; ##########################################################################################################   

    .code                   
    start:

    include janela.inc 

    WndProc proc hWin   :DWORD,
             uMsg   :DWORD,
             wParam :DWORD,
             lParam :DWORD

    LOCAL hDC:HDC
    LOCAL Ps:PAINTSTRUCT

    invoke GetProcessHeap
    mov hHeap, eax

    .if uMsg == WM_CREATE


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
        invoke LoadBitmap,hInstance, b2
        mov hBmp, eax
        invoke BeginPaint,hWin,ADDR Ps
        mov hDC, eax
        invoke Paint_Proc,hWin,hDC      ; chamamos a função que recorta o Sprite e o exibe
        invoke EndPaint, hWin, ADDR Ps
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

    Paint_Proc proc hWin:DWORD, hDC:DWORD

        LOCAL hOld:DWORD
        LOCAL memDC :DWORD
    
        invoke CreateCompatibleDC,hDC
    
        mov memDC, eax
	    invoke SelectObject,memDC,hBmp  ; selecionei o novo bitmap
        mov hOld, eax

        ; ##########################################################################################################
        ;                                      Algorítimo de seleção da cor
        ; ##########################################################################################################

        xor eax, eax     ; limpamos eax
        push ebx         ; armazenamos o conteúdo de ebx na pilhas
        mov ebx, 32 
        mov eax, tipo    ; atribuímos o valor do tipo ao registrador de 8 bits, o al
        mul ebx          ; multiplicamos o valor armazenado em al por 32
        mov ebx, eax     ; salvamos o valor de al em tipo, que será usado para o ponto de início do corte no eixo X
        add eax, 32      ; adicionamos 32 pixels ao al, que será usado para o limite do corte no eixo X
    
        ; ##########################################################################################################

         INVOKE  TransparentBlt,hDC,200,30,32,32,memDC,0,256,ebx,eax,CREF_TRANSPARENT    ; cortamos a imagem  
        ;pop ebx          ; retiramos da pilha o valor anterior de ebx
        invoke SelectObject,hDC,hOld
        invoke DeleteDC,memDC

        return 0

    Paint_Proc endp

end start











