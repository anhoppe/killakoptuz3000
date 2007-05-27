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
   CWeapon(float t_xPos, float t_yPos);
   CWeapon(TiXmlNode* t_nodePtr);
   ~CWeapon();

   virtual VeObjectType getType() { return e_weapon; };

   virtual bool      load(TiXmlNode* t_nodePtr);

   virtual void      update(CLevel* t_levelPtr, std::vector<CObject*>::iterator& t_it, std::vector<CObject*>::iterator& t_endIt);

   /** Fire one standard shot */
   void              fire();

   // Controls m_angle for being in the allowed interval
   void              controlAngle();

   float             m_minAngle;
   float             m_maxAngle;
   float             m_startAngle;

   unsigned int      m_maxShots;
   unsigned int      m_framesPerShot;
   unsigned int      m_framesSinceShot;
   float             m_shotRadius;

   CShot*            m_shotPtr;
};

#endif

