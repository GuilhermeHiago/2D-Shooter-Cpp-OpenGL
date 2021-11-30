// **********************************************************************
// PUCRS/Escola Polit�cnica
// COMPUTA��O GR�FICA
//
// Programa basico para criar aplicacoes 2D em OpenGL
//
// Guilherme Hiago Costa dos Santos
// guilherme.hiago@edu.pucrs.br
// **********************************************************************

// Para uso no Xcode:
// Abra o menu Product -> Scheme -> Edit Scheme -> Use custom working directory
// Selecione a pasta onde voce descompactou o ZIP que continha este arquivo.
//

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#define PI 3.14159265


using namespace std;

#ifdef WIN32
#include <windows.h>
#include <glut.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <glut.h>
#endif

#include "Ponto.h"
#include "Poligono.h"
#include "Shape.h"
#include "ShapeQuad.h"
#include "ShapePoli.h"
#include "Character.h"

#include "Temporizador.h"
Temporizador T;
double AccumDeltaT=0;

Poligono A, B, Uniao,Intersecao, DiferencaAB, DiferencaBA;

// Limites l�gicos da �rea de desenho
Ponto Min, Max;
Ponto Meio, Terco, Largura;

bool desenha = false;

float angulo=0.0;

const float SCALA = 0.25;


// **********************************************************************
//    Variaveis do jogo
// **********************************************************************
bool fim = false;
float tempo = 15;
bool mover = false;

int cor[4][3] = {{0, 0, 0}, {255, 0, 0}, {0, 255, 0}, {255, 255, 255}};

// variaveis jogador
Ponto Curva1[3];
float velDisparo = 68;
float TempoAni = 0.0;
Ponto Velocidades[1] = {Ponto(0,0)};
//Ponto Direcoes[1] = {Ponto(0,0)};
vector<Ponto> Direcoes = {Ponto(0,0)};
float pRotation = 0.0;

Character ch = Character(Ponto(30,30));
Character ch2 = Character(Ponto(50, 0));
vector<Character> personagens;

const int TOTAL_INIMIGOS = 12;
int NumInimigos = TOTAL_INIMIGOS;
float VelDisparoInimigo = 4;
Ponto curvasDisparosInimigos[TOTAL_INIMIGOS][3];
float delayTiroInimigos[TOTAL_INIMIGOS] = {};

// Variaveis predios
vector<Character> predios;

const int TOTAL_PREDIOS = 4;
int NumPredios = TOTAL_PREDIOS;

// Variaveis projeteis
vector<Character*> disparos2;
const int TOTAL_DISPAROS = 100;
bool criaDisparos = false;
vector<Character> disparos;
vector<vector<Ponto>> caminhos;
vector<float> TemposTotaisDisparos;
vector<float> TemposAnimacoes;
float TempoDaAnimacao = 0;
int total = 0;

bool animando = false;

// **********************************************************************
float CalculaAlcance(double velocidade, double angulo){
    // divide por 10 pois gravidade = 10ms
    return ((velocidade * velocidade * sin(2*angulo*PI/180))/10);
}
// **********************************************************************
float CalculaAltura(double velocidade, double angulo){
    // divide por 10 pois gravidade = 10ms
    return (velocidade * velocidade * sin(angulo*PI/180) * sin(angulo*PI/180))/ (10*2);
}
// **********************************************************************
//
// **********************************************************************
void LePoligono(const char *nome, Poligono &P)
{
    ifstream input;
    input.open(nome, ios::in);
    if (!input)
    {
        cout << "Erro ao abrir " << nome << ". " << endl;
        exit(0);
    }
    cout << "Lendo arquivo " << nome << "...";
    string S;
    int nLinha = 0;
    unsigned int qtdVertices;

    input >> qtdVertices;

    for (int i=0; i< qtdVertices; i++)
    {
        double x,y;
        // Le cada elemento da linha
        input >> x >> y;
        if(!input)
            break;
        nLinha++;

        P.insereVertice(Ponto(x,y));
    }
    cout << "Poligono lido com sucesso!" << endl;

}

// **********************************************************************
// Metodo que salva o poligono em um txt
void EscrevePoligono(const char *nome, Poligono &P){
	ofstream out(nome);

    out << P.getNVertices();
    out << "\n";

	for(int i = 0; i < P.getNVertices(); i++){
        out << P.getVertice(i).x;
        out << " ";
        out << P.getVertice(i).y;
        out << "\n";
	}
	out.close();
}


