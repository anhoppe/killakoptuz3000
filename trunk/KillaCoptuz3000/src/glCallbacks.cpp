// ***************************************************************
//  glCallbacks   version:  1.0   ·  date: 06/10/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "glCallbacks.h"

#include <map>

#include "CGame.h"
#include "CLevel.h"

#include "CObjectStorage.h"

#include "globals.h"

#include "Objects/CObject.h"
#include "Objects/CPlayer.h"

//////////////////////////////////////////////////////////////////////////
// Global variables
//////////////////////////////////////////////////////////////////////////
/** stored from CGame */
CLevel*  g_levelPtr  = 0;

//////////////////////////////////////////////////////////////////////////
// Help functions
//////////////////////////////////////////////////////////////////////////
void renderBitmapString(float t_x, float t_y, char *t_string, void *t_font)
{  
   char *a_c;
   glRasterPos3f(t_x, t_y, 0.0);
   for (a_c=t_string; *a_c != '\0'; a_c++)
   {
      glutBitmapCharacter(t_font, *a_c);
   }
}

//////////////////////////////////////////////////////////////////////////
// Callbacks used while level is running
//////////////////////////////////////////////////////////////////////////
void setCallbackLevelPtr(CLevel* t_levelPtr)
{
   g_levelPtr = t_levelPtr;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Callbacks for level
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void CLevel_timerCallback(int value)
{
   assert(g_levelPtr);

   // Iterate over all objects
   // and update them (events are generated)   
   CObjectStorage::getInstance().updateObjects(g_levelPtr);
   
   // Detect collisions with quad tree (events are generated)

   // ... (Quad tree here...) Collision check
   CObjectStorage::getInstance().m_quadTreeRootPtr->getCollisionEvents();

   // Process events
   CObjectStorage::getInstance().processEvents();

   // Delete dead objects
   CObjectStorage::getInstance().processDeleteMap();

   if(CObjectStorage::getInstance().isGameOver())
   {
      CGame::getInstance().m_gameState    = e_menu;
      CGame::getInstance().m_menuName     = "gameover";
      CGame::getInstance().gameControl();
   }
   else   
   {
      glutTimerFunc (25, CLevel_timerCallback, value);
   }
}

void CLevel_renderScene(void)
{
   CPlayer*       a_playerPtr = CObjectStorage::getInstance().getPlayerPtr();

   static float   s_xCenter   = 0.0;
   static float   s_yCenter   = 0.0;
   

   if (a_playerPtr)
   {
      s_xCenter = a_playerPtr->m_xPos;
      s_yCenter = a_playerPtr->m_yPos;
   }

   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   glPushMatrix();     

   glTranslatef(-s_xCenter, -s_yCenter, 0.0);

   //////////////////////////////////////////////////////////////////////////
   // draw the level
   std::list<unsigned int>::iterator a_it;

   for(a_it = CObjectStorage::getInstance().m_drawList.begin(); a_it != CObjectStorage::getInstance().m_drawList.end(); a_it++)
   {
      CObjectStorage::getInstance().m_objectMap[(*a_it)]->draw();
   }

   //////////////////////////////////////////////////////////////////////////
   // Can be deleted (hit point output)
   // char a_str[100];
   // _itoa_s(a_playerPtr->m_hitPoints, a_str, 10);
   //renderBitmapString(g_playerPtr->m_xPos + 2.3, 1.5, a_str, GLUT_BITMAP_8_BY_13);

   //////////////////////////////////////////////////////////////////////////
   // Test. Draw the quad tree
   if(g_showQuadTree)
   {
      CObjectStorage::getInstance().m_quadTreeRootPtr->draw();
   }

   glPopMatrix();
   
   // swapping the buffers causes the rendering above to be  
   // shown
   glutSwapBuffers();
}


void CLevel_processNormalKeys(unsigned char key, int x, int y)
{
   CPlayer* a_playerPtr = CObjectStorage::getInstance().getPlayerPtr();

   switch (key) 
   {
   case 32: // Space flips th player direction
      a_playerPtr->flip();
      break;
   case 'c': // player fires primary weapon
      a_playerPtr->fireWeapon();
      break;
   case 'n': // player switches weapon
      a_playerPtr->nextWeapon();
      break;
   }
}

void CLevel_pressKey(int key, int x, int y) 
{   
   CPlayer* a_playerPtr = CObjectStorage::getInstance().getPlayerPtr();

   switch (key) 
   {

   case GLUT_KEY_F1:
      g_windowedMode = !g_windowedMode;
      // Destroy old window
      if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE))
      {
         glutLeaveGameMode();
      }
      if (g_windowId)
      {
         glutDestroyWindow(g_windowId);
         g_windowId = 0;
      }

      if (g_windowedMode)
      {
         // Create a windowed display
         glutInitWindowSize(g_winWidth, g_winHeight);
         g_windowId = glutCreateWindow("Les commanderZ presetez: KillaKoptuz 3000");    
      }
      else
      {
         // Full screen display
         glutGameModeString("1280x1024:32");
         if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))
         {
            glutEnterGameMode();
         }
         else
            exit(-1);

      }
      break;	
   case GLUT_KEY_F6: 
      // return to default window         
      if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE))
      {
         glutLeaveGameMode();
      }
      if (g_windowId)
      {
         glutDestroyWindow(g_windowId);
         g_windowId = 0;
      }
      glutDisplayFunc(0);
      break;
   case GLUT_KEY_RIGHT:
      a_playerPtr->m_rightPressed = true;         
      break;
   case GLUT_KEY_LEFT:
      a_playerPtr->m_leftPressed  = true;
      break;
   case GLUT_KEY_UP:
      a_playerPtr->m_upPressed    = true;
      break;
   case GLUT_KEY_DOWN:
      a_playerPtr->m_downPressed  = true;
      break;
   }

   // Check for modifying keys pressed (shift, ctrl, alt)
   int a_mod = glutGetModifiers();

   // Shift
   if (a_mod == GLUT_ACTIVE_SHIFT)
      a_playerPtr->m_shiftPressed = true;
   else
      a_playerPtr->m_shiftPressed = false;
}

void CLevel_releaseKey(int key, int x, int y)
{
   CPlayer* a_playerPtr = CObjectStorage::getInstance().getPlayerPtr();

   switch (key) 
   {
   case GLUT_KEY_RIGHT:
      a_playerPtr->m_rightPressed = false;
      break;
   case GLUT_KEY_LEFT:
      a_playerPtr->m_leftPressed  = false;
      break;
   case GLUT_KEY_UP:
      a_playerPtr->m_upPressed    = false;
      break;
   case GLUT_KEY_DOWN:
      a_playerPtr->m_downPressed  = false;
      break;
   }
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Callbacks for menu
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CMenu_timerCallback(int t_value)
{
   CGame::getInstance().m_menu.update();
   glutTimerFunc (100, CMenu_timerCallback, t_value);
}

void CMenu_renderScene()
{
   CGame::getInstance().m_menu.draw();
}

void CMenu_pressKey(int t_key, int t_x, int t_y)
{
   switch (t_key) 
   {

   case GLUT_KEY_UP:
      CGame::getInstance().m_menu.next();
      break;
   case GLUT_KEY_DOWN:
      CGame::getInstance().m_menu.previous();
      break;
   }
}

void CMenu_releaseKey(int t_key, int t_x, int t_y)
{

}

void CMenu_processNormalKeys(unsigned char t_key, int t_x, int t_y)
{
   switch (t_key) 
   {
   case 13: // Access with return
      //CGame::getInstance().actat);
      CGame::getInstance().m_menu.performAction();
      break;
   }
}
