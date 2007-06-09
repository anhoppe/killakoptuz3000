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

enum EGameState {e_intro, e_startMenue, e_level, e_shop, e_highscore};

class CGame
{
public:
   /** Standard constructor */
   CGame();

   /** Standard destructor */
   ~CGame();

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

   /** State of the game */
   EGameState m_gameState;

   /** Current level*/
   CLevel m_level;

   /** Current pointer*/
   CPlayer m_player;
};