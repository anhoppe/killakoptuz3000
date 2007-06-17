// ***************************************************************
//  CGame   version:  1.0   ·  date: 06/04/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "CLevel.h"

#include "Objects/CPlayer.h"

#include "Menu/CMenu.h"

enum EGameState 
{  
   e_intro     = 0, 
   e_gameOver  = 1,
   e_startMenu = 2, 
   e_level     = 3, 
   e_shop      = 4, 
   e_highscore = 5
};

class CGame
{
public:
   /** Standard constructor */
   CGame();

   /** Standard destructor */
   ~CGame();

   static CGame& getInstance();
   //////////////////////////////////////////////////////////////////////////
   // Methods
   //////////////////////////////////////////////////////////////////////////

   void gameControl();

   void setGameState(EGameState t_gameState);

private:
   //////////////////////////////////////////////////////////////////////////
   // Functions
   //////////////////////////////////////////////////////////////////////////

   /** Load the next level*/
   void loadNextLevel();

   //////////////////////////////////////////////////////////////////////////
   // Members
   //////////////////////////////////////////////////////////////////////////
public:
   CMenu      m_menu;

   /** State of the game */
   EGameState m_gameState;

private:

   /** Current level*/
   CLevel m_level;

   /** Current pointer*/
   CPlayer m_player;
};