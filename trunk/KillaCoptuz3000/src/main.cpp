// ***************************************************************
//  main   version:  1.0   ·  date: 05/06/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "fmod.h"
#include "CHashMap.h"
#include "CGame.h"
 #include "globals.h"
// 
// //////////////////////////////////////////////////////////////////////////   
// //////////////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////////////
// 
// //////////////////////////////////////////////////////////////////////////
// /// Function declarations
// //////////////////////////////////////////////////////////////////////////
// 
// void renderScene();
// void pressKey(int key, int x, int y);
void changeSize(int w, int h);
// void pressKey(int key, int x, int y);
// void releaseKey(int key, int x, int y);
// void processNormalKeys(unsigned char key, int x, int y);
// bool initGame();   
void setDisplay();
void setupSound();
void initGL(int t_argc, char* t_argv);
// void renderBitmapString(float t_x, float t_y, char *t_string, void *font = GLUT_BITMAP_TIMES_ROMAN_24);
// 
// //////////////////////////////////////////////////////////////////////////
// /// Function implementation
// //////////////////////////////////////////////////////////////////////////
// 
// // Timer Callback
// void timerCallback(int value)
// {   
//    if(!g_level.m_levelOver)
//    {
//       g_playerPtr->updatePlayer(&g_level);
//       g_level.updateLevel();
//    }
// 
//    switch (value)
//    {
//    case TIMER_CYCLE_TEXTURES:
//       {         
//          g_playerPtr->nextTexture();
//          g_level.nextTexture();
// 
//          // Timer Callback redefinieren, da er sonst gelöscht wird
//          glutTimerFunc (25, timerCallback, TIMER_CYCLE_TEXTURES);
//       }
// 
//       break;
//    }
// }
// 
// void renderScene(void)
// {   
//    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
// 
//    glPushMatrix();     
// 
//    if(g_level.m_levelOver)
//    {
//       glBegin(GL_QUADS);
//       glColor3f(1., 1., 1.);
//       glVertex2d(-1.0, 1.0);
//       glVertex2d(1.0, 1.0);
//       glVertex2d(1.0, -1.0);
//       glVertex2d(-1.0, -1.0);
//       glEnd();
//    }
//    else
//    {      
//       glTranslatef(-g_playerPtr->m_xPos, 0.0, 0.0);
// 
//       //////////////////////////////////////////////////////////////////////////
//       // draw the level
//       g_level.drawLevel();
// 
//       //////////////////////////////////////////////////////////////////////////
//       // Can be deleted (hit point output)
//       char a_str[100];
//       _itoa_s(g_playerPtr->m_hitPoints, a_str, 10);
//       renderBitmapString(g_playerPtr->m_xPos + 2.3, 1.5, a_str, GLUT_BITMAP_8_BY_13);
//    }
// 
//    glPopMatrix();   
// 
//    // swapping the buffers causes the rendering above to be  
//    // shown
//    glutSwapBuffers();
// }
// 
// Setup window
// Switches between windowed and full screen mode
// void setupDisplay()
// {   
//     glutInit(&t_argc, t_argv);
//    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
// 
//    // Destroy old window
//    if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE))
//    {
//       glutLeaveGameMode();
//    }
//    if (g_windowId)
//    {
//       glutDestroyWindow(g_windowId);
//       g_windowId = 0;
//    }
// 
//    if (g_windowedMode)
//    {
//       // Create a windowed display
//       glutInitWindowSize(g_winWidth, g_winHeight);
//       g_windowId = glutCreateWindow("Les commanderZ presetez: KillaKoptuz 3000");    
//    }
//    else
//    {
//       // Fullscreen display
//       glutGameModeString("1280x1024:32");
//       if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))
//       {
//          glutEnterGameMode();
//       }
//       else
//          exit(-1);
// 
//    }   
// }

void setupSound()
{
   //Set up fmod sound api
   if (!FSOUND_Init(44100, 32, 0))
   {
      printf ("fmod initialization failed!\n");
   }
}

void registerMainCallbacks()
{

}

