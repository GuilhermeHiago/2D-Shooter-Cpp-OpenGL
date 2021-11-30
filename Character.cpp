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

#include "Shape.h"
#include "Ponto.h"
#include <vector>
#include "Character.h"

using namespace std;

//class Character{

//    public:
//        vector<Shape*> shapes;
//        Ponto position;
//        float rotation;
//        int layer;
//        double vida;

Character::Character(){
}

Character::Character(Ponto position){
    this->position = position;
    this->rotation = 0;
    this->layers = vector<int>{0,0};
    this->vida = 1;
}

Character::Character(Ponto position, vector<Shape*> shapes){
    this->position = position;
    this->shapes = shapes;
    this->rotation = 0;
    this->layers = vector<int>{0};
    this->vida = 1;

    // atualiza o centro dos shapes
    for(Shape* s : shapes){
        s->colisionShape.centro.x += (this->position.x);
        s->colisionShape.centro.y += (this->position.y);
    }
}

void Character::draw(){
    glPushMatrix();
    glTranslatef(position.x, position.y, 0);

    // rotaciona no meio
    // A classe character considera o primeiro shap como central
    glTranslatef(shapes[0]->getWidth()/2, 0, 0);
    glRotatef(rotation, 0, 0, 1);
    glTranslatef(-shapes[0]->getWidth()/2, 0, 0);

    for(Shape* s : shapes){
        //glScalef(escala.x, escala.y, escala.z);
        s->draw();
    }

    glPopMatrix();
}

void Character::setPosition(Ponto pos){
    // atualiza o centro dos shapes
    for(Shape* s : shapes){
        s->colisionShape.centro.x += (pos.x - this->position.x);
        s->colisionShape.centro.y += (pos.y - this->position.y);
    }

    this->position = pos;
}

bool Character::colide(Shape &s2){
    //cout << "s2 pos: " << s2.getAABB().centro.x << ", " << s2.getAABB().centro.y << " size: " << s2.getAABB().meiaLargura.x << endl;

    for(Shape* s : shapes){
        if(s->colide(s2.getAABB())){
            return true;
        }
    }

    return false;
}

bool Character::colide(Character &c2){
    bool mesmoLayer = false;

    // verifica se tem layer igual
    for(int l : layers){
        for(int l2 : c2.layers){
            if(l == l2){
                mesmoLayer = true;
                break;
            }
        }

        if(mesmoLayer)
            break;
    }

    // layers diferentes nao colidem
    if(!mesmoLayer)
        return false;

    for(Shape *s : shapes){
//                if(c2.colide(s)){
//                    return true;
//                }
        for(Shape *s2 : c2.shapes){
//            cout << "AABB predio: "; s2->colisionShape
            if(s->colide(s2->getAABB())){
                return true;
            }
        }
    }

    return false;
}
//};
