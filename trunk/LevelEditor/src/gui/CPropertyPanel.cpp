// ***************************************************************
//  CPropertyPanel   version:  1.0   ·  date: 06/24/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "CPropertyPanel.h"

#include "../data/CDataStorage.h"
#include "../data/CUpdateContainer.h"

#include "CObjectPropertyPanel.h"
#include "CWeaponPropertyPanel.h"


//////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////
CPropertyPanel::CPropertyPanel(wxWindow* t_parentPtr)
: wxPanel(t_parentPtr, wxID_ANY)
{
   wxBoxSizer* a_sizerPtr  = new wxBoxSizer(wxVERTICAL);

   // add the notebook
   m_notebookPtr = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_BOTTOM);
   
   // add the object panel to the notebook
   m_objectPanelPtr = new CObjectPropertyPanel(m_notebookPtr);
   m_objectPanelPtr->Hide();
   //m_notebookPtr->AddPage(m_objectPanelPtr, "Object", true);

   // add the weapon panel to the notebook
   m_weaponPanelPtr = new CWeaponPropertyPanel(m_notebookPtr);
   m_weaponPanelPtr->Hide();
   //m_notebookPtr->AddPage(m_weaponPanelPtr, "Weapons", false);

   // add the notebook to the sizer
   a_sizerPtr->Add(m_notebookPtr, 1, wxGROW|wxEXPAND);

   SetSizer(a_sizerPtr);

   CUpdateContainer::getInstance().add(this);
}

CPropertyPanel::~CPropertyPanel()
{

}

void CPropertyPanel::setObject(int t_index)
{
   CObject* a_objectPtr = CDataStorage::getInstance().m_objects[t_index];

   if(0 != a_objectPtr)
   {
      // remove all pages from the notebook
      while(m_notebookPtr->GetPageCount() > 0)
      {
         m_notebookPtr->RemovePage(0);
      }

      // add the object page
      m_notebookPtr->AddPage(m_objectPanelPtr, "Object", true);

      // display the weapons panel
      if(a_objectPtr->getType() == e_enemy)
      {
         if(!m_weaponPanelPtr->IsShown())
         {
            m_weaponPanelPtr->Show();
         }
         m_notebookPtr->AddPage(m_weaponPanelPtr, "Weapon", false);
      }
      else
      {
         m_weaponPanelPtr->Hide();
      }

      if(!m_objectPanelPtr->IsShown())
      {
         m_objectPanelPtr->Show();
      }

      m_notebookPtr->Update();
      m_notebookPtr->Refresh();
   }
}
