// ***************************************************************
//  CSprite   version:  1.0   ·  date: 04/07/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "Objects/CSprite.h"
#include "CPoint.h"
#include "CLevel.h"
#include "glut.h"
#include "globals.h"
#include <memory.h>


CSprite::CSprite()
{
   m_trackIndex            = 0;
   m_behaviorData.m_dxPtr  = 0;
   m_behaviorData.m_dyPtr  = 0;
   m_angle                 = 0.0;
   m_direction             = false;
}

void CSprite::flip()
{
   m_direction = !m_direction;
}

CSprite::CSprite(float t_xPos, float t_yPos)
{
   m_xPos                  = t_xPos;
   m_yPos                  = t_yPos;
   m_trackIndex            = 0;
   m_behaviorData.m_dxPtr  = 0;
   m_behaviorData.m_dyPtr  = 0;
   m_angle                 = 0.0;
}

CSprite::~CSprite()
{

}

void CSprite::draw()
{
   glEnable( GL_TEXTURE_2D );
   std::string a0 = m_textureKeys[m_activeAnimationPhase];
   CTexture* a1 = CLevel::M_textureMap[m_textureKeys[m_activeAnimationPhase]];
   GLuint a2 = a1->m_textureIdVector[m_activeTexture];
   glBindTexture(GL_TEXTURE_2D, a2/*CLevel::M_textureMap[m_textureKeys[m_activeAnimationPhase]]->m_textureIdVector[m_activeTexture]*/);

   if (m_activeTexture==1)
   {
      m_activeTexture = m_activeTexture;
   }

   glPushMatrix();

   //////////////////////////////////////////////////////////////////////////
   // rotate around parent angle
   if(m_parentPtr != 0)
   {
      glTranslatef(m_xPos + m_width/2.0 - m_parentPtr->m_width/2., m_yPos + m_height/2.0 - m_parentPtr->m_height/2., 0.0);
      glRotatef(m_parentPtr->m_angle, 0., 0., 1.);
      glTranslatef(-m_xPos - m_width/2.0 + m_parentPtr->m_width/2., -m_yPos - m_height/2.0 + m_parentPtr->m_height/2., 0.0);
   }      
   //////////////////////////////////////////////////////////////////////////
   // rotate around own axis
   glTranslatef(m_xPos + m_width/2.0, m_yPos + m_height/2.0, 0.0);
   if (m_direction)
   {
      glRotatef(180.0, 0., 1., 0);
   }
   glRotatef(m_angle, 0., 0., 1.);   
   glTranslatef(-m_xPos - m_width/2.0, -m_yPos - m_height/2.0, 0.0);

   glBegin( GL_QUADS );
   glTexCoord2d(0.0,0.0); 
   glVertex2d(m_xPos,m_yPos);

   glTexCoord2d(1.0,0.0);
   glVertex2d(m_width+m_xPos,m_yPos);

   glTexCoord2d(1.0,1.0); 
   glVertex2d(m_width+m_xPos,m_height+m_yPos);

   glTexCoord2d(0.0,1.0); 
   glVertex2d(m_xPos,m_height+m_yPos);

   glEnd();

   glPopMatrix();
   glDisable( GL_TEXTURE_2D );

   //////////////////////////////////////////////////////////////////////////
   // The following section is for hull drawing (only for debugging purposes)
   //////////////////////////////////////////////////////////////////////////
   // DEBUG: Show Hull
   if (g_showBox)
   {
      // Show bounding rect
      glPushMatrix();
      glTranslatef(m_xPos + m_width/2.0, m_yPos + m_height/2.0, 0.0);
      if (m_direction)
      {
         glRotatef(180.0, 0., 1., 0);
      }
      glRotatef(m_angle, 0., 0., 1.);   
      glTranslatef(-m_xPos - m_width/2.0, -m_yPos - m_height/2.0, 0.0);

      glColor4f(1.0,1.0,1.0,0.5);
      glBegin(GL_LINE_LOOP);
      glVertex2d(m_xPos,m_yPos);
      glVertex2d(m_width+m_xPos,m_yPos);
      glVertex2d(m_width+m_xPos,m_height+m_yPos);
      glVertex2d(m_xPos,m_height+m_yPos);
      glEnd();
      glPopMatrix();
      glColor4f(1.0,1.0,1.0,1.0);
   }

   // DEBUG: Show Hull poly
   CPolygon* a_polygonPtr = CLevel::M_textureMap[m_textureKeys[m_activeAnimationPhase]]->m_hullPolygonPtr;
   if (g_showHull && 0 != a_polygonPtr)
   {         
      glColor4f(1.0,1.0,1.0,0.5);
      // Scale polygon to correct width and height
      a_polygonPtr->rescale(m_width / a_polygonPtr->m_width, m_height / a_polygonPtr->m_height);     

      // Translate polygon to correct position
      a_polygonPtr->translate(m_xPos, m_yPos);

      // Rotate polygon
      a_polygonPtr->rotate(-m_angle, m_xPos + m_width/2.0, m_yPos + m_height/2.0);

      // Flip polygon
      if (m_direction)
      {
         a_polygonPtr->flip(m_xPos + m_width/2.0);
      }

      glBegin(GL_LINE_LOOP);      
      std::vector<CPoint*>::iterator a_it;
      for (a_it = a_polygonPtr->m_points.begin(); a_it != a_polygonPtr->m_points.end(); a_it++)
      {      
         glVertex2d((*a_it)->x, (*a_it)->y);      
      }
      glEnd();

      // Undo of: Flip polygon
      if (m_direction)
      {
         a_polygonPtr->flip(m_xPos + m_width/2.0);
      }

      // Undo of: Rotate polygon
      a_polygonPtr->rotate(m_angle, m_xPos + m_width/2.0, m_yPos + m_height/2.0);

      // Undo of: Translate polygon to correct position
      a_polygonPtr->translate(-m_xPos, -m_yPos);

      // Undo of: Scale polygon to correct width and height
      a_polygonPtr->rescale(a_polygonPtr->m_width / m_width, a_polygonPtr->m_height / m_height);

      glColor4f(1.0,1.0,1.0,1.0);
      // DEBUG END
   }
}

float CSprite::trackAngle(float t_xTrg, float t_yTrg)
{
   CPoint   a_diffVec;
   float    r_ret       = 0.;


   if(m_trackIndex < m_trackList.size())
   {
      a_diffVec.x = m_xPos - t_xTrg;
      a_diffVec.y = m_yPos - t_yTrg;

      r_ret = a_diffVec.getAngle();
   }

   return r_ret;
}