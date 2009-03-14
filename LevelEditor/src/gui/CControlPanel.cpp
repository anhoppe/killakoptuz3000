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

#include "../data/CDataStorage.h"
#include "../data/CUpdateContainer.h"

#include "CLayerControl.h"
#include "CPropertyPanel.h"

//////////////////////////////////////////////////////////////////////////
// Definitions
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Event Table
//////////////////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(CControlPanel, wxPanel)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////
CControlPanel::CControlPanel(wxWindow* t_parentPtr)
: wxPanel(t_parentPtr, wxID_ANY, wxDefaultPosition, wxSize(200, 360))
{
   wxBoxSizer*    a_sizerPtr        = 0;
   
   //////////////////////////////////////////////////////////////////////////
   // Register listener
   CUpdateContainer::getInstance().add((ISetObject*)this);


   //////////////////////////////////////////////////////////////////////////
   // create GUI
   a_sizerPtr = new wxBoxSizer(wxVERTICAL);

   //////////////////////////////////////////////////////////////////////////
   // set the layer control panel
   m_panelLayerControlPtr = new CLayerControl(this);
   a_sizerPtr->Add(m_panelLayerControlPtr, 1, wxEXPAND);

   //////////////////////////////////////////////////////////////////////////
   // read property sheet
   m_panelPropertyPtr = new CPropertyPanel(this);
   a_sizerPtr->Add(m_panelPropertyPtr, 1, wxEXPAND|wxALIGN_BOTTOM);

   //////////////////////////////////////////////////////////////////////////
   // Register sizer
   SetSizer(a_sizerPtr);   
}

CControlPanel::~CControlPanel()
{
   CUpdateContainer::getInstance().remove((ISetObject*)this);
}


void CControlPanel::setObject(int t_index)
{
   CDataStorage::getInstance().setActiveObjectByIndex(t_index);
}

void CControlPanel::insertLayers(std::vector<int>& t_layerIndices)
{
   m_panelLayerControlPtr->insertLayers(t_layerIndices);
}

//////////////////////////////////////////////////////////////////////////
// Events
//////////////////////////////////////////////////////////////////////////
