   // ***************************************************************
//  CSprite   version:  1.0   �  date: 04/07/2007
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

   /** Switches direction*/
   void flip();

   /** Get tracking angle to tracked object*/
   float trackAngle(float t_trgX, float t_trgY);
   
   /** Objects to track*/
   std::vector<CObject*>   m_trackList;
   
   /** Tracked object index*/
   unsigned int            m_trackIndex;

   /** Script of sprite*/
   CScript*                m_scriptPtr;      
   
   /** Behavior structure*/
   SBehaviorData           m_behaviorData;

};

#endif
