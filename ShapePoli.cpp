#include "ShapePoli.h"
#include "Ponto.h"
#include "Poligono.h"
#include <vector>
#include <math.h>

ShapePoli::ShapePoli(){}

ShapePoli::ShapePoli(Poligono corpo, int cor[3], GLenum mode){
    this->corpo = corpo;
    this->cor[0] = cor[0];
    this->cor[1] = cor[1];
    this->cor[2] = cor[2];
    this->mode = mode;
    this->position = Ponto(0,0);
    this->rotation = 0.0;
    this->escala = Ponto(1,1);

    atualizaAABB();
}

void ShapePoli::alteraCorpo(Poligono corpo){
    this->corpo = Poligono();

    for(int i = 0; i < corpo.getNVertices(); i++){
        this->corpo.insereVertice(Ponto(corpo.getVertice(i).x, corpo.getVertice(i).y));
    }

    atualizaAABB();
}

void ShapePoli::draw(){
    glPushMatrix();
    {
        glTranslatef(position.x, position.y, 0);
        glScalef(escala.x, escala.y, escala.z);
        glRotatef(rotation, 0, 0, 1);

        glBegin(mode);

        // rotaciona no meio
        //glTranslatef(corpo.Max.x/2-0.8, 0, 0);
        //glTranslatef(-corpo.Max.x/2-0.8, 0, 0);
        glColor3f(cor[0], cor[1], cor[2]);
        for (int i=0; i<corpo.getNVertices(); i++){
            glVertex3f(corpo.getVertice(i).x,corpo.getVertice(i).y,corpo.getVertice(i).z);
        }

        glEnd();
    }
    glPopMatrix();
}

void ShapePoli::atualizaAABB(){
    Ponto Min, Max;
    corpo.obtemLimites(Min, Max);

    float somaX = 0.0;
    float somaY = 0.0;

    for(int i = 0; i < corpo.getNVertices(); i++){
        somaX += corpo.getVertice(i).x;
        somaY += corpo.getVertice(i).y;
    }

//    somaX = floor(somaX / 0.0001 + 0.5) * 0.1;
//    somaY = floor(somaY / 0.0001 + 0.5) * 0.1;
    somaX = floor(somaX * 1.0) / 1.0;
    somaY = floor(somaY * 1.0) / 1.0;


    float centroX = position.x + (somaX/corpo.getNVertices());
    float centroY = position.y + (somaY/corpo.getNVertices());

    centroX = ceil(centroX * 1.0) / 1.0;
    centroY = ceil(centroY * 1.0) / 1.0;

//    rotaciona ponto
//    float x = (centroX*cos(rotation))+(centroY*sin(rotation));
//    float y = (-centroX*sin(rotation))+(centroY*cos(rotation));
    colisionShape = {Ponto(centroX, centroY), Ponto((Max.x-Min.x)/2, (Max.y-Min.y)/2)};
}


bool ShapePoli::colide(Ponto p1, Ponto p2){
    AABB s2 = {p1,p2};

    return colide(s2);
}

bool ShapePoli::colide(AABB shape2){
    bool naoColidiuX = abs(int(colisionShape.centro.x - shape2.centro.x)) > colisionShape.meiaLargura.x + shape2.meiaLargura.x;
    bool naoColidiuY = abs(int(colisionShape.centro.y - shape2.centro.y)) > colisionShape.meiaLargura.y + shape2.meiaLargura.y;



//    cout << "centro 1: " << colisionShape.centro.x  << " , " << colisionShape.centro.y << " centro2: " <<  shape2.centro.x << " , " << shape2.centro.y << endl;
//    cout << "tam1: " << colisionShape.meiaLargura.x << " , " << colisionShape.meiaLargura.y << endl;
//    cout << "tam2: " << shape2.meiaLargura.x << " , " << shape2.meiaLargura.y << endl;

    if(naoColidiuX || naoColidiuY){
        return false;
    }

    return true;
}

float ShapePoli::getWidth(){
    Ponto p1, p2;

    this->corpo.obtemLimites(p1, p2);
    return p2.x - p1.x;
}

float ShapePoli::getHeight(){
    Ponto p1, p2;

    this->corpo.obtemLimites(p1, p2);
    return p2.y - p1.y;
}

void ShapePoli::setPosition(float x, float y){
//    colisionShape.centro.x += x - position.x;
//    colisionShape.centro.y += y - position.y;
    this->position = Ponto(x,y);
    atualizaAABB();
};

GLenum ShapePoli::getDrawMode(){
    return mode;
}
