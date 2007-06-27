// ***************************************************************
//  CEnemy   version:  1.0   ·  date: 05/05/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "KillaCoptuz3000/src/CLevel.h"
#include "KillaCoptuz3000/src/Objects/CEnemy.h"
#include "KillaCoptuz3000/src/Objects/CWeapon.h"
#include "KillaCoptuz3000/src/CObjectStorage.h"
#include "KillaCoptuz3000/src/Functions.h"
#include "KillaCoptuz3000/src/globals.h"
#include "KillaCoptuz3000/src/main.h"

//////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////

CEnemy::CEnemy()
{
   m_scriptPtr       = 0;

   m_behaviorData.m_xPosPtr         = &m_xPos;
   m_behaviorData.m_yPosPtr         = &m_yPos;
   m_behaviorData.m_direction       = &m_direction;
   m_behaviorData.m_dxPtr           = &m_dx;
   m_behaviorData.m_dyPtr           = &m_dy;   
}

CEnemy::~CEnemy()
{
   if (!m_scriptPtr)
      delete m_scriptPtr;

}

bool CEnemy::load(TiXmlNode* t_nodePtr)
{
   TiXmlNode*     a_nodePtr   = 0;
   TiXmlElement*  a_elemPtr   = 0;
   CWeapon*       a_weaponPtr = 0;
   
   std::string    a_str;

   m_scriptPtr    = 0;

   CCombatant::load(t_nodePtr);

   // Loop over all weapons
   a_nodePtr = t_nodePtr->FirstChild("script");
   if(0 != a_nodePtr)
   {
      a_elemPtr = a_nodePtr->ToElement();

      // Create a new script                  
      if (getAttributeStr(a_elemPtr, "filename", a_str))
      {            
         m_scriptPtr = new CScript(a_str.c_str());  
      }
   }

   // The player is the default tracked object
   m_trackList.push_back(CObjectStorage::getInstance().m_playerId);

   return true;
}



void CEnemy::update(CLevel* t_levelPtr)
{
   if(m_scriptPtr)
   {
      m_scriptPtr->update(m_behaviorData);      
   }

   CSprite::update(t_levelPtr);
}
