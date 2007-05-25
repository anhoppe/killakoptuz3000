// ***************************************************************
//  CPlayer   version:  1.0   ·  date: 04/13/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef CPLAYER_H
#define CPLAYER_H

#include "Objects/CSprite.h"

class CLevel;

class CPlayer : public CSprite
{
public:
   CPlayer();   

   virtual VeObjectType getType() { return e_player; };

   // Update player position (keys and flying 'physics')
   void updatePlayer(CLevel*t_levelPtr);

   // Check if position of player is inside level
   bool positionAllowed(float t_x, float t_y, CLevel*t_levelPtr);

   bool loadPlayer(std::string t_fileName);

   virtual void collisionImpact(CObject* t_objectPtr);

   bool m_leftPressed;
   bool m_rightPressed;
   bool m_upPressed;
   bool m_downPressed;
   bool m_shiftPressed;

   // helicopter acceleration vector
   double m_accel;

   double m_maxAccel;

   double m_velocityX;
   double m_velocityY;

   double m_velocityMax;
};

#endif
