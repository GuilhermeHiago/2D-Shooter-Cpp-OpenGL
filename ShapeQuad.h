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

#ifndef ShapeQuad_h
#define ShapeQuad_h

#include "Shape.h"
#include "Ponto.h"
#include <vector>


using namespace std;

class ShapeQuad : public Shape{

    private:
        int numLinhas;
        int numColunas;
        vector<vector<int>> corpo;
    public:
        ShapeQuad();
        Point(const ShapeQuad &sq);
        ShapeQuad(int numLinhas, int numColunas, vector<vector<int>> corpo, GLenum modo);
        Ponto position;
        bool colide(Ponto p1, Ponto p2);
        bool colide(AABB shape2);
        void draw();
        void atualizaAABB();
        float getWidth();
        float getHeight();
        void setPosition(float x, float y);
        GLenum getDrawMode();

};

#endif
