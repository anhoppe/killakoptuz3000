// ***************************************************************
//  CShot   version:  1.0   ·  date: 05/05/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef CSHOT_H
#define CSHOT_H

#include "Objects/CSprite.h"
#include "tinyxml.h"

class CLevel;

class CShot : public CSprite
{
public:
   CShot(float t_xPos, float t_yPos);
   // Copy constructor creates shot from default shot
   CShot(CShot* t_shotPtr);
   CShot(TiXmlNode* t_nodePtr);
   ~CShot();

   // Velocity (per frame)
   float m_v;

   virtual bool load(TiXmlNode* t_nodePtr);

   virtual void update(CLevel* t_levelPtr, std::vector<CObject*>::iterator& t_it, std::vector<CObject*>::iterator& t_endIt);

   virtual void collisionImpact(CObject* t_objectPtr, bool t_checkOther /* = true */);

   // Check if position is allowed by level description
   bool positionAllowed(float t_x, float t_y, CLevel* t_levelPtr);
};

#endif
