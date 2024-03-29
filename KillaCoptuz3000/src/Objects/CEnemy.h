// ***************************************************************
//  CEnemy   version:  1.0   �  date: 05/05/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef CENEMY_H
#define CENEMY_H

#include <list>
#include "KillaCoptuz3000/src/objects/CCombatant.h"
#include "tinyxml/tinyxml.h"

class CWeapon;
class CLevel;

class CEnemy : public CCombatant
{
public:
   CEnemy();
   ~CEnemy();

   virtual VeObjectType getType() { return e_enemy; };

   virtual bool load(TiXmlNode* t_nodePtr);
   
   virtual void update(CLevel* t_levelPtr);
};

#endif
