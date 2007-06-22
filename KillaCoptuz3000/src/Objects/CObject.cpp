// ***************************************************************
//  CObject   version:  1.0   ·  date: 04/07/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************


#include "KillaCoptuz3000/src/Objects/CObject.h"
#include "KillaCoptuz3000/src/CObjectStorage.h"

#include "KillaCoptuz3000/src/CTgaLoader.h"
#include "KillaCoptuz3000/src/Functions.h"
#include "lodepng/lodepng.h"
#include "KillaCoptuz3000/src/CLevel.h"
#include "KillaCoptuz3000/src/globals.h"
#include "KillaCoptuz3000/src/CTexture.h"

#include "KillaCoptuz3000/src/Objects/CShot.h"

#include <stdio.h>
#include <string>


CObject::CObject()
{
   m_activeTexture         = 0;
   m_timeCounter           = 0;      
   m_angle                 = 0.;   
   m_dx                    = 0.;
   m_dy                    = 0.;
   m_hitPoints             = 1;
   m_maxHitPoints          = m_hitPoints;
   m_invincible            = false;
   m_damagePoints          = 0;   
   m_activeAnimationPhase  = 0;
   m_explosionIndex        = -1;
   m_isDying               = false;
   m_direction             = false;
   m_cycleInterval         = 10;   
   m_startAngle            = 0.0;
   m_drawLayer             = 0;

   m_id                    = 0;
   m_parentId              = 0;
}


CObject::~CObject()
{   
}

void CObject::nextTexture()
{
   m_timeCounter++;
   
   if (m_timeCounter >= m_cycleInterval)
   {
      m_activeTexture++;
     
      if (m_activeTexture >= CObjectStorage::getInstance().m_textureMap[m_textureKeys[m_activeAnimationPhase]->m_textureKey]->m_textureIdVector.size())
      {
         m_activeTexture = 0;

         // Check, if we were in death sequence
         if (m_isDying)
         {
            // We are finished with dying now

            // Generate delete event
            CEvent* a_event;

            a_event = new CEvent(e_delete);

            // Attach this object
            a_event->m_objectList.push_back(m_id);

            /*
            // Attach child objects
            for (unsigned int i = 0; i < m_children.size(); i++)
            {
               a_event->m_objectList.push_back(m_children[i]->m_id);
            }
            */

            CObjectStorage::getInstance().m_eventList.push_back(a_event);
         }
      }

      m_timeCounter = 0;
   }
}


// Return: number of textures in m_textureIdVector
size_t CObject::getTextureCount()
{
   return CObjectStorage::getInstance().m_textureMap[m_textureKeys[m_activeAnimationPhase]->m_textureKey]->m_textureIdVector.size();
}

bool CObject::load(TiXmlNode* t_nodePtr)
{
   TiXmlNode*     a_nodePtr         = 0;
   TiXmlElement*  a_elemPtr         = 0;

   TiXmlNode*     a_subNodePtr      = 0;
   TiXmlElement*  a_subElemPtr      = 0;

   CTextureInfo*  a_textureInfoPtr  = 0;

   std::string    a_str             = "";
   std::string    a_dummy           = "";
   bool           r_ret             = true;
   

   a_elemPtr = t_nodePtr->ToElement();
   
   r_ret = r_ret & getAttributeStr(a_elemPtr, "xpos", a_str);
   m_xPos         = atof(a_str.c_str());

   r_ret = r_ret & getAttributeStr(a_elemPtr, "ypos", a_str);
   m_yPos         = atof(a_str.c_str());

   r_ret = r_ret & getAttributeStr(a_elemPtr, "width", a_str);
   m_width        = atof(a_str.c_str());

   r_ret = r_ret & getAttributeStr(a_elemPtr, "height", a_str);
   m_height       = atof(a_str.c_str());

   if(getAttributeStr(a_elemPtr, "drawLayer", a_str))
   {
      m_drawLayer = atoi(a_str.c_str());
   }

   if(getAttributeStr(a_elemPtr, "cycleInterval", a_str))
   {
      m_cycleInterval= atoi(a_str.c_str());  
   }

   // iterate over all texture tags in a texture list
   TiXmlNode*  a_textureNodePtr = 0;

   a_textureNodePtr = t_nodePtr->FirstChild("texturelist");

   if(0 != a_textureNodePtr)
   {
      for(a_subNodePtr = a_textureNodePtr->FirstChild("texture"); a_subNodePtr; a_subNodePtr = a_textureNodePtr->IterateChildren("texture", a_subNodePtr))
      {
         a_subElemPtr = a_subNodePtr->ToElement();

         r_ret &= getAttributeStr(a_subElemPtr, "key", a_str);

         a_textureInfoPtr = new CTextureInfo;

         // Create the polygon         
         a_textureInfoPtr->m_polygonPtr = new CPolygon(CObjectStorage::getInstance().m_textureMap[a_str]->m_hullPolygonPtr);

         // Scale the new polygon
         a_textureInfoPtr->m_polygonPtr->rescale(m_width / a_textureInfoPtr->m_polygonPtr->m_width, m_height / a_textureInfoPtr->m_polygonPtr->m_height);

         // check if explosion sequence exists
         if(getAttributeStr(a_subElemPtr, "explosion", a_dummy))
         {
            m_explosionIndex = (unsigned int)m_textureKeys.size();
         }

         if(r_ret)
         {
            a_textureInfoPtr->m_textureKey = a_str;
            m_textureKeys.push_back(a_textureInfoPtr);
         }
      }
   }

   if (getAttributeStr(a_elemPtr, "hitpoints", a_str))
   {
      m_hitPoints    = atoi(a_str.c_str());
      m_maxHitPoints = m_hitPoints;
   }
   else
   {
      m_invincible = true;
   }

   if (getAttributeStr(a_elemPtr, "damagepoints", a_str))
   {
      m_damagePoints = atoi(a_str.c_str());
   }   

   if (getAttributeStr(a_elemPtr, "background", a_str))
   {
      m_isBackground = atoi(a_str.c_str()) == 1;
   }
   else
   {
      m_isBackground = false;
   }   
   return r_ret;
}

