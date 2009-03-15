// ***************************************************************
//  CCombatant   version:  1.0   ·  date: 06/09/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "KillaCoptuz3000/src/Objects/CCombatant.h"

#include "KillaCoptuz3000/src/CObjectStorage.h"

CCombatant::CCombatant()
{
   m_activeWeapon = 0;
   m_trackIndex   = 0;
}

#if(PRODUCT == KK3000)
bool CCombatant::load(TiXmlNode* t_nodePtr)
{
   TiXmlNode*     a_nodePtr   = 0;
   TiXmlElement*  a_elemPtr   = 0;
   CWeapon*       a_weaponPtr = 0;

   std::string    a_str;
   unsigned int   a_weaponId  = 0;

   m_scriptPtr    = 0;


   // load object specific content
   CObject::load(t_nodePtr);

   // Loop over all weapons
   for(a_nodePtr = t_nodePtr->FirstChild("weapon"); a_nodePtr; a_nodePtr = t_nodePtr->IterateChildren("weapon", a_nodePtr))
   {
      a_weaponId  = CObjectStorage::getInstance().add(a_nodePtr, e_weapon, m_id);
      m_weaponList.push_back(a_weaponId);
      m_children .push_back(a_weaponId);

      // Player weapons don't autotrack
      if (getType() == e_player)
      {         
         ((CWeapon*)(CObjectStorage::getInstance().m_objectMap[a_weaponId]))->m_isTracking = false;
      }
   }

   return true;
}
#else
bool CCombatant::load(TiXmlNode* t_nodePtr)
{
   TiXmlNode*     a_nodePtr   = 0;
   TiXmlElement*  a_elemPtr   = 0;
   CWeapon*       a_weaponPtr = 0;

   std::string    a_str;
   unsigned int   a_weaponId  = 0;

   m_scriptPtr    = 0;


   // load object specific content
   CObject::load(t_nodePtr);

//    // Loop over all weapons
//    for(a_nodePtr = t_nodePtr->FirstChild("weapon"); a_nodePtr; a_nodePtr = t_nodePtr->IterateChildren("weapon", a_nodePtr))
//    {
//       a_weaponId  = CObjectStorage::getInstance().add(a_nodePtr, e_weapon, m_id);
//       m_weaponList.push_back(a_weaponId);
//       m_children .push_back(a_weaponId);
// 
//       // Player weapons don't autotrack
//       if (getType() == e_player)
//       {         
//          ((CWeapon*)(CObjectStorage::getInstance().m_objectMap[a_weaponId]))->m_isTracking = false;
//       }
//    }

   return true;
}
#endif

void CCombatant::fireWeapon()
{   
#if(PRODUCT == KK3000)
   if (m_activeWeapon < m_weaponList.size())
   {
      ((CWeapon*)(CObjectStorage::getInstance().m_objectMap[m_weaponList[m_activeWeapon]]))->fire();
   }
#endif
}

void CCombatant::nextWeapon()
{
   m_activeWeapon++;

   m_activeWeapon = m_activeWeapon % m_weaponList.size();
}
