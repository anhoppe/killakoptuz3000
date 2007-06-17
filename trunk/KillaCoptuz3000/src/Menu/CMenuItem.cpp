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

#include "glut/glut.h"

//////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////
CMenuItem::CMenuItem()
{
   m_text            = "";
   m_succeedingMenu  = "";
   m_action          = e_quitGame;
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

      r_ret = getAttributeStr(a_elemPtr, "text", m_text);
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

void CMenuItem::draw(float t_yPos)
{
   int   a_index  = 0;
   float a_xPos   = -1.;


   glRasterPos3f(a_xPos, t_yPos, 1.00);

   for(a_index = 0; a_index < m_text.size(); a_index++)
   {
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, m_text[a_index]);
   }
}
