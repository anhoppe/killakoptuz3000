// ***************************************************************
//  CMainFrame   version:  1.0   ·  date: 06/18/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "CMainFrame.h"

#include "CControlPanel.h"
#include "CGLCanvas.h"

#include "CDataStorage.h"
#include "CDlgTypeSelection.h"

#include "CUpdateContainer.h"


#include "wx/sizer.h"
#include "wx/menu.h"

//////////////////////////////////////////////////////////////////////////
// Definitions
//////////////////////////////////////////////////////////////////////////
#define ID_MENU_NEW_OBJECT 1001

//////////////////////////////////////////////////////////////////////////
// Event Table
//////////////////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(CMainFrame, wxFrame)
   EVT_MENU(ID_MENU_NEW_OBJECT, onMenuNewObject)
   EVT_KEY_UP(onKeyUp)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////
CMainFrame::CMainFrame()
: wxFrame(0, wxID_ANY, "KillaKoptuz3000 - Level Editor")
{
   wxFlexGridSizer* a_sizerPtr = new wxFlexGridSizer(1, 2, 1, 1);

   a_sizerPtr->AddGrowableCol(1);
   a_sizerPtr->AddGrowableRow(0);

   //////////////////////////////////////////////////////////////////////////
   // create control panel
   m_controlPanelPtr = new CControlPanel(this);
   a_sizerPtr->Add(m_controlPanelPtr, 0, wxGROW, 0);

   //////////////////////////////////////////////////////////////////////////
   // creat eopenGL canvas
   m_glCanvasPtr = new CGLCanvas(this);
   a_sizerPtr->Add(m_glCanvasPtr, 0, wxGROW, 0);

   SetSizer(a_sizerPtr);

   createMenuBar();
}

CMainFrame::~CMainFrame()
{

}

//////////////////////////////////////////////////////////////////////////
// Private
//////////////////////////////////////////////////////////////////////////
void CMainFrame::createMenuBar()
{
   wxMenuBar*  a_menuBarPtr   = 0;
   wxMenu*     a_menuPtr      = 0;

   a_menuBarPtr = new wxMenuBar();

   a_menuPtr = new wxMenu();
   a_menuPtr->Append(ID_MENU_NEW_OBJECT, "New Object");

   a_menuBarPtr->Append(a_menuPtr, "File");
   SetMenuBar(a_menuBarPtr);
}

//////////////////////////////////////////////////////////////////////////
// Events
//////////////////////////////////////////////////////////////////////////
void CMainFrame::onMenuNewObject(wxCommandEvent& t_event)
{
   CDlgTypeSelection* a_dlgPtr = new CDlgTypeSelection(this);

   m_glCanvasPtr->SetCurrent();

   if(a_dlgPtr->ShowModal() == wxID_OK)
   {
      CDataStorage::getInstance().add(a_dlgPtr->m_selection);
      CUpdateContainer::getInstance().update();
      m_glCanvasPtr->update();
   }
}

void CMainFrame::onKeyUp(wxKeyEvent& t_event)
{
   switch(t_event.GetKeyCode())
   {
   case WXK_UP:
      CDataStorage::getInstance().m_yPos += .1;
      break;
   case WXK_DOWN:
      CDataStorage::getInstance().m_yPos -= .1;
      break;
   case WXK_LEFT:
      CDataStorage::getInstance().m_xPos -= .1;
      break;
   case WXK_RIGHT:
      CDataStorage::getInstance().m_xPos += .1;
      break;
   }
   m_glCanvasPtr->update();
   
}
