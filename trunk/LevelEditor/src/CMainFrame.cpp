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
#define ID_MENU_FILE_NEW            1000
#define ID_MENU_LOAD                1001
#define ID_MENU_FILE_SAVE           1002
#define ID_MENU_FILE_SAVE_AS        1003
#define ID_MENU_PROJECT_ADD_TEXTURE 1004

#define GFX_BASE_FILE               "..\\KillaCoptuz3000\\data\\gfx\\"
#define LEVEL_BASE_DIR              "..\\KillaKoptuz3000\\data\\levels\\"
//////////////////////////////////////////////////////////////////////////
// Event Table
//////////////////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(CMainFrame, wxFrame)
   EVT_MENU(ID_MENU_FILE_NEW, onMenuNew)
   EVT_MENU(ID_MENU_LOAD, onMenuLoad)
   EVT_MENU(ID_MENU_FILE_SAVE, onMenuSave)
   EVT_MENU(ID_MENU_FILE_SAVE_AS, onMenuSaveAs)
   EVT_MENU(ID_MENU_PROJECT_ADD_TEXTURE, onMenuAddTextures)
   EVT_SIZE(onSize)
   EVT_SHOW(onShow)
   EVT_KEY_UP(onKeyUp)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////
CMainFrame::CMainFrame()
: wxFrame(0, wxID_ANY, "KillaKoptuz3000 - Level Editor")
{
   wxBoxSizer* a_sizerPtr  = new wxBoxSizer(wxHORIZONTAL);

   //////////////////////////////////////////////////////////////////////////
   // create control panel
   m_controlPanelPtr = new CControlPanel(this);
   a_sizerPtr->Add(m_controlPanelPtr, 0, wxGROW|wxALIGN_LEFT, 0);

   //////////////////////////////////////////////////////////////////////////
   // creates openGL canvas
   m_glCanvasPtr = new CGLCanvas(this);

   a_sizerPtr->Add(m_glCanvasPtr, 4, wxEXPAND|wxGROW, 0);

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
   a_menuPtr->Append(ID_MENU_FILE_NEW, "New");
   a_menuPtr->Append(ID_MENU_LOAD, "Load...");
   a_menuPtr->Append(ID_MENU_FILE_SAVE, "Save");
   a_menuPtr->Append(ID_MENU_FILE_SAVE_AS, "Save as...");
   a_menuBarPtr->Append(a_menuPtr, "File");

   //////////////////////////////////////////////////////////////////////////
   // create project menu
   a_menuPtr = new wxMenu();
   a_menuPtr->Append(ID_MENU_PROJECT_ADD_TEXTURE, "Add Textures");
   a_menuBarPtr->Append(a_menuPtr, "Project");

   SetMenuBar(a_menuBarPtr);
}

bool CMainFrame::setFileName()
{
   wxString a_dir = ::wxGetWorkingDirectory();
   a_dir += LEVEL_BASE_DIR;
   wxFileDialog   a_fd(this, "Select File Name", a_dir, "*.xml");
   bool           r_ok = false;

   if(a_fd.ShowModal() == wxID_OK)
   {
      CDataStorage::getInstance().m_fileName = a_fd.GetPath();
      r_ok = true;
   }

   return r_ok;
}

//////////////////////////////////////////////////////////////////////////
// Events
//////////////////////////////////////////////////////////////////////////
void CMainFrame::onMenuNew(wxCommandEvent& t_event)
{
   bool  a_clear = true;

   if(!CDataStorage::getInstance().isEmpty())
   {
      if(wxMessageBox("Delete current level ?", "Query", wxYES|wxNO) == wxNO)
      {
         a_clear = false;
      }
   }

   if(a_clear)
   {
      CDataStorage::getInstance().clear();
   }
}

void CMainFrame::onMenuLoad(wxCommandEvent& t_event)
{
   bool  a_load = true;


   if(!CDataStorage::getInstance().isEmpty())
   {
      if(wxMessageBox("Overwrite current level?", "Query", wxYES|wxNO) == wxNO)
      {
         a_load = false;
      }
   }

   if(a_load)
   {
      wxString a_dir = ::wxGetWorkingDirectory();
      a_dir += +LEVEL_BASE_DIR;
      wxFileDialog a_fd(this, "Select level to load", a_dir, wxEmptyString, "XML files (*.xml)|*.xml");

      if(a_fd.ShowModal() == wxID_OK)
      {
         std::vector<int>  a_layerIndices;

         CDataStorage::getInstance().clear();
         CDataStorage::getInstance().m_fileName = a_fd.GetPath();
         
         if(!CDataStorage::getInstance().load(GFX_BASE_FILE, a_layerIndices))
         {
            wxMessageBox("Corrupt level file could not be opened", "Error");
            CDataStorage::getInstance().clear();
         }
         else
         {
            m_controlPanelPtr->insertLayers(a_layerIndices);
            CUpdateContainer::getInstance().update();
         }
      }
   }
}
   
void CMainFrame::onMenuSave(wxCommandEvent& t_event)
{
   bool a_saveFile = true;


   if(CDataStorage::getInstance().m_fileName == "")
   {
      if(!setFileName())
      {
         a_saveFile = false;
      }
   }

   if(a_saveFile)
   {
      CDataStorage::getInstance().save();
   }
}

void CMainFrame::onMenuSaveAs(wxCommandEvent& t_event)
{
   if(setFileName())
   {
      CDataStorage::getInstance().save();
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
            a_texturePtr->loadFromBaseFile(GFX_BASE_FILE, (*a_it).m_baseFileName.c_str(), (*a_it).m_gfxType.c_str(), (*a_it).m_hullPoints);
            CDataStorage::getInstance().m_textureMap[(*a_it).m_baseFileName] = a_texturePtr;
         }
      }

      CUpdateContainer::getInstance().update();
   }
}

void CMainFrame::onSize(wxSizeEvent& t_event)
{
   CUpdateContainer::getInstance().update();
   t_event.Skip(true);
}

void CMainFrame::onShow(wxShowEvent& t_event)
{
   CUpdateContainer::getInstance().update();
   t_event.Skip(true);
}

void CMainFrame::onKeyDown(wxKeyEvent& t_event)
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

void CMainFrame::onKeyUp(wxKeyEvent& t_event)
{
//    switch(t_event.GetKeyCode())
//    {
//    case WXK_UP:
//       CDataStorage::getInstance().m_yPos += .1;
//       break;
//    case WXK_DOWN:
//       CDataStorage::getInstance().m_yPos -= .1;
//       break;
//    case WXK_LEFT:
//       CDataStorage::getInstance().m_xPos -= .1;
//       break;
//    case WXK_RIGHT:
//       CDataStorage::getInstance().m_xPos += .1;
//       break;
//    }
//    m_glCanvasPtr->update();

}
