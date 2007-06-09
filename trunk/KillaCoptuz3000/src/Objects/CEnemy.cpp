// ***************************************************************
//  CEnemy   version:  1.0   ·  date: 05/05/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "CLevel.h"
#include "Objects/CEnemy.h"
#include "Objects/CWeapon.h"
#include "Functions.h"
#include "globals.h"
#include "main.h"

//////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////

CEnemy::CEnemy(TiXmlNode* t_nodePtr)
{
   m_scriptPtr       = 0;

   m_behaviorData.m_xPosPtr         = &m_xPos;
   m_behaviorData.m_yPosPtr         = &m_yPos;
   m_behaviorData.m_x0              = m_xPos;
   m_behaviorData.m_y0              = m_yPos;
   m_behaviorData.m_dxPtr           = &m_dx;
   m_behaviorData.m_dyPtr           = &m_dy;   
}

CEnemy::~CEnemy()
{
   if (!m_scriptPtr)
      delete m_scriptPtr;

}

bool CEnemy::load(TiXmlNode* t_nodePtr)
{
   TiXmlNode*     a_nodePtr   = 0;
   TiXmlElement*  a_elemPtr   = 0;
   CWeapon*       a_weaponPtr = 0;
   
   std::string    a_str;

   m_scriptPtr    = 0;

   CObject::load(t_nodePtr);

   // Loop over all weapons
   a_nodePtr = t_nodePtr->FirstChild("script");
   if(0 != a_nodePtr)
   {
      // Create a new script                  
      if (getAttributeStr(a_elemPtr, "filename", a_str))
      {            
         m_scriptPtr = new CScript(a_str.c_str());  
      }
   }

   return true;
}



void CEnemy::update(CLevel* t_levelPtr)
{
   if(m_scriptPtr)
   {
      m_scriptPtr->update(m_behaviorData);      
   }

   CObject::update(t_levelPtr);
}
