// ***************************************************************
//  CDlgTypeSelection   version:  1.0   ·  date: 06/18/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "CDlgTypeSelection.h"

//////////////////////////////////////////////////////////////////////////
// Definitions
//////////////////////////////////////////////////////////////////////////
#define ID_BUTTON_OK       1001
#define ID_BUTTON_CANCEL   1002

//////////////////////////////////////////////////////////////////////////
// Event Table
//////////////////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(CDlgTypeSelection, wxDialog)
   EVT_BUTTON(ID_BUTTON_OK, onButtonOk)
   EVT_BUTTON(ID_BUTTON_CANCEL, onButtonCancel)
END_EVENT_TABLE()

CDlgTypeSelection::CDlgTypeSelection(wxWindow* t_parentPtr)
: wxDialog(t_parentPtr, wxID_ANY, "Type Selection", wxDefaultPosition, wxDefaultSize)
{
   wxBoxSizer* a_mainSizerPtr    = 0;
   wxBoxSizer* a_buttonSizerPtr  = 0;

   a_mainSizerPtr = new wxBoxSizer(wxVERTICAL);

   //////////////////////////////////////////////////////////////////////////
   // Create selection
   wxArrayString a_selection;
   a_selection.Add("object");
   a_selection.Add("enemy");
   
   m_comboSelectotPtr = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, a_selection);
   a_mainSizerPtr->Add(m_comboSelectotPtr);
   m_comboSelectotPtr->SetSelection(0);

   //////////////////////////////////////////////////////////////////////////
   // create buttons
   wxButton* a_buttonPtr = 0;
   
   a_buttonSizerPtr = new wxBoxSizer(wxHORIZONTAL);

   a_buttonPtr = new wxButton(this, ID_BUTTON_OK, "Ok");
   a_buttonSizerPtr->Add(a_buttonPtr);

   a_buttonPtr = new wxButton(this, ID_BUTTON_CANCEL, "Ok");
   a_buttonSizerPtr->Add(a_buttonPtr);

   a_mainSizerPtr->Add(a_buttonSizerPtr);

   SetSizer(a_mainSizerPtr);
}


CDlgTypeSelection::~CDlgTypeSelection()
{

}

void CDlgTypeSelection::onButtonOk(wxCommandEvent& t_event)
{
   m_selection = m_comboSelectotPtr->GetSelection();

   EndModal(wxID_OK);
}

void CDlgTypeSelection::onButtonCancel(wxCommandEvent& t_event)
{
   EndModal(wxID_CANCEL);
}