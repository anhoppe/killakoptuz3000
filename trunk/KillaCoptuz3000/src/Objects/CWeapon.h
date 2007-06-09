// ***************************************************************
//  CWeapon   version:  1.0   ·  date: 05/05/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef CWEAPON_H
#define CWEAPON_H

#include "Objects/CSprite.h"
#include "tinyxml.h"

#include <list>

class CShot;
class CLevel;

class CWeapon : public CSprite
{
public:
   CWeapon(TiXmlNode* t_nodePtr);
   ~CWeapon();

   virtual VeObjectType getType() { return e_weapon; };

   virtual bool      load(TiXmlNode* t_nodePtr);

   virtual void      update(CLevel* t_levelPtr);

   /** Fire one standard shot */
   void              fire();

   // Controls m_angle for being in the allowed interval
   void              controlAngle();

   float             m_minAngle;
   float             m_maxAngle;

   unsigned int      m_maxShots;
   unsigned int      m_framesPerShot;
   unsigned int      m_framesSinceShot;
   float             m_shotRadius;

   CShot*            m_shotPtr;
};

#endif


