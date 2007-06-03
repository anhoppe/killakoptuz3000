/********************************************************************
	created:	2007/05/10
	created:	10:5:2007   18:03
	filename: 	\Data\Header\CPolygon.h
	file path:	\Data\Header
	file base:	CPolygon
	file ext:	h
	author:		Fabian
	
	purpose:	
*********************************************************************/

#ifndef CPOLYGON_H
#define CPOLYGON_H

#include "CPoint.h"
#include <vector>

class CPolygon
{
public:
   CPolygon();
   CPolygon(CPolygon* t_polygonPtr);
   ~CPolygon();   

   // Create hull polygon
   void createHull(const unsigned char* t_imagePtr, unsigned int t_width, unsigned int t_height, unsigned int t_n);

   // Check if point is inside polygon
   bool isInside(CPoint* t_pointPtr);

   void translate(float t_xPos, float t_yPos);

   void rescale(float t_xFactor, float t_yFactor);

   void rotate(float t_angle, float t_xCenter, float t_yCenter);

   // Mirror the polygon at the x-axis around t_x
   void flip(float t_x);

   CPoint getHullPoint(CPoint* t_startPointPtr, CPoint* t_imageCenterPtr, const unsigned char* t_imagePtr, unsigned int a_witdth);

   CPolygon& operator=(CPolygon& t_srcPtr);

   // Points of polygon
   std::vector<CPoint*> m_points;

   unsigned int         m_width;
   unsigned int         m_height;
};

#endif