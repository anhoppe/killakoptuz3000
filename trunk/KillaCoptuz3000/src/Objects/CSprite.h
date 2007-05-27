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
   CSprite(float t_xPos, float t_yPos);
   ~CSprite();

   virtual VeObjectType getType() { return e_sprite; };

  
   /** Draws the object via OpenGL*/
   virtual void draw();

   /** Switches direction*/
   void flip();

   /** Get tracking angle to tracked object*/
   float trackAngle(float t_trgX, float t_trgY);
   
   /** direction*/
   bool                    m_direction;
   /** Objects to track*/
   std::vector<CObject*>   m_trackList;
   /** Tracked object index*/
   unsigned int            m_trackIndex;
   /** Script of sprite*/
   CScript*                m_scriptPtr;      
   /** Behavior structure*/
   SBehaviorData           m_behaviorData;
   
   /** Start angle (only valid for child objects */
   float                   m_startAngle;

};

#endif
