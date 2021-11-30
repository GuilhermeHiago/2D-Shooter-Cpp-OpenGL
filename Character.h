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

#ifndef Character_h
#define Character_h

#include "Shape.h"
#include "Ponto.h"
#include <vector>

using namespace std;

class Character{

    public:
        vector<Shape*> shapes;
        Ponto position;
        float rotation;
        vector<int> layers;
        double vida;

        Character();
        Character(Ponto position);
        Character(Ponto position, vector<Shape*> shapes);
        void draw();
        void setPosition(Ponto pos);
        bool colide(Shape &s2);
        bool colide(Character &c2);
};

#endif
