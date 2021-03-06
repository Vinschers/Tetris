;//////////////////////////////////////////////////////////////////////////////////
;feito por:
;18188 Guilherme Salim de Barros
;18178 Felipe Scherer Vicentin
;18343 Lorenna Joanna Leal Nunes 
;18190 Maria Eduarda Elias Rocha
;para o prof Sergio na matéria 'linguagem de montagem'
;todos direitos reservados -- 2020  ©
;//////////////////////////////////////////////////////////////////////////////////

;procedimento que desenha a janela 
desenharTela proc hWin:DWORD, hdc:DWORD
    ;variáveis locais
    LOCAL corDeFundo:BYTE
    LOCAL corDeFundoRect:BYTE
    LOCAL corTxt:BYTE
    LOCAL hfont:HFONT

    mov corDeFundo, 0       ; cor de fundo da tela
    mov corDeFundoRect, 255 ; cor de fundo dos retangulos de peças
    mov corTxt, 20          ; cor de fundo dos textos

    .if tema == 0               ; configura se o tema for claro
        mov corDeFundoRect, 255
        mov corDeFundo, 240
    .else                       ; configura se o tema for escuro
        mov corDeFundoRect, 33
        mov corDeFundo, 20
        mov corTxt, 255
    .endif
    
    ; configuração do mapa
    mov rect.top, 0
    mov rect.left, 0
    mov rect.bottom, 700
    mov rect.right, 650
    RGB corDeFundo, corDeFundo, corDeFundo
    invoke CreateSolidBrush, eax
    invoke FillRect, hdc, OFFSET rect, eax

    ; espaço que será mostrado a peça segurada
	mov rect.top, 10
	mov rect.bottom, 652
	mov rect.left, 10
	mov rect.right, 332

	RGB corDeFundoRect, corDeFundoRect, corDeFundoRect
    invoke CreateSolidBrush, eax
    invoke FillRect, hdc, OFFSET rect, eax

    ; peça segurada texto, nesse trecho configura a cor da fonte e do background para todos os textos da janela
    invoke SelectObject, hdc, eax
    mov    hfont,eax
    RGB    corTxt,corTxt,corTxt
    invoke SetTextColor,hdc,eax
    RGB    corDeFundoRect,corDeFundoRect,corDeFundoRect
    invoke SetBkColor,hdc,eax
    invoke TextOut,hdc,400,20,ADDR pecaSeguradaTxt,SIZEOF pecaSeguradaTxt
    invoke SelectObject,hdc, hfont

    ; proxima peça retangulo
	mov rect.top, 50
	mov rect.bottom, 210
	mov rect.left, 390
	mov rect.right, 582

	RGB corDeFundoRect, corDeFundoRect, corDeFundoRect
    invoke CreateSolidBrush, eax
    invoke FillRect, hdc, OFFSET rect, eax

    ; proxima peça texto
	invoke  lstrlen, ADDR proxPecaTxt
	invoke  TextOut, hdc, 400, 250, ADDR proxPecaTxt, eax

    ; texto do nível
    xor eax, eax
    mov al, nivel


    invoke wsprintf, OFFSET buffer, OFFSET formatacaoNivel, eax
    invoke GetClientRect, hWin, ADDR rect
    
    
    mov rect.left, 400
    mov rect.top, 470
    mov rect.right, 650
    mov rect.bottom, 500
    invoke DrawText, hdc, ADDR buffer, -1, ADDR rect, DT_SINGLELINE

    ; texto da pontuação
    mov eax, pontuacao


    invoke wsprintf, OFFSET buffer, OFFSET formatacaoPontuacao, eax
    invoke GetClientRect, hWin, ADDR rect
    
    
    mov rect.left, 400
    mov rect.top, 530
    mov rect.right, 650
    mov rect.bottom, 550
    invoke DrawText, hdc, ADDR buffer, -1, ADDR rect, DT_SINGLELINE

	ret
desenharTela endp

;esse procedimento desenha o tetromino passado por parâmetro em sua posição específica
desenharTetromino proc hWin:DWORD, hDC:DWORD, pTetromino:DWORD, cor:BYTE
    mov edx, pTetromino
    mov edx, (TETROMINO ptr[edx]).mat
    ;limpa os registradores
    xor eax, eax
    xor ecx, ecx
    mov al, (MATRIZ ptr[edx]).largura
    mov cl, (MATRIZ ptr[edx]).altura
    mul ecx

    mov edx, pTetromino
    mov edx, (TETROMINO ptr[edx]).mat
    mov edx, (MATRIZ ptr[edx]).ponteiro
    ;limpa os registradores
    xor ebx, ebx
    xor ecx, ecx

    forT: ;percorre a matriz lógica do desenho de cada tetromino
    cmp ebx, eax
    je fim

    mov cl, byte ptr[edx]
    cmp cl, 0 ;só é necessário desenhar algo se houver 1 na posição atual da matriz do tetromino. Se o valor atual for 0, o for continua para a próxima posição
    je cont

    push eax ;registradores são salvos
    push ebx
    push ecx
    push edx

    ;as operações abaixo calculam a posição da matriz lógica onde o "pixel" atual do tetromino ficaria
    xor eax, eax
    mov al, bl ;al tem o contador
    xor ebx, ebx

    mov edx, pTetromino
    mov edx, (TETROMINO ptr[edx]).mat
    mov bl, (MATRIZ ptr[edx]).largura  ;bl tem a largura

    xor edx, edx

    div ebx   ;al tem a linha

    mov esi, edx

    mov ebx, 16 ; multiplica a linha pelo número de colunas do mapa
    mul ebx

    mov edx, esi

    add ax, dx

    xor ecx,ecx; limpa registrador
    mov edx, pTetromino
    mov ecx, (TETROMINO ptr[edx]).posicao
    add eax, ecx

    ;ax tem a posição onde o pixel deve ser desenhado

    mov cl, cor
    
    invoke desenharBloco, hWin, hDC, cl, ax ;desenho do pixel

    pop edx ;registradores são restorados
    pop ecx
    pop ebx
    pop eax

    cont:
    inc ebx
    inc edx
    jmp forT

    fim:
    ret
