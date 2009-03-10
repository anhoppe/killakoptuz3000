// ***************************************************************
//  CMainFrame   version:  1.0   ·  date: 06/18/2007
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

   /** Key events are triggered in the application class */
   void onKeyDown(wxKeyEvent& event);
   void onKeyUp(wxKeyEvent& t_event);

private:
   void createMenuBar();

   /** Sets the file name in data storage */
   bool setFileName();

   //////////////////////////////////////////////////////////////////////////
   // Events
   //////////////////////////////////////////////////////////////////////////
   DECLARE_EVENT_TABLE()
   void onMenuNew(wxCommandEvent& t_event);
   void onMenuLoad(wxCommandEvent& t_event);
   void onMenuSave(wxCommandEvent& t_event);
   void onMenuSaveAs(wxCommandEvent& t_event);

   void onMenuAddTextures(wxCommandEvent& t_event);

   void onSize(wxSizeEvent& t_event);
   void onShow(wxShowEvent& t_event);


   //////////////////////////////////////////////////////////////////////////
   // Variables
   //////////////////////////////////////////////////////////////////////////
   CControlPanel* m_controlPanelPtr;
   CGLCanvas*     m_glCanvasPtr;
};

#endif
