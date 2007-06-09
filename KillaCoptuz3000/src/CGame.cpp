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
         glutTimerFunc (25, (void (*)(int))(m_level.timerCallback), 0);
         glutKeyboardFunc(m_level.processNormalKeys);
         glutSpecialFunc(m_level.pressKey);
         glutSpecialUpFunc(m_level.releaseKey);
         glutDisplayFunc(m_level.renderScene);
         glutIdleFunc(m_level.renderScene);
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


   if(a_doc.LoadFile("level3.xml"))
   {
      //////////////////////////////////////////////////////////////////////////
      // load levels 
      a_nodePtr = a_doc.FirstChild("texturlist");
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
