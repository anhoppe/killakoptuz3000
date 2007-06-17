// ***************************************************************
//  glCallbacks   version:  1.0   ·  date: 06/10/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef GL_CALLBACKS_H
#define GL_CALLBACKS_H

class CLevel;

//////////////////////////////////////////////////////////////////////////
// GL callbacks used while level is running
//////////////////////////////////////////////////////////////////////////
void setCallbackLevelPtr(CLevel* t_levelPtr);

void CLevel_timerCallback(int t_value);
void CLevel_renderScene();
void CLevel_pressKey(int t_key, int t_x, int t_y);      
void CLevel_releaseKey(int t_key, int t_x, int t_y);
void CLevel_processNormalKeys(unsigned char t_key, int t_x, int t_y);

void CMenu_timerCallback(int t_value);
void CMenu_renderScene();
void CMenu_pressKey(int t_key, int t_x, int t_y);      
void CMenu_releaseKey(int t_key, int t_x, int t_y);
void CMenu_processNormalKeys(unsigned char t_key, int t_x, int t_y);

#endif