void LeShapeQuad(const char *nome, ShapeQuad &Sq)
{
    ifstream input;
    input.open(nome, ios::in);
    if (!input)
    {
        cout << "\nErro ao abrir " << nome << ". \n" << endl;
        exit(0);
    }
    cout << "Lendo arquivo " << nome << "...\n";
    string S;
    int nLinha = 0;
    int numLinhas, numColunas;

    input >> numLinhas >> numColunas;

    vector<vector<int>> corpo(numLinhas, vector<int>(numColunas));

    for (int i=0; i< numLinhas; i++)
    {
        // Le cada elemento da linha
        for(int j = 0; j < numColunas; j++){
            if(!input)
                break;
            input >> corpo[i][j];
        }

        nLinha++;

    }


    cout << "Poligono Quadricular lido com sucesso!\n" << endl;
    Sq = ShapeQuad(numLinhas, numColunas, corpo, GL_QUADS);
}
// **********************************************************************
ShapeQuad* LeShapeQuad(const char *nome)
{
    ifstream input;
    input.open(nome, ios::in);
    if (!input)
    {
        cout << "\nErro ao abrir " << nome << ". \n" << endl;
        exit(0);
    }
    cout << "Lendo arquivo " << nome << "...\n";
    string S;
    int nLinha = 0;
    int numLinhas, numColunas;

    input >> numLinhas >> numColunas;

    vector<vector<int>> corpo(numLinhas, vector<int>(numColunas));

    for (int i=0; i< numLinhas; i++)
    {
        // Le cada elemento da linha
        for(int j = 0; j < numColunas; j++){
            if(!input)
                break;
            input >> corpo[i][j];
        }

        nLinha++;

    }

    cout << "Poligono Quadricular lido com sucesso!\n" << endl;
    return new ShapeQuad(numLinhas, numColunas, corpo, GL_QUADS);
}
// **********************************************************************
Poligono GenerateCircle(float cx, float cy, float r, int num_segments) {
    Poligono p = Poligono();

    for (int ii = 0; ii < num_segments; ii++)   {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle
        float x = r * cosf(theta);//calculate the x component
        float y = r * sinf(theta);//calculate the y component
        p.insereVertice(Ponto(cx+x, cy+y));
        //glVertex2f(x + cx, y + cy);//output vertex
    }

    return p;
}
// **********************************************************************
void GeraDisparos(){
    for(int i = 0; i < TOTAL_DISPAROS; i++){
        disparos2.push_back(new Character(Ponto(-100,-100), vector<Shape*> {new ShapePoli(GenerateCircle(0,0,2,10), new int[3]{0,1,0}, GL_POLYGON)})); // {LeShapeQuad("Inimigo1.txt")}));
        disparos2[i]->vida = 0;
        TemposAnimacoes.push_back(0);
        TemposTotaisDisparos.push_back(0);
        caminhos.push_back(vector<Ponto> {Ponto(-100,-100),Ponto(-100,-100),Ponto(-100,-100)});
        //TemposTotaisDisparos.push_back((2*velDisparo*sin((90+pRotation)*PI/180))/10); // 68
    }
}

