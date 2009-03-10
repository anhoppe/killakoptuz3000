/********************************************************************
	created:	2007/05/10
	created:	10:5:2007   18:06
	filename: 	\Data\Source\CPolygon.cpp
	file path:	\Data\Source
	file base:	CPolygon
	file ext:	cpp
	author:		Fabian
	
	purpose:	
*********************************************************************/

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include "CPolygon.h"
#include "Functions.h"

CPolygon::CPolygon()
{

}

CPolygon::CPolygon(CPolygon* t_polygonPtr)
{
   unsigned int a_i;

   if(0 != t_polygonPtr)
   {
       for (a_i = 0; a_i < t_polygonPtr->m_points.size(); a_i++)
       {
           m_points.push_back(new CPoint(t_polygonPtr->m_points[a_i]->x, t_polygonPtr->m_points[a_i]->y));
       }

       m_width  = t_polygonPtr->m_width;
       m_height = t_polygonPtr->m_height;
   }
   else
   {
       printf("what the hell\n");
       assert(false);
   }
}


CPolygon::~CPolygon()
{
   CPoint* a_pointPtr;

   std::vector<CPoint*>::iterator a_it;
   for (a_it = m_points.begin(); a_it != m_points.end(); a_it++)
   {
      a_pointPtr = *a_it;
      delete a_pointPtr;
   }
}

bool CPolygon::isInside(CPoint* t_pointPtr)
{
   bool r_ret = true;

   int d = 0;
   int qa, qb;
   float xa = m_points[m_points.size()-1]->x;
   float ya = m_points[m_points.size()-1]->y;   
   float xb;
   float yb;
   float xs;

   if (ya<=t_pointPtr->y) if (xa<=t_pointPtr->x) qa=0; else qa=1;
   else if (xa<=t_pointPtr->x) qa=3; else qa=2;
   
   for (unsigned int i = 0; i < m_points.size(); i++ )
   {
      xb = m_points[i]->x; 
      yb = m_points[i]->y; 

      if (yb<=t_pointPtr->y) if (xb<=t_pointPtr->x) qb=0; else qb=1;
      else if (xb<=t_pointPtr->x) qb=3; else qb=2;
      
      switch ((qb-qa)&3)
      {
         case 0:	break;
         case 1:	d++;	break;
         case 3:	d--;	break;
         default:
         {
            if (t_pointPtr->x== (xs=((xb-xa)*(t_pointPtr->y-ya)/(yb-ya))+xa) ) return(0);
            if ((t_pointPtr->x>xs)==(yb>ya)) d-=2; else d+=2;   
         }
      }
      xa = xb;
      ya = yb;
      qa = qb;
   
   }

   switch (d)
   {
      case 0:
         return false;
      default:
         return true;   
   }   
}

// Mirror the polygon at the x-axis around t_x
void CPolygon::flip(float t_x)
{
   for (unsigned int i = 0; i < m_points.size(); i++ )
   {
      m_points[i]->x = m_points[i]->x - 2*(m_points[i]->x - t_x);
   }
}

void CPolygon::createHull(const unsigned char* t_imagePtr, unsigned int t_width, unsigned int t_height, unsigned int t_n /*= 4*/)
{
   // Total number of polygon points
   // Must be multiple of 4   
   while (t_n % 4 != 0)
   {
      t_n++;
   }
   unsigned int a_i;
   CPoint*      a_pointPtr;

   // Distribute t_n points along the image frame

   // 1. Top
   for (a_i = 0; a_i < t_n/4; a_i++)
   {
      a_pointPtr = new CPoint(static_cast<float>(t_width/(t_n/4)*a_i), 0.);
      m_points.push_back(a_pointPtr);
   }

   // 2. Right
   for (a_i = 0; a_i < t_n/4; a_i++)
   {
      a_pointPtr = new CPoint(static_cast<float>(t_width), static_cast<float>(t_height/(t_n/4)*a_i));
      m_points.push_back(a_pointPtr);
   }

   // 3. Bottom
   for (a_i = 0; a_i < t_n/4; a_i++)
   {
      a_pointPtr = new CPoint(static_cast<float>(t_width - t_width/(t_n/4)*a_i), static_cast<float>(t_height));
      m_points.push_back(a_pointPtr);
   }

   // 4. Left
   for (a_i = 0; a_i < t_n/4; a_i++)
   {
      a_pointPtr = new CPoint(0., static_cast<float>(t_height - t_height/(t_n/4)*a_i));
      m_points.push_back(a_pointPtr);
   }

   // Contract the polygon   
   CPoint a_center(static_cast<float>(t_width/2), static_cast<float>(t_height/2));
   CPoint a_point;
   
   for (a_i=0; a_i < m_points.size(); a_i++)
   {            
      a_point = getHullPoint(m_points[a_i], &a_center, t_imagePtr, t_width);      
      m_points[a_i]->x = a_point.x;
      m_points[a_i]->y = a_point.y;
   }        

   m_width  = t_width;
   m_height = t_height;
}

void CPolygon::translate(float t_xPos, float t_yPos)
{
   std::vector<CPoint*>::iterator a_it;

   for (a_it = m_points.begin(); a_it != m_points.end(); a_it++)
   {
      (*a_it)->x += t_xPos;
      (*a_it)->y += t_yPos;
   }
}

void CPolygon::rescale(float t_xFactor, float t_yFactor)
{
   std::vector<CPoint*>::iterator a_it;

   for (a_it = m_points.begin(); a_it != m_points.end(); a_it++)
   {
      (*a_it)->x = (*a_it)->x*t_xFactor;
      (*a_it)->y = (*a_it)->y*t_yFactor;
   }
}

CPoint CPolygon::getHullPoint(CPoint* t_startPointPtr, CPoint* t_imageCenterPtr, const unsigned char* t_imagePtr, unsigned int a_width)
{
   // FIXME: implement Bresenham line algorithm
   CPoint r_point = *t_startPointPtr;
   CPoint a_deltaP;   
   unsigned int a_divs = 50;
   a_deltaP.x = (t_imageCenterPtr->x - r_point.x)/a_divs;
   a_deltaP.y = (t_imageCenterPtr->y - r_point.y)/a_divs;   
   int y0 = static_cast<int>(t_startPointPtr->y);
   int x0 = static_cast<int>(t_startPointPtr->x);
   int y1 = static_cast<int>(t_imageCenterPtr->y);
   int x1 = static_cast<int>(t_imageCenterPtr->x);
   unsigned int a_count = 0;

   float min_dist = sqrt(a_deltaP.x*a_deltaP.x + a_deltaP.y*a_deltaP.y);   

   do
   {
      r_point.x += a_deltaP.x;
      r_point.y += a_deltaP.y;      
      a_count ++;
   }
   while(a_count < a_divs && t_imagePtr[4 * int(r_point.y) * a_width + 4 * int(r_point.x) + 3] != 255);   
   
   return r_point;
}

void CPolygon::rotate(float t_angle, float t_xCenter, float t_yCenter)
{   
   std::vector<CPoint*>::iterator a_it;

   for (a_it = m_points.begin(); a_it < m_points.end(); a_it++)
   {
      (*a_it)->x -= t_xCenter;
      (*a_it)->y -= t_yCenter;
      (*a_it)->rotate(t_angle);
      (*a_it)->x += t_xCenter;
      (*a_it)->y += t_yCenter;
   }   
}

CPolygon& CPolygon::operator=(CPolygon& t_srcPtr)
{
   std::vector<CPoint*>::iterator   a_it;
   CPoint*                          a_pointPtr  = 0;


   // copy attributes
   m_width = t_srcPtr.m_width;
   m_height = t_srcPtr.m_height;

   // copy point list
   for(a_it = t_srcPtr.m_points.begin(); a_it != t_srcPtr.m_points.end(); a_it++)
   {
      a_pointPtr = new CPoint((*a_it)->x, (*a_it)->y);
   }

   return *this;
}
