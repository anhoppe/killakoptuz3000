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
void changeSize(int w, int h);

void setDisplay();
void setupSound();
void initGL(int t_argc, char* t_argv);

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
   {
      h = 1;
   }

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

