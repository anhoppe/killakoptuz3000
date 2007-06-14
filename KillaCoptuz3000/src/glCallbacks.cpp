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
// Callbacks used while level is running
//////////////////////////////////////////////////////////////////////////
void setCallbackLevelPtr(CLevel* t_levelPtr)
{
   g_levelPtr = t_levelPtr;
}

void CLevel_timerCallback(int value)
{
   std::map<unsigned int, CObject*>::iterator   a_it;

   assert(g_levelPtr);

   // Iterate over all objects
   // and update them (events are generated)   
  
   if (CObjectStorage::getInstance().m_objectMap.iterate(true))
   {
      do 
      {
         CObjectStorage::getInstance().m_objectMap.m_current.m_value->update(g_levelPtr);
      } 
      while(CObjectStorage::getInstance().m_objectMap.iterate());
   }
   
   // Detect collisions with quad tree (events are generated)

   // ... (Quad tree here...)

   // Process events
   CObjectStorage::getInstance().processEvents();

   // Delete dead objects
   CObjectStorage::getInstance().processDeleteMap();
   
   glutTimerFunc (25, CLevel_timerCallback, value);
}

void CLevel_renderScene(void)
{
   CPlayer* a_playerPtr = CObjectStorage::getInstance().getPlayerPtr();

   assert(a_playerPtr);

   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   glPushMatrix();     

   glTranslatef(-a_playerPtr->m_xPos, 0.0, 0.0);

   //////////////////////////////////////////////////////////////////////////
   // draw the level
   std::map<unsigned int, CObject*>::iterator   a_it;

   // first draw all objects
   if (CObjectStorage::getInstance().m_objectMap.iterate(true))
   {
      do 
      {
         if (e_object == CObjectStorage::getInstance().m_objectMap.m_current.m_value->getType())
         {            
            CObjectStorage::getInstance().m_objectMap.m_current.m_value->draw();
         }
      } 
      while(CObjectStorage::getInstance().m_objectMap.iterate());
   }

   // second draw all non objects
   if (CObjectStorage::getInstance().m_objectMap.iterate(true))
   {
      do 
      {
         if (e_object != CObjectStorage::getInstance().m_objectMap.m_current.m_value->getType())
         {            
            CObjectStorage::getInstance().m_objectMap.m_current.m_value->draw();
         }
      } 
      while(CObjectStorage::getInstance().m_objectMap.iterate());
   }

   CObjectStorage* a_obj = &CObjectStorage::getInstance();

   a_obj->m_objectMap;

   //////////////////////////////////////////////////////////////////////////
   // Can be deleted (hit point output)
   char a_str[100];
   _itoa_s(a_playerPtr->m_hitPoints, a_str, 10);
   //renderBitmapString(g_playerPtr->m_xPos + 2.3, 1.5, a_str, GLUT_BITMAP_8_BY_13);

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
         // Fullscreen display
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
