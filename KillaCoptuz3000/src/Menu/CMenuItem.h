// ***************************************************************
//  CMenuItem   version:  1.0   ·  date: 06/17/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef CMENU_ITEM_H
#define CMENU_ITEM_H

#include "tinyxml/tinyxml.h"

#include <string>

enum EMenuAction
{
   e_startGame       = 0,
   e_quitGame        = 1,
   e_succeedingMenu  = 2
};

class CMenuItem
{
   //////////////////////////////////////////////////////////////////////////
   // Methods
   //////////////////////////////////////////////////////////////////////////
public:
   CMenuItem();
   ~CMenuItem();

   bool load(TiXmlNode* t_nodePtr);

   void draw(float t_position);

   //////////////////////////////////////////////////////////////////////////
   // Variables
   //////////////////////////////////////////////////////////////////////////
public:

   /** This text is displayed for the menu */
   std::string m_text;

   /** This is the succeeding menu when this is selected. Only set if menu action is 'menu' */
   std::string m_succeedingMenu;

   /** Type of action (start game / end program / menu) */
   EMenuAction m_action;
};

#endif