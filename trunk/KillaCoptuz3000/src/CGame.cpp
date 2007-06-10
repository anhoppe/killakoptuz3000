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


   if(a_doc.LoadFile("Data/player/player.xml"))
   {
      // load player textures 
      a_nodePtr = a_doc.FirstChild("texturelist");
      CObjectStorage::getInstance().addTextureMap(a_nodePtr);

      // load player
      a_nodePtr = a_doc.FirstChild("object");
      CObjectStorage::getInstance().add(a_nodePtr, e_player);
   }

   m_gameState = e_level;
}

CGame::~CGame()
{   
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
   TiXmlDocument   a_doc;
   TiXmlNode*      a_nodePtr  = 0;


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
      }
   }
}
