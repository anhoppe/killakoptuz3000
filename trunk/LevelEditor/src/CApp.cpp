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

#include "gui/CMainFrame.h"

IMPLEMENT_APP(CApp)


bool CApp::OnInit()
{
   m_mainFramePtr = new CMainFrame();

   m_mainFramePtr->Show();

   return true;
}

int CApp::OnExit()
{
   return 69;
}

int CApp::FilterEvent(wxEvent& t_event)
{
   int r_result = -1;

   if(t_event.GetEventType() == wxEVT_KEY_DOWN)
   {
      wxKeyEvent* a_keyEvent = dynamic_cast<wxKeyEvent*>(&t_event);
      m_mainFramePtr->onKeyDown(*a_keyEvent);
      r_result = FALSE;
   }

   return r_result;
}