desenharTetromino endp

;Tarefa assíncrona que fara o tetromino descer na matriz
ThreadProcDescer PROC USES ecx Param:DWORD
    invoke WaitForSingleObject,hEventStart, velocidade ; aguada os objetos
        .IF eax == WAIT_TIMEOUT ; verifica se o tetrimino já desceu o máximo e pode descer o próximo
            pintar PP_DESCER ; desce o tretromino
            jmp ThreadProcDescer ; continua com a tarefa assíncrona
        .ENDIF
        .if perdeu != 1 ; verifica se o usuário não perdeu 
            jmp ThreadProcDescer ; continua com a tarefa assíncrona
        .endif
    ret
ThreadProcDescer endp ; final da tarefa

desenharBloco proc hWin:DWORD, hDC:DWORD, cor:BYTE, posicao:WORD

    LOCAL hOld:DWORD
    LOCAL memDC :DWORD

    invoke CreateCompatibleDC,hDC

    mov memDC, eax
    invoke SelectObject,memDC,hBmp  ; selecionei o novo bitmap
    mov hOld, eax

    xor ebx, ebx; limpa registrador
    invoke getPixel, posicao
    mov bx, ax ; ebx coluna e eax linha
    shr eax, 16

    mov ecx, eax
    mov edx, ebx

    xor eax, eax     ; limpamos eax
    xor ebx, ebx     ; limpar ebx
    mov ebx, 32 
    mov al, cor     ; atribuímos o valor do tipo ao registrador de 8 bits, o al
    push edx
    mul ebx          ; multiplicamos o valor armazenado em al por 32
    pop edx
    mov ebx, eax     ; salvamos o valor de al em tipo, que será usado para o ponto de início do corte no eixo X

    INVOKE  TransparentBlt,hDC,edx,ecx,32,32,memDC,ebx,0,32,32,CREF_TRANSPARENT    ; cortamos a imagem  

    invoke SelectObject,hDC,hOld
    invoke DeleteDC,memDC

    return 0

desenharBloco endp

apagarTetromino proc hWin:DWORD, hdc:DWORD, pTetromino:DWORD
	invoke desenharTetromino, hWin, hdc, pTetromino, NADA   ; chama desenharTetromino passando como tipo de peça NADA, assim apenas apaga da parte visual o tetromino
    ret
apagarTetromino endp

;ax -> coluna
;resto -> linha
getPixel proc pos:WORD
    ; limpa registradores
    xor edx, edx
    xor eax, eax

    mov ax, pos ; posição é onde do vetor o dado está, mas precisamos saber onde em na matriz visual o bloco deve estar, para isso extraimos a linha e a coluna da posicao
    mov ecx, 16 ; 16 colunas
    div ecx     ; divide posição pelo número de colunas, resto é o número de linhas
    sub eax, 3  ; como a matriz lógica tem 3 linhas e 3 colunas a mais sempre se diminui 3 das duas direções para termos os valores na matriz visual
    sub edx, 3

    shl eax, 5  ; são 32 pixels por bloco, e como usamos o método para ter a posição visual é necessário multiplicar o número de linhas e de colunas
    shl edx, 5  ; por 32
    add eax, 11 ; o mapa começa na posição x = 11 e y = 11, então esses valores são adicionados a linha e coluna
    add edx, 11
    shl eax, 16 ; o valor da coluna vai 16 bits para a esquerda para colocar a linha nas outras 16
    mov ax, dx  ; o registrador eax fica com a linha e a coluna
    ret
getPixel endp



