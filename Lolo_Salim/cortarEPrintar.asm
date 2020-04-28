
    ; ##########################################################################################################
    ; #                                         Cabeçalho                                                      #
    ; ##########################################################################################################   

    .386
    .model flat, stdcall            ; 32 bit memory model
    option casemap :none            ; case sensitive

    include bitblt.inc              ; local includes for this file
	
	b2			equ		111         ; código para link da imagem, no caso, o SPRITE

    ; possíveis variáveis para novos locais X e Y 
    ;newX db 300                
    ;newY db 100
    tipo dd 3                       ; variável para definição do tipo, ou seja, cor da peça

	CREF_TRANSPARENT  EQU 0FF00FFh  ; transparência da imagem

    ; ######################################################################
    ; #          Tabela de cores e seus respectivos números:               #
    ; ######################################################################
    ; # 0         1       2       3           4           5       6        #
    ; # verde     roxo    azul    laranja     amarelo     preto   vermelho #
    ; ######################################################################





    ; ##########################################################################################################
    ; #                                           Código                                                       #
    ; ##########################################################################################################   

    .code                   
    start: 

    main proc 
        call printarBloco
    main endp

    printarBloco proc hWin   :DWORD,
             uMsg   :DWORD,
             wParam :DWORD,
             lParam :DWORD

        LOCAL var    :DWORD
        LOCAL caW    :DWORD
        LOCAL Rct    :RECT
        LOCAL hDC    :DWORD
        LOCAL Ps     :PAINTSTRUCT

        invoke LoadBitmap,hInstance, b2
        mov hBmp2, eax

        invoke BeginPaint,hWin,ADDR Ps
        mov hDC, eax
        invoke Paint_Proc,hWin,hDC      ; chamamos a função que recorta o Sprite e o exibe



    printarBloco endp

    Paint_Proc proc hWin:DWORD, hDC:DWORD

        LOCAL hOld:DWORD
        LOCAL memDC :DWORD
    
        invoke CreateCompatibleDC,hDC
    
        mov memDC, eax
	    invoke SelectObject,memDC,hBmp2  ; selecionei o novo bitmap
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

        INVOKE  TransparentBlt,hDC,ebx,30,32,284,memDC,0,256,eax,32,CREF_TRANSPARENT    ; cortamos a imagem  
        pop ebx          ; retiramos da pilha o valor anterior de ebx
        invoke SelectObject,hDC,hOld
        invoke DeleteDC,memDC

        return 0

    Paint_Proc endp

end start











