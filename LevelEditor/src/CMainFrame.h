// ***************************************************************
//  CMainFrame   version:  1.0   �  date: 06/18/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef CMAIN_FRAME_H
#define CMAIN_FRAME_H

#include "wx/wx.h"

class CControlPanel;
class CGLCanvas;

class CMainFrame : public wxFrame
{
   //////////////////////////////////////////////////////////////////////////
   // Methods
   //////////////////////////////////////////////////////////////////////////
public:
   CMainFrame();
   ~CMainFrame();

private:
   void createMenuBar();

   //////////////////////////////////////////////////////////////////////////
   // Events
   //////////////////////////////////////////////////////////////////////////
   DECLARE_EVENT_TABLE()
   void onMenuNewObject(wxCommandEvent& t_event);
   void onMenuAddTextures(wxCommandEvent& t_event);

   void onKeyUp(wxKeyEvent& t_event);

   //////////////////////////////////////////////////////////////////////////
   // Variables
   //////////////////////////////////////////////////////////////////////////
   CControlPanel* m_controlPanelPtr;
   CGLCanvas*     m_glCanvasPtr;
};

#endif