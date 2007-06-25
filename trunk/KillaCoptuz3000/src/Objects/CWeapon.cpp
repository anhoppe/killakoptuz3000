// ***************************************************************
//  CWeapon   version:  1.0   ·  date: 05/05/2007
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

#include "KillaCoptuz3000/src/Functions.h"
#include "KillaCoptuz3000/src/globals.h"

#include "KillaCoptuz3000/src/CLevel.h"

#include "KillaCoptuz3000/src/CObjectStorage.h"

#include "KillaCoptuz3000/src/Objects/CWeapon.h"
#include "KillaCoptuz3000/src/Objects/CShot.h"
#include "KillaCoptuz3000/src/Objects/CCombatant.h"
#include "KillaCoptuz3000/src/Objects/CPlayer.h"
#include "KillaCoptuz3000/src/Objects/CEnemy.h"

#define MAX_FIRE_ANGLE_DEVIATION 10

//////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////
CWeapon::CWeapon()
{
   m_angle           = 0.0;
   m_maxAngle        = 0.0;
   m_minAngle        = 0.0;
   m_maxShots        = 1;
   m_framesSinceShot = 0;
   m_framesPerShot   = 100;
   m_shotPtr         = 0;
   m_isTracking      = true;
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
         m_shotPtr->m_parentId = m_id;
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

void CWeapon::update(CLevel* t_levelPtr)
{   
   float a_dist = 0.;
   float a_trackAngle = 0.;
   float a_oldAngle = m_angle;
   float a_xPos, a_yPos;
   CCombatant* a_parentPtr = (CCombatant*) CObjectStorage::getInstance().m_objectMap[m_parentId];   
   CObject*    a_trackObject = 0;

   if (0 != a_parentPtr->m_trackList.size() && a_parentPtr->m_trackIndex < a_parentPtr->m_trackList.size())
   {
      a_trackObject = CObjectStorage::getInstance().m_objectMap[a_parentPtr->m_trackList[a_parentPtr->m_trackIndex]];
   }
   else
   {
      return;
   }

   // Proceed only if weapon is tracking 
   if (!m_isTracking || a_trackObject == 0)
   {
      return;
   }

   // Calculate distance of weapon and current target
   a_xPos = m_xPos + a_parentPtr->m_xPos;
   a_yPos = m_yPos + a_parentPtr->m_yPos;
   a_dist = sqrt((a_trackObject->m_xPos - a_xPos)*(a_trackObject->m_xPos - a_xPos) + (a_trackObject->m_yPos - a_yPos)*(a_trackObject->m_yPos - a_yPos));   

   //////////////////////////////////////////////////////////////////////////
   // track the enemy
   a_trackAngle = trackAngle(a_trackObject->m_xPos, a_trackObject->m_yPos);
   m_angle = a_trackAngle;   
   
   // Controls m_angle for being in the allowed interval         
   controlAngle();

   //////////////////////////////////////////////////////////////////////////
   // Shooting
   // 1.) New shots
   if (/*m_maxShots > m_shotList.size() && */
      m_framesSinceShot >= m_framesPerShot &&
      a_dist <= m_shotRadius &&
      fabs(m_angle - a_trackAngle) <= MAX_FIRE_ANGLE_DEVIATION)
   {
      if(a_parentPtr->getType() != e_player)
      {
         fire();
      }
   }

   m_framesSinceShot++;

   CSprite::update(t_levelPtr);
}

float CWeapon::trackAngle(float t_xTrg, float t_yTrg)
{
   CPoint         a_diffVec;
   float          r_ret       = 0.;
   CCombatant*    a_parentPtr = (CCombatant*) CObjectStorage::getInstance().m_objectMap[m_parentId];


   if(a_parentPtr->m_trackIndex < a_parentPtr->m_trackList.size())
   {
      a_diffVec.x = a_parentPtr->m_xPos - t_xTrg;
      a_diffVec.y = a_parentPtr->m_yPos - t_yTrg;

      r_ret = a_diffVec.getAngle();
   }

   return r_ret;
}

void CWeapon::fire()
{
   float                   a_angle        = 0.;
   unsigned int            a_newId        = 0;
   CShot*                  a_newShotPtr   = 0;
   CObject*                a_parentPtr    = 0;
   std::list<unsigned int> a_friendlyObjects;

   // Create a new shot
   if(0 != m_shotPtr)
   {
      // add new shot
      a_friendlyObjects.push_back(m_parentId);
      a_friendlyObjects.push_back(m_id);
      a_newId = CObjectStorage::getInstance().add(m_shotPtr, 0, &a_friendlyObjects);

      // get pointer to new shot
      a_newShotPtr = (CShot*)(CObjectStorage::getInstance().m_objectMap[a_newId]);

      // get pointer to weapons parent
      assert(0 != m_parentId);
      
      if(0 != m_parentId)
      {
         a_parentPtr = CObjectStorage::getInstance().m_objectMap[m_parentId];
      }

      // Add parents velocity to ballistic shots velocity
      if (a_newShotPtr->m_shotType == e_shotBallistic)
      {
         float a_dvx, a_dvy;
         if (a_parentPtr->getType() == e_player)
         {
            a_dvx = ((CPlayer*)a_parentPtr)->m_velocityX;
            a_dvy = ((CPlayer*)a_parentPtr)->m_velocityY;
         }
         else if (a_parentPtr->getType() == e_enemy)
         {
            a_dvx = *((CEnemy*)a_parentPtr)->m_behaviorData.m_dxPtr;
            a_dvy = *((CEnemy*)a_parentPtr)->m_behaviorData.m_dyPtr;
         }

         a_newShotPtr->m_v += sqrt(a_dvx*a_dvx + a_dvy*a_dvy);
         a_newShotPtr->m_velocityX += a_dvx;
         a_newShotPtr->m_velocityY += a_dvy;
      }

      
      //       a_shotPtr->m_xPos = m_xPos + m_width / 2.0;
//       a_shotPtr->m_yPos = m_yPos + m_height / 2.0;


      //////////////////////////////////////////////////////////////////////////
      // set start position of the shot
      a_newShotPtr->m_xPos += m_xPos + m_width / 2.0;
      a_newShotPtr->m_yPos += m_yPos + m_height / 2.0;

      if(0 != a_parentPtr)
      {
         a_newShotPtr->m_xPos += a_parentPtr->m_xPos;
         a_newShotPtr->m_yPos += a_parentPtr->m_yPos;
      }

      CPoint a_pos(a_newShotPtr->m_xPos,
                   a_newShotPtr->m_yPos);

      a_pos.x -=  a_parentPtr->m_xPos + a_parentPtr->m_width/2.0;
      a_pos.y -=  a_parentPtr->m_yPos + a_parentPtr->m_height/2.0;
      
      if(a_parentPtr->m_direction)
      {
         a_angle += 180.;
         a_angle += a_parentPtr->m_angle;
      }
      else
      {
         a_angle -= a_parentPtr->m_angle;
      }
      a_pos.rotate(a_angle);

      a_pos.x += a_parentPtr->m_xPos + a_parentPtr->m_width/2.;
      a_pos.y += a_parentPtr->m_yPos + a_parentPtr->m_height/2.;

      a_newShotPtr->m_xPos = a_pos.x;
      a_newShotPtr->m_yPos = a_pos.y;

      //////////////////////////////////////////////////////////////////////////
      // set direction of shot
      a_newShotPtr->m_angle = m_angle + m_startAngle;

      if(0 != a_parentPtr)
      {
         if(a_parentPtr->m_direction)
         {
            a_newShotPtr->m_angle += 180;
            a_newShotPtr->m_angle -= a_parentPtr->m_angle;
         }
         else
         {
            a_newShotPtr->m_angle += a_parentPtr->m_angle;
         }
      }

//       CLevel::M_addList.push_back(a_shotPtr);
      m_framesSinceShot = 0;
   }   
}
