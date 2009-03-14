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
   m_notebookPtr->AddPage(m_objectPanelPtr, "Object", true);

   // add the weapon panel to the notebook
   m_weaponPanelPtr = new CWeaponPropertyPanel(m_notebookPtr);
   m_notebookPtr->AddPage(m_weaponPanelPtr, "Weapons", false);

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
      if(a_objectPtr->getType() == e_enemy)
      {
         m_weaponPanelPtr->Enable(true);
      }
      else
      {
         m_weaponPanelPtr->Enable(false);
      }
   }
}