paint proc hWin:DWORD, hdc:DWORD, pTetromino:DWORD, pMapa:DWORD, pFantasma:DWORD, vetPecas:DWORD, atual:DWORD
	LOCAL copiaTetromino:DWORD
	LOCAL copiaMatriz:DWORD
	LOCAL colidiu:BYTE
    LOCAL corDeFundoRect:BYTE
    LOCAL corTxt:BYTE
    LOCAL hfont:HFONT

    mov corDeFundoRect, 255
    mov corTxt, 0

    cmp tema, 2
    jne temaClaro

    mov corDeFundoRect, 33
    mov corTxt, 255
    temaClaro:
	invoke copiarTetromino, pTetromino
    mov copiaTetromino, eax
    
    invoke colocarMatrizLogica, pTetromino, pMapa, 0

    ; variaveis necessárias são guardadas em registradores já que quando o if é efetuado os valores das variaveis se perdem
    mov al, paintParam
    mov dl, pecaGuardada
    mov cl, jaGuardou

    .if al == PP_DESENHAR           ; atualiza a tela
        invoke desenharTela, hWin ,hdc
        mov cl, NADA
        add cl, tema
        invoke desenharTetromino, hWin, hdc, pFantasma, cl
        invoke atualizarFantasma, pFantasma, pTetromino, pMapa
        mov esi, pFantasma
        mov cl, (TETROMINO ptr[esi]).tipo
        invoke desenharTetromino, hWin, hdc, pFantasma, cl
        jmp desenharProximaPeca

    .elseif al == PP_DESCER
        mov cl, NADA
        add cl, tema
        invoke desenharTetromino, hWin, hdc, pTetromino, cl ; o tetromino é apagado da posição atual
        mov esi, pTetromino
        add (TETROMINO ptr[esi]).posicao, 16    ; moviementa a peça 1 linha para baixo

    .elseif al == PP_ROTACIONAR
        mov cl, NADA    
        add cl, tema
        invoke desenharTetromino, hWin, hdc, pTetromino, cl ; o tetromino é apagado da posição atual
        mov esi, pTetromino                                 ; SCHERER
        invoke rotacionarMatriz, (TETROMINO ptr[esi]).mat
        mov esi, pTetromino
        add (TETROMINO ptr[esi]).rotacao, 1
        mov cl, (TETROMINO ptr[esi]).rotacao
        .if cl == 4
        	mov esi, pTetromino
            mov (TETROMINO ptr[esi]).rotacao, 0
        .endif

    .elseif al == PP_MOVER_DIREITA
        mov cl, NADA
        add cl, tema
        invoke desenharTetromino, hWin, hdc, pTetromino, cl ; o tetromino é apagado da posição atual
        mov esi, pTetromino
        inc (TETROMINO ptr[esi]).posicao                    ; é incrementado um a sua posição(movimenta uma posição a direita)

    .elseif al == PP_MOVER_ESQUERDA
        mov cl, NADA
        add cl, tema
        invoke desenharTetromino, hWin, hdc, pTetromino, cl ; o tetromino é apagado da posição atual
        mov esi, pTetromino
        dec (TETROMINO ptr[esi]).posicao                    ; é retirado um a sua posição(movimenta uma posição para a esquerda)

    .elseif al == PP_REFAZER                                ; caso necessite trocar uma peça pela próxima ou por uma já guardada
        cmp cl, 0
        jne fimRefazer 
        
        push ecx
        push edx

        ; forma o retangulo de peça guardada novamente
        mov rect.top, 50
	    mov rect.bottom, 210
	    mov rect.left, 390
	    mov rect.right, 582
	    RGB corDeFundoRect, corDeFundoRect, corDeFundoRect
        invoke CreateSolidBrush, eax
        invoke FillRect, hdc, OFFSET rect, eax
        pop edx
        pop ecx

        mov jaGuardou, 1 ; há peça guardada 
        push edx
        mov cl, NADA
        add cl, tema
        invoke desenharTetromino, hWin, hdc, pTetromino, cl ; apaga o tetromino da posição atual
        mov cl, NADA
        add cl, tema
        invoke desenharTetromino, hWin, hdc, pFantasma, cl ; apaga o fantasma da peça atual
        mov esi, pTetromino
        mov bl, (TETROMINO ptr[esi]).tipo ; bl recebeu o tipo da peça que será guardada 
        mov pecaGuardada, bl
        invoke desenharTetrominoGuardado, hWin, hdc, pTetromino, bl ; desenha a peça guardada
        mov esi, pTetromino
        pop edx
        cmp dl, 7
        jne jaTemGuardada
        jmp proximaPeca

        jaTemGuardada:
            invoke refazerTetrominoTipo, pTetromino, dl    ; muda o tipo da peça do mapa
        fimRefazer:

    .elseif al == PP_CAIR_TUDO
        mov cl, NADA
        add cl, tema
        invoke desenharTetromino, hWin, hdc, pTetromino, cl ; apaga o tetromino da posição atual
        invoke descerTudo, pTetromino, pMapa                
        mov colidiu, 1                                      ; a peça chegou ao fim então ela colidiu
        jmp atualizarVariaveis

    .elseif al == PP_MUDAR_TEMA
        mov perdeu, 1                                       ; se for mudar o tema a pessoa perde a música para e o jogo reinicia
        invoke PlaySound,NULL,NULL,SND_ASYNC
        invoke MessageBox,hWin, addr temaTxt, NULL, MB_YESNO

        .if eax == IDYES
            .if tema == 0   ; troca o tema atual pelo outro tema 
                mov tema, 2
            .else
                mov tema, 0
            .endif
            mov pontuacao, 0        ; zera a pontuacao e o nivel e recomeça o jogo
            mov nivel, 0
            invoke comecarJogo, hWin
            invoke desenharTela, hWin,hdc

            mov edx, pTetromino
            mov cl, (TETROMINO ptr[edx]).tipo
            invoke desenharTetromino, hWin, hdc, pTetromino, cl
            invoke atualizarFantasma, pFantasma, pTetromino, pMapa
            mov esi, pFantasma
            mov cl, (TETROMINO ptr[esi]).tipo
            invoke desenharTetromino, hWin, hdc, pFantasma, cl
            mov ecx, vetPecas
            inc ecx
            mov cl, byte ptr[ecx]
            invoke desenharTetrominoProx, hWin, hdc, cl
        .else
            mov perdeu, 0           ; senão quiser alterar o tema a pessoa volta a jogar
            invoke PlaySound, OFFSET CAMINHO_MUSICA,NULL,SND_ASYNC or SND_LOOP
        .endif

    .endif

    mov esi, pTetromino
    mov edi, (TETROMINO ptr[esi]).mat
    invoke copiarMatriz, edi
    mov copiaMatriz, eax

    invoke adicionarMatrizLogica, pTetromino, pMapa
    mov esi, pTetromino
    mov edi, (TETROMINO ptr[esi]).mat
    invoke verificarColisao, edi

    mov colidiu, al

    mov esi, pTetromino
    mov edi, (TETROMINO ptr[esi]).mat

    invoke atribuirMatriz, edi, copiaMatriz
    invoke destruirMatriz, copiaMatriz, 1

    mov al, paintParam
    .if colidiu == 1
        .if al == PP_ROTACIONAR
            invoke testesRotacao, pTetromino, pMapa
            .if eax == 0
                invoke atribuirTetromino, pTetromino, copiaTetromino
            .endif
        .else
            invoke atribuirTetromino, pTetromino, copiaTetromino
        .endif

    .endif

    atualizarVariaveis:

    invoke destruirTetromino, copiaTetromino    ; da free nos ponteiros que fazem referência ao tetromino

    mov cl, NADA
    add cl, tema
    invoke desenharTetromino, hWin, hdc, pFantasma, cl
    invoke atualizarFantasma, pFantasma, pTetromino, pMapa
    mov esi, pFantasma
    mov cl, (TETROMINO ptr[esi]).tipo
    invoke desenharTetromino, hWin, hdc, pFantasma, cl

    invoke colocarMatrizLogica, pTetromino, pMapa, 1        ; coloca o novo tetromino na matriz lógica
    mov esi, pTetromino
    mov cl, (TETROMINO ptr[esi]).tipo
    invoke desenharTetromino, hWin, hdc, pTetromino, cl     ; desenha o tetromino atualizado

    mov al, paintParam
    .if colidiu == 1 ; verifia se o tetromino que esta decendo encontrou com outro
        .if al == PP_DESCER || al == PP_CAIR_TUDO
            mov jaGuardou, 0 ; não haverá a peça guardada
        	mov esi, pTetromino

        	mov ecx, (TETROMINO ptr[esi]).posicao ; guarda a posição do tetromino
            .if ecx < 67 ; verifica se a matriz do jogo foi preenchida verticalmente
                mov perdeu, 1 ; se sim, o jogo acabou

                ; atualiza a tela 
                invoke perder, hWin, hdc
                invoke desenharTela, hWin,hdc
                invoke atualizarFantasma, pFantasma, pTetromino, pMapa
                jmp desenharProximaPeca
            .else
                invoke trabalharComLinhas, hWin, hdc, pMapa ; verifica se alguma linha foi preenchida, se foi limpa e tira da matriz lógica
                mov velocidade, 500
                ; configura o próximo tetromino que irá descer
                proximaPeca: 
                mov ebx, atual
                inc byte ptr [ebx]
                mov cl, byte ptr [ebx]
                .if cl == 7   ; caso o registrador cl seja igual a 7, significa que chegamos ao fim do vetor de peças
                    mov ebx, atual
                    mov byte ptr [ebx], 0
                .endif 
                mov cl, byte ptr [ebx]
                invoke refazerTetrominoVetor, pTetromino, vetPecas, cl ; a nova peça será guardada no vetor
                invoke atualizarFantasma, pFantasma, pTetromino, pMapa ; a sombra será atualizada para o tetromino escolhido

                ; a cada peça caída a pontuação aumenta 100*nivel do jogador
                pontuacaoPecaCaida: 
                    ;gurda as informações na pilha
                    push eax
                    push ebx
                    push ecx
                    push edx
                    ; 
                    xor ebx,ebx ; limpa o registrador
                    mov bl, nivel ; guarda o nível em que está
                    inc ebx       ; aumenta o nível
                    mov eax, 100  ; aumenta a pontuação
                    mul ebx       ; atualiza o nível 
                    
                    add pontuacao, eax ; guarda a pontuação na variável
                    ; guarda as informações na pilha
                    push eax
                    push ebx
                    push ecx
                    push edx

                    mov eax, pontuacao

                    ; exibe a pontuação atualizada
                    invoke wsprintf, OFFSET buffer, OFFSET formatacaoPontuacao, eax
                    invoke GetClientRect, hWin, ADDR rect
                     
                    mov rect.left, 400
                    mov rect.top, 530
                    mov rect.right, 650
                    mov rect.bottom, 550

                    invoke SelectObject, hdc, eax
                    mov    hfont,eax
                    RGB    corTxt,corTxt,corTxt
                    invoke SetTextColor,hdc,eax
                    RGB    corDeFundoRect,corDeFundoRect,corDeFundoRect
                    invoke SetBkColor,hdc,eax
                    invoke DrawText, hdc, ADDR buffer, -1, ADDR rect, DT_SINGLELINE
                    invoke SelectObject,hdc, hfont

                    ; desempilha as informações
                    pop edx
                    pop ecx
                    pop ebx
                    pop eax
                    pop edx
                    pop ecx
                    pop ebx
                    pop eax

                ; vê se já chegou no final do vetor de peças e sortea novamente
                desenharProximaPeca:
                mov ebx, atual
                xor ecx, ecx ; limpa registrador
                mov cl, byte ptr [ebx]
                inc cl 

                cmp cl, 7 ; verifica se já chegou no final do vetor de peças
                jne atualizarProx ; sortea novamente

                invoke randomizarVetor, vetPecas
                mov cl, 0

                atualizarProx: ; pega o proximo tipo do vetor de tipos e atualiza o tetromino
                mov edx, vetPecas
                mov cl, byte ptr[edx + ecx] ; seleciona uma peça 
                invoke desenharTetrominoProx, hWin, hdc, cl ; desenha
                mov esi, pFantasma 
                mov cl, (TETROMINO ptr[esi]).tipo
                invoke desenharTetromino, hWin, hdc, pFantasma, cl ; desenha a sombra
                mov esi, pTetromino
			    mov cl, (TETROMINO ptr[esi]).tipo
			    invoke desenharTetromino, hWin, hdc, pTetromino, cl
            .endif
        .endif
    .endif
	ret
