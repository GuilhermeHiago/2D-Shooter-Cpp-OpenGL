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

#ifndef ShapePoli_h
#define ShapePoli_h

#include "Shape.h"
#include "Ponto.h"
#include "Poligono.h"
#include <vector>

using namespace std;

class ShapePoli : public Shape{

    private:
        Poligono corpo;
        int cor[3];
    public:
        ShapePoli();
        ShapePoli(Poligono corpo, int cor[3], GLenum modo);
        Ponto position;
        bool colide(Ponto p1, Ponto p2);
        bool colide(AABB shape2);
        void atualizaAABB();
        void alteraCorpo(Poligono corpo);
        void draw();
        float getWidth();
        float getHeight();
        void setPosition(float x, float y);
        GLenum getDrawMode();

};

#endif
