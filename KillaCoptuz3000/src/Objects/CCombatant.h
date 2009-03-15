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

#include "KillaCoptuz3000/src/Objects/CSprite.h"
#include "KillaCoptuz3000/src/Objects/CWeapon.h"

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
   /** Objects to track*/
   std::vector<unsigned int>   m_trackList;

   /** Tracked object index*/
   unsigned int                m_trackIndex;


#if(PRODUCT == KK3000)
protected:
   /** List of ID's of weapons */
   std::vector<unsigned int>   m_weaponList;

#else

public:
   /** in LE3000 the enemy holds the weapon objects */
   std::vector<CWeapon*>       m_weaponList;
#endif


   /** Index of active weapon */
   unsigned int                m_activeWeapon;

};

#endif
