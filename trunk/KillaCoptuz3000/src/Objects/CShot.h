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
   // Copy constructor creates shot from default shot
   CShot(CShot* t_shotPtr);
   CShot(TiXmlNode* t_nodePtr);
   ~CShot();

   // Velocity (per frame)
   float m_v;

   virtual VeObjectType getType() { return e_shot; };
   
   virtual bool load(TiXmlNode* t_nodePtr);

   virtual void update(CLevel* t_levelPtr);
};

#endif
