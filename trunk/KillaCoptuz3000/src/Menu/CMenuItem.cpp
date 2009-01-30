// ***************************************************************
//  CMenuItem   version:  1.0   ·  date: 06/17/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "Menu/CMenuItem.h"

#include "Functions.h"

#include "CMenu.h"


#include "glut/glut.h"

//////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////
CMenuItem::CMenuItem(CMenu* t_parentPtr)
: m_parentPtr(t_parentPtr)
{
   m_textureKey      = "";
   m_succeedingMenu  = "";
   m_action          = e_quitGame;

   m_width           = 2.5;
   m_height          = 0.5;

   m_textureIndex    = 0;
}

CMenuItem::~CMenuItem()
{

}

bool CMenuItem::load(TiXmlNode* t_nodePtr)
{
   TiXmlElement*  a_elemPtr   = 0;
   std::string    a_str       = "";

   bool           r_ret       = false;


   assert(t_nodePtr);

   if(0 != t_nodePtr)
   {
      a_elemPtr = t_nodePtr->ToElement();

      //////////////////////////////////////////////////////////////////////////
      // get texture ID
      r_ret = getAttributeStr(a_elemPtr, "texturekey", m_textureKey);      

      //////////////////////////////////////////////////////////////////////////
      // get and save action
      r_ret &= getAttributeStr(a_elemPtr, "action", a_str);

      if(a_str == "startgame")
      {
         m_action = e_startGame;
      }
      else if(a_str == "menu")
      {
         m_action = e_succeedingMenu;

         // get succeeding menu
         r_ret &= getAttributeStr(a_elemPtr, "menu", m_succeedingMenu);
      }
      else if(a_str == "exit")
      {
         m_action  = e_quitGame;
      }      
   }

   return r_ret;
}

void CMenuItem::update()
{
   m_textureIndex++;

   if(m_textureIndex >= m_parentPtr->m_textureMap[m_textureKey]->m_textureIdVector.size())
   {
      m_textureIndex = 0;
   }
}

void CMenuItem::draw(float t_yPos, float t_depth)
{
   int   a_index  = 0;
   float a_xPos   = -1.;

   glPushMatrix();

   glTranslatef(0., 0., t_depth);

   glEnable( GL_TEXTURE_2D );
   glBindTexture(GL_TEXTURE_2D, m_parentPtr->m_textureMap[m_textureKey]->m_textureIdVector[m_textureIndex]);

   glBegin( GL_QUADS );
   glTexCoord2d(0.0,0.0); 
   glVertex2d(a_xPos, t_yPos);

   glTexCoord2d(1.0,0.0);
   glVertex2d(m_width+a_xPos, t_yPos);

   glTexCoord2d(1.0,1.0); 
   glVertex2d(m_width+a_xPos, m_height+t_yPos);

   glTexCoord2d(0.0,1.0); 
   glVertex2d(a_xPos,m_height+t_yPos);

   glEnd();

   glPopMatrix();

   glDisable( GL_TEXTURE_2D );

}
