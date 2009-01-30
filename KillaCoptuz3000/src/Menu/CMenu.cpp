// ***************************************************************
//  CMenu   version:  1.0   ·  date: 06/17/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "Menu/CMenu.h"

#include "CGame.h"

#include "Functions.h"

#include "Menu/CMenuItem.h"

#include "glut/glut.h"

//////////////////////////////////////////////////////////////////////////
// Definitions
//////////////////////////////////////////////////////////////////////////
#define MENU_HEIGHT  1.2
#define ITEM_DIST    .5

#define GFX_BASE     "data\\menugfx\\"
#define SOUND_BASE   "data\\sound\\"

//////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////
CMenu::CMenu()
{
   m_currentMenuItem = 0;
}

CMenu::~CMenu()
{

}

bool CMenu::load(TiXmlNode* t_nodePtr)
{
   TiXmlNode*     a_nodePtr      = 0;
   TiXmlNode*     a_subNodePtr   = 0;
   TiXmlElement*  a_elemPtr      = 0;
   CMenuItem*     a_menuItemPtr  = 0;

   std::string    a_key          = "";
   std::string    a_str          = "";

   bool           r_ret          = true;
   
   FSOUND_STREAM* a_sound        = 0;


   //////////////////////////////////////////////////////////////////////////
   // clear old menu
   clear();

   //////////////////////////////////////////////////////////////////////////
   // Start to play level music

   a_elemPtr = t_nodePtr->ToElement();
   getAttributeStr(a_elemPtr, "music", a_str);
   a_str = SOUND_BASE + a_str;
   a_sound = FSOUND_Stream_Open(a_str.c_str(), FSOUND_LOOP_NORMAL, 0, 0);
   
   if (a_sound  && !g_mute)
   {
      FSOUND_Stream_Play(0, a_sound);
   }

   //////////////////////////////////////////////////////////////////////////
   // load textures for menu
   a_nodePtr = t_nodePtr->FirstChild("texturelist");

   for(a_subNodePtr = a_nodePtr->FirstChild("texture"); a_subNodePtr; a_subNodePtr = a_nodePtr->IterateChildren("texture", a_subNodePtr))
   {
      a_elemPtr = a_subNodePtr->ToElement();

      if(!getAttributeStr(a_elemPtr, "key", a_key))
      {
         r_ret = false;
      }
 
      if(r_ret)
      {
         if(m_textureMap[a_key] == 0)
         {
            m_textureMap[a_key] = new CTexture(a_elemPtr, GFX_BASE);
         }
      }
   }

   //////////////////////////////////////////////////////////////////////////
   // load menu items
   for(a_nodePtr = t_nodePtr->FirstChild("menuitem"); a_nodePtr; a_nodePtr = t_nodePtr->IterateChildren("menuitem", a_nodePtr))
   {
      a_menuItemPtr = new CMenuItem(this);
      r_ret &= a_menuItemPtr->load(a_nodePtr);
      m_menuItems.push_back(a_menuItemPtr);
   }

   return r_ret;
}

void CMenu::update()
{
   std::vector<CMenuItem*>::iterator a_it;

   for(a_it = m_menuItems.begin(); a_it != m_menuItems.end(); a_it++)
   {
      (*a_it)->update();
   }

}

void CMenu::clear()
{
   CMenuItem* a_itemPtr = 0;

   //////////////////////////////////////////////////////////////////////////
   // delete menu items
   while(m_menuItems.size() > 0)
   {
      a_itemPtr = m_menuItems.back();
      m_menuItems.pop_back();
      delete a_itemPtr;
   }

   //////////////////////////////////////////////////////////////////////////
   // delete textures
   // FIXME
}

void CMenu::next()
{
   if(m_currentMenuItem == 0)
   {
      m_currentMenuItem = (unsigned int) m_menuItems.size()-1;
   }
   else
   {
      m_currentMenuItem--;
   }
}

void CMenu::previous()
{
   m_currentMenuItem = (m_currentMenuItem+1) % m_menuItems.size();
}

void CMenu::performAction()
{
   if(m_menuItems.size()>0)
   {
      switch(m_menuItems[m_currentMenuItem]->m_action)
      {
      case e_startGame:
         CGame::getInstance().m_gameState = e_level;
         CGame::getInstance().gameControl();
         break;
      case e_quitGame:
         exit(-1);
         break;
      case e_succeedingMenu:
         CGame::getInstance().m_gameState = e_menu;
         CGame::getInstance().m_menuName  = m_menuItems[m_currentMenuItem]->m_succeedingMenu;
         CGame::getInstance().gameControl();
         break;
      }
   }
}

void CMenu::draw()
{
   std::vector<CMenuItem*>::iterator   a_it;
   float                               a_yPos   = 0.;
   unsigned int                        a_index  = 0;

   float                               a_depth  = 0.;


   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   //////////////////////////////////////////////////////////////////////////
   // draw menu item
   for(a_it = m_menuItems.begin(); a_it != m_menuItems.end(); a_it++)
   {
      a_depth = 0.;            
      
      a_yPos = (((m_menuItems.size()-1)/2.0 - a_index)/m_menuItems.size()*(MENU_HEIGHT+ITEM_DIST)) - (a_index * ITEM_DIST);
      if(a_index == m_currentMenuItem)
      {
         a_depth = 1.;
      }
      (*a_it)->draw(a_yPos, a_depth);
      a_index++;
   }


   // swapping the buffers causes the rendering above to be  
   // shown
   glutSwapBuffers();

}
