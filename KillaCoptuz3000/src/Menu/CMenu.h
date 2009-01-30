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

#include "CTexture.h"

#include <map>
#include "tinyxml/tinyxml.h"
#include "fmod375/fmod.h"

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
   void update();

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

public:
   /** map of all textures for a menu */
   std::map<std::string, CTexture*> m_textureMap;

private:
   /** Currently selected menu item */
   unsigned int                     m_currentMenuItem;

   /** vector with all menu items */
   std::vector<CMenuItem*>          m_menuItems;
};

#endif
