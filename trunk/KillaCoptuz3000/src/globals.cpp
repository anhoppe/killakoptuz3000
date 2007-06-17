
// ***************************************************************
//  globals   version:  1.0   ·  date: 05/12/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "CLevel.h"
#include "objects/CPlayer.h"
#include "globals.h"

// Show bounding boxes for collision detection
bool     g_showBox      = true;

// Show hull polygons for collision detection
bool     g_showHull     = false;

// Mute (==1) or not
bool     g_mute         = true;

// The level object
CLevel   g_level;

// The player object pointer
// CPlayer* g_playerPtr    = 0;

// Windowed (==1) or full screen
bool	   g_windowedMode = 1;

// Width of game window
int      g_winWidth     = 800;

// Height of game window
int      g_winHeight    = 400;             

// Window id
int      g_windowId     = 0;
