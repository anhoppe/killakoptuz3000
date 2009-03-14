// ***************************************************************
//  CDlgAddTexture   version:  1.0   ·  date: 06/28/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "CDlgAddTextures.h"

#include "../data/CDataStorage.h"

#include "../KillaCoptuz3000/src/CTexture.h "

#include <wx/filename.h>

#include <map>

//////////////////////////////////////////////////////////////////////////
// Definitions
//////////////////////////////////////////////////////////////////////////
#define ID_LIST_TEXTURES   1001
#define ID_BUTTON_ADD      1002
#define ID_BUTTON_REMOVE   1003
#define ID_BUTTON_OK       1004
#define ID_BUTTON_CANCEL   1005

#define DLG_SIZE           wxSize(400, 360)

#define DEFAULT_DIR        "..\\KillaKoptuz3000\\data\\gfx"

//////////////////////////////////////////////////////////////////////////
// Event Table
//////////////////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(CDlgAddTextures, wxDialog)
   EVT_BUTTON(ID_BUTTON_ADD, onButtonAdd)
   EVT_BUTTON(ID_BUTTON_REMOVE, onButtonRemove)
   EVT_BUTTON(ID_BUTTON_OK, onButtonOk)
   EVT_BUTTON(ID_BUTTON_CANCEL, onButtonCancel)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////
CDlgAddTextures::CDlgAddTextures(wxWindow* t_parentPtr)
: wxDialog(t_parentPtr, wxID_ANY, "Add Textures", wxDefaultPosition, DLG_SIZE)
{
   wxButton* a_buttonPtr = 0;

   // texture list
   m_listTexturePtr = new wxListBox(this, wxID_ANY, wxPoint(5, 5), wxSize(200, 300));
   
   // add button
   a_buttonPtr       = new wxButton(this, ID_BUTTON_ADD, "Add", wxPoint(210, 50), wxSize(60, 25));

   // remove button
   a_buttonPtr       = new wxButton(this, ID_BUTTON_REMOVE, "Remove", wxPoint(210, 80), wxSize(60, 25));

   // ok button
   a_buttonPtr       = new wxButton(this, ID_BUTTON_OK, "Ok", wxPoint(10, 310), wxSize(75, 25));

   // cancel button
   a_buttonPtr       = new wxButton(this, ID_BUTTON_CANCEL, "Cancel", wxPoint(100, 310), wxSize(75, 25));

   // load existing textures from data storage
   getTexturesFromDataStorage();

   // update
   update();
}

CDlgAddTextures::~CDlgAddTextures()
{
}

//////////////////////////////////////////////////////////////////////////
// Private Methods
//////////////////////////////////////////////////////////////////////////
void CDlgAddTextures::getTexturesFromDataStorage()
{
   std::map<wxString, CTexture*>::iterator   a_it;
   STextureDesc                              a_desc;

   
   for(a_it = CDataStorage::getInstance().m_textureMap.begin(); a_it != CDataStorage::getInstance().m_textureMap.end(); a_it++)
   {
      a_desc.m_baseFileName   = (a_it->second)->m_baseFileName;
      a_desc.m_gfxType        = (a_it->second)->m_gfxType;
      a_desc.m_hullPoints     = (a_it->second)->m_hullPoints;

      m_textures.push_back(a_desc);
   }
}

void CDlgAddTextures::update()
{
   std::vector<STextureDesc>::iterator a_it;
   wxString                            a_str = "";


   m_listTexturePtr->Clear();

   for(a_it = m_textures.begin();  a_it != m_textures.end(); a_it++)
   {
      a_str.sprintf("%s (poly: %d)", (*a_it).m_baseFileName.c_str(), (*a_it).m_hullPoints);
      m_listTexturePtr->Append(a_str);
   }
}
//////////////////////////////////////////////////////////////////////////
// Events
//////////////////////////////////////////////////////////////////////////
void CDlgAddTextures::onButtonAdd(wxCommandEvent& t_event)
{
   wxFileDialog   a_fd(this, "Choose files", DEFAULT_DIR, "", "", wxOPEN|wxMULTIPLE);
   wxArrayString  a_fileNames;
   wxFileName     a_fileName;

   unsigned int   a_index     = 0;
   STextureDesc   a_desc;

   if(a_fd.ShowModal())
   {
      a_fd.GetFilenames(a_fileNames);

      for(a_index = 0; a_index < a_fileNames.GetCount(); a_index++)
      {
         a_fileName = a_fileNames[a_index];

         a_desc.m_baseFileName = CDataStorage::getInstance().getKey(a_fileNames[a_index].c_str());
         a_desc.m_hullPoints   = 5;
         a_desc.m_gfxType      = a_fileName.GetExt();

         m_textures.push_back(a_desc);
      }
      update();
   }
}

void CDlgAddTextures::onButtonRemove(wxCommandEvent& t_event)
{

}

void CDlgAddTextures::onButtonOk(wxCommandEvent& t_event)
{
   EndModal(wxID_OK);
}

void CDlgAddTextures::onButtonCancel(wxCommandEvent& t_event)
{
   EndModal(wxID_CANCEL);
}