void restart(){
    // **********************************************************************
    //    Variaveis do jogo
    // **********************************************************************
    fim = false;
    mover = false;

    // variaveis jogador
    Ponto Curva1[3];
//    float velDisparo = 68;
    TempoAni = 0.0;
    Velocidades[1] = Ponto(0,0);
    //Ponto Direcoes[1] = {Ponto(0,0)};
    Direcoes = vector<Ponto>{Ponto(0,0)};
    pRotation = 0.0;

    ch = Character(Ponto(30,30));
    ch2 = Character(Ponto(50, 0));
    personagens = vector<Character>{};
    NumInimigos = TOTAL_INIMIGOS;

    //curvasDisparosInimigos = Ponto[TOTAL_INIMIGOS][3];
    for(int i = 0; i < TOTAL_INIMIGOS; i++){
        curvasDisparosInimigos[i][0] = Ponto(0,0);
        curvasDisparosInimigos[i][1] = Ponto(0,0);
        curvasDisparosInimigos[i][2] = Ponto(0,0);
    }

    // Variaveis predios
    NumPredios = TOTAL_PREDIOS;
    predios = vector<Character>{};

    // Variaveis projeteis
    disparos2 = vector<Character*>{};

    criaDisparos = false;
    caminhos = vector<vector<Ponto>>{};
    TemposTotaisDisparos = vector<float>{};
    TemposAnimacoes = vector<float>{};
    TempoDaAnimacao = 0;
    total = 0;

    for(int i = 0; i < TOTAL_INIMIGOS; i++){
        delayTiroInimigos[i] = 0.0;
    }

    animando = false;
}
// **********************************************************************
//
// **********************************************************************
void init()
{
    Ponto MinAux, MaxAux;

    // Define a cor do fundo da tela (AZUL)
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    // Atualiza os limites globais ap�s cada leitura
    Min = Ponto(0,0);
    Max = Ponto(1000*SCALA, 500*SCALA);

    cout << "Limites Globais" << endl;
    cout << "\tMinimo:"; Min.imprime();
    cout << "\tMaximo:"; Max.imprime();
    cout <<  endl;

    // Ajusta a largura da janela l�gica
    // em fun��o do tamanho dos pol�gonos
    Largura.x = Max.x-Min.x;
    Largura.y = Max.y-Min.y;

    // Calcula 1/3 da largura da janela
    Terco = Largura;
    double fator = 1.0/3.0;
    Terco.multiplica(fator, fator, fator);

    // Calcula 1/2 da largura da janela
    Meio.x = (Max.x+Min.x)/2;
    Meio.y = (Max.y+Min.y)/2;
    Meio.z = (Max.z+Min.z)/2;

    //------------------
    // iniciando personagens
    //------------------
    Velocidades[0].x = Max.x/(tempo);///0.27;
    Velocidades[0].y = Max.y/(tempo);

    //------------------
    // configura jogador

    vector<Shape*> vec = {new ShapeQuad(), new ShapePoli(), new ShapePoli()};
    vec[1]->position.x = 10;

    vec[0] = LeShapeQuad("Player.txt");


    Ponto p = Ponto((vec[0]->getWidth()/2), (vec[0]->getHeight()));

    int soma = 6;

    DiferencaAB.insereVertice(Ponto(p.x-0.5, p.y));
    DiferencaAB.insereVertice(Ponto(p.x-0.5, p.y+soma));
    DiferencaAB.insereVertice(Ponto(p.x+0.5, p.y+soma));
    DiferencaAB.insereVertice(Ponto(p.x+0.5, p.y));

    DiferencaBA.insereVertice(Ponto(p.x-1, p.y+soma));
    DiferencaBA.insereVertice(Ponto(p.x, p.y+soma+2));
    DiferencaBA.insereVertice(Ponto(p.x+1, p.y+soma));

    vec[1] = new ShapePoli(DiferencaAB, new int[3]{0,0,1}, GL_POLYGON);
    vec[2] = new ShapePoli(DiferencaBA, new int[3]{0,0,1}, GL_POLYGON);

    ch = Character(Ponto(0,0), vec);
    ch.rotation = 0.0;

    ch2 = Character(Ponto(0, 110), vector<Shape*> {LeShapeQuad("Inimigo2.txt")});

    personagens = {ch};

    personagens[0].vida = 3;

    int somaX = 20;
    int somaY = 10;
    int ultimoX = 20;

    for(int i = 0; i < (TOTAL_INIMIGOS)/3; i++){
        personagens.push_back(Character(Ponto(ultimoX+somaX, 100 + somaY), vector<Shape*> {LeShapeQuad("Inimigo2.txt")}));

        ultimoX += somaX;

        personagens.push_back(Character(Ponto(ultimoX+somaX, 100 + somaY), vector<Shape*> {LeShapeQuad("Inimigo3.txt")}));

        ultimoX += somaX;

        personagens.push_back(Character(Ponto(ultimoX+somaX, 100 + somaY), vector<Shape*> {LeShapeQuad("Inimigo1.txt")}));

        ultimoX += somaX;
        somaY *= -1;
    }

    // define layer de colisao e direcoes inimigos
    for(int i = 0; i < TOTAL_INIMIGOS; i++){
        Direcoes.push_back(Ponto(-1, 0));
        //personagens[i+1].layers = vector<int> {1};
        personagens[i+1].layers[0] = 1;
    }

    //------------------
    // configura predios

    vector<Shape*> vec2 = {LeShapeQuad("Predio3.txt"), new ShapePoli()};

    A.insereVertice(Ponto(vec2[0]->position.x-2, vec2[0]->position.y + vec2[0]->getHeight()));
    A.insereVertice(Ponto(vec2[0]->position.x + vec2[0]->getWidth() -1, vec2[0]->position.y + vec2[0]->getHeight()));
    A.insereVertice(Ponto(vec2[0]->position.x + ((vec2[0]->getWidth()-1)/2)-1, vec2[0]->position.y + vec2[0]->getHeight() + 10));

    vec2[1] = new ShapePoli(A, new int[3]{1,0,0}, GL_POLYGON);

    Character pred1 = Character(Ponto(100, 0), vec2);
    //pred1.layers[0] = 3;
    predios = {pred1};
    predios[0].vida = 1;

    predios.push_back(Character(Ponto(170, 0), vector<Shape*>{LeShapeQuad("Predio2.txt")}));
    predios.push_back(Character(Ponto(190, 0), vector<Shape*>{LeShapeQuad("Predio2.txt")}));
    predios.push_back(Character(Ponto(210, 0), vector<Shape*>{LeShapeQuad("Predio1.txt")}));

    for(Character &p : predios){
        p.layers = vector<int>{3};
    }



    //------------------
    // inicia canhao
    //------------------
    Curva1[0] = Ponto(personagens[0].position.x + (personagens[0].shapes[0]->getWidth()/2), personagens[0].position.y + (personagens[0].shapes[0]->getHeight()));
    Curva1[1] = Ponto(personagens[0].position.x + (personagens[0].shapes[0]->getWidth()/2)+20, personagens[0].position.y + (personagens[0].shapes[0]->getHeight())+30);
    Curva1[2] = Ponto(personagens[0].position.x + (personagens[0].shapes[0]->getWidth()/2)+40, 0);

    //------------------
    // inicia disparos
    //------------------
    GeraDisparos();

    //------------------
    // inicia trajetorias dos disparos inimigos
    //------------------
    for(int i = 0; i < TOTAL_INIMIGOS; i++){
        curvasDisparosInimigos[i][0] = Ponto(0,0);
        curvasDisparosInimigos[i][1] = Ponto(0,0);
        curvasDisparosInimigos[i][2] = Ponto(0,0);
    }
}

