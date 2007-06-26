// ***************************************************************
//  CApp   version:  1.0   ·  date: 06/18/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "CApp.h"

#include "CMainFrame.h"

IMPLEMENT_APP(CApp)


bool CApp::OnInit()
{
   CMainFrame* a_framePtr = new CMainFrame();

   a_framePtr->Show();

   return true;
}

int CApp::OnExit()
{
   return 69;
}