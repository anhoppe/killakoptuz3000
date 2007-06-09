// ***************************************************************
//  CCombatant   version:  1.0   ·  date: 06/09/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "Objects/CCombatant.h"

CCombatant::CCombatant()
{
   m_activeWeapon = 0;
}

bool CCombatant::load(TiXmlNode* t_nodePtr)
{
   TiXmlNode*     a_nodePtr   = 0;
   TiXmlElement*  a_elemPtr   = 0;
   CWeapon*       a_weaponPtr = 0;

   std::string    a_str;

   m_scriptPtr    = 0;


   // load object specific content
   CObject::load(t_nodePtr);

   // Loop over all weapons
   for(a_nodePtr = t_nodePtr->FirstChild("weapon"); a_nodePtr; a_nodePtr = t_nodePtr->IterateChildren("weapon", a_nodePtr))
   {
      CObjectStorage::getInstance()->add(a_nodePtr, e_weapon, m_id);
   }

   return true;
}

void CPlayer::fireWeapon()
{   
   if (m_activeWeapon < m_weaponList.size())
   {
      m_weaponList[m_activeWeapon]->fire();
   }
}

void CPlayer::nextWeapon()
{
   m_activeWeapon++;

   m_activeWeapon = m_activeWeapon % m_weaponList.size();
}