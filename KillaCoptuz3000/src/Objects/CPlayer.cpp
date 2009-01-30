/********************************************************************
	created:	2007/04/13
	created:	13:4:2007   22:03
	filename: 	h:\develop\KillaCoptuz3000\KillaCoptuz3000\CPlayer.cpp
	file path:	h:\develop\KillaCoptuz3000\KillaCoptuz3000
	file base:	CPlayer
	file ext:	cpp
	author:		Dr Visdent
	
	purpose:	
*********************************************************************/

#include "KillaCoptuz3000/src/Objects/CPlayer.h"
#include "KillaCoptuz3000/src/CLevel.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>

#include "KillaCoptuz3000/src/Functions.h"
#include "KillaCoptuz3000/src/globals.h"
#include "KillaCoptuz3000/src/Objects/CWeapon.h"

#define ACCEL_DELTA     .00009
#define ANGLE_DELTA     2
#define MINMAX_ANGLE    45
#define FRICTION_COEFF  0.98

CPlayer::CPlayer()
{
   m_leftPressed  = false;
   m_rightPressed = false;
   m_upPressed    = false;
   m_downPressed  = false;
   m_shiftPressed = false;   

   m_angle        = 0;
   m_direction    = 0;
   m_maxAccel     = 0.0030;
   m_accel        = 0.;
   m_velocityX    = 0.;
   m_velocityY    = 0.;

   m_velocityMax  = .018;
   m_isBackground = false;

}

void CPlayer::update(CLevel* t_levelPtr)
{  
   float a_oldAngle = m_angle;

   // Prevent keyboard control, when exploding
   if (m_isDying)
   {
      m_upPressed       = false;
      m_downPressed     = false;
      m_leftPressed     = false;
      m_rightPressed    = false;

      // Velocity remains, but strong friction
      m_velocityX       = m_velocityX*0.83;
      m_velocityY       = m_velocityY*0.83;
   }

   if (m_upPressed)
   {
      // Speed up rotor
      if (m_accel < m_maxAccel - ACCEL_DELTA)
      {
         m_accel += ACCEL_DELTA;         
      }
   }

   if (m_downPressed)
   {
      // Speed up rotor
      if (m_accel > ACCEL_DELTA)
      {
         m_accel -= ACCEL_DELTA;         
      }
   }

   //////////////////////////////////////////////////////////////////////////
   // Rotation of the helicopter
   //////////////////////////////////////////////////////////////////////////
   if (m_leftPressed)
   {
      if (m_direction)               
         m_angle -= ANGLE_DELTA;      
      else      
         m_angle += ANGLE_DELTA;      
   }

   if (m_rightPressed)
   {
      if (m_direction)
         m_angle += ANGLE_DELTA;
      else
         m_angle -= ANGLE_DELTA;
   }

   // Keep angle in the interval [0..360]
   if (m_angle > 360.0)
      m_angle -= 360.0;

   if (m_angle < 0.0)
      m_angle += 360.0;

   // Angle has to be in the interval 360-45... 0 ... 45
   if (!(m_angle >= 360 - MINMAX_ANGLE || m_angle <= MINMAX_ANGLE))
   {
      if (m_angle < 180.0)
         m_angle = MINMAX_ANGLE;
      else
         m_angle = 360 - MINMAX_ANGLE;
   }

   //////////////////////////////////////////////////////////////////////////
   // Adjust rotor cycling to acceleration
   //////////////////////////////////////////////////////////////////////////
   if(!m_isDying)
   {
      m_cycleInterval = 1 + 8*(1.0 - m_accel/(m_accel + m_maxAccel/20.0));            
   }

   //////////////////////////////////////////////////////////////////////////
   // Update velocity
   //////////////////////////////////////////////////////////////////////////
   m_velocityY = m_velocityY*FRICTION_COEFF + m_accel*cos(M_PI*m_angle/180) - g_gravity;   

   if (m_direction)
      m_velocityX = m_velocityX*FRICTION_COEFF - m_accel*sin(-M_PI*m_angle/180.0);
   else 
      m_velocityX = m_velocityX*FRICTION_COEFF - m_accel*sin(M_PI*m_angle/180.0);

   // Update position if (1) no collision and (2) the position is allowed for the level
   // Movement in y
   m_dx = 0.;
   m_dy = 0.;

   if (t_levelPtr->positionAllowed(m_xPos + m_width/2.0, m_yPos + m_height/2.0 + m_velocityY))
   {
      m_dy = m_velocityY;
      m_yPos += m_velocityY;         
   }
   else      
   {
      m_velocityY = 0.0;
   }

   // Movement in x
   if (t_levelPtr->positionAllowed(m_xPos + m_width/2.0 + m_velocityX, m_yPos + m_height/2.0))
   {
      m_dx = m_velocityX;
      m_xPos += m_velocityX;         
   }
   else               
   {
      m_velocityX = 0.0;
   }

   CSprite::update(t_levelPtr);
}


void CPlayer::collisionImpact(CObject* t_objectPtr)
{

}

