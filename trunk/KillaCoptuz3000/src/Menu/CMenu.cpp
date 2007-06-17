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

#include "Menu/CMenuItem.h"

#include "glut/glut.h"

//////////////////////////////////////////////////////////////////////////
// Definitions
//////////////////////////////////////////////////////////////////////////
#define MENU_HEIGHT  1.2


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
   TiXmlElement*  a_elemPtr      = 0;
   CMenuItem*     a_menuItemPtr  = 0;

   bool           r_ret          = true;

   clear();

   for(a_nodePtr = t_nodePtr->FirstChild("menuitem"); a_nodePtr; a_nodePtr = t_nodePtr->IterateChildren("menuitem", a_nodePtr))
   {
      a_menuItemPtr = new CMenuItem();
      r_ret &= a_menuItemPtr->load(a_nodePtr);
      m_menuItems.push_back(a_menuItemPtr);
   }

   return r_ret;
}

void CMenu::clear()
{
   CMenuItem* a_itemPtr = 0;

   while(m_menuItems.size() > 0)
   {
      a_itemPtr = m_menuItems.back();
      m_menuItems.pop_back();
      delete a_itemPtr;
   }
}

void CMenu::next()
{
   if(m_currentMenuItem == 0)
   {
      m_currentMenuItem = m_menuItems.size()-1;
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

      }
   }
}

void CMenu::draw()
{
   std::vector<CMenuItem*>::iterator   a_it;
   float                               a_yPos   = 0.;
   unsigned int                        a_index  = 0;

   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   //////////////////////////////////////////////////////////////////////////
   // draw menu item
   for(a_it = m_menuItems.begin(); a_it != m_menuItems.end(); a_it++)
   {
      a_yPos = static_cast<float>( static_cast<float>(m_menuItems.size())/2. - static_cast<float>(a_index)) / (static_cast<float>(m_menuItems.size())) * MENU_HEIGHT;
      (*a_it)->draw(a_yPos);

      if(a_index == m_currentMenuItem)
      {
         glBegin(GL_LINE_LOOP);
         
         float xLeft = -2;
         float xMid  = -1.8;
         float xRight = -1.6;
         float yTop   = a_yPos+MENU_HEIGHT/2.;
         float yMid     = a_yPos-.2+MENU_HEIGHT/2.;
         float yBottom  = a_yPos-.3+MENU_HEIGHT/2.;

         glVertex2f(xLeft, yBottom);
         glVertex2f(xLeft, yMid);
         glVertex2f(xMid, yTop);
         glVertex2f(xRight, yMid);
         glVertex2f(xRight, yBottom);
         glVertex2f(xLeft, yMid);
         glVertex2f(xRight, yMid);
         glVertex2f(xLeft, yBottom);
         glVertex2f(xRight, yBottom);

         glEnd();
      }

      a_index++;
   }


   // swapping the buffers causes the rendering above to be  
   // shown
   glutSwapBuffers();

}
