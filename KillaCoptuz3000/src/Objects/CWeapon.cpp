// ***************************************************************
//  CWeapon   version:  1.0   �  date: 05/05/2007
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
#include "Objects/CWeapon.h"
#include "Objects/CShot.h"
#include "CLevel.h"
#include "Functions.h"
#include "globals.h"

#define MAX_FIRE_ANGLE_DEVIATION 10

//////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////
CWeapon::CWeapon(float t_xPos, float t_yPos)
: CSprite(t_xPos, t_yPos)
{
   m_angle           = 0.0;
   m_maxAngle        = 0.0;
   m_minAngle        = 0.0;
   m_maxShots        = 1;
   m_framesSinceShot = 0;
   m_framesPerShot   = 100;
   m_shotPtr         = 0;
}

CWeapon::CWeapon(TiXmlNode* t_nodePtr)
{
   m_angle           = 0.0;
   m_framesSinceShot = 0;
   m_shotPtr         = 0;

   load(t_nodePtr);
}

CWeapon::~CWeapon()
{
   if (m_shotPtr)
      delete m_shotPtr;
}

bool CWeapon::load(TiXmlNode* t_nodePtr)
{
   TiXmlNode*     a_nodePtr   = 0;
   TiXmlElement*  a_elemPtr   = 0;
   bool           r_ret       = true;
   std::string    a_str;   

   a_elemPtr = t_nodePtr->ToElement();

   // Get weapon properties
   r_ret = r_ret & getAttributeStr(a_elemPtr, "minAngle", a_str);
   m_minAngle         = atof(a_str.c_str());

   r_ret = r_ret & getAttributeStr(a_elemPtr, "maxAngle", a_str);
   m_maxAngle         = atof(a_str.c_str());

   if (getAttributeStr(a_elemPtr, "startAngle", a_str))
   {
      m_startAngle = atof(a_str.c_str());
   }

   r_ret = r_ret & getAttributeStr(a_elemPtr, "maxShots", a_str);
   m_maxShots         = atoi(a_str.c_str());   

   r_ret = r_ret & getAttributeStr(a_elemPtr, "shotRadius", a_str);
   m_shotRadius       = atof(a_str.c_str());

   r_ret = r_ret & getAttributeStr(a_elemPtr, "framesPShot", a_str);
   m_framesPerShot    = atof(a_str.c_str());   

   // Find shot of weapon
   a_nodePtr = t_nodePtr->FirstChild();
   do
   {
      a_elemPtr = a_nodePtr->ToElement();
      if(!strcmp("shot", a_elemPtr->Value()))
      {
         m_shotPtr = new CShot(a_nodePtr);   
         break;  
      }
      a_nodePtr = t_nodePtr->IterateChildren(a_nodePtr);      
   }
   while(a_nodePtr);   

   CObject::load(t_nodePtr);

   return r_ret;
}

void CWeapon::controlAngle()
{
   if (m_angle < 0.0)
   {
      if (m_angle < m_minAngle)
         m_angle = m_minAngle;
   }
   else
   {
      if (m_angle > m_maxAngle)
         m_angle = m_maxAngle;
   }
}

void CWeapon::update(CLevel* t_levelPtr, std::vector<CObject*>::iterator& t_it, std::vector<CObject*>::iterator& t_endIt)
{
   float a_playerDist = 0.;
   float a_trackAngle = 0.;
   float a_oldAngle = m_angle;

   if(m_trackIndex < m_trackList.size())
   {
      a_playerDist = sqrt((m_trackList[m_trackIndex]->m_xPos - m_xPos)*(m_trackList[m_trackIndex]->m_xPos - m_xPos) + (m_trackList[m_trackIndex]->m_yPos - m_yPos)*(m_trackList[m_trackIndex]->m_yPos - m_yPos));
   }
   //////////////////////////////////////////////////////////////////////////
   // since weapons are mounted to a potential movable object, weapons add
   // the parent movement
   if(0 != m_parentPtr)
   {
      m_xPos += m_parentPtr->m_dx;
      m_yPos += m_parentPtr->m_dy;      
   }

   //////////////////////////////////////////////////////////////////////////
   // track the enemy
   if(m_trackIndex < m_trackList.size())
   {
      a_trackAngle = trackAngle(m_trackList[m_trackIndex]->m_xPos, m_trackList[m_trackIndex]->m_yPos);
      m_angle = a_trackAngle;
   }
   // Controls m_angle for being in the allowed interval
   if (m_parentPtr != g_playerPtr)
   {
      controlAngle();
   }
//    else
//    {
//       m_angle = m_parentPtr->m_angle + m_startAngle;    
//    }

   //////////////////////////////////////////////////////////////////////////
   // Shooting
   // 1.) New shots
   if (/*m_maxShots > m_shotList.size() && */
      m_framesSinceShot >= m_framesPerShot &&
      a_playerDist <= m_shotRadius &&
      fabs(m_angle - a_trackAngle) <= MAX_FIRE_ANGLE_DEVIATION)
   {
      if(m_parentPtr != g_playerPtr)
      {
         fire();
      }
   }

   m_framesSinceShot++;

   CSprite::update(t_levelPtr, t_it, t_endIt);   
}

void CWeapon::fire()
{
   float a_angle  = 0.;

   // Create a new shot
   if(0 != m_shotPtr)
   {
      //////////////////////////////////////////////////////////////////////////
      // create a new shot
      CShot* a_shotPtr = new CShot(m_shotPtr);


      
      //       a_shotPtr->m_xPos = m_xPos + m_width / 2.0;
//       a_shotPtr->m_yPos = m_yPos + m_height / 2.0;


      //////////////////////////////////////////////////////////////////////////
      // set start position of the shot
      a_shotPtr->m_xPos += m_xPos + m_width / 2.0;
      a_shotPtr->m_yPos += m_yPos + m_height / 2.0;

      CPoint a_pos(a_shotPtr->m_xPos,
                   a_shotPtr->m_yPos);

      a_pos.x -=  m_parentPtr->m_xPos + m_parentPtr->m_width/2.0;
      a_pos.y -=  m_parentPtr->m_yPos + m_parentPtr->m_height/2.0;
      
      if(static_cast<CSprite*>(m_parentPtr)->m_direction)
      {
         a_angle += 180.;
         a_angle += m_parentPtr->m_angle;
      }
      else
      {
         a_angle -= m_parentPtr->m_angle;
      }
      a_pos.rotate(a_angle);

      a_pos.x += m_parentPtr->m_xPos + m_parentPtr->m_width/2.;
      a_pos.y += m_parentPtr->m_yPos + m_parentPtr->m_height/2.;

      a_shotPtr->m_xPos = a_pos.x;
      a_shotPtr->m_yPos = a_pos.y;

      //////////////////////////////////////////////////////////////////////////
      // set direction of shot
      a_shotPtr->m_angle = m_angle + m_startAngle;

      if(0 != m_parentPtr)
      {
         if(static_cast<CSprite*>(m_parentPtr)->m_direction)
         {
             a_shotPtr->m_angle += 180;
             a_shotPtr->m_angle -= m_parentPtr->m_angle;
         }
         else
         {
            a_shotPtr->m_angle += m_parentPtr->m_angle;
         }
      }

      CLevel::M_addList.push_back(a_shotPtr);
      m_framesSinceShot = 0;
   }   
}
