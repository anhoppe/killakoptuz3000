// ***************************************************************
//  CMenu   version:  1.0   ·  date: 06/17/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef CMENU_H
#define CMENU_H

#include "CHashMap.h"

#include <map>

#include "tinyxml/tinyxml.h"

class CMenuItem;

class CMenu
{
   //////////////////////////////////////////////////////////////////////////
   // Methods
   //////////////////////////////////////////////////////////////////////////
public:
   CMenu();
   ~CMenu();

   bool load(TiXmlNode* t_nodePtr);
   void draw();

   /** Switches to the next (lower) menu item */
   void next();

   /** Switches to the previous (upper) menu item */
   void previous();

   /** Performs action of current menu item */
   void performAction();

private:
   void clear();

   //////////////////////////////////////////////////////////////////////////
   // Variables
   //////////////////////////////////////////////////////////////////////////
private:
   unsigned int            m_currentMenuItem;
   std::vector<CMenuItem*> m_menuItems;
};

#endif