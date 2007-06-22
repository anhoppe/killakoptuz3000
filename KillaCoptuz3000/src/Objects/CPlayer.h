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

#include <vector>

#include "KillaCoptuz3000/src/Objects/CCombatant.h"


class CPlayer : public CCombatant
{
public:
   CPlayer();   

   virtual VeObjectType getType() { return e_player; };

   /** Update player position (keys and flying 'physics') */
   virtual void update(CLevel*t_levelPtr);

   virtual void collisionImpact(CObject* t_objectPtr);

   bool m_leftPressed;
   bool m_rightPressed;
   bool m_upPressed;
   bool m_downPressed;
   bool m_shiftPressed;

   /** helicopter acceleration vector */
   double m_accel;

   double m_maxAccel;

   double m_velocityX;
   double m_velocityY;

   double m_velocityMax;

};

#endif