double nFrames=0;
double TempoTotal=0;
// **********************************************************************
//
// **********************************************************************
void AtualizaMiraJogador(){

    float alc = CalculaAlcance(velDisparo, 90+pRotation);
    float alt = CalculaAltura(velDisparo, 90+pRotation);

    DiferencaAB = Poligono();
    DiferencaBA = Poligono();
    // Ponto p = Ponto(ch.position.x + (ch.shapes[0]->getWidth()/2), ch.position.y + (ch.shapes[0]->getHeight()));
    Ponto p = Ponto((personagens[0].shapes[0]->getWidth()/2), (personagens[0].shapes[0]->getHeight()));

    int soma = 0;

    if(velDisparo == 68){
        soma = 6;
    }
    else{
        soma = 3;
    }

    DiferencaAB.insereVertice(Ponto(p.x-0.5, p.y));
    DiferencaAB.insereVertice(Ponto(p.x-0.5, p.y+soma));
    DiferencaAB.insereVertice(Ponto(p.x+0.5, p.y+soma));
    DiferencaAB.insereVertice(Ponto(p.x+0.5, p.y));

    DiferencaBA.insereVertice(Ponto(p.x-1, p.y+soma));
    DiferencaBA.insereVertice(Ponto(p.x, p.y+soma+2));
    DiferencaBA.insereVertice(Ponto(p.x+1, p.y+soma));

    ((ShapePoli*)personagens[0].shapes[1])->alteraCorpo(DiferencaAB);
    ((ShapePoli*)personagens[0].shapes[2])->alteraCorpo(DiferencaBA);

    Curva1[0] = Ponto(personagens[0].position.x + (personagens[0].shapes[0]->getWidth()/2), personagens[0].position.y + (personagens[0].shapes[0]->getHeight()));
    Curva1[1] = Ponto(personagens[0].position.x + (personagens[0].shapes[0]->getWidth()/2) + (alc/2), personagens[0].position.y + (personagens[0].shapes[0]->getHeight())+alt);
    Curva1[2] = Ponto(personagens[0].position.x + (personagens[0].shapes[0]->getWidth()/2)+alc, 0);
    //cout << "altura flexa: " << personagens[0].shapes[1]->getHeight();
//        personagens[0].shapes[1] = new ShapePoli(DiferencaAB, new int[3]{0,0,1}, GL_POLYGON);
//        personagens[0].shapes[2] = new ShapePoli(DiferencaBA, new int[3]{0,0,1}, GL_POLYGON);
}
// **********************************************************************
void AtualizaMiraInimigos(){
    for(int i = 1; i < personagens.size(); i++){
        if(personagens[i].vida > 0){
            float alc = CalculaAlcance(68, 175);
            float alt = CalculaAltura(68, 175);

            Ponto p = Ponto(personagens[i].position.x - (personagens[i].shapes[0]->getWidth()/2), personagens[i].position.y + (personagens[i].shapes[0]->getHeight())/2);

            curvasDisparosInimigos[i-1][0] = Ponto(personagens[i].position.x + (personagens[i].shapes[0]->getWidth()/2), personagens[i].position.y + (personagens[i].shapes[0]->getHeight()/2));
            curvasDisparosInimigos[i-1][1] = Ponto(personagens[i].position.x + (personagens[i].shapes[0]->getWidth()/2) + (alc/2), personagens[i].position.y + (personagens[i].shapes[0]->getHeight())+alt);
            curvasDisparosInimigos[i-1][2] = Ponto(personagens[i].position.x + (personagens[i].shapes[0]->getWidth()/2)+alc, 0);
        }
    }
}
// **********************************************************************
void AvancaJogador(double dt)
{
    Ponto Deslocamento;
    Deslocamento.x = dt * Velocidades[0].x * Direcoes[0].x;
    Deslocamento.y = dt * Velocidades[0].y * Direcoes[0].y;

    Ponto aux = Ponto(personagens[0].position.x + Deslocamento.x, personagens[0].position.y + Deslocamento.y);
    personagens[0].setPosition(aux);

    if(personagens[0].position.x+20 < (Min.x) && Direcoes[0].x != 1){
        //Direcoes[0].x *= -1;
        personagens[0].setPosition(Ponto(Max.x, 0));
    }

    if(personagens[0].position.x > (Max.x) && TempoAni > 0){
        //Direcoes[0].x *= -1;
        personagens[0].setPosition(Ponto(Min.x-20, 0));
        TempoAni = 0;
    }

    AtualizaMiraJogador();
}
// **********************************************************************
//
// **********************************************************************
Ponto CalculaBezier3(Ponto PC[], double t)
{
    Ponto P;
    double UmMenosT = 1-t;

    P =  PC[0] * UmMenosT * UmMenosT + PC[1] * 2 * UmMenosT * t + PC[2] * t*t;
    return P;
}
// **********************************************************************
void TracaBezier3Pontos(Ponto Curva[])
{
    double t=0.0;
    double DeltaT = 1.0/10;

    glBegin(GL_LINE_STRIP);
    while(t<0.7)
    {
        Ponto P = CalculaBezier3(Curva, t);
        glVertex2f(P.x, P.y);

        t += DeltaT;
    }
    glEnd();
}
// **********************************************************************
void AvancaDisparos(){
//    cout << "aqui1\n";
    for(int i = 0; i < disparos2.size(); i++){
        if(disparos2[i]->vida <= 0){
            continue;
        }

        double t;
        t = TemposAnimacoes[i]/TemposTotaisDisparos[i];
        if (t>1.0)
        {
            // zera os valores do disparo e remove da tela
            disparos2[i]->vida = -1;
            //caminhos[i] = vector<Ponto> {Ponto(0,0), Ponto(0,0), Ponto(0,0)};
            caminhos[i][0] = Ponto(-100,-100);
            caminhos[i][1] = Ponto(-100,-100);
            caminhos[i][2] = Ponto(-100,-100);
            TemposAnimacoes[i] = 0;
            TemposTotaisDisparos[i] = 0;
            total--;
        }else{
            Ponto p[3] = {caminhos[i][0], caminhos[i][1], caminhos[i][2]};
            //Ponto[3]{caminhos[i][0], caminhos[i][1], caminhos[i][2]}
            disparos2[i]->setPosition(CalculaBezier3(p, t));
        }
    }
//    cout << "aqui2\n\n";
}
// **********************************************************************
void AvancaInimigos(double dt)
{

    for(int i = 1; i < personagens.size(); i++){
        if(personagens[i].vida > 0){
            Ponto Deslocamento;
            // velocidade.x == Max.x/(tempo)
            Deslocamento.x = dt * Velocidades[0].x * Direcoes[i].x;
            Deslocamento.y = dt * Velocidades[0].y * Direcoes[i].y;

            Ponto aux = Ponto(personagens[i].position.x + Deslocamento.x, personagens[i].position.y + Deslocamento.y);
            personagens[i].setPosition(aux);


            if(personagens[i].position.x+20 < (Min.x) && Direcoes[i].x != 1){
                int maior = 2;
                int menor = 1;
                int aleatorio = rand()%(maior-menor+1) + menor;

                bool troca = true;

                for(int p = 1; p < TOTAL_INIMIGOS; p++){
                    if(personagens[p].position.x <= Max.x && personagens[p].position.x > Max.x-10){
                        troca = false;
                        break;
                    }
                }

                if(troca){
                    personagens[i].setPosition(Ponto(Max.x, personagens[i].position.y));

                    if(aleatorio == 1){
                        if(personagens[i].position.y == 110){
                            personagens[i].setPosition(Ponto(Max.x, 90));
                        }
                        else{
                            personagens[i].setPosition(Ponto(Max.x, 110));
                        }
                    }
                }
            }

            if(personagens[i].position.x > (Max.x) && TempoAni > 0){
                //Direcoes[0].x *= -1;
                personagens[i].setPosition(Ponto(Min.x-20, personagens[i].position.y));
                TempoAni = 0;
            }

            if(delayTiroInimigos[i-1] > 0){
                delayTiroInimigos[i-1] -= dt;
            }
        }
    }

}
// **********************************************************************
void CriaDisparoPersonagem()
{
     // cria disparos do jogador
    if(criaDisparos){
        for(int i = 0; i < disparos2.size(); i++){
            if(disparos2[i]->vida <= 0){
                Ponto p = personagens[0].shapes[0]->colisionShape.centro;
                p.y += personagens[0].shapes[0]->getHeight()/2;
                disparos2[i]->setPosition(Ponto(personagens[0].shapes[0]->colisionShape.centro.x, personagens[0].shapes[0]->colisionShape.centro.y));
                TemposAnimacoes[i] = 0;;
                //caminhos[i] = vector<Ponto> {Curva1[0], Curva1[1], Curva1[2]};
                caminhos[i][0] = Curva1[0];
                caminhos[i][1] = Curva1[1];
                caminhos[i][2] = Curva1[2];
                TemposTotaisDisparos[i] = (2*velDisparo*sin((90+pRotation)*PI/180))/10; // 68
                disparos2[i]->layers = vector<int>{1, 3};
                disparos2[i]->vida = 1;

                criaDisparos = false;
                animando = true;
                total++;
                break;
            }
        }
    }
}
// **********************************************************************
void CriaDisparoInimigos()
{
    for(int i = 1; i < personagens.size(); i++){
        // se personagem desativado ou em delay de disparo
        if(personagens[i].vida <= 0 || delayTiroInimigos[i-1] > 0){
            continue;
        }

        int maior = 300;
        int menor = 1;
        int aleatorio = rand()%(maior-menor+1) + menor;

        if(aleatorio == 1 && personagens[i].vida > 0){
            for(int j = 0; j < disparos2.size(); j++){
                if(disparos2[j]->vida <= 0){
                    int vel = 4.0 / ((2*sin((175)*PI/180))/10);

                    delayTiroInimigos[i-1] = 1;
                    Ponto p = personagens[i].shapes[0]->colisionShape.centro;
                    p.y += personagens[i].shapes[0]->getHeight()/2;
                    disparos2[j]->setPosition(Ponto(personagens[i].shapes[0]->colisionShape.centro.x-1, personagens[i].shapes[0]->colisionShape.centro.y));
                    TemposAnimacoes[j] = 0;
                    TemposTotaisDisparos[j] = VelDisparoInimigo; // 68 // era x2 mas ja esta no mais alto
                    caminhos[j] = vector<Ponto> {curvasDisparosInimigos[i-1][0], curvasDisparosInimigos[i-1][1], curvasDisparosInimigos[i-1][2]};
                    disparos2[j]->layers = {0,3};
                    disparos2[j]->vida = 1;

                    total++;
                    break;
                }
            }
        }
    }
}
// **********************************************************************
void VerificaColisaoDisparos()
{
        // caso exita disparo em funcionamento
    if(total > 0){
        // verifica todos os disparos
        for(Character *d : disparos2){
            // se a vida estiver zerada nao faz nada
            if(d->vida <= 0 || d->position.x > Max.x || d->position.x < Min.x || d->position.y > Max.y || d->position.y < Min.y){
                continue;
            }

            // verifica colisão com predios
            for(Character &p : predios){
                // se vida do disparo esta zerada pula este
                if(d->vida <= 0){
                    break;
                }

                // se vida do predio esta zerada pula este
                if(p.vida <= 0)
                    continue;

                // caso disparo colida diminui a vida de ambos
                if(d->colide(p)){
                    p.vida -= 1;

                    total--;
                    d->vida--;
                    d->setPosition(Ponto(-100,-100));
                    NumPredios--;
                    cout << "quantidade predios: " << NumPredios << endl;
                }
            }

            // verifica colisao com inimigos
            for(int i = 0; i < personagens.size(); i++){
                if(d->vida <= 0){
                    break;
                }

                if(personagens[i].vida <= 0)
                    continue;


                if(i == 0 && d->colide(personagens[i])){

                    // confirma colisao com jogador
                    float difX = abs(personagens[i].shapes[0]->colisionShape.centro.x - d->shapes[0]->colisionShape.centro.x);
                    float difY = abs(personagens[i].shapes[0]->colisionShape.centro.y - d->shapes[0]->colisionShape.centro.y);

                    if(d->position.x == -100){
                        continue;
                    }

                    if(difX > personagens[i].shapes[0]->colisionShape.meiaLargura.x + d->shapes[0]->colisionShape.meiaLargura.x){
                        continue;
                    }

                    if(difY > personagens[i].shapes[0]->colisionShape.meiaLargura.y + d->shapes[0]->colisionShape.meiaLargura.y){
                        continue;
                    }

                    cout << "centro 1: " << personagens[i].shapes[0]->colisionShape.centro.x  << " , " << personagens[i].shapes[0]->colisionShape.centro.y << " centro2: " <<  d->shapes[0]->colisionShape.centro.x << " , " << d->shapes[0]->colisionShape.centro.y << endl;
                    cout << "tam1: " << personagens[i].shapes[0]->colisionShape.meiaLargura.x << " , " << personagens[i].shapes[0]->colisionShape.meiaLargura.y << endl;
                    cout << "tam2: " << d->shapes[0]->colisionShape.meiaLargura.x << " , " << d->shapes[0]->colisionShape.meiaLargura.y << endl;

                    personagens[i].vida--;
                    d->vida--;
                    d->setPosition(Ponto(-100,-100));

                    cout << "\ncolidiu com player: " << personagens[0].vida << "\n";
                    cout << "pos: "; personagens[0].position.imprime();cout << endl;
                    cout << "pos diparo: "; d->position.imprime();cout << endl;
                }
                else if(i > 0 && d->colide(personagens[i])){
                    cout << "\n\nMATOU: "  << i << "\n\n";

                    personagens[i].vida--;
                    d->vida--;
                    d->setPosition(Ponto(-100,-100));

                    // se o personagem for inimigo
                    curvasDisparosInimigos[i-1][0] = Ponto(-100,-100);
                    curvasDisparosInimigos[i-1][1] = Ponto(-100,-100);
                    curvasDisparosInimigos[i-1][2] = Ponto(-100,-100);

                    total--;
                    NumInimigos--;
                }
            }
        }
    }
}
// **********************************************************************
void animate()
{
    double dt;
    dt = T.getDeltaT();
    AccumDeltaT += dt;
    TempoTotal += dt;
    nFrames++;

    if (AccumDeltaT > 1.0/30) // fixa a atualiza��o da tela em 30
    {
        AccumDeltaT = 0;
        //angulo+=0.05;
        glutPostRedisplay();
    }
    if (TempoTotal > 5.0)
    {
        // cout << "Tempo Acumulado: "  << TempoTotal << " segundos. " ;
        // cout << "Nros de Frames sem desenho: " << nFrames << endl;
        // cout << "FPS(sem desenho): " << nFrames/TempoTotal << endl;
        TempoTotal = 0;
        nFrames = 0;
    }

    if(fim)
        return;


    AtualizaMiraJogador();
    AtualizaMiraInimigos();
    AvancaInimigos(dt);
    //Direcoes[0].x != 0
    if(mover){
        AvancaJogador(dt);
        TempoAni += dt;
    }

    // animando
    if(total > 0){
        //AvancaComBezier(disparos[0], new Ponto[3]{caminhos[0][0], caminhos[0][1], caminhos[0][2]}, TemposAnimacoes[0]);
        AvancaDisparos();

        for(float &t : TemposAnimacoes){
            t += dt;
        }
        //TemposAnimacoes[0] += dt;
    }
}
// **********************************************************************
//  void reshape( int w, int h )
//  trata o redimensionamento da janela OpenGL
//
// **********************************************************************
void reshape( int w, int h )
{
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define a area a ser ocupada pela area OpenGL dentro da Janela
    glViewport(0, 0, w, h);
    // Define os limites logicos da area OpenGL dentro da Janela
    glOrtho(Min.x,Max.x,
            Min.y,Max.y,
            0,1);

//	// Especifica a projeção perspectiva
//	gluPerspective(90,1000,0.1,500);
//
//	// Especifica posição do observador e do alvo
//	gluLookAt(150,150,1, 150,150,1, 0,1,0);


    cout << "max: " << Max.x << endl;
    cout << "width: " << w << endl;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}
