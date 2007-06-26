// ***************************************************************
//  CControlPanel   version:  1.0   ·  date: 06/18/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "CControlPanel.h"

#include "../KillaCoptuz3000/src/Objects/CObject.h"

#include "CDataStorage.h"
#include "CUpdateContainer.h"

//////////////////////////////////////////////////////////////////////////
// Definitions
//////////////////////////////////////////////////////////////////////////
#define ID_COMBO_OBJECT_SELECTION   1001

//////////////////////////////////////////////////////////////////////////
// Event Table
//////////////////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(CControlPanel, wxPanel)
   EVT_COMBOBOX(ID_COMBO_OBJECT_SELECTION, onComboObjectSelection)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////
CControlPanel::CControlPanel(wxWindow* t_parentPtr)
: wxPanel(t_parentPtr, wxID_ANY, wxDefaultPosition, wxSize(400, 360))
{
   wxBoxSizer*    a_sizerPtr        = 0;
   
   //////////////////////////////////////////////////////////////////////////
   // Register listener
   CUpdateContainer::getInstance().add(this);


   //////////////////////////////////////////////////////////////////////////
   // create GUI
   a_sizerPtr = new wxBoxSizer(wxVERTICAL);

   //////////////////////////////////////////////////////////////////////////
   // Create object selectorS
   m_comboObjectSelectionPtr = new wxComboBox(this, ID_COMBO_OBJECT_SELECTION, "", wxPoint(10, 35), wxSize(130, 20));
   a_sizerPtr->Add(m_comboObjectSelectionPtr, 0);

   //////////////////////////////////////////////////////////////////////////
   // read property sheet
   m_panelPropertyPtr = new CPropertyPanel(this);
   a_sizerPtr->Add(m_panelPropertyPtr, 1, wxGROW|wxALIGN_BOTTOM);

   //////////////////////////////////////////////////////////////////////////
   // Register sizer
   SetSizer(a_sizerPtr);   
}

CControlPanel::~CControlPanel()
{
   CUpdateContainer::getInstance().remove(this);
}

void CControlPanel::update()
{
   std::vector<CObject*>::iterator  a_it;
   wxString                         a_str    = "";
   int                              a_count  = 0;


   //////////////////////////////////////////////////////////////////////////
   // Delete content of the object list
   m_comboObjectSelectionPtr->Clear();

   for(a_it = CDataStorage::getInstance().m_objects.begin(); a_it != CDataStorage::getInstance().m_objects.end(); a_it++)
   {
      a_str.sprintf("Object%d", a_count);
      m_comboObjectSelectionPtr->Append(a_str);

      a_count++;
   }
}

//////////////////////////////////////////////////////////////////////////
// Events
//////////////////////////////////////////////////////////////////////////
void CControlPanel::onComboObjectSelection(wxCommandEvent& t_event)
{
   int a_index = 0;

   a_index = m_comboObjectSelectionPtr->GetSelection();

   m_panelPropertyPtr->setData(a_index);
}
