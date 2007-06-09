// ***************************************************************
//  CCombatant   version:  1.0   ·  date: 06/09/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef CCOMBATAN_H
#define CCOMBATAN_H

#include "tinyxml/tinyxml.h"

#include "Objects/CSprite.h"
#include "Objects/CWeapon.h"

class CCombatant : public CSprite
{
   //////////////////////////////////////////////////////////////////////////
   // Methods
   //////////////////////////////////////////////////////////////////////////
public:
   CCombatant();
   CCombatant(TiXmlNode* t_nodePtr);

   virtual VeObjectType getType() { return e_combatant; };

   /** Fire the active weapon */
   void fireWeapon();

   /** Switch to next weapon */
   void nextWeapon();

   /** Load method */
   virtual bool load(TiXmlNode* t_nodePtr);

   //////////////////////////////////////////////////////////////////////////
   // Variables
   //////////////////////////////////////////////////////////////////////////
public:
protected:
   /** List of weapons */
   std::vector<CWeapon*>   m_weaponList;

   /** Index of active weapon */
   unsigned int            m_activeWeapon;


};

#endif
