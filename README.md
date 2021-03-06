# 2D-Shooter-Cpp-OpenGL
Jogo 2D criado usando C++ e OpenGL

<img src="https://github.com/GuilhermeHiago/2D-Shooter-Cpp-OpenGL/blob/main/img/capa.png" />

## Funcionamento do Jogo

* Há duas fileiras de inimigos no topo da tela, que realizam disparos aleatórios para baixo
* O jogo acaba por 3 possibilidades: 
  * Se o jogador destruir todos os inimigos (realizando disparos)
  * Se os inimigos atingirem o jogador 3 vezes 
  * Se os inimigos destruírem todos os prédios
* O jogo pode ser reiniciado no fim da partida

## Funcionamento do Jogador

* O jogador pode se mover para esquerda e direita usando os direcionais esquerda e direita do teclado
* Com os direcionais cima e baixo mudam o ângulo do veiculo (e consequentemente o ângulo de disparo)
* As teclas "b" e "n" alteram a força do disparo.
* A tecla "ESPAÇO" realiza é usada para realizar disparos.

## Características do Projeto

* Os arquivos de texto presentes no projeto representam os sprites utilizados. Os números representam as cores de cada pixel a ser pintado.
* O calculo dos disparos é realizado utilizando formulas da física real de disparo de projéteis.