#if(PRODUCT == KK30000)
void CObject::draw()
{  
   std::string a0       = m_textureKeys[m_activeAnimationPhase]->m_textureKey;
   CTexture*   a1       = CObjectStorage::getInstance().m_textureMap[m_textureKeys[m_activeAnimationPhase]->m_textureKey];
   GLuint      a2       = a1->m_textureIdVector[m_activeTexture];

   float       a_xPos   = m_xPos;
   float       a_yPos   = m_yPos;

   glEnable( GL_TEXTURE_2D );
   glBindTexture(GL_TEXTURE_2D, a2/*CLevel::M_textureMap[m_textureKeys[m_activeAnimationPhase]]->m_textureIdVector[m_activeTexture]*/);

   if (m_activeTexture==1)
   {
      m_activeTexture = m_activeTexture;
   }
   
   glPushMatrix();

   //////////////////////////////////////////////////////////////////////////
   // rotate around parent angle
   CObject* a_parentPtr = 0;
   if(0 != m_parentId)
   {
      a_parentPtr = CObjectStorage::getInstance().m_objectMap[m_parentId];
      a_xPos += a_parentPtr->m_xPos;
      a_yPos += a_parentPtr->m_yPos;
   }

   if(a_parentPtr != 0)
   {
      if (getType() != e_shot)
      {
         glTranslatef(a_parentPtr->m_xPos + a_parentPtr->m_width/2.0, a_parentPtr->m_yPos + a_parentPtr->m_height/2.0, 0.0);
         if(0 != a_parentPtr)
         {
            if(a_parentPtr->m_direction)
            {
               glRotatef(180.0, 0., 1., 0);
            }
         }

         glRotatef(a_parentPtr->m_angle, 0., 0., 1.);
         glTranslatef(-a_parentPtr->m_xPos - a_parentPtr->m_width/2.0, -a_parentPtr->m_yPos - a_parentPtr->m_height/2.0, 0.0);
      }      
   }      
   //////////////////////////////////////////////////////////////////////////
   // rotate around own axis
   glTranslatef(a_xPos + m_width/2.0, a_yPos + m_height/2.0, 0.0);
   if (m_direction)
   {
      glRotatef(180.0, 0., 1., 0);
   }
   glRotatef(m_angle+m_startAngle, 0., 0., 1.);   
   glTranslatef(-a_xPos - m_width/2.0, -a_yPos - m_height/2.0, 0.0);

   glBegin( GL_QUADS );
   glTexCoord2d(0.0,0.0); 
   glVertex2d(a_xPos, a_yPos);

   glTexCoord2d(1.0,0.0);
   glVertex2d(m_width+a_xPos, a_yPos);

   glTexCoord2d(1.0,1.0); 
   glVertex2d(m_width+a_xPos, m_height+a_yPos);

   glTexCoord2d(0.0,1.0); 
   glVertex2d(a_xPos,m_height+a_yPos);

   glEnd();

   glPopMatrix();
   glDisable( GL_TEXTURE_2D );

   //////////////////////////////////////////////////////////////////////////
   // The following section is for hull drawing (only for debugging purposes)
   //////////////////////////////////////////////////////////////////////////
   // DEBUG: Show Hull         

   // If object has a parent, rotate with parent angle around parent center
   float a_angle     = 0.0;
   float a_xCenter   = 0.0;
   float a_yCenter   = 0.0;
   bool  a_direction = 0.0;
   
   if (g_showBox || g_showBox)
   {
      if (0 != a_parentPtr)
      {
         // Object has parent, use parent coordinates
         if (getType() == e_shot)
         {  
            if (a_parentPtr->getType() != e_player)
            {
               a_angle     =  a_parentPtr->m_angle;
            }         
            else
            {
               a_angle     = m_angle;
            }
         }
         else
         {
            a_angle     = a_parentPtr->m_angle + m_angle;      
         }      
         a_xCenter   = a_parentPtr->m_xPos + a_parentPtr->m_width/2.0;
         a_yCenter   = a_parentPtr->m_yPos + a_parentPtr->m_height/2.0;
         a_direction = a_parentPtr->m_direction;
      }
      else
      {
         // Object has no parent, use own coordinates
         a_angle     = m_angle;
         a_xCenter   = a_xPos + m_width/2.0;
         a_yCenter   = a_yPos + m_height/2.0;
         a_direction = m_direction;
      }
   }

   if (g_showBox)
   {
      // Show bounding rect
      glPushMatrix();

      // Rotation around object (or parent, if present) center with a_angle
      glTranslatef(a_xCenter, a_yCenter, 0.0);
      if (a_direction)
      {
         glRotatef(180.0, 0., 1., 0);
      }
      glRotatef(a_angle, 0.0, 0.0, 1.0);
      glTranslatef(-a_xCenter, -a_yCenter, 0.0);

      // Rotation around object angle with m_startAngle
      glTranslatef(a_xPos + m_width/2.0, a_yPos + m_height/2.0, 0.0);
      if (a_direction)
      {
         glRotatef(180.0, 0., 1., 0);
      }
      glRotatef(m_startAngle, 0.0, 0.0, 1.0);
      glTranslatef(-a_xPos - m_width/2.0, -a_yPos - m_height/2.0, 0.0); 

      if (getType() == e_shot)
      {
         glTranslatef(a_xPos + m_width/2.0, a_yPos + m_height/2.0, 0.0);

         glRotatef(m_angle, 0.0, 0.0, 1.0);   

         glTranslatef(-a_xPos - m_width/2.0, -a_yPos - m_height/2.0, 0.0); 
      }

      glColor4f(1.0,0.0,0.0,0.9);
      glBegin(GL_LINE_LOOP);
      glVertex2d(a_xPos, a_yPos);
      glVertex2d(m_width+a_xPos,a_yPos);
      glVertex2d(m_width+a_xPos,m_height+a_yPos);
      glVertex2d(a_xPos,m_height+a_yPos);
      glEnd();
      glPopMatrix();
      glColor4f(1.0,1.0,1.0,1.0);
   }

   // DEBUG: Show Hull poly
   CPolygon* a_polygonPtr = CObjectStorage::getInstance().m_textureMap[m_textureKeys[m_activeAnimationPhase]->m_textureKey]->m_hullPolygonPtr;
   if (g_showHull && 0 != a_polygonPtr)
   {         
      glColor4f(1.0,1.0,1.0,0.5);
      // Scale polygon to correct width and height
      a_polygonPtr->rescale(m_width / a_polygonPtr->m_width, m_height / a_polygonPtr->m_height);     

      // Translate polygon to correct position
      a_polygonPtr->translate(m_xPos, m_yPos);

      // Add start angle to rotation
      if (0.0 != m_startAngle)
      {
         a_polygonPtr->rotate(-m_startAngle, a_xPos + m_width/2.0, a_yPos + m_height/2.0);
      }

      // Rotate polygon
      a_polygonPtr->rotate(-a_angle, a_xCenter, a_yCenter);

      // Rotate polygon around center
      if (getType() == e_shot)
      {
          a_polygonPtr->rotate(-m_angle, a_xPos + m_width/2.0, a_yPos + m_height/2.0);
      }      

      // Flip polygon
      if (a_direction)
      {         
         if (0 != a_parentPtr)
            a_polygonPtr->flip(a_parentPtr->m_xPos + a_parentPtr->m_width/2.0);
         else
            a_polygonPtr->flip(a_xPos + m_width/2.0);
      }

      glBegin(GL_LINE_LOOP);      
      std::vector<CPoint*>::iterator a_it;
      for (a_it = a_polygonPtr->m_points.begin(); a_it != a_polygonPtr->m_points.end(); a_it++)
      {      
         glVertex2d((*a_it)->x, (*a_it)->y);      
      }
      glEnd();

      // Undo of: Flip polygon
      if (a_direction)
      {         
         if (0 != a_parentPtr)
            a_polygonPtr->flip(a_parentPtr->m_xPos + a_parentPtr->m_width/2.0);
         else
            a_polygonPtr->flip(a_xPos + m_width/2.0);
      }
      
      if (getType() == e_shot)
      {
         // Undo: Rotate polygon around center
         a_polygonPtr->rotate(m_angle, a_xPos + m_width/2.0, a_yPos + m_height/2.0);
      }      

      // Undo of: Rotate polygon
      a_polygonPtr->rotate(a_angle, a_xCenter, a_yCenter);

      // Undo of: Add start angle to rotation
      if (0.0 != m_startAngle)
      {
         a_polygonPtr->rotate(m_startAngle, a_xPos + m_width/2.0, a_yPos + m_height/2.0);
      }

      // Undo of: Translate polygon to correct position
      a_polygonPtr->translate(-a_xPos, -a_yPos);

      // Undo of: Scale polygon to correct width and height
      a_polygonPtr->rescale(a_polygonPtr->m_width / m_width, a_polygonPtr->m_height / m_height);

      glColor4f(1.0,1.0,1.0,1.0);
      // DEBUG END
   }
}
#else
void CObject::draw()
{
   if(m_textureKeys.size() > 0)
   {
      std::string a0       = m_textureKeys[m_activeAnimationPhase]->m_textureKey;
      CTexture*   a1       = CObjectStorage::getInstance().m_textureMap[m_textureKeys[m_activeAnimationPhase]->m_textureKey];
      GLuint      a2       = a1->m_textureIdVector[m_activeTexture];

      float       a_xPos   = m_xPos;
      float       a_yPos   = m_yPos;

      glEnable( GL_TEXTURE_2D );
      glBindTexture(GL_TEXTURE_2D, a2/*CLevel::M_textureMap[m_textureKeys[m_activeAnimationPhase]]->m_textureIdVector[m_activeTexture]*/);

      if (m_activeTexture==1)
      {
         m_activeTexture = m_activeTexture;
      }

      glPushMatrix();

      //////////////////////////////////////////////////////////////////////////
      // rotate around parent angle
      CObject* a_parentPtr = 0;
      if(0 != m_parentId)
      {
         a_parentPtr = CObjectStorage::getInstance().m_objectMap[m_parentId];
         a_xPos += a_parentPtr->m_xPos;
         a_yPos += a_parentPtr->m_yPos;
      }

      if(a_parentPtr != 0)
      {
         if (getType() != e_shot)
         {
            glTranslatef(a_parentPtr->m_xPos + a_parentPtr->m_width/2.0, a_parentPtr->m_yPos + a_parentPtr->m_height/2.0, 0.0);
            if(0 != a_parentPtr)
            {
               if(a_parentPtr->m_direction)
               {
                  glRotatef(180.0, 0., 1., 0);
               }
            }

            glRotatef(a_parentPtr->m_angle, 0., 0., 1.);
            glTranslatef(-a_parentPtr->m_xPos - a_parentPtr->m_width/2.0, -a_parentPtr->m_yPos - a_parentPtr->m_height/2.0, 0.0);
         }      
      }      
      //////////////////////////////////////////////////////////////////////////
      // rotate around own axis
      glTranslatef(a_xPos + m_width/2.0, a_yPos + m_height/2.0, 0.0);
      if (m_direction)
      {
         glRotatef(180.0, 0., 1., 0);
      }
      glRotatef(m_angle+m_startAngle, 0., 0., 1.);   
      glTranslatef(-a_xPos - m_width/2.0, -a_yPos - m_height/2.0, 0.0);

      glBegin( GL_QUADS );
      glTexCoord2d(0.0,0.0); 
      glVertex2d(a_xPos, a_yPos);

      glTexCoord2d(1.0,0.0);
      glVertex2d(m_width+a_xPos, a_yPos);

      glTexCoord2d(1.0,1.0); 
      glVertex2d(m_width+a_xPos, m_height+a_yPos);

      glTexCoord2d(0.0,1.0); 
      glVertex2d(a_xPos,m_height+a_yPos);

      glEnd();

      glPopMatrix();
      glDisable( GL_TEXTURE_2D );

      //////////////////////////////////////////////////////////////////////////
      // The following section is for hull drawing (only for debugging purposes)
      //////////////////////////////////////////////////////////////////////////
      // DEBUG: Show Hull         

      // If object has a parent, rotate with parent angle around parent center
      float a_angle     = 0.0;
      float a_xCenter   = 0.0;
      float a_yCenter   = 0.0;
      bool  a_direction = 0.0;

      if (g_showBox || g_showBox)
      {
         if (0 != a_parentPtr)
         {
            // Object has parent, use parent coordinates
            if (getType() == e_shot)
            {  
               if (a_parentPtr->getType() != e_player)
               {
                  a_angle     =  a_parentPtr->m_angle;
               }         
               else
               {
                  a_angle     = m_angle;
               }
            }
            else
            {
               a_angle     = a_parentPtr->m_angle + m_angle;      
            }      
            a_xCenter   = a_parentPtr->m_xPos + a_parentPtr->m_width/2.0;
            a_yCenter   = a_parentPtr->m_yPos + a_parentPtr->m_height/2.0;
            a_direction = a_parentPtr->m_direction;
         }
         else
         {
            // Object has no parent, use own coordinates
            a_angle     = m_angle;
            a_xCenter   = a_xPos + m_width/2.0;
            a_yCenter   = a_yPos + m_height/2.0;
            a_direction = m_direction;
         }
      }

      if (g_showBox)
      {
         // Show bounding rect
         glPushMatrix();

         // Rotation around object (or parent, if present) center with a_angle
         glTranslatef(a_xCenter, a_yCenter, 0.0);
         if (a_direction)
         {
            glRotatef(180.0, 0., 1., 0);
         }
         glRotatef(a_angle, 0.0, 0.0, 1.0);
         glTranslatef(-a_xCenter, -a_yCenter, 0.0);

         // Rotation around object angle with m_startAngle
         glTranslatef(a_xPos + m_width/2.0, a_yPos + m_height/2.0, 0.0);
         if (a_direction)
         {
            glRotatef(180.0, 0., 1., 0);
         }
         glRotatef(m_startAngle, 0.0, 0.0, 1.0);
         glTranslatef(-a_xPos - m_width/2.0, -a_yPos - m_height/2.0, 0.0); 

         if (getType() == e_shot)
         {
            glTranslatef(a_xPos + m_width/2.0, a_yPos + m_height/2.0, 0.0);

            glRotatef(m_angle, 0.0, 0.0, 1.0);   

            glTranslatef(-a_xPos - m_width/2.0, -a_yPos - m_height/2.0, 0.0); 
         }

         glColor4f(1.0,0.0,0.0,0.9);
         glBegin(GL_LINE_LOOP);
         glVertex2d(a_xPos, a_yPos);
         glVertex2d(m_width+a_xPos,a_yPos);
         glVertex2d(m_width+a_xPos,m_height+a_yPos);
         glVertex2d(a_xPos,m_height+a_yPos);
         glEnd();
         glPopMatrix();
         glColor4f(1.0,1.0,1.0,1.0);
      }

      // DEBUG: Show Hull poly
      CPolygon* a_polygonPtr = CObjectStorage::getInstance().m_textureMap[m_textureKeys[m_activeAnimationPhase]->m_textureKey]->m_hullPolygonPtr;
      if (g_showHull && 0 != a_polygonPtr)
      {         
         glColor4f(1.0,1.0,1.0,0.5);
         // Scale polygon to correct width and height
         a_polygonPtr->rescale(m_width / a_polygonPtr->m_width, m_height / a_polygonPtr->m_height);     

         // Translate polygon to correct position
         a_polygonPtr->translate(m_xPos, m_yPos);

         // Add start angle to rotation
         if (0.0 != m_startAngle)
         {
            a_polygonPtr->rotate(-m_startAngle, a_xPos + m_width/2.0, a_yPos + m_height/2.0);
         }

         // Rotate polygon
         a_polygonPtr->rotate(-a_angle, a_xCenter, a_yCenter);

         // Rotate polygon around center
         if (getType() == e_shot)
         {
            a_polygonPtr->rotate(-m_angle, a_xPos + m_width/2.0, a_yPos + m_height/2.0);
         }      

         // Flip polygon
         if (a_direction)
         {         
            if (0 != a_parentPtr)
               a_polygonPtr->flip(a_parentPtr->m_xPos + a_parentPtr->m_width/2.0);
            else
               a_polygonPtr->flip(a_xPos + m_width/2.0);
         }

         glBegin(GL_LINE_LOOP);      
         std::vector<CPoint*>::iterator a_it;
         for (a_it = a_polygonPtr->m_points.begin(); a_it != a_polygonPtr->m_points.end(); a_it++)
         {      
            glVertex2d((*a_it)->x, (*a_it)->y);      
         }
         glEnd();

         // Undo of: Flip polygon
         if (a_direction)
         {         
            if (0 != a_parentPtr)
               a_polygonPtr->flip(a_parentPtr->m_xPos + a_parentPtr->m_width/2.0);
            else
               a_polygonPtr->flip(a_xPos + m_width/2.0);
         }

         if (getType() == e_shot)
         {
            // Undo: Rotate polygon around center
            a_polygonPtr->rotate(m_angle, a_xPos + m_width/2.0, a_yPos + m_height/2.0);
         }      

         // Undo of: Rotate polygon
         a_polygonPtr->rotate(a_angle, a_xCenter, a_yCenter);

         // Undo of: Add start angle to rotation
         if (0.0 != m_startAngle)
         {
            a_polygonPtr->rotate(m_startAngle, a_xPos + m_width/2.0, a_yPos + m_height/2.0);
         }

         // Undo of: Translate polygon to correct position
         a_polygonPtr->translate(-a_xPos, -a_yPos);

         // Undo of: Scale polygon to correct width and height
         a_polygonPtr->rescale(a_polygonPtr->m_width / m_width, a_polygonPtr->m_height / m_height);

         glColor4f(1.0,1.0,1.0,1.0);
         // DEBUG END
      }
   }
   else
   {
      float       a_xPos   = m_xPos;
      float       a_yPos   = m_yPos;

      glPushMatrix();

      //////////////////////////////////////////////////////////////////////////
      // rotate around parent angle
      CObject* a_parentPtr = 0;
      if(0 != m_parentId)
      {
         a_parentPtr = CObjectStorage::getInstance().m_objectMap[m_parentId];
         a_xPos += a_parentPtr->m_xPos;
         a_yPos += a_parentPtr->m_yPos;
      }

      if(a_parentPtr != 0)
      {
         if (getType() != e_shot)
         {
            glTranslatef(a_parentPtr->m_xPos + a_parentPtr->m_width/2.0, a_parentPtr->m_yPos + a_parentPtr->m_height/2.0, 0.0);
            if(0 != a_parentPtr)
            {
               if(a_parentPtr->m_direction)
               {
                  glRotatef(180.0, 0., 1., 0);
               }
            }

            glRotatef(a_parentPtr->m_angle, 0., 0., 1.);
            glTranslatef(-a_parentPtr->m_xPos - a_parentPtr->m_width/2.0, -a_parentPtr->m_yPos - a_parentPtr->m_height/2.0, 0.0);
         }      
      }      
      //////////////////////////////////////////////////////////////////////////
      // rotate around own axis
      glTranslatef(a_xPos + m_width/2.0, a_yPos + m_height/2.0, 0.0);
      if (m_direction)
      {
         glRotatef(180.0, 0., 1., 0);
      }
      glRotatef(m_angle+m_startAngle, 0., 0., 1.);   
      glTranslatef(-a_xPos - m_width/2.0, -a_yPos - m_height/2.0, 0.0);

      glBegin(GL_LINE_LOOP);
      
      glVertex2f(m_xPos, m_yPos);
      glVertex2f(m_xPos+m_width, m_yPos);
      glVertex2f(m_xPos+m_width, m_yPos+m_height);
      glVertex2f(m_xPos, m_yPos+m_height);
      glVertex2f(m_xPos, m_yPos);

      glEnd();

      glPopMatrix();
   }
}
#endif


