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
#include "Objects/CShot.h"
#include "CObjectStorage.h"
#include "CEvent.h"
#include "CLevel.h"
#include "Functions.h"


CShot::CShot(CShot* t_shotPtr)
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

   for(a_i = 0; a_i < t_shotPtr->m_textureKeys.size(); a_i++)
   {      
      m_textureKeys.push_back(new CTextureInfo(t_shotPtr->m_textureKeys[a_i]));
   }

   m_activeTexture   = t_shotPtr->m_activeTexture;

   m_timeCounter     = 0;

   m_v               = t_shotPtr->m_v;   

   m_invincible      = t_shotPtr->m_invincible;

   m_hitPoints       = t_shotPtr->m_hitPoints;
   m_maxHitPoints    = t_shotPtr->m_maxHitPoints;   

   m_parentId        = t_shotPtr->m_parentId;

   m_drawLayer       = t_shotPtr->m_drawLayer;
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

   CObject::load(t_nodePtr);
   return true;
}

void CShot::update(CLevel* t_levelPtr)
{
   float a_dx = -m_v*sin(m_angle*M_PI/180);
   float a_dy = m_v*cos(m_angle*M_PI/180);

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