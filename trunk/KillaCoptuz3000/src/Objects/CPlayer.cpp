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

#include "Objects/CPlayer.h"
#include "CLevel.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>

#include "Functions.h"
#include "globals.h"
#include "Objects/CWeapon.h"

#define ACCEL_DELTA     .00004
#define GRAVITY_VALUE   .0006
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
   m_maxAccel     = 0.0015;
   m_accel        = 0.;
   m_velocityX    = 0.;
   m_velocityY    = 0.;

   m_velocityMax  = .007;
   m_isBackground = false;

}

void CPlayer::update(CLevel* t_levelPtr)
{  
   float a_oldAngle = m_angle;

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
      m_cycleInterval = (int)fabs(m_maxAccel - m_accel)/m_maxAccel;   
   }

   //////////////////////////////////////////////////////////////////////////
   // Update velocity
   //////////////////////////////////////////////////////////////////////////
   m_velocityY = m_velocityY*FRICTION_COEFF + m_accel*cos(M_PI*m_angle/180) - GRAVITY_VALUE;   

   if (m_direction)
      m_velocityX = m_velocityX*FRICTION_COEFF - m_accel*sin(-M_PI*m_angle/180.0);
   else 
      m_velocityX = m_velocityX*FRICTION_COEFF - m_accel*sin(M_PI*m_angle/180.0);

   // Update position if (1) no collision and (2) the position is allowed for the level
   // Movement in y
   m_dx = 0.;
   m_dy = 0.;

   if (positionAllowed(m_xPos, m_yPos + m_velocityY, t_levelPtr))
   {
      m_dy = m_velocityY;
      m_yPos += m_velocityY;         
   }
   else      
      m_velocityY = 0.0;      

   // Movement in x
   if (positionAllowed(m_xPos + m_velocityX, m_yPos, t_levelPtr))
   {
      m_dx = m_velocityX;
      m_xPos += m_velocityX;         
   }
   else               
   {
      m_velocityX = 0.0;
   }
}

// Check if position of player is allowed by level description
bool CPlayer::positionAllowed(float t_x, float t_y, CLevel* t_levelPtr)
{   
   if (t_x > t_levelPtr->m_maxX)
      return false;
   if (t_x < t_levelPtr->m_minX)
      return false;
   if (t_y > t_levelPtr->m_maxY)
      return false;
   if (t_y < t_levelPtr->m_minY)   
      return false;   

   return true;
}


void CPlayer::collisionImpact(CObject* t_objectPtr)
{

}