// void CObject::draw() 
// {
//    glEnable( GL_TEXTURE_2D );
//    glBindTexture(GL_TEXTURE_2D, CObjectStorage::getInstance().m_textureMap[m_textureKeys[m_activeAnimationPhase]->m_textureKey]->m_textureIdVector[m_activeTexture]);
//       
//    glPushMatrix();
// 
//    glBegin( GL_QUADS );
//    glTexCoord2d(0.0,0.0); 
//    glVertex2d(0.0+m_xPos,0.0+m_yPos);
// 
//    glTexCoord2d(1.0,0.0);
//    glVertex2d(m_width+m_xPos,0.0+m_yPos);
// 
//    glTexCoord2d(1.0,1.0); 
//    glVertex2d(m_width+m_xPos,m_height+m_yPos);
// 
//    glTexCoord2d(0.0,1.0); 
//    glVertex2d(0.0+m_xPos,m_height+m_yPos);
// 
//    glEnd();
// 
//    glPopMatrix();
//    glDisable( GL_TEXTURE_2D );
// 
//    CPolygon* a_polygonPtr = m_textureKeys[m_activeAnimationPhase]->m_polygonPtr;
// 
//    if (g_showHull && 0 != a_polygonPtr)
//    {      
//       // DEBUG: SHOW POLYGON HULL FOR COLLISION DETECTION
// 
//       // Scale polygon to correct width and height
//       a_polygonPtr->rescale(m_width / a_polygonPtr->m_width, m_height / a_polygonPtr->m_height);
// 
//       // Translate polygon to correct position
//       a_polygonPtr->translate(m_xPos, m_yPos);
// 
//       glBegin(GL_LINE_LOOP);
//       glColor3f(1.0,1.0,1.0);
//       std::vector<CPoint*>::iterator a_it;
//       for (a_it = a_polygonPtr->m_points.begin(); a_it != a_polygonPtr->m_points.end(); a_it++)
//       {      
//          glVertex2d((*a_it)->x, (*a_it)->y);      
//       }
//       glEnd();
// 
//       // Undo of: Translate polygon to correct position
//       a_polygonPtr->translate(-m_xPos, -m_yPos);
// 
//       // Undo of: Scale polygon to correct width and height
//       a_polygonPtr->rescale(a_polygonPtr->m_width / m_width, a_polygonPtr->m_height / m_height);
// 
//       // DEBUG END
//    }
// }



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Collision stuff
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Test for intersection of the segments
// See: Sedgewick, Algorithms
bool CObject::segmentsIntersect(CLine& l1, CLine& l2)
{
   bool r_ret;

   r_ret =  ((ccw(l1.p1, l1.p2, l2.p1)*ccw(l1.p1,l1.p2,l2.p2))<=0)&&
      ((ccw(l2.p1, l2.p2, l1.p1)*ccw(l2.p1,l2.p2,l1.p2))<=0);

   return r_ret;
}

