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

#include "KillaCoptuz3000/src/Objects/CSprite.h"
#include "tinyxml/tinyxml.h"

class CLevel;

enum EShotType
{
   e_shotNormal    = 0,
   e_shotBallistic = 1
};

class CShot : public CSprite
{
public:
   // Copy constructor creates shot from default shot
   CShot(CShot* t_shotPtr, std::list<unsigned int>* t_friendObjectsListPtr);
   CShot(TiXmlNode* t_nodePtr);
   ~CShot();

   virtual VeObjectType getType() { return e_shot; };
   
   virtual bool load(TiXmlNode* t_nodePtr);

   virtual void update(CLevel* t_levelPtr);

   bool         isFriend(unsigned int t_objectPtr);

   EShotType               m_shotType;

   float                   m_velocityX;
   float                   m_velocityY;

private:
   std::list<unsigned int> m_friendObjects;   
};

#endif