paint endp

;a variável desenhandoTetromino controla o estado de desenho da GUI. Se o valor dela for 1, o programa está executando um evento paint.
;se outro evento paint for requisitado enquanto o anterior ainda não acabou, esse procedimento faz o código esperar o evento paint "ficar disponível"
esperarDesenho proc
    comparar:
    cmp desenhandoTetromino, 1
    je comparar
    ret
esperarDesenho endp

perder proc hWin:DWORD, hdc:DWORD
    invoke PlaySound,NULL,NULL,SND_ASYNC    ; para de tocar a música de fundo
    mov   open_lpstrDeviceType, 0h         
    mov   open_lpstrElementName,OFFSET CAMINHO_GAME
    invoke mciSendCommand,0,MCI_OPEN, MCI_OPEN_ELEMENT,offset open_dwCallback 
    invoke mciSendCommand,open_wDeviceID,MCI_PLAY,MCI_FROM or MCI_NOTIFY,offset play_dwCallback ; inicia a tocar a música de gameover
    invoke MessageBox,hWin, addr perdeuTxt, NULL, MB_YESNO  ; avisa que perdeu e se deseja continuar
    invoke mciSendCommandA,open_wDeviceID,MCI_STOP,MCI_FROM or MCI_NOTIFY,offset play_dwCallback
    mov pontuacao, 0    ; pontuacao e nivel recebem 0
    mov nivel, 0
    .if eax == IDYES    ; caso queira continuar o jogo é reiniciado
        invoke comecarJogo, hWin    
    .else
        invoke ExitProcess,0
    .endif
    ret
perder endp

trabalharComLinhas proc hWin:DWORD, hdc:DWORD, pMapa:DWORD

    LOCAL retanguloTextoPontuacao:RECT
    LOCAL corTxt:BYTE
    LOCAL corDeFundoRect:BYTE
    LOCAL hfont:HFONT

    mov corDeFundoRect, 255
    mov corTxt, 0

    cmp tema, 2
    jne temaClaro

    mov corDeFundoRect, 33
    mov corTxt, 255
    temaClaro:
        invoke acharLinhas, hWin ,pMapa ; procura linhas completas
        mov esi, pMapa
        mov esi, (MATRIZ ptr[esi]).ponteiro
        xor ecx, ecx; limpa registrador
        forAchados:
            cmp ecx, 8
            je fimAchados
            cmp word ptr[eax], 0    ; caso a posição do vetor de linhas achadas for 0 acabou as linhas encontradas
            je fimAchados

            push esi
            push ecx
            push eax

            mov dx, word ptr[eax]
            invoke limparLinha, hWin, hdc, pMapa, dx    ; limpa a linha e a tira da matriz visual e lógica
            inc linhasAux 

            pop eax
            push eax

            mov dx, word ptr[eax]
            invoke descerMatriz, hWin, hdc, pMapa, dx   ; desce toda a matriz visual e lógica que sobrou

            pop eax
            pop ecx
            pop esi

            add ecx, 2
            add eax, 2
            jmp forAchados

    fimAchados:
        xor ecx, ecx; limpa registrador
        ; aumenta a pontuação de acordo com o número de linhas e o nível do usuário (foi usado um sistema de pontuação do tetris nintendo)
        mov cl, linhasAux
        cmp cl, 0
        je retornar
        .if cl == 1
            mov eax, 40
        .elseif cl == 2
            mov eax, 100
        .elseif cl == 3
            mov eax, 300
        .elseif cl == 4
            mov eax, 1200
        .endif

        xor ebx, ebx; limpa registrador
        mov bl, nivel

        inc bl 
        mul ebx
        add pontuacao, eax
        
        push eax
        push ebx
        push ecx
        push edx

        ;atualiza a pontuação na tela
        mov eax, pontuacao


        invoke wsprintf, OFFSET buffer, OFFSET formatacaoPontuacao, eax
        invoke GetClientRect, hWin, ADDR retanguloTextoPontuacao
        
        
        mov retanguloTextoPontuacao.left, 400
        mov retanguloTextoPontuacao.top, 530
        mov retanguloTextoPontuacao.right, 650
        mov retanguloTextoPontuacao.bottom, 550


        invoke SelectObject, hdc, eax
        mov    hfont,eax
        RGB    corTxt,corTxt,corTxt
        invoke SetTextColor,hdc,eax
        RGB    corDeFundoRect,corDeFundoRect,corDeFundoRect
        invoke SetBkColor,hdc,eax
        invoke DrawText, hdc, ADDR buffer, -1, ADDR retanguloTextoPontuacao, DT_SINGLELINE
        invoke SelectObject,hdc, hfont


        pop edx
        pop ecx
        pop ebx
        pop eax

        dec bl
        mov linhasAux, 0
        add linhas, cl

        xor ecx, ecx ; limpa registrador
        ; caso tiver completado 10 linhas feitas naquele nível o usuário sobre de nível
        mov cl, linhas
        cmp ecx, 10
        jl retornar

        inc bl
        mov nivel, bl

        push eax
        push ebx
        push ecx
        push edx

        ; atualiza o nível na tela

        xor eax, eax
        mov al, nivel


        invoke wsprintf, OFFSET buffer, OFFSET formatacaoNivel, eax
        invoke GetClientRect, hWin, ADDR retanguloTextoPontuacao
        
        
        mov retanguloTextoPontuacao.left, 400
        mov retanguloTextoPontuacao.top, 470
        mov retanguloTextoPontuacao.right, 650
        mov retanguloTextoPontuacao.bottom, 500

        
        invoke SelectObject, hdc, eax
        mov    hfont,eax
        RGB    corTxt,corTxt,corTxt
        invoke SetTextColor,hdc,eax
        RGB    corDeFundoRect,corDeFundoRect,corDeFundoRect
        invoke SetBkColor,hdc,eax
        invoke DrawText, hdc, ADDR buffer, -1, ADDR retanguloTextoPontuacao, DT_SINGLELINE
        invoke SelectObject,hdc, hfont

        pop edx
        pop ecx
        pop ebx
        pop eax

        mov linhas, 0
        ; aumenta a velocidade a cada nível subido
        mov edx, velocidade
        sub edx, 60
        mov velocidade, edx
        
    retornar:
        ret
