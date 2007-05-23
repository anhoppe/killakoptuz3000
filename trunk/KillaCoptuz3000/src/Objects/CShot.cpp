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
#include "CLevel.h"
#include "Functions.h"


CShot::CShot(float t_xPos, float t_yPos)
: CSprite(t_xPos, t_yPos)
{

}

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

   m_gfxType         = t_shotPtr->m_gfxType;

   unsigned int a_i;

   for(a_i = 0; a_i < t_shotPtr->m_textureKeys.size(); a_i++)
   {      
      m_textureKeys.push_back(t_shotPtr->m_textureKeys[a_i]);
   }

//    m_hullPolygonPtr = new CPolygon(t_shotPtr->m_hullPolygonPtr);

   m_activeTexture   = t_shotPtr->m_activeTexture;
   m_timeCounter     = 0;

   m_v               = t_shotPtr->m_v;
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

void CShot::update(CLevel* t_levelPtr, std::vector<CObject*>::iterator& t_it, std::vector<CObject*>::iterator& t_endIt)
{
   float a_dx = -m_v*sin(m_angle*M_PI/180);
   float a_dy = m_v*cos(m_angle*M_PI/180);

   m_xPos += a_dx;
   m_yPos += a_dy;   

   // check if shot is out of range
   if(!positionAllowed(m_xPos, m_yPos, t_levelPtr))
   {
      if(!this->m_isDeleted)
      {
         CLevel::M_deleteList.push_back(this);
         this->m_isDeleted = true;
      }
   }

   CSprite::update(t_levelPtr, t_it, t_endIt);
}

// Check if position is allowed by level description
bool CShot::positionAllowed(float t_x, float t_y, CLevel* t_levelPtr)
{
   if (t_x > t_levelPtr->m_maxX)
      return false;
   if (t_x < t_levelPtr->m_minX)
      return false;
   if (t_y > t_levelPtr->m_maxY)
      return false;
   if (t_y < t_levelPtr->m_minY)   
      return false;   

   return true;
}

void CShot::collisionImpact(CObject* t_objectPtr, bool t_checkOther /* = true */)
{
      // shot deletes itself as soon as it get hit
   if(!this->m_isDeleted)
   {
      CLevel::M_deleteList.push_back(this);
      this->m_isDeleted = true;
   }
   CObject::collisionImpact(t_objectPtr, t_checkOther);
}
