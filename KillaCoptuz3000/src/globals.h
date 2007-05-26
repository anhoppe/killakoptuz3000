/********************************************************************
	created:	2007/05/12
	created:	12:5:2007   11:33
	filename: 	c:\source\KillaCoptuz3000\KillaCoptuz3000\Data\Header\globals.h
	file path:	c:\source\KillaCoptuz3000\KillaCoptuz3000\Data\Header
	file base:	globals
	file ext:	h
	author:		Fabs
	
	purpose:	
*********************************************************************/
#ifndef KC_GLOBALS
#define KC_GLOBALS

class CPlayer;
class CLevel;

//////////////////////////////////////////////////////////////////////////
// global variables
//////////////////////////////////////////////////////////////////////////

// Show bounding boxes for collision detection
extern bool     g_showBox;

// Show hull polygons for collision detection
extern bool     g_showHull;

// Mute (==1) or not
extern bool     g_mute;

// The level object
extern CLevel   g_level;

// The player object pointer
extern CPlayer* g_playerPtr;

// Windowed (==1) or full screen
extern bool	    g_windowedMode;

// Width of game window
extern int      g_winWidth;

// Height of game window
extern int      g_winHeight;

// Window id
extern int      g_windowId;

#endif