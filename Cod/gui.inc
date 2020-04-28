mov    hdc, eax
mov rect.top, 10
mov rect.bottom, 640
mov rect.left, 10
mov rect.right, 330 

invoke  Rectangle, hdc, rect.left, rect.top, rect.right, rect.bottom  

invoke  lstrlen, ADDR pecaSeguradaTxt
invoke  TextOut, hdc, 400, 20, ADDR pecaSeguradaTxt, eax

mov rect.top, 50
mov rect.bottom, 200
mov rect.left, 390
mov rect.right, 580 
invoke  Rectangle, hdc, rect.left, rect.top, rect.right, rect.bottom  


invoke  lstrlen, ADDR proxPecaTxt
invoke  TextOut, hdc, 400, 250, ADDR proxPecaTxt, eax

mov rect.top, 280
mov rect.bottom, 450
mov rect.left, 390
mov rect.right, 580 
invoke  Rectangle, hdc, rect.left, rect.top, rect.right, rect.bottom  

invoke  lstrlen, ADDR pontuacaoTxt
invoke  TextOut, hdc, 400, 480, ADDR pontuacaoTxt, eax