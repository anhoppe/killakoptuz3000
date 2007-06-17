// ***************************************************************
//  CGame   version:  1.0   ·  date: 06/04/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "CGame.h"

#include "CObjectStorage.h"
#include "glCallbacks.h"


//////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////
CGame::CGame()
{
   // Load player
   TiXmlDocument  a_doc;
   TiXmlNode*     a_nodePtr   = 0;

   // load player
   if(a_doc.LoadFile("Data/player/player.xml"))
   {
      // load player textures 
      a_nodePtr = a_doc.FirstChild("texturelist");
      CObjectStorage::getInstance().addTextureMap(a_nodePtr);

      // load player
      a_nodePtr = a_doc.FirstChild("object");
      CObjectStorage::getInstance().add(a_nodePtr, e_player);
   }

   m_gameState = e_menu;
   m_menuName  = "main";
}

CGame::~CGame()
{
}

CGame& CGame::getInstance()
{
   static CGame* r_gamePtr = 0;

   if(0 == r_gamePtr)
   {
      r_gamePtr = new CGame();
   }

   return *r_gamePtr;
}

// Game control
void CGame::gameControl()
{
   switch (m_gameState)
   {
   case e_level:
      {
         loadNextLevel();

         // Register callback functions
         setCallbackLevelPtr(&m_level);

         glutKeyboardFunc(CLevel_processNormalKeys);
         glutSpecialFunc(CLevel_pressKey);
         glutSpecialUpFunc(CLevel_releaseKey);
         glutDisplayFunc(CLevel_renderScene);
         glutIdleFunc(CLevel_renderScene);

         glutTimerFunc (25, CLevel_timerCallback, 1);
         break;
      }
   case e_menu:
      {
         // load start menu
         TiXmlDocument  a_doc;
         std::string    a_menuFileName = "";

         a_menuFileName = "Data/menu/";
         a_menuFileName += m_menuName;
         a_menuFileName += ".xml";

         if(a_doc.LoadFile(a_menuFileName.c_str()))
         {
            m_menu.load(a_doc.FirstChild("menu"));

            glutKeyboardFunc(CMenu_processNormalKeys);
            glutSpecialFunc(CMenu_pressKey);
            glutSpecialUpFunc(CMenu_releaseKey);
            glutDisplayFunc(CMenu_renderScene);
            glutIdleFunc(CMenu_renderScene);

            glutTimerFunc (25, CMenu_timerCallback, 1);
         }

         
      }

   default:
      {
         break;
      }
   }
}

//
void CGame::setGameState(EGameState t_gameState)
{
   m_gameState = t_gameState;
}

// 
void CGame::loadNextLevel()
{
   TiXmlDocument  a_doc;
   TiXmlNode*     a_nodePtr = 0;
   
   if(a_doc.LoadFile("data\\levels\\level3.xml"))
   {
      //////////////////////////////////////////////////////////////////////////
      // load levels 
      a_nodePtr = a_doc.FirstChild("texturelist");
      assert(a_nodePtr);
      if(0 != a_nodePtr)
      {
         CObjectStorage::getInstance().addTextureMap(a_nodePtr);
      }
      //////////////////////////////////////////////////////////////////////////
      // load level content
      a_nodePtr = a_doc.FirstChild("level");

      if(0 != a_nodePtr)
      {
         // To do: real level control
         m_level.load(a_nodePtr);

         // Initialize Quad - tree
         CObjectStorage::getInstance().initializeQuadTree(m_level.m_maxY, m_level.m_minX, m_level.m_maxX, m_level.m_minY);
      }      
   }
}
