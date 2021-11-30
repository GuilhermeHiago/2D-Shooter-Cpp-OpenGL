#include "Shape.h"
#include "Ponto.h"
#include <vector>

//static int colors[4][3] = {{0, 0, 0}, {255, 0, 0}, {0, 255, 0}, {255, 255, 255}};

//Shape::Shape(GLenum mode){
//     this->mode = mode;
//     this->position = Ponto(0,0);
//}

void Shape::draw(){}

bool Shape::colide(Ponto p1, Ponto p2){
    return true;
}

GLenum Shape::getDrawMode(){
    return mode;
}