trabalharComLinhas endp



limparLinha proc hWin:DWORD, hdc:DWORD, pMapa:DWORD, indice:WORD 

    mov   open_lpstrDeviceType, 0h
    mov   open_lpstrElementName, OFFSET CAMINHO_LINHA
    invoke mciSendCommand,0,MCI_OPEN, MCI_OPEN_ELEMENT,offset open_dwCallback 
    invoke mciSendCommand,open_wDeviceID,MCI_PLAY,MCI_FROM or MCI_NOTIFY,offset play_dwCallback
    invoke mciSendCommand,0,MCI_CLOSE,MCI_FROM or MCI_NOTIFY,offset play_dwCallback ; efeito de linha completa

    xor edx,edx

    add dx, indice
    mov ecx, edx
    add ecx, 10
    forLimpar:
        cmp edx, ecx    ; caso já tenha apagado 10 blocos (equivalente a uma linha)
        je fimForLimpar

        push ecx
        push edx

        mov ecx, pMapa                      ; apaga o dado da posição do bloco na matriz lógica
        mov ecx, (MATRIZ ptr[ecx]).ponteiro
        mov byte ptr[edx + ecx], 0  

        mov cl, NADA                        ; apaga o bloco da matriz visual
        add cl, tema
        invoke desenharBloco, hWin, hdc, cl, dx
        invoke Sleep, 75                    ; espera 75 milisegundos para apagar o prox, cria o efeito

        pop edx
        pop ecx

        inc edx
        jmp forLimpar
    fimForLimpar:
        ret
limparLinha endp

;se uma linha for preenchida, a matriz lógica e visual deve descer uma posição
descerMatriz proc hWin:DWORD, hdc:DWORD, pMapa:DWORD, indice:WORD
    mov edx, pMapa
    mov esi, (MATRIZ ptr[edx]).ponteiro
    xor ecx, ecx; limpa registrador
    mov cx, indice
    sub ecx, 16 ;ecx aponta para o início da linha anterior da preenchida
    ; limpa registradores
    xor ebx,ebx
    xor edx,edx
    forLinhas: ;percorre todas as linhas de baixa para cima. Para cada uma, o conteúdo da linha atual é copiado para a linha de baixo
        cmp ecx, 51
        jle fimForLinhas
        mov ebx, ecx
        add ebx, 10
        forPercorreLinha:
            cmp ecx, ebx
            je fimForPercorreLinha
            mov dl, byte ptr[esi + ecx]
            add ecx, 16
            mov byte ptr[esi + ecx], dl
            sub ecx, 16
            inc ecx
            jmp forPercorreLinha
        fimForPercorreLinha:  
            sub ecx, 26
            jmp forLinhas

    fimForLinhas:
        mov ecx, 51
        mov ebx, 61
        forParaLimparPrimeira: ;por conta do modo de como as paredes da matriz lógica foram contruídas, ao descer a matriz lógica, a linha visual mais acima
                               ;necessariamente estará preenchida com 1. Por isso, deve-se limpar essa linha
            cmp ecx, ebx
            je fimForParaLimparPrimeira
            mov byte ptr[esi + ecx], 0
            inc ecx
            jmp forParaLimparPrimeira
        fimForParaLimparPrimeira:
            ;o código abaixo copia o retângulo visual acima da linha preenchida para baixo. Basicamente, isso faz todo o jogo descer uma linha visualmente falando
            mov rectParaApagar.top, 11
            mov rectParaApagar.left, 11
            invoke getPixel, indice
            mov bx, ax ; ebx coluna e eax linha
            shr eax, 16
            mov rectParaApagar.bottom, eax
            mov ecx, rectParaApagar.bottom
            sub ecx, 11
            push ecx
            mov rectParaApagar.right,ebx
            invoke GetWindowRect, hWnd, OFFSET rectParaApagar
            pop ecx
            invoke BitBlt, hdc, 11, 43, 320, ecx, hdc, 11, 11, SRCCOPY
            ret
    
descerMatriz endp


