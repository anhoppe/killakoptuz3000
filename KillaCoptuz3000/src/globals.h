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
// 'good' old trick
#define KK3000 1
#define LE3000 2

#define PRODUCT LE3000

// Show bounding boxes for collision detection
extern bool     g_showBox;

// Show hull polygons for collision detection
extern bool     g_showHull;

// SHow quad tree
extern bool     g_showQuadTree;

// Mute (==1) or not
extern bool     g_mute;

// The level object
extern CLevel   g_level;

// The player object pointer
// extern CPlayer* g_playerPtr;

// Windowed (==1) or full screen
extern bool	    g_windowedMode;

// Width of game window
extern int      g_winWidth;

// Height of game window
extern int      g_winHeight;

// Window id
extern int      g_windowId;

//////////////////////////////////////////////////////////////////////////
// Game Physics
//////////////////////////////////////////////////////////////////////////
extern float    g_gravity;

// How far distant sounds can be heard. The higher, the lower is the decay of volume.
// Formula used: volume = volume_max*exp(-distance/g_soundDecay).
// Distance is in units of Heli height. 
extern float    g_soundDecay;

#endif