/********************************************************************
	created:	2009/03/03
	created:	3:3:2009   23:34
	filename: 	c:\hoppe\develop\KK3000\LevelEditor\src\CObjectControl.cpp
	file path:	c:\hoppe\develop\KK3000\LevelEditor\src
	file base:	CObjectControl
	file ext:	cpp
	author:		anhoppe
	
	purpose:	
*********************************************************************/
#include "CObjectControl.h"

#include "../KillaCoptuz3000/src/Objects/CObject.h"

//////////////////////////////////////////////////////////////////////////
// Definitions
//////////////////////////////////////////////////////////////////////////
#define PANDEL_DIST 0.02
#define PANEL_SIZE 0.05

//////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////
CObjectControl::CObjectControl()
{
   m_activeObjectPtr = 0;
   m_selectedControl = e_none;
   m_startX = 0.;
   m_startY = 0.;
   m_startWidth = 0.;
   m_startHeight = 0.;
}

CObjectControl::~CObjectControl()
{

}

CObjectControl& CObjectControl::getInstance()
{
   static CObjectControl* s_instPtr = 0;

   if(0 == s_instPtr)
   {
      s_instPtr = new CObjectControl();
   }

   return *s_instPtr;
}

bool CObjectControl::selectSizer(double t_x, double t_y)
{
   bool  r_selected  = false;

   m_selectedControl = e_none;

   if(0 != m_activeObjectPtr)
   {
      if(isOverRect(e_left, t_x, t_y))
      {
         m_selectedControl = e_left;
      }
      else if(isOverRect(e_top, t_x, t_y))
      {
         m_selectedControl = e_top;
      }
      else if(isOverRect(e_right, t_x, t_y))
      {
         m_selectedControl = e_right;
      }
      else if(isOverRect(e_bottom, t_x, t_y))
      {
         m_selectedControl = e_bottom;
      }

      if(e_none != m_selectedControl)
      {
         m_startX = t_x;
         m_startY = t_y;

         m_deltaX = t_x - m_activeObjectPtr->m_xPos;
         m_deltaY = t_y - m_activeObjectPtr->m_yPos;

         m_startWidth = m_activeObjectPtr->m_width;
         m_startHeight = m_activeObjectPtr->m_height;

         r_selected = true;
      }
   }

   return r_selected;
}

bool CObjectControl::moveControl(double t_x, double t_y)
{
   bool  r_selected  = false;

   m_selectedControl = e_none;

   if(0 != m_activeObjectPtr)
   {
      if(isOverRect(e_move, t_x, t_y))
      {
         m_selectedControl = e_move;


         m_startX = t_x;
         m_startY = t_y;

         m_deltaX = t_x - m_activeObjectPtr->m_xPos;
         m_deltaY = t_y - m_activeObjectPtr->m_yPos;

         m_startWidth = m_activeObjectPtr->m_width;
         m_startHeight = m_activeObjectPtr->m_height;

         r_selected = true;
      }
   }

   return r_selected;
}

void CObjectControl::processMouseMove(double t_x, double t_y)
{
   if(0 != m_activeObjectPtr)
   {
      switch(m_selectedControl)
      {
      case e_left:
         m_activeObjectPtr->m_xPos = t_x - m_deltaX;
         m_activeObjectPtr->m_width = m_startWidth + (m_startX - t_x);
         break;
      case e_top:
         m_activeObjectPtr->m_height = m_startHeight + (t_y- m_startY);
         break;
      case e_right:
         m_activeObjectPtr->m_width = m_startWidth + (t_x - m_startX);
         break;
      case e_bottom:
         m_activeObjectPtr->m_yPos = t_y - m_deltaY;
         m_activeObjectPtr->m_height = m_startHeight + (m_startY - t_y);
         break;
      case e_move:
         m_activeObjectPtr->m_xPos = t_x - m_deltaX;
         m_activeObjectPtr->m_yPos = t_y - m_deltaY;
         break;
      }
   }
}

void CObjectControl::releaseControl()
{
   m_selectedControl = e_none;
}

void CObjectControl::draw()
{
   if(0 != m_activeObjectPtr)
   {
      glPushMatrix();

      // draw the left sizer
      drawSizerRect(e_left);

      // draw the top sizer
      drawSizerRect(e_top);

      // draw the right sizer
      drawSizerRect(e_right);

      // draw the bottom sizer
      drawSizerRect(e_bottom);

      glPopMatrix();
   }
}


//////////////////////////////////////////////////////////////////////////
// Private Methods
//////////////////////////////////////////////////////////////////////////
bool CObjectControl::isOverRect(EDirection t_dir, double t_x, double t_y)
{
   double   a_minX    = 0.;
   double   a_maxX    = 0.;
   double   a_minY    = 0.;
   double   a_maxY    = 0.;

   bool     r_result = false;


   getRect(t_dir, a_minX, a_minY, a_maxX, a_maxY);

   if(t_x > a_minX && t_x < a_maxX &&
      t_y > a_minY && t_y < a_maxY)
   {
      r_result = true;
   }

   return r_result;
}

void CObjectControl::drawSizerRect(EDirection t_dir)
{
   double a_minX = 0.;
   double a_maxX = 0.;
   double a_minY = 0.;
   double a_maxY = 0.;

   getRect(t_dir, a_minX, a_minY, a_maxX, a_maxY);
   glBegin(GL_POLYGON);
   glColor3f(1.0, 0.0, 0.0);
   glVertex2d(a_minX, a_maxY);
   glVertex2d(a_maxX, a_maxY);
   glVertex2d(a_maxX, a_minY);
   glVertex2d(a_minX, a_minY);
   glColor3f(1.0, 1.0, 1.0);
   glEnd();
}

void CObjectControl::getRect(EDirection t_dir, double& t_minX, double& t_minY, double& t_maxX, double& t_maxY)
{
   if(0 != m_activeObjectPtr)
   {
      double a_x        = m_activeObjectPtr->m_xPos;
      double a_y        = m_activeObjectPtr->m_yPos;
      double a_width    = m_activeObjectPtr->m_width;
      double a_height   = m_activeObjectPtr->m_height;

      switch(t_dir)
      {
      case e_left:
         t_minX = a_x - PANDEL_DIST - PANEL_SIZE;
         t_maxX = a_x - PANDEL_DIST;
         t_minY = a_y + a_height / 2. - PANEL_SIZE / 2.;
         t_maxY = a_y + a_height / 2. + PANEL_SIZE / 2.;
         break;
      case e_right:
         t_minX = a_x + a_width + PANDEL_DIST;
         t_maxX = a_x + a_width + PANDEL_DIST + PANEL_SIZE;
         t_minY = a_y + a_height / 2. - PANEL_SIZE / 2.;
         t_maxY = a_y + a_height / 2. + PANEL_SIZE / 2.;
         break;
      case e_top:
         t_minX = a_x + a_width / 2 - PANEL_SIZE / 2;
         t_maxX = a_x + a_width / 2 + PANEL_SIZE / 2;
         t_maxY = a_y + a_height + PANDEL_DIST + PANEL_SIZE;
         t_minY = a_y + a_height + PANDEL_DIST;
         break;
      case e_bottom:
         t_minX = a_x + a_width / 2 - PANEL_SIZE / 2;
         t_maxX = a_x + a_width / 2 + PANEL_SIZE / 2;
         t_minY = a_y - PANDEL_DIST - PANEL_SIZE;
         t_maxY = a_y - PANDEL_DIST;
         break;
      case e_move:
         t_minX = m_activeObjectPtr->m_xPos;
         t_maxX = m_activeObjectPtr->m_xPos + m_activeObjectPtr->m_width;
         t_minY = m_activeObjectPtr->m_yPos;
         t_maxY = m_activeObjectPtr->m_yPos + m_activeObjectPtr->m_height;
      }
   }
}
