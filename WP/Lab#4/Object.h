
#ifndef OBJECTS_H
#define OBJECTS_H

#include <iostream>
#include <windows.h>
#include <cmath>
#include <mmsystem.h>
#include <time.h>
#include <tchar.h>
#include <stdio.h>

#define ID_TIMER 100

using namespace std;

class Object {

protected:
    COLORREF clr;
public:
    POINT center;
    int xSpeed,ySpeed;
    bool square=false;

    Object(POINT center,const int &speed);
    bool Color(const COLORREF &clr);
    bool Accelerate(const int& dSpeedX,const int& dSpeedY);
    bool Collision(const RECT &rect);
    virtual bool Move(const HDC &hdc,const RECT& rect,HBRUSH &hBrush)=0;
};

class Circle : public Object {
public:
    Circle(POINT center,const int &speed): Object(center,speed){}
    bool Move(const HDC &hdc,const RECT& rect,HBRUSH &hBrush);
};

bool Interaction(Object &ob1,Object &ob2);
#endif // _Objects