// Test 3 points for being counter clockwise ordered
// See: Sedgewick, Algorithms
int CObject::ccw(CPoint& p0, CPoint& p1, CPoint& p2)
{
   float dx1, dx2, dy1, dy2;
   int   r_ret;

   dx1 = p1.x - p0.x;
   dy1 = p1.y - p0.y;

   dx2 = p2.x - p0.x;
   dy2 = p2.y - p0.y;

   if (dx1*dy2 > dy1*dx2)
      r_ret =  1;
   if (dx1*dy2 < dy1*dx2)
      r_ret =  -1;
   if (dx1*dy2 == dy1*dx2)
   {
      if ((dx1*dx2<0)||(dy1*dy2<0))
         r_ret =  -1;
      else
      {
         if ((dx1*dx1+dy1*dy1)>=(dx2*dx2+dy2*dy2))
            r_ret =  0;
         else
            r_ret =  1;
      }
   }

   return r_ret;
}


// Check for collisions of player with level objects
bool CObject::isCollided(CObject* t_firstPtr, CObject* t_secondPtr)
{   
   bool           r_ret = false;
   unsigned int   a_n;
   CPoint         a_p1, a_p2, a_p3, a_p4;
   CPoint         a_o1, a_o2, a_o3, a_o4;
   CLine          a_pl1, a_pl2, a_pl3, a_pl4;
   CLine          a_ol1, a_ol2, a_ol3, a_ol4;      


   //////////////////////////////////////////////////////////////////////////
   // Two CObjects cant collide
   if (t_firstPtr->getType() == e_object && t_secondPtr->getType() == e_object)
   {
      return false;
   }

   //////////////////////////////////////////////////////////////////////////
   // two invincible Objects cant collide
   if (t_firstPtr->m_invincible && t_secondPtr->m_invincible)
   {
      return false;
   }

   //////////////////////////////////////////////////////////////////////////
   // two objects which do not make damage cant collide
   if (t_firstPtr->m_damagePoints == 0 && t_secondPtr->m_damagePoints == 0)
   {
      return false;
   }

   //////////////////////////////////////////////////////////////////////////
   // one object invincible, and makes no damage => no collision
   if (t_firstPtr->m_damagePoints == 0 && t_firstPtr->m_invincible ||
       t_secondPtr->m_damagePoints == 0 && t_secondPtr->m_invincible)
   {
      return false;
   }

   //////////////////////////////////////////////////////////////////////////
   // shots don't kill their friends
   if(t_firstPtr->getType() == e_shot)
   {
      if(static_cast<CShot*>(t_firstPtr)->isFriend(t_secondPtr->m_id))
      {
         return false;
      }
   }
   if(t_secondPtr->getType() == e_shot)
   {
      if(static_cast<CShot*>(t_secondPtr)->isFriend(t_firstPtr->m_id))
      {
         return false;
      }
   }

   //////////////////////////////////////////////////////////////////////////
   // Dying objects don't collide anymore
   if (t_firstPtr->m_isDying || t_secondPtr->m_isDying)
   {
      return false;
   }

   //////////////////////////////////////////////////////////////////////////
   // We can only collide with non - background objects
   if(t_firstPtr->m_isBackground || t_secondPtr->m_isBackground)
   {
      return false;
   }

   //////////////////////////////////////////////////////////////////////////
   // Two shots cant collide
   if(t_firstPtr->getType() == e_shot && t_secondPtr->getType() == e_shot)
   {
      return false;
   }
   
   //////////////////////////////////////////////////////////////////////////
   // Collision detection
   //////////////////////////////////////////////////////////////////////////

   //////////////////////////////////////////////////////////////////////////
   // First: a simple distance test (is collsion possible)

   float          a_radius1 = 0.0;
   float          a_radius2 = 0.0;   
   CPoint         a_middlePoint1 = CPoint(t_firstPtr->m_xPos + t_firstPtr->m_width/2, t_firstPtr->m_yPos + t_firstPtr->m_height/2);
   CPoint         a_middlePoint2 = CPoint(t_secondPtr->m_xPos + t_secondPtr->m_width/2, t_secondPtr->m_yPos + t_secondPtr->m_height/2);

   if (t_firstPtr->m_width > t_firstPtr->m_height)
   {
      a_radius1 = t_firstPtr->m_width;
   }
   else
   {
      a_radius1 = t_firstPtr->m_height;
   }

   if (t_secondPtr->m_width > t_secondPtr->m_height)
   {
      a_radius2 = t_secondPtr->m_width;
   }
   else
   {
      a_radius2 = t_secondPtr->m_height;
   }
   
   if (a_middlePoint1.dist(a_middlePoint2) < a_radius1+a_radius2)
   {
      r_ret = true;
   }

//    // Object 1 square coordinates
//    a_o1.x = t_firstPtr->m_xPos;
//    a_o1.y = t_firstPtr->m_yPos;
// 
//    a_o2.x = t_firstPtr->m_xPos + t_firstPtr->m_width;
//    a_o2.y = t_firstPtr->m_yPos;
// 
//    a_o3.x = t_firstPtr->m_xPos + t_firstPtr->m_width;
//    a_o3.y = t_firstPtr->m_yPos + t_firstPtr->m_height;
// 
//    a_o4.x = t_firstPtr->m_xPos;
//    a_o4.y = t_firstPtr->m_yPos + t_firstPtr->m_height;
// 
//    // Object 2 square coordinates  
//    a_p1.x = t_secondPtr->m_xPos;
//    a_p1.y = t_secondPtr->m_yPos;
// 
//    a_p2.x = t_secondPtr->m_xPos + t_secondPtr->m_width;
//    a_p2.y = t_secondPtr->m_yPos;
// 
//    a_p3.x = t_secondPtr->m_xPos + t_secondPtr->m_width;
//    a_p3.y = t_secondPtr->m_yPos + t_secondPtr->m_height;
// 
//    a_p4.x = t_secondPtr->m_xPos;        
//    a_p4.y = t_secondPtr->m_yPos + t_secondPtr->m_height;
// 
//    a_middlePoint1 = CPoint(a_o1.x + t_firstPtr->m_width/2, a_o1.y + t_firstPtr->m_height/2);
//    a_middlePoint2 = CPoint(a_p1.x + t_secondPtr->m_width/2, a_p1.y + t_secondPtr->m_height/2);
// 
//    // Rotate with first object angle         
//    a_o1 = a_o1 - a_middlePoint1;
//    a_o2 = a_o2 - a_middlePoint1;
//    a_o3 = a_o3 - a_middlePoint1;
//    a_o4 = a_o4 - a_middlePoint1;
// 
//    a_o1.rotate(-t_firstPtr->m_angle);   
//    a_o2.rotate(-t_firstPtr->m_angle);
//    a_o3.rotate(-t_firstPtr->m_angle);
//    a_o4.rotate(-t_firstPtr->m_angle);
// 
//    a_o1 = a_o1 + a_middlePoint1;
//    a_o2 = a_o2 + a_middlePoint1;
//    a_o3 = a_o3 + a_middlePoint1;
//    a_o4 = a_o4 + a_middlePoint1;
//   
//    a_ol1.p1 = a_o1;
//    a_ol1.p2 = a_o2;
// 
//    a_ol2.p1 = a_o2;
//    a_ol2.p2 = a_o3;
// 
//    a_ol3.p1 = a_o3;
//    a_ol3.p2 = a_o4;
// 
//    a_ol4.p1 = a_o4;
//    a_ol4.p2 = a_o1;
// 
//    // Rotate Object2 with object angle      
//    a_p1 = a_p1 - a_middlePoint2;
//    a_p2 = a_p2 - a_middlePoint2;
//    a_p3 = a_p3 - a_middlePoint2;
//    a_p4 = a_p4 - a_middlePoint2;
// 
//    a_p1.rotate(-t_secondPtr->m_angle);   
//    a_p2.rotate(-t_secondPtr->m_angle);
//    a_p3.rotate(-t_secondPtr->m_angle);
//    a_p4.rotate(-t_secondPtr->m_angle);
// 
//    a_p1 = a_p1 + a_middlePoint2;
//    a_p2 = a_p2 + a_middlePoint2;
//    a_p3 = a_p3 + a_middlePoint2;
//    a_p4 = a_p4 + a_middlePoint2;
// 
//    // Lines
//    a_pl1.p1 = a_p1;
//    a_pl1.p2 = a_p2;
// 
//    a_pl2.p1 = a_p2;
//    a_pl2.p2 = a_p3;
// 
//    a_pl3.p1 = a_p3;
//    a_pl3.p2 = a_p4;
// 
//    a_pl4.p1 = a_p4;
//    a_pl4.p2 = a_p1;           
// 
//    //////////////////////////////////////////////////////////////////////////
//    // (1.) 
//    // Test for intersection of object 1 
//    // square and object 2 square
//    //////////////////////////////////////////////////////////////////////////
// 
//    // Test all 4 border lines of player square
//    // for intersection with all 4 object square lines   
//    if (segmentsIntersect(a_pl1, a_ol1))
//    {
//       r_ret = true;
//    }
//    else if (segmentsIntersect(a_pl1, a_ol2))
//    {
//       r_ret = true;
//    }
//    else if (segmentsIntersect(a_pl1, a_ol3))
//    {
//       r_ret = true;
//    }
//    else if (segmentsIntersect(a_pl1, a_ol4))
//    {
//       r_ret = true;
//    }
//    else if (segmentsIntersect(a_pl2, a_ol1))
//    {
//       r_ret = true;
//    }
//    else if (segmentsIntersect(a_pl2, a_ol2))
//    {
//       r_ret = true;
//    }
//    else if (segmentsIntersect(a_pl2, a_ol3))
//    {
//       r_ret = true;
//    }
//    else if (segmentsIntersect(a_pl2, a_ol4))
//    {
//       r_ret = true;
//    }
//    else if (segmentsIntersect(a_pl3, a_ol1))
//    {
//       r_ret = true;
//    }
//    else if (segmentsIntersect(a_pl3, a_ol2))
//    {
//       r_ret = true;
//    }
//    else if (segmentsIntersect(a_pl3, a_ol3))
//    {
//       r_ret = true;
//    }
//    else if (segmentsIntersect(a_pl3, a_ol4))
//    {
//       r_ret = true;
//    }
//    else if (segmentsIntersect(a_pl4, a_ol1))
//    {
//       r_ret = true;
//    }
//    else if (segmentsIntersect(a_pl4, a_ol2))
//    {
//       r_ret = true;
//    }
//    else if (segmentsIntersect(a_pl4, a_ol3))
//    {
//       r_ret = true;
//    }
//    else if (segmentsIntersect(a_pl4, a_ol4))
//    {
//       r_ret = true;
//    }
//    // Center point inside bounding rect of the other?
//    else if (pointInRect(a_middlePoint1, a_p1, a_p2, a_p3, a_p4))
//    {
//       r_ret = true;
//    }

   //////////////////////////////////////////////////////////////////////////
   // (2.) 
   // We have a collision candidate, look closer
   // Check all polygon points for being inside target polygon
   //////////////////////////////////////////////////////////////////////////
   if (r_ret)
   {            
      // r_ret will be set to true if polygons collide
      r_ret = false;

      CPolygon* a_polygonAPtr = t_firstPtr->m_textureKeys[t_firstPtr->m_activeAnimationPhase]->m_polygonPtr;
      CPolygon* a_polygonBPtr = t_secondPtr->m_textureKeys[t_secondPtr->m_activeAnimationPhase]->m_polygonPtr;
      
      if((0 != a_polygonAPtr) &&
         (0 != a_polygonBPtr)    )
      {
         // Translate polygon A to correct position
         a_polygonAPtr->translate(t_firstPtr->m_xPos, t_firstPtr->m_yPos);

         // Rotate polygon A
         if (0.0 != t_firstPtr->m_angle)
         {
            a_polygonAPtr->rotate(-t_firstPtr->m_angle, t_firstPtr->m_xPos + t_firstPtr->m_width/2.0, t_firstPtr->m_yPos + t_firstPtr->m_height/2.0);
         }

         // Translate polygon B to correct position
         a_polygonBPtr->translate(t_secondPtr->m_xPos, t_secondPtr->m_yPos);

         // Rotate polygon B
         if (0.0 != t_secondPtr->m_angle)
         {
            a_polygonBPtr->rotate(-t_secondPtr->m_angle, t_secondPtr->m_xPos + t_secondPtr->m_width/2.0, t_secondPtr->m_yPos + t_secondPtr->m_height/2.0);
         }         

         // Check polygon A for having points in B
         for (a_n = 0; a_n < a_polygonAPtr->m_points.size(); a_n++)
         {
            if (a_polygonBPtr->isInside(a_polygonAPtr->m_points[a_n]))
            {
               r_ret = true;
               break;
            }
         }

         if (!r_ret)
         {
            // Check polygon B for having points in A
            for (a_n = 0; a_n < a_polygonBPtr->m_points.size(); a_n++)
            {
               if (a_polygonAPtr->isInside(a_polygonBPtr->m_points[a_n]))
               {
                  r_ret = true;
                  break;
               }
            }
         }

         // Undo of: Rotate polygon A
         a_polygonAPtr->rotate(t_firstPtr->m_angle, t_firstPtr->m_xPos + t_firstPtr->m_width/2.0, t_firstPtr->m_yPos + t_firstPtr->m_height/2.0);

         // Undo of: Translate polygon A to correct position
         a_polygonAPtr->translate(-t_firstPtr->m_xPos, -t_firstPtr->m_yPos);

         // Undo of: Rotate polygon B
         a_polygonBPtr->rotate(t_secondPtr->m_angle, t_secondPtr->m_xPos + t_secondPtr->m_width/2.0, t_secondPtr->m_yPos + t_secondPtr->m_height/2.0);

         // Undo of: Translate polygon B to correct position
         a_polygonBPtr->translate(- t_secondPtr->m_xPos, - t_secondPtr->m_yPos);
      }   
   }

   return r_ret;
}



void CObject::update(CLevel* t_levelPtr)
{ 
   nextTexture();
}


void CObject::collisionImpact(CObject* t_objectPtr, bool t_checkOther)
{
   m_hitPoints -= t_objectPtr->m_damagePoints;


   if(t_checkOther)
   {
      t_objectPtr->collisionImpact(this, false);
   }   
}

bool CObject::intersects(CObject* t_objectPtr, float t_top, float t_left, float t_right, float t_bottom)
{
   bool r_ret = false;
   
   float          a_pseudoWidth  = 0.0;
   float          a_pseudoHeight = 0.0;

   float          a_objLeft    = 0.0;
   float          a_objTop     = 0.0;
   float          a_objRight   = 0.0;
   float          a_objBottom  = 0.0;

   if (t_objectPtr->getType() != e_object)
   {
      if (t_objectPtr->m_width > t_objectPtr->m_height)
      {
         a_pseudoWidth  = t_objectPtr->m_width;
         a_pseudoHeight = t_objectPtr->m_width;
      }
      else 
      {
         a_pseudoWidth  = t_objectPtr->m_height;
         a_pseudoHeight = t_objectPtr->m_height;
      }      
   }
   else
   {
      a_pseudoWidth  = t_objectPtr->m_width;
      a_pseudoHeight = t_objectPtr->m_height;
   }

   a_objLeft   = t_objectPtr->m_xPos + t_objectPtr->m_width/2.0 - a_pseudoWidth/2.0;
   a_objRight  = t_objectPtr->m_xPos + t_objectPtr->m_width/2.0 + a_pseudoWidth/2.0;

   a_objTop    = t_objectPtr->m_yPos + t_objectPtr->m_height/2.0 + a_pseudoHeight/2.0;
   a_objBottom = t_objectPtr->m_yPos + t_objectPtr->m_height/2.0 - a_pseudoHeight/2.0;  
  

   // Object and Node intersect
   if ((a_objLeft            <= t_left    &&
        a_objRight           > t_right   &&
         (
            (a_objBottom <= t_top && a_objBottom > t_bottom) ||
            (a_objTop <= t_top    && a_objTop > t_bottom)
         )
       ) 
       ||
       (
        a_objBottom           <= t_top     &&
        a_objTop              > t_bottom  &&        
         (
            (a_objLeft >=  t_left && a_objLeft  < t_right) ||
            (a_objRight>=  t_left && a_objRight < t_right)
            
         )
       ))
   {
      r_ret = true;
   }

   // Object bigger than quad tree node?
   else if ( a_objLeft            <= t_left    &&
             a_objRight           > t_right   &&
             a_objTop             >= t_top     &&
             a_objBottom          < t_bottom )
   {
      r_ret = true;
   }

   return r_ret;
}

void CObject::startDying()
{   
   std::vector<CObject*>::iterator  a_it;

   // Initiate explosion
   if (m_explosionIndex != -1 && !m_isDying)
   {
      // Set explosion as active texture
      m_activeAnimationPhase = m_explosionIndex;

      // Set animation to start
      m_timeCounter           = 0;
      m_activeTexture         = 0;

      // Set explosion speed
      m_cycleInterval         = 20*m_hitPoints/(m_hitPoints + 5);
   }
   else
   {
      // This object has no explosion texture

      // Set cycle interval to 0, so it will be finished with his animation very soon
      m_cycleInterval = 0;
   }

   // Set dying flag
   m_isDying               = true;

   // Delete all children
   CObjectStorage::getInstance().deleteChildren(m_id);
}