acharLinhas proc hWin:DWORD, pMapa:DWORD
    invoke GlobalAlloc, 0, 8    ; cria um vetor com 4 posições, com 2 bytes cada
    push eax
    mov dword ptr[eax], 0 ;os primeiros 4 bytes são zerados
    add eax, 4
    mov dword ptr[eax], 0 ;os últimos 4 bytes são zerados
    sub eax, 4
    mov edx, pMapa
    mov esi, (MATRIZ ptr[edx]).ponteiro
    xor ecx, ecx; limpa registrador
    mov ecx, 51 ;a matriz visual começa na posição 51 da matriz lógica
    ; limpa registradores
    xor ebx,ebx
    xor edx,edx
    forLinhas:
        cmp edx, 0      ;se o edx estiver vazio, o for deve continuar sem adicionar nada ao vetor
        je continuarForLinhas
        mov word ptr[eax], dx ;coloca edx na posição atual do vetor
        add eax, 2 ;move o eax para a próxima posição do vetor (cada posição tem 2 bytes)
        continuarForLinhas:
            cmp ecx, 365 ;a matriz visual termina na posiçãp 365 da matriz lógica
            jge fimForLinhas ;se a matriz visual acabar, o for termina
            mov ebx, ecx
            add ebx, 10 ;ebx aponta para o final da linha visual atual
            forPercorreLinha: ;esse for percorre a linha atual da esquerda para a direita. Quando o ecx chegar ao fim, a linha visual acabou
                cmp ecx, ebx ;se ecx chegar ao final da linha, significa que a linha está preenchida
                je achouLinha ;achou-se uma linha preenchida
                xor edx, edx; limpa registrador
                mov dl,  byte ptr[esi + ecx]  ;como edx foi limpo, edx tem o valor da posição atual da matriz lógica
                cmp dl, 0 ;se houver 0 na matriz lógica, não há peça na matriz visual e, portanto, a linha não está preenchida
                je voltarAbruptamente
                inc ecx ;ecx é incrementado para continuar percorrendo a linha
                jmp forPercorreLinha
                voltarAbruptamente: ;o fluxo de execução chega aqui se a linha não estiver completamente preenchida
                    mov ecx, ebx ;ecx vai para o final da linha
                    add ecx, 6 ;com mais 6, ecx agora aponta para o início da próxima linha
                    xor edx,edx; limpa edx
                    jmp forLinhas ;volta para o for linha para executar a próxima linha
                achouLinha:
                    mov edx, ecx ;edx aponta para o fim da linha preenchida
                    sub edx, 10 ;edx aponta para o início da linha preenchida
                    add ecx, 6 ;ecx aponta para o início da próxima linha
                    jmp forLinhas
    fimForLinhas:
        pop eax ;com isso, o retorno desse procedimento é o endereço do vetor contendo o endereço de início das linhas preenchidas
        ret
acharLinhas endp

;P + Tcol + Tlin*6
;procedimento que desenha na matriz o tetromino que foi guardado pelo usuário
desenharTetrominoGuardado proc hWin:DWORD, hDC:DWORD, pTetromino:DWORD, cor:BYTE

    mov edx, pTetromino
    mov edx, (TETROMINO ptr[edx]).mat


    ;limpa registradores
    xor eax, eax
    xor ecx, ecx

    mov al, (MATRIZ ptr[edx]).largura
    mov cl, (MATRIZ ptr[edx]).altura
    mul ecx ; eax tem o valor de posições da matriz do tetronimo, é usado para percorrer

    mov edx, pTetromino
    mov edx, (TETROMINO ptr[edx]).mat
    mov edx, (MATRIZ ptr[edx]).ponteiro

    ;limpa registradores
    xor ebx, ebx
    xor ecx, ecx

    forT:
        cmp ebx, eax    ; enquanto o contador(ebx) não tiver percorrido todo o vetor do tetromino
        je fim

        mov cl, byte ptr[edx]   ; verifica se na posição atual o valor é 0, se for não precisa desenhar nada
        cmp cl, 0
        je cont

        push eax
        push ebx
        push ecx
        push edx

        xor eax, eax    ; limpa registrador
        mov al, bl      ; al tem o contador
        xor ebx, ebx    ; limpa registrador

        mov edx, pTetromino
        mov edx, (TETROMINO ptr[edx]).mat
        mov bl, (MATRIZ ptr[edx]).largura  ; bl tem a largura

        xor edx, edx ; limpa registrador

        div ebx   ; al tem a linha e edx tem a coluna

        mov esi, edx    ; esi recebe a coluna

        mov ebx, 6      ; multiplica a linha por 6 que é o tamanho em largura da matriz visual de peça segurada 
        mul ebx

        mov edx, esi

        add ax, dx  ; eax tem a posição em forma de vetor de onde o bloco deve ser desenhado 

        xor ecx,ecx; limpa registrador
        mov edx, pTetromino

        mov cl, cor
        
        invoke desenharBlocoGuardado, hWin, hDC, cl, ax     ; desenha o bloco da peça na região delimitada para a peça guardada

        pop edx
        pop ecx
        pop ebx
        pop eax

    cont:
        inc ebx
        inc edx
        jmp forT

    fim:
    ret
desenharTetrominoGuardado endp

desenharBlocoGuardado proc hWin:DWORD, hDC:DWORD, cor:BYTE, posicao:WORD

    LOCAL hOld:DWORD
    LOCAL memDC :DWORD

    invoke CreateCompatibleDC,hDC

    mov memDC, eax
    invoke SelectObject,memDC,hBmp  ; selecionei o novo bitmap
    mov hOld, eax
    xor ebx, ebx; limpa registrador
    invoke getPixelGuardado, posicao
    mov bx, ax ; ebx coluna e eax linha
    shr eax, 16

    mov ecx, eax
    mov edx, ebx
    ; limpa registradores
    xor eax, eax     ; limpamos eax
    xor ebx, ebx     ; limpar ebx
    mov ebx, 32 
    mov al, cor     ; atribuímos o valor do tipo ao registrador de 8 bits, o al
    push edx
    mul ebx          ; multiplicamos o valor armazenado em al por 32
    pop edx
    mov ebx, eax     ; salvamos o valor de al em tipo, que será usado para o ponto de início do corte no eixo X
    INVOKE  TransparentBlt,hDC,edx,ecx,32,32,memDC,ebx,0,32,32,CREF_TRANSPARENT    ; cortamos a imagem  

    invoke SelectObject,hDC,hOld
    invoke DeleteDC,memDC

    return 0

desenharBlocoGuardado endp

getPixelGuardado proc pos:WORD
    ; limpa registradores
    xor edx, edx
    xor eax, eax

    mov ax, pos
    mov ecx, 6  ; recebe o tamanho em colunas da parte visual
    div ecx

    shl eax, 5  ; são 32 pixels por bloco, e como usamos o método para ter a posição visual é necessário multiplicar o número de linhas e de colunas
    shl edx, 5  ; por 32

    add eax, 51 ; inicio x do retangulo delimitado para a peça guardada
    add edx, 451    ; inicio y do retangulo delimitado para a peça guardada
    shl eax, 16     ; linha fica nos primeiros 16 bits e a coluna nos últimos 16 bits de eax
    mov ax, dx
    
    ret

getPixelGuardado endp

;P + Tcol + Tlin*6
desenharTetrominoProx proc hWin:DWORD, hDC:DWORD, cor:BYTE
    LOCAL pTetromino:DWORD
    LOCAL corDeFundoRect:BYTE
    mov corDeFundoRect, 255

    cmp tema, 2
    jne temaClaro

    mov corDeFundoRect, 33

    temaClaro:
    invoke GlobalAlloc, 0, sizeof TETROMINO
    mov pTetromino, eax

    ; desenha a caixa de prox peça
    mov rect.top, 280
	mov rect.bottom, 450
	mov rect.left, 390
	mov rect.right, 582
	RGB corDeFundoRect, corDeFundoRect, corDeFundoRect
    invoke CreateSolidBrush, eax
    invoke FillRect, hDC, OFFSET rect, eax


    ; monta o tetrimino que será a prox peça
    invoke montarTetromino, pTetromino, cor
    mov edx, pTetromino
    mov edx, (TETROMINO ptr[edx]).mat
    ; limpa registradores
    xor eax, eax
    xor ecx, ecx
    mov al, (MATRIZ ptr[edx]).largura
    mov cl, (MATRIZ ptr[edx]).altura
    mul ecx ; eax tem o valor de posições da matriz do tetromino, é usado para percorrer

    mov edx, pTetromino
    mov edx, (TETROMINO ptr[edx]).mat
    mov edx, (MATRIZ ptr[edx]).ponteiro
    ; limpa registradores
    xor ebx, ebx
    xor ecx, ecx

    forT:
    cmp ebx, eax
    je fim

    mov cl, byte ptr[edx]
    cmp cl, 0
    je cont

    push eax
    push ebx
    push ecx
    push edx

    xor eax, eax; limpa registrador
    mov al, bl ;al tem o contador
    xor ebx, ebx; limpa registrador

    mov edx, pTetromino
    mov edx, (TETROMINO ptr[edx]).mat
    mov bl, (MATRIZ ptr[edx]).largura  ;bl tem a largura

    xor edx, edx; limpa registrador

    div ebx   ;al tem a linha

    mov esi, edx

    mov ebx, 6 ; número da linha é multiplicado pelo número de colunas da caixa da próxima peça
    mul ebx

    mov edx, esi

    add ax, dx

    xor ecx,ecx; limpa registrador
    mov edx, pTetromino

    mov cl, cor
    
    invoke desenharBlocoProx, hWin, hDC, cl, ax ; desenha o bloco na posição da proxima peça

    pop edx
    pop ecx
    pop ebx
    pop eax

    cont:
    inc ebx
    inc edx
    jmp forT

    fim:
    invoke destruirTetromino, pTetromino
    ret
desenharTetrominoProx endp

desenharBlocoProx proc hWin:DWORD, hDC:DWORD, cor:BYTE, posicao:WORD

    LOCAL hOld:DWORD
    LOCAL memDC :DWORD

    invoke CreateCompatibleDC,hDC

    mov memDC, eax
    invoke SelectObject,memDC,hBmp  ; selecionei o novo bitmap
    mov hOld, eax
    xor ebx, ebx; limpa registrador
    invoke getPixelProx, posicao
    mov bx, ax ; ebx coluna e eax linha
    shr eax, 16

    mov ecx, eax
    mov edx, ebx
    ; limpa registrador
    xor eax, eax     ; limpamos eax
    xor ebx, ebx     ; limpar ebx
    mov ebx, 32 
    mov al, cor     ; atribuímos o valor do tipo ao registrador de 8 bits, o al
    push edx
    mul ebx          ; multiplicamos o valor armazenado em al por 32
    pop edx
    mov ebx, eax     ; salvamos o valor de al em tipo, que será usado para o ponto de início do corte no eixo X
    INVOKE  TransparentBlt,hDC,edx,ecx,32,32,memDC,ebx,0,32,32,CREF_TRANSPARENT    ; cortamos a imagem  

    invoke SelectObject,hDC,hOld
    invoke DeleteDC,memDC

    return 0

desenharBlocoProx endp

getPixelProx proc pos:WORD
    ; limpa registradores
   ; limpa registradores
    xor edx, edx
    xor eax, eax

    mov ax, pos
    mov ecx, 6  ; recebe o tamanho em colunas da parte visual
    div ecx

    shl eax, 5  ; são 32 pixels por bloco, e como usamos o método para ter a posição visual é necessário multiplicar o número de linhas e de colunas
    shl edx, 5  ; por 32

    add eax, 321 ; inicio x do retangulo delimitado para a peça guardada
    add edx, 451    ; inicio y do retangulo delimitado para a peça guardada
    shl eax, 16     ; linha fica nos primeiros 16 bits e a coluna nos últimos 16 bits de eax
    mov ax, dx

    ret

getPixelProx endp