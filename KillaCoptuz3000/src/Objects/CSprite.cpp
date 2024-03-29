// ***************************************************************
//  CSprite   version:  1.0   �  date: 04/07/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "KillaCoptuz3000/src/Objects/CSprite.h"

#include "KillaCoptuz3000/src/CPoint.h"
#include "KillaCoptuz3000/src/CLevel.h"
#include "KillaCoptuz3000/src/CObjectStorage.h"

#include "glut/glut.h"
#include "KillaCoptuz3000/src/globals.h"

#include <memory.h>

#include <math.h>

CSprite::CSprite()
{   
   m_behaviorData.m_dxPtr  = 0;
   m_behaviorData.m_dyPtr  = 0;
   m_v                     = 0.;
}

CSprite::~CSprite()
{

}

void CSprite::flip()
{
   m_direction = !m_direction;
}

void CSprite::update(CLevel* t_levelPtr)
{
   CObject::update(t_levelPtr);
}
// void CSprite::draw()
// {  
//    std::string a0 = m_textureKeys[m_activeAnimationPhase]->m_textureKey;
//    CTexture* a1 = CLevel::M_textureMap[m_textureKeys[m_activeAnimationPhase]->m_textureKey];
//    GLuint a2 = a1->m_textureIdVector[m_activeTexture];   
// 
//    glEnable( GL_TEXTURE_2D );
//    glBindTexture(GL_TEXTURE_2D, a2/*CLevel::M_textureMap[m_textureKeys[m_activeAnimationPhase]]->m_textureIdVector[m_activeTexture]*/);
// 
//    if (m_activeTexture==1)
//    {
//       m_activeTexture = m_activeTexture;
//    }
//    
//    glPushMatrix();
// 
//    //////////////////////////////////////////////////////////////////////////
//    // rotate around parent angle
//    if(m_parentPtr != 0)
//    {
//       if (getType() != e_shot)
//       {
//          glTranslatef(m_parentPtr->m_xPos + m_parentPtr->m_width/2.0, m_parentPtr->m_yPos + m_parentPtr->m_height/2.0, 0.0);
//          if(0 != m_parentPtr)
//          {
//             if(static_cast<CSprite*>(m_parentPtr)->m_direction)
//             {
//                glRotatef(180.0, 0., 1., 0);
//             }
//          }
// 
//          glRotatef(m_parentPtr->m_angle, 0., 0., 1.);
//          glTranslatef(-m_parentPtr->m_xPos - m_parentPtr->m_width/2.0, -m_parentPtr->m_yPos - m_parentPtr->m_height/2.0, 0.0);
//       }      
//    }      
//    //////////////////////////////////////////////////////////////////////////
//    // rotate around own axis
//    glTranslatef(m_xPos + m_width/2.0, m_yPos + m_height/2.0, 0.0);
//    if (m_direction)
//    {
//       glRotatef(180.0, 0., 1., 0);
//    }
//    glRotatef(m_angle+m_startAngle, 0., 0., 1.);   
//    glTranslatef(-m_xPos - m_width/2.0, -m_yPos - m_height/2.0, 0.0);
// 
//    glBegin( GL_QUADS );
//    glTexCoord2d(0.0,0.0); 
//    glVertex2d(m_xPos,m_yPos);
// 
//    glTexCoord2d(1.0,0.0);
//    glVertex2d(m_width+m_xPos,m_yPos);
// 
//    glTexCoord2d(1.0,1.0); 
//    glVertex2d(m_width+m_xPos,m_height+m_yPos);
// 
//    glTexCoord2d(0.0,1.0); 
//    glVertex2d(m_xPos,m_height+m_yPos);
// 
//    glEnd();
// 
//    glPopMatrix();
//    glDisable( GL_TEXTURE_2D );
// 
//    //////////////////////////////////////////////////////////////////////////
//    // The following section is for hull drawing (only for debugging purposes)
//    //////////////////////////////////////////////////////////////////////////
//    // DEBUG: Show Hull         
// 
//    // If object has a parent, rotate with parent angle around parent center
//    float a_angle     = 0.0;
//    float a_xCenter   = 0.0;
//    float a_yCenter   = 0.0;
//    bool  a_direction = 0.0;
//    
//    if (g_showBox || g_showBox)
//    {
//       if (0 != m_parentPtr)
//       {
//          // Object has parent, use parent coordinates
//          if (getType() == e_shot)
//          {  
//             if (this->m_parentPtr->getType() != e_player)
//             {
//                a_angle     =  m_parentPtr->m_angle;
//             }         
//             else
//             {
//                a_angle     = m_angle;
//             }
//          }
//          else
//          {
//             a_angle     = m_parentPtr->m_angle + m_angle;      
//          }      
//          a_xCenter   = m_parentPtr->m_xPos + m_parentPtr->m_width/2.0;
//          a_yCenter   = m_parentPtr->m_yPos + m_parentPtr->m_height/2.0;
//          a_direction = static_cast<CSprite*>(m_parentPtr)->m_direction;
//       }
//       else
//       {
//          // Object has no parent, use own coordinates
//          a_angle     = m_angle;
//          a_xCenter   = m_xPos + m_width/2.0;
//          a_yCenter   = m_yPos + m_height/2.0;
//          a_direction = m_direction;
//       }
//    }
// 
//    if (g_showBox)
//    {
//       // Show bounding rect
//       glPushMatrix();
// 
//       // Rotation around object (or parent, if present) center with a_angle
//       glTranslatef(a_xCenter, a_yCenter, 0.0);
//       if (a_direction)
//       {
//          glRotatef(180.0, 0., 1., 0);
//       }
//       glRotatef(a_angle, 0.0, 0.0, 1.0);
//       glTranslatef(-a_xCenter, -a_yCenter, 0.0);
// 
//       // Rotation around object angle with m_startAngle
//       glTranslatef(m_xPos + m_width/2.0, m_yPos + m_height/2.0, 0.0);
//       if (a_direction)
//       {
//          glRotatef(180.0, 0., 1., 0);
//       }
//       glRotatef(m_startAngle, 0.0, 0.0, 1.0);
//       glTranslatef(-m_xPos - m_width/2.0, -m_yPos - m_height/2.0, 0.0); 
// 
//       if (getType() == e_shot)
//       {
//          glTranslatef(m_xPos + m_width/2.0, m_yPos + m_height/2.0, 0.0);
// 
//          glRotatef(m_angle, 0.0, 0.0, 1.0);   
// 
//          glTranslatef(-m_xPos - m_width/2.0, -m_yPos - m_height/2.0, 0.0); 
//       }
// 
//       glColor4f(1.0,1.0,1.0,0.5);
//       glBegin(GL_LINE_LOOP);
//       glVertex2d(m_xPos,m_yPos);
//       glVertex2d(m_width+m_xPos,m_yPos);
//       glVertex2d(m_width+m_xPos,m_height+m_yPos);
//       glVertex2d(m_xPos,m_height+m_yPos);
//       glEnd();
//       glPopMatrix();
//       glColor4f(1.0,1.0,1.0,1.0);
//    }
// 
//    // DEBUG: Show Hull poly
//    CPolygon* a_polygonPtr = CLevel::M_textureMap[m_textureKeys[m_activeAnimationPhase]->m_textureKey]->m_hullPolygonPtr;
//    if (g_showHull && 0 != a_polygonPtr)
//    {         
//       glColor4f(1.0,1.0,1.0,0.5);
//       // Scale polygon to correct width and height
//       a_polygonPtr->rescale(m_width / a_polygonPtr->m_width, m_height / a_polygonPtr->m_height);     
// 
//       // Translate polygon to correct position
//       a_polygonPtr->translate(m_xPos, m_yPos);
// 
//       // Add start angle to rotation
//       if (0.0 != m_startAngle)
//       {
//          a_polygonPtr->rotate(-m_startAngle, m_xPos + m_width/2.0, m_yPos + m_height/2.0);
//       }
// 
//       // Rotate polygon
//       a_polygonPtr->rotate(-a_angle, a_xCenter, a_yCenter);
// 
//       // Rotate polygon around center
//       if (getType() == e_shot)
//       {
//           a_polygonPtr->rotate(-m_angle, m_xPos + m_width/2.0, m_yPos + m_height/2.0);
//       }      
// 
//       // Flip polygon
//       if (a_direction)
//       {         
//          if (0 != m_parentPtr)
//             a_polygonPtr->flip(m_parentPtr->m_xPos + m_parentPtr->m_width/2.0);
//          else
//             a_polygonPtr->flip(m_xPos + m_width/2.0);
//       }
// 
//       glBegin(GL_LINE_LOOP);      
//       std::vector<CPoint*>::iterator a_it;
//       for (a_it = a_polygonPtr->m_points.begin(); a_it != a_polygonPtr->m_points.end(); a_it++)
//       {      
//          glVertex2d((*a_it)->x, (*a_it)->y);      
//       }
//       glEnd();
// 
//       // Undo of: Flip polygon
//       if (a_direction)
//       {         
//          if (0 != m_parentPtr)
//             a_polygonPtr->flip(m_parentPtr->m_xPos + m_parentPtr->m_width/2.0);
//          else
//             a_polygonPtr->flip(m_xPos + m_width/2.0);
//       }
//       
//       if (getType() == e_shot)
//       {
//          // Undo: Rotate polygon around center
//          a_polygonPtr->rotate(m_angle, m_xPos + m_width/2.0, m_yPos + m_height/2.0);
//       }      
// 
//       // Undo of: Rotate polygon
//       a_polygonPtr->rotate(a_angle, a_xCenter, a_yCenter);
// 
//       // Undo of: Add start angle to rotation
//       if (0.0 != m_startAngle)
//       {
//          a_polygonPtr->rotate(m_startAngle, m_xPos + m_width/2.0, m_yPos + m_height/2.0);
//       }
// 
//       // Undo of: Translate polygon to correct position
//       a_polygonPtr->translate(-m_xPos, -m_yPos);
// 
//       // Undo of: Scale polygon to correct width and height
//       a_polygonPtr->rescale(a_polygonPtr->m_width / m_width, a_polygonPtr->m_height / m_height);
// 
//       glColor4f(1.0,1.0,1.0,1.0);
//       // DEBUG END
//    }
// }