// **********************************************************************
void CalculaPonto(Ponto p, Ponto &out) {

    GLfloat ponto_novo[4];
    GLfloat matriz_gl[4][4];
    int  i;

    glGetFloatv(GL_MODELVIEW_MATRIX,&matriz_gl[0][0]);

    for(i=0; i<4; i++) {
        ponto_novo[i] = matriz_gl[0][i] * p.x +
                        matriz_gl[1][i] * p.y +
                        matriz_gl[2][i] * p.z +
                        matriz_gl[3][i];
    }
    out.x = ponto_novo[0];
    out.y = ponto_novo[1];
    out.z = ponto_novo[2];

}
// **********************************************************************
//  void display( void )
//
// **********************************************************************
void display( void )
{
	// Limpa a tela coma cor de fundo
    //	glClear(GL_COLOR_BUFFER_BIT);
    //GLbitfielb
    //0.25,0.4,0.9
    glClearColor(1,1,1,0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Define os limites l�gicos da �rea OpenGL dentro da Janela
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

//	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//	// Coloque aqui as chamadas das rotinas que desenham os objetos
//	// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//    glScalef(0.33, 0.5, 1);
//    glScalef(0.27, 0.5, 1);
    //NumInimigos > 0 && personagens[0].vida > 0

    glPushMatrix();

    // desenha perdios
    for(Character &p : predios){
        if(p.vida > 0){
            p.draw();
        }
    }

    // desenha personagens
    for(Character &p : personagens){
        if(p.vida > 0){
            p.draw();
        }
    }

    // desenha mira do jogador
    glColor3f(0.3,0.4,1);
    //TracaBezier3Pontos(Curva1);

    // desenha mira dos inimigos
    for(int i = 1; i < personagens.size(); i++){
        if(personagens[i].vida > 0){
            //TracaBezier3Pontos(new Ponto[3]{curvasDisparosInimigos[i-1][0], curvasDisparosInimigos[i-1][1], curvasDisparosInimigos[i-1][2]});
        }
    }

    // desenha disparos
    if(total > 0){
        for(int i = 0; i < disparos2.size(); i++){
            if(disparos2[i]->vida > 0){
                disparos2[i]->draw();
            }
        }
    }

    if(NumInimigos <= 0){
        glRasterPos2i(100, 75);
        glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"Voce Ganhou!!!");
        fim = true;
    }

    if(personagens[0].vida <= 0 || NumPredios <= 0){
        glRasterPos2i(100, 75);
        glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
        glColor3f(1.0f, 1.0f, 0.0f);
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"Voce Perdeu!!!");
        fim = true;
    }
    else if(!fim){
        personagens[0].rotation = pRotation;

        // cria disparos personagem
        CriaDisparoPersonagem();
        // cria disparos dos inimigos
        CriaDisparoInimigos();

        VerificaColisaoDisparos();
    }

    glPopMatrix();

	glutSwapBuffers();
}
// **********************************************************************
// ContaTempo(double tempo)
//      conta um certo n�mero de segundos e informa quanto frames
// se passaram neste per�odo.
// **********************************************************************
void ContaTempo(double tempo)
{
    Temporizador T;

    unsigned long cont = 0;
    cout << "Inicio contagem de " << tempo << "segundos ..." << flush;
    while(true)
    {
        tempo -= T.getDeltaT();
        cont++;
        if (tempo <= 0.0)
        {
            cout << "fim! - Passaram-se " << cont << " frames." << endl;
            break;
        }
    }

}
// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
//
// **********************************************************************

void keyboard ( unsigned char key, int x, int y )
{

	switch ( key )
	{
		case 27:        // Termina o programa qdo
			exit ( 0 );   // a tecla ESC for pressionada
			break;
        case 't':
            ContaTempo(3);
            break;
        case ' ':
            //desenha = !desenha;
            if(total < TOTAL_DISPAROS){
                criaDisparos = true;
            }
            break;
            //break;
        case 'b':
            if(velDisparo > 63)
                velDisparo -= 5;
            break;
        case 'n':
            if(velDisparo < 68)
                velDisparo += 5;
            break;
        case 'r':
            if(fim){
                restart();
                init();
            }
            break;
		default:
			break;
	}
}
// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )
//
//
// **********************************************************************
void arrow_keys ( int a_keys, int x, int y )
{
	switch ( a_keys )
	{
		case GLUT_KEY_UP:       // Se pressionar UP
            if(pRotation < 80)
                pRotation += 1;

			//glutFullScreen ( ); // Vai para Full Screen
			break;
	    case GLUT_KEY_DOWN:     // Se pressionar UP
								// Reposiciona a janela
            //glutPositionWindow (50,50);
			//glutReshapeWindow ( 700, 500 );

			if(pRotation > -80)
                pRotation -= 1;

			break;
        case GLUT_KEY_LEFT:
            if(mover){
                mover = false;
            }
            else{
                Direcoes[0].x = -1;
                mover = true;
            }
            break;
        case GLUT_KEY_RIGHT:
            if(mover){
                mover = false;
            }
            else{
                Direcoes[0].x = 1;
                mover = true;
            }
            break;
		default:
			break;
	}
}

void mouseInput(int button, int state, int x, int y){
    cout << button << state << x << y << endl;

    if(button == GLUT_LEFT_BUTTON){
        if(state == GLUT_DOWN)
            cout << "subiu";
        else
            cout << "desceu";
    }

};

// **********************************************************************
//  void main ( int argc, char** argv )
//
// **********************************************************************
int  main ( int argc, char** argv )
{
    cout << "Programa OpenGL" << endl;

    // inicia semente aleatoria
    srand((unsigned)time(0));

    glutInit            ( &argc, argv );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    glutInitWindowPosition (0,0);

    // Define o tamanho inicial da janela grafica do programa
    glutInitWindowSize  ( 1000, 500);

    // Cria a janela na tela, definindo o nome da
    // que aparecera na barra de titulo da janela.
    glutCreateWindow    ( "2D Shooter" );

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );

    // executa algumas inicializa��es
    init ();

    // Define que o tratador de evento para
    // o redesenho da tela. A funcao "display"
    // ser� chamada automaticamente quando
    // for necess�rio redesenhar a janela
    glutDisplayFunc ( display );

    // Define que o tratador de evento para
    // o invalida��o da tela. A funcao "display"
    // ser� chamada automaticamente sempre que a
    // m�quina estiver ociosa (idle)
    glutIdleFunc(animate);

    // Define que o tratador de evento para
    // o redimensionamento da janela. A funcao "reshape"
    // ser� chamada automaticamente quando
    // o usu�rio alterar o tamanho da janela
    glutReshapeFunc ( reshape );

    // Define que o tratador de evento para
    // as teclas. A funcao "keyboard"
    // ser� chamada automaticamente sempre
    // o usu�rio pressionar uma tecla comum
    glutKeyboardFunc ( keyboard );

    // Define que o tratador de evento para
    // as teclas especiais(F1, F2,... ALT-A,
    // ALT-B, Teclas de Seta, ...).
    // A funcao "arrow_keys" ser� chamada
    // automaticamente sempre o usu�rio
    // pressionar uma tecla especial
    glutSpecialFunc ( arrow_keys );


    glutMouseFunc(mouseInput);
    // inicia o tratamento dos eventos
    glutMainLoop ();

    return 0;
}
