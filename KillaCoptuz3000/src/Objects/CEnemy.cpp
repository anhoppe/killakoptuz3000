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
CEnemy::CEnemy(float t_xPos, float t_yPos)
: CSprite(t_xPos, t_yPos)
{
   m_activeTexture   = 0;
   m_timeCounter     = 0;
   m_scriptPtr       = 0;
}

CEnemy::CEnemy(TiXmlNode* t_nodePtr)
{
   m_scriptPtr       = 0;
   load(t_nodePtr);

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
   for(a_nodePtr = t_nodePtr->FirstChild(); a_nodePtr; a_nodePtr = t_nodePtr->IterateChildren(a_nodePtr))
   {
      a_elemPtr = a_nodePtr->ToElement();

      a_str = a_elemPtr->Value();            
      if(a_str == "weapon")
      {
         // Create a new weapon
         a_weaponPtr = new CWeapon(a_nodePtr);
         
         // Make initial weapon position relative to host
         a_weaponPtr->m_xPos += m_xPos;
         a_weaponPtr->m_yPos += m_yPos;

         a_weaponPtr->m_trackList.push_back(static_cast<CObject*>(g_playerPtr));

         a_weaponPtr->m_parentPtr = this;

         // put into object list
         CLevel::M_objects.push_back(a_weaponPtr);

         // remember child pointer
         m_children.push_back(a_weaponPtr);
      }
      else if(a_str == "script")
      {
         // Create a new script                  
         if (getAttributeStr(a_elemPtr, "filename", a_str))
         {            
            m_scriptPtr = new CScript(a_str.c_str());  
         }
      }
   }

   return true;
}



void CEnemy::update(CLevel* t_levelPtr, std::vector<CObject*>::iterator& t_it, std::vector<CObject*>::iterator& t_endIt)
{
   if(m_scriptPtr)
   {
      m_scriptPtr->update(m_behaviorData);      
   }

   CSprite::update(t_levelPtr, t_it, t_endIt);
}
