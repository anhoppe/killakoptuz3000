// ***************************************************************
//  CShot   version:  1.0   ·  date: 05/05/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include "KillaCoptuz3000/src/Objects/CShot.h"
#include "KillaCoptuz3000/src/CObjectStorage.h"
#include "KillaCoptuz3000/src/CEvent.h"
#include "KillaCoptuz3000/src/CLevel.h"
#include "KillaCoptuz3000/src/Functions.h"

#include "KillaCoptuz3000/src/globals.h"

CShot::CShot(CShot* t_shotPtr, std::list<unsigned int>* t_friendObjectsListPtr)
{
   // Copy constructor creates shot from default shot
   m_xPos            = t_shotPtr->m_xPos;
   m_yPos            = t_shotPtr->m_yPos;

   m_width           = t_shotPtr->m_width;
   m_height          = t_shotPtr->m_height;

   m_angle           = t_shotPtr->m_angle;

   m_cycleInterval   = t_shotPtr->m_cycleInterval;

   m_isBackground    = t_shotPtr->m_isBackground;

   m_explosionIndex  = t_shotPtr->m_explosionIndex;

   m_activeAnimationPhase  = t_shotPtr->m_activeAnimationPhase;

   unsigned int a_i;

#if (PRODUCT == KK3000)
   for(a_i = 0; a_i < t_shotPtr->m_textureKeys.size(); a_i++)
   {      
      m_textureKeys.push_back(new CTextureInfo(t_shotPtr->m_textureKeys[a_i]));
   }
#else
   for(a_i = 0; a_i < t_shotPtr->m_textureKeys.size(); a_i++)
   {      
      m_textureKeys.push_back(t_shotPtr->m_textureKeys[a_i]);
   }
#endif

   m_activeTexture   = t_shotPtr->m_activeTexture;

   m_timeCounter     = 0;

   m_v               = t_shotPtr->m_v;
   m_velocityX       =  -t_shotPtr->m_v*sin(m_angle/180.0*M_PI);
   m_velocityY       =  t_shotPtr->m_v*cos(m_angle/180.0*M_PI);

   m_invincible      = t_shotPtr->m_invincible;

   m_hitPoints       = t_shotPtr->m_hitPoints;
   m_maxHitPoints    = t_shotPtr->m_maxHitPoints;   

   m_parentId        = t_shotPtr->m_parentId;

   m_drawLayer       = t_shotPtr->m_drawLayer;

   m_damagePoints    = t_shotPtr->m_damagePoints;

   // copy shot type
   m_shotType        = t_shotPtr->m_shotType;


   // copy friend objects. bit sorry that they are not in the t_srcPtr
   if(0 != t_friendObjectsListPtr)
   {
      std::list<unsigned int>::iterator a_it;

      for(a_it = t_friendObjectsListPtr->begin(); a_it != t_friendObjectsListPtr->end(); a_it++)
      {
         m_friendObjects.push_back(*a_it);
      }
   }

}

CShot::CShot(TiXmlNode* t_nodePtr)
{
   load(t_nodePtr);
}

CShot::~CShot()
{   
}

bool CShot::load(TiXmlNode* t_nodePtr)
{
   TiXmlNode*     a_nodePtr   = 0;
   TiXmlElement*  a_elemPtr   = 0;

   std::string    a_str;   

   a_elemPtr = t_nodePtr->ToElement();
   
   if (getAttributeStr(a_elemPtr, "v", a_str))
   {      
      m_v = atof(a_str.c_str());
   }

   m_shotType = e_shotNormal;
   
   if(getAttributeStr(a_elemPtr, "ballistic", a_str))
   {
      if(a_str == "1")
      {
         m_shotType = e_shotBallistic;
      }
   }

   CObject::load(t_nodePtr);
   return true;
}

void CShot::update(CLevel* t_levelPtr)
{
   float          a_dx = 0.;
   float          a_dy = 0.;

//    static double  s_startAngle = m_angle;   
  
   if (m_shotType == e_shotNormal)
   {
      m_velocityX       = -m_v*sin(m_angle/180.0*M_PI);
      m_velocityY       = m_v*cos(m_angle/180.0*M_PI);
   }
   else if(m_shotType == e_shotBallistic)
   {
      m_velocityY -= g_gravity;

      if (0.0 != m_velocityY)
      {
         m_angle = -atan(m_velocityX / m_velocityY)* (180.0 / M_PI) + 180.0;
      }
      else
      {
         //m_angle = -180.0;
      }

      m_v = sqrt(m_velocityX*m_velocityX + m_velocityY*m_velocityY);
   }

   a_dx = m_velocityX;
   a_dy = m_velocityY;


   //////////////////////////////////////////////////////////////////////////
   // move shot and check if shot must be deleted
   if (t_levelPtr->positionAllowed(m_xPos + m_width/2.0 + a_dx, m_yPos + m_height/2.0 + a_dy))
   {
      m_xPos += a_dx;
      m_yPos += a_dy;
   }
   else
   {
      // Create a delete event
      CEvent* a_event         = new CEvent;
      a_event->m_event        = e_delete;
      a_event->m_objectList.push_back(m_id);

      CObjectStorage::getInstance().m_eventList.push_back(a_event);
   }

   CSprite::update(t_levelPtr);
}

bool CShot::isFriend(unsigned int t_objectId)
{
   bool r_ret = false;
   std::list<unsigned int>::iterator a_it;

   for(a_it = m_friendObjects.begin(); a_it != m_friendObjects.end(); a_it++)
   {
      if(*a_it == t_objectId)
      {
         r_ret = true;
         break;
      }
   }

   return r_ret;
}
