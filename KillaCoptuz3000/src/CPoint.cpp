#include "CPoint.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>

CPoint::CPoint(float t_x, float t_y)
:x(t_x), y(t_y)
{
}


void CPoint::rotate(float t_degree)
{
   float a_x, a_y;
   
   float a_rad = static_cast<float>(-t_degree*M_PI/180.0);
   float a_cos = cos(a_rad);
   float a_sin = sin(a_rad);

   a_x = float(x*a_cos - y*a_sin);
   a_y = float(x*a_sin + y*a_cos);

   x = a_x;
   y = a_y;
}

CPoint CPoint::operator*(float t_a)
{
   return CPoint(t_a*this->x, t_a*this->y);
}

CPoint CPoint::operator+(CPoint& t_point)
{
   return CPoint(this->x + t_point.x, this->y + t_point.y);
}

CPoint CPoint::operator-(CPoint& t_point)
{
   return CPoint(this->x - t_point.x, this->y - t_point.y);
}

float CPoint::getAngle()
{
   float r_ret = 90.;

   if(0. != y)
   {
      r_ret = static_cast<float>(-atanf(x/y)*180.0/M_PI);
   }
   return r_ret;
}

float CPoint::dist(CPoint& t_p)
{
   return (sqrt((t_p.x-this->x)*(t_p.x-this->x) + (t_p.y-this->y)*(t_p.y-this->y)));
}
