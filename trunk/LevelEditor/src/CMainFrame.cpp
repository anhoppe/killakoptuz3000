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

#include "CDlgAddTextures.h"

#include "wx/sizer.h"
#include "wx/menu.h"

//////////////////////////////////////////////////////////////////////////
// Definitions
//////////////////////////////////////////////////////////////////////////
#define ID_MENU_NEW_OBJECT          1001
#define ID_MENU_PROJECT_ADD_TEXTURE 1002

#define GFX_BASE_FILE               "..\\KillCoptuz3000\\data\\gfx"

//////////////////////////////////////////////////////////////////////////
// Event Table
//////////////////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(CMainFrame, wxFrame)
   EVT_MENU(ID_MENU_NEW_OBJECT, onMenuNewObject)
   EVT_MENU(ID_MENU_PROJECT_ADD_TEXTURE, onMenuAddTextures)
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

   //////////////////////////////////////////////////////////////////////////
   // create file menu
   a_menuPtr = new wxMenu();
   a_menuPtr->Append(ID_MENU_NEW_OBJECT, "New Object");
   a_menuBarPtr->Append(a_menuPtr, "File");

   //////////////////////////////////////////////////////////////////////////
   // create project menu
   a_menuPtr = new wxMenu();
   a_menuPtr->Append(ID_MENU_PROJECT_ADD_TEXTURE, "Add Textures");
   a_menuBarPtr->Append(a_menuPtr, "Project");

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

void CMainFrame::onMenuAddTextures(wxCommandEvent& t_event)
{
   CDlgAddTextures                     a_at(this);
   std::vector<STextureDesc>::iterator a_it;
   CTexture*                           a_texturePtr   = 0;


   if(a_at.ShowModal() == wxID_OK)
   {
      // add all textures to data storage
      for(a_it = a_at.m_textures.begin(); a_it != a_at.m_textures.end(); a_it++)
      {
         if(CDataStorage::getInstance().m_textureMap[(*a_it).m_baseFileName] == 0)
         {
            a_texturePtr = new CTexture;
            a_texturePtr->loadFromBaseFile(GFX_BASE_FILE, (*a_it).m_baseFileName, (*a_it).m_gfxType, (*a_it).m_hullPoints);
            CDataStorage::getInstance().m_textureMap[(*a_it).m_baseFileName] = a_texturePtr;
         }
      }

      CUpdateContainer::getInstance().update();
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
