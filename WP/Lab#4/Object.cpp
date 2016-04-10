#include "Object.h"

Object::Object(POINT center,const int &speed) {

    this->center=center;
    this->xSpeed=speed;
    this->ySpeed=speed;
    clr=RGB(0,0,0);

}

bool Object::Accelerate(const int& dSpeedX,const int& dSpeedY) {
    xSpeed=dSpeedX;
    ySpeed=dSpeedY;
    return TRUE;
}

bool Object::Color(const COLORREF &clr) {
    this->clr=clr;
    return TRUE;
}

bool Circle::Move(const HDC &hdc,const RECT& rect,HBRUSH &hBrush) {
    hBrush=CreateSolidBrush(clr);
    SelectObject(hdc,hBrush);
    Collision(rect);
    center.x+=xSpeed;
    center.y+=ySpeed;
    if(square==true)
    {
        Rectangle(hdc, center.x-15, center.y-15, center.x+15, center.y+15 );
        SelectObject(hdc, GetStockObject(WHITE_BRUSH));
        DeleteObject(hBrush);
        return TRUE;
    }
    Ellipse(hdc,center.x-15,center.y-15,center.x+15,center.y+15);
    SelectObject(hdc,GetStockObject(WHITE_BRUSH));
    DeleteObject(hBrush);
    return TRUE;

}

bool Object::Collision(const RECT &rect) {

    if(center.x+15>rect.right-1){
      xSpeed=-abs(xSpeed);
      square = true;
    }
    if(center.x-15<rect.left+1){
      xSpeed=abs(xSpeed);
      square = true;
    }
    if(center.y+15>rect.bottom-1){
        ySpeed=-abs(ySpeed);
    }
    if(center.y-15<rect.top+1){
       ySpeed=abs(ySpeed);
    }


    return TRUE;
}

bool Interaction(Object &ob1,Object &ob2) {

    float distance;
    distance=sqrt( pow(ob1.center.x-ob2.center.x,2)+ pow(ob1.center.y-ob2.center.y,2) );

    if ( distance < 45 ) {
    ob1.Accelerate(-ob1.xSpeed,-ob1.ySpeed);
    ob2.Accelerate(-ob2.xSpeed,-ob2.ySpeed);
    ob1.Color(RGB(0,255,0));
    ob2.Color(RGB(0,0,255));
    }

    return TRUE;
}