void changeSize(int w, int h)
{

   // Prevent a divide by zero, when window is too short
   // (you cant make a window of zero width).
   if(h == 0)
      h = 1;

   float ratio = 1.0* w / h;

   // Reset the coordinate system before modifying
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   // Set the view port to be the entire window
   glViewport(0, 0, w, h);

   // Set the correct perspective.
   gluPerspective(45,ratio,1,1000);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(0.0,0.0,5.0, 
      0.0,0.0,-1.0,
      0.0f,1.0f,0.0f);


}

void setDisplay()
{   
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

}


void initGL(int t_argc, char** t_argv)
{
   glutInit(&t_argc, t_argv);
   glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

   setDisplay();

   glutIgnoreKeyRepeat(1);    
   glutReshapeFunc(changeSize);
   
   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
// 
// void processNormalKeys(unsigned char key, int x, int y)
// {
//    switch (key) 
//    {
//    case 32: // Space flips th player direction
//       g_playerPtr->flip();
//       break;
//    case 'c': // player fires primary weapon
//       g_playerPtr->fireWeapon();
//       break;
//    case 'n': // player switches weapon
//       g_playerPtr->nextWeapon();
//       break;
//    }
// }
// 
// void renderBitmapString(float t_x, float t_y, char *t_string, void *font)
// {  
//    char* a_c;
//    glRasterPos3f(t_x, t_y, 1.00);
//    for (a_c=t_string; *a_c != '\0'; a_c++)
//    {
//       glutBitmapCharacter(font, *a_c);
//    }
// }
// 
// void pressKey(int key, int x, int y) 
// {   
//    switch (key) 
//    {
// 
//    case GLUT_KEY_F1:
//       g_windowedMode = !g_windowedMode;
//       setDisplay();		   
//       break;	
//    case GLUT_KEY_F6: 
//       // return to default window         
//       if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE))
//       {
//          glutLeaveGameMode();
//       }
//       if (g_windowId)
//       {
//          glutDestroyWindow(g_windowId);
//          g_windowId = 0;
//       }
//       glutDisplayFunc(0);
//       break;
//    case GLUT_KEY_RIGHT:
//       g_playerPtr->m_rightPressed = true;         
//       break;
//    case GLUT_KEY_LEFT:
//       g_playerPtr->m_leftPressed  = true;
//       break;
//    case GLUT_KEY_UP:
//       g_playerPtr->m_upPressed    = true;
//       break;
//    case GLUT_KEY_DOWN:
//       g_playerPtr->m_downPressed  = true;
//       break;
//    }
// 
//    // Check for modifying keys pressed (shift, ctrl, alt)
//    int a_mod = glutGetModifiers();
// 
//    // Shift
//    if (a_mod == GLUT_ACTIVE_SHIFT)
//       g_playerPtr->m_shiftPressed = true;
//    else
//       g_playerPtr->m_shiftPressed = false;
// }
// 
// void releaseKey(int key, int x, int y)
// {
//    switch (key) 
//    {
//    case GLUT_KEY_RIGHT:
//       g_playerPtr->m_rightPressed = false;
//       break;
//    case GLUT_KEY_LEFT:
//       g_playerPtr->m_leftPressed  = false;
//       break;
//    case GLUT_KEY_UP:
//       g_playerPtr->m_upPressed    = false;
//       break;
//    case GLUT_KEY_DOWN:
//       g_playerPtr->m_downPressed  = false;
//       break;
//    }
// }
// 
// bool initGame()
// {
//    bool r_ret;
// 
//    // 'load' the level...
//    r_ret = g_level.load("data/levels/level3.xml");
// 
//    // load the player   
//    r_ret &= g_playerPtr->loadPlayer("data/player/player.xml");
// 
//    return r_ret;
// }
// 

int main(int t_argc, char **t_argv) 
{      
   //Init OpenGL and setup screen
   initGL(t_argc, t_argv);

   // Set up sound system
   setupSound();

   // Create CGame object and start game
   CGame::getInstance().gameControl();

   // Start glut main loop
   glutMainLoop();

   return 69;
}

