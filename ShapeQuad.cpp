#include "ShapeQuad.h"
#include "Ponto.h"
#include <vector>
#include <math.h>
#include <iostream>

ShapeQuad::ShapeQuad(){}

ShapeQuad::ShapeQuad(int numLinhas, int numColunas, vector<vector<int>> corpo, GLenum mode){
    this->numLinhas = numLinhas;
    this->numColunas = numColunas;
    this->corpo = corpo;
    this->mode = mode;
    this->position = Ponto(0,0);
    this->rotation = 0.0;
    this->escala = Ponto(1,1);
    atualizaAABB();
    //this->colisionShape = {Ponto(), Ponto()};
}

//Shape::Shape(int numColunas, vector<vector<int>> corpo, GLenum mode, int color[3]){
//    this->numLinhas = 1;
//    this->numColunas = numColunas;
//    this->corpo = corpo;
//    this->mode = mode;
//    this->color = color;
//    this->position = Ponto(0,0);
//}

void ShapeQuad::draw(){
    //cout << "\ndesenha shapequad =" << this->position.x << " , " << this->position.y << "\n";
    glPushMatrix();
    {
        glTranslatef(this->position.x, this->position.y, 0);
        glScalef(escala.x, escala.y, escala.z);

        // rotaciona no meio
        glTranslatef(numColunas/2-0.8, 0, 0);
        glRotatef(rotation, 0, 0, 1);
        glTranslatef(-numColunas/2-0.8, 0, 0);

        glBegin(mode);

        if(mode == GL_QUADS){
            for (int i=0; i<numLinhas; i++){
                for (int j=0; j<numColunas; j++){
                    if(colors[corpo[i][j]-1][0] == 255 && colors[corpo[i][j]-1][1] == 255 && colors[corpo[i][j]-1][2] == 255){
                        glColor4f(colors[corpo[i][j]-1][0], colors[corpo[i][j]-1][1], colors[corpo[i][j]-1][2], 0.0);
                    }
                    else{
                        glColor3f(colors[corpo[i][j]-1][0], colors[corpo[i][j]-1][1], colors[corpo[i][j]-1][2]);
                    }

                    glVertex2f(j+1, (numLinhas-1-i));
                    glVertex2f(j+1, (numLinhas-i));
                    glVertex2f(j, (numLinhas-i));
                    glVertex2f(j, (numLinhas-1-i));
                }
            }
        }

        glEnd();
        //glTranslatef(-4, -4, 0);
    }
    glPopMatrix();
}

void ShapeQuad::atualizaAABB(){
    float centroX = position.x + (numColunas/2);
    float centroY = position.y + (numLinhas/2);

//    rotaciona ponto
//    float x = (centroX*cos(rotation))+(centroY*sin(rotation));
//    float y = (-centroX*sin(rotation))+(centroY*cos(rotation));

    colisionShape = {Ponto(centroX, centroY), Ponto(numColunas/2, numLinhas/2)};
}

bool ShapeQuad::colide(Ponto p1, Ponto p2){
    AABB s2 = {p1,p2};

    return colide(s2);
}

bool ShapeQuad::colide(AABB shape2){
    bool naoColidiuX = abs(int(colisionShape.centro.x - shape2.centro.x)) > colisionShape.meiaLargura.x + shape2.meiaLargura.x;
    bool naoColidiuY = abs(int(colisionShape.centro.y - shape2.centro.y)) > colisionShape.meiaLargura.y + shape2.meiaLargura.y;


//    cout << "centro 1: " << colisionShape.centro.x  << " , " << colisionShape.centro.y << " centro2: " <<  shape2.centro.x << " , " << shape2.centro.y << endl;
//    cout << "tam2: " << shape2.meiaLargura.x << " , " << shape2.meiaLargura.y << endl;

    if(naoColidiuX || naoColidiuY)
        return false;

    return true;
}

float ShapeQuad::getWidth(){return this->numColunas;}

float ShapeQuad::getHeight(){return this->numLinhas;}

void ShapeQuad::setPosition(float x, float y){
//    colisionShape.centro.x += x - position.x;
//    colisionShape.centro.y += y - position.y;
    this->position = Ponto(x,y);
    atualizaAABB();
};

GLenum ShapeQuad::getDrawMode(){
    return mode;
}
