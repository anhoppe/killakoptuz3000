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
: CSprite(0., 0.)
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

   m_activeWeapon = 0;
}

void CPlayer::updatePlayer(CLevel* t_levelPtr)
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

bool CPlayer::loadPlayer(std::string t_fileName)
{
   TiXmlNode*     a_nodePtr      = 0;
   TiXmlElement*  a_elemPtr      = 0;
   TiXmlNode*     a_subNodePtr   = 0;
   TiXmlElement*  a_subElemPtr   = 0;
   TiXmlDocument  a_doc;
   std::string    a_str          = "";
   std::string    a_dummy        = "";
   
   bool           r_ret          = true;
   

   if (!a_doc.LoadFile(t_fileName.c_str()))
   {
      r_ret = false;
   }
   else
   {
      //////////////////////////////////////////////////////////////////////////
      // read player object info
      a_nodePtr = a_doc.FirstChild("object");
      a_elemPtr = a_nodePtr->ToElement();

      r_ret = r_ret & getAttributeStr(a_elemPtr, "xpos", a_str);
      m_xPos         = atof(a_str.c_str());

      r_ret = r_ret & getAttributeStr(a_elemPtr, "ypos", a_str);
      m_yPos         = atof(a_str.c_str());

      r_ret = r_ret & getAttributeStr(a_elemPtr, "width", a_str);
      m_width        = atof(a_str.c_str());

      r_ret = r_ret & getAttributeStr(a_elemPtr, "height", a_str);
      m_height       = atof(a_str.c_str());

      r_ret = r_ret & getAttributeStr(a_elemPtr, "hitpoints", a_str);
      m_hitPoints = atoi(a_str.c_str());      

      //////////////////////////////////////////////////////////////////////////
      // load texture list
      a_subNodePtr = a_nodePtr->FirstChild("texturelist");

      TiXmlNode*  a_subSubNodePtr = 0;
      for(a_subSubNodePtr = a_subNodePtr->FirstChild("texture"); a_subSubNodePtr; a_subSubNodePtr = a_subNodePtr->IterateChildren("texture", a_subSubNodePtr))
      {
         a_subElemPtr = a_subSubNodePtr->ToElement();

         if(!strcmp("texture", a_subElemPtr->Value()))
         {
            r_ret &= getAttributeStr(a_subElemPtr, "key", a_str);

            // check if explosion sequence exists
            if(getAttributeStr(a_subElemPtr, "explosion", a_dummy))
            {
               m_explosionIndex = m_textureKeys.size();
            }

            if(r_ret)
            {
               m_textureKeys.push_back(a_str);
            }
         }
      }

      //////////////////////////////////////////////////////////////////////////
      // load player weapons
      CWeapon* a_weaponPtr = 0;

      a_subNodePtr = a_nodePtr->FirstChild("weapon");
      
      for(a_subNodePtr = a_nodePtr->FirstChild("weapon"); a_subNodePtr; a_subNodePtr = a_nodePtr->IterateChildren("weapon", a_subNodePtr))
      {
         a_elemPtr = a_subNodePtr->ToElement();

         // Create a new weapon
         a_weaponPtr = new CWeapon(a_subNodePtr);

         // Make initial weapon position relative to host
         a_weaponPtr->m_xPos += m_xPos;
         a_weaponPtr->m_yPos += m_yPos;         

         a_weaponPtr->m_parentPtr = this;

         // Add the new weapon to the weapon list
         m_weaponList.push_back(a_weaponPtr);

         CLevel::M_objects.push_back(a_weaponPtr);
      }


      //////////////////////////////////////////////////////////////////////////
      // read player texture list
      a_nodePtr = a_doc.FirstChild("texturelist");
      g_level.loadTextureMap(a_nodePtr);
   }

   return r_ret;
}

void CPlayer::collisionImpact(CObject* t_objectPtr)
{

}

void CPlayer::fireWeapon()
{   
   if (m_activeWeapon < m_weaponList.size())
   {
       m_weaponList[m_activeWeapon]->fire();
   }
}

void CPlayer::nextWeapon()
{
   m_activeWeapon++;

   m_activeWeapon = m_activeWeapon % m_weaponList.size();
}