// ***************************************************************
//  CGame   version:  1.0   ·  date: 06/04/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

CGame::CGame()
{     
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
         glutTimerFunc (25, m_level.timerCallback, 0);
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
   // To do: real level control

   m_levelPtr->load("level3.xml");
}