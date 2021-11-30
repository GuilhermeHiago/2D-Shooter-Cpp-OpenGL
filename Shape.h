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
//#include <vector>

#ifndef Shape_hpp
#define Shape_hpp

using namespace std;

class Shape{

    protected:
        float colors[6][3] = {{0, 0, 0}, {255, 0, 0}, {0, 255, 0},
                            {255, 255, 255}, {0.3,0.4,1}, {0.9,0.5,0}};
        typedef struct AABB{
            Ponto centro;
            Ponto meiaLargura;
        };

        GLenum mode;

    public:
        Ponto position, escala;
        float rotation = 0;
        AABB colisionShape;
        virtual bool colide(Ponto p1, Ponto p2);
        //virtual bool colide(AABB shape2)=0;
        virtual void draw();
        virtual float getWidth()=0;
        virtual float getHeight()=0;
        virtual void atualizaAABB()=0;
        AABB getAABB(){return colisionShape;};
        virtual void setPosition(float x, float y){
            this->position = Ponto(x,y);
            atualizaAABB();
        };
        virtual bool colide(AABB shape2){
            bool naoColidiuX = abs(int(colisionShape.centro.x - shape2.centro.x)) > colisionShape.meiaLargura.x + shape2.meiaLargura.x;
            bool naoColidiuY = abs(int(colisionShape.centro.y - shape2.centro.y)) > colisionShape.meiaLargura.y + shape2.meiaLargura.y;


            cout << "centro 1: " << colisionShape.centro.x  << " , " << colisionShape.centro.y << " centro2: " <<  shape2.centro.x << " , " << shape2.centro.y << endl;
            cout << "tam1: " << colisionShape.meiaLargura.x << " , " << colisionShape.meiaLargura.y << endl;
            cout << "tam2: " << shape2.meiaLargura.x << " , " << shape2.meiaLargura.y << endl;

            if(naoColidiuX || naoColidiuY){
                return false;
            }

            return true;
        };
        GLenum getDrawMode();


};

#endif
