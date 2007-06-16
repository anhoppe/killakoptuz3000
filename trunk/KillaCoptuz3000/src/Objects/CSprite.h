   // ***************************************************************
//  CSprite   version:  1.0   ·  date: 04/07/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef CSPRITE_H
#define CSPRITE_H

#include <tchar.h>
#include <vector>
#include "behaviour/CScript.h"

#include "objects/CObject.h"

class CSprite : public CObject
{
public:
   CSprite();   
   ~CSprite();

   virtual VeObjectType getType() { return e_sprite; };
   virtual void update(CLevel* m_levelPtr);

   /** Switches direction*/
   void flip();

   /** Script of sprite*/
   CScript*                m_scriptPtr;      
   
   /** Behavior structure*/
   SBehaviorData           m_behaviorData;

};

#endif
