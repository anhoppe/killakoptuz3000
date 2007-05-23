#ifndef CPOINT_H
#define CPOINT_H

class CPoint
{
public:
   CPoint() {};
   CPoint(float t_x, float t_y);  
   float x;
   float y;

   void rotate(float t_degree);

   float getAngle();

   CPoint operator*(float t_a);
   CPoint operator+(CPoint& t_point);   
   CPoint operator-(CPoint& t_point);
};

class CLine
{
public:
   CPoint p1;
   CPoint p2;
};

#endif