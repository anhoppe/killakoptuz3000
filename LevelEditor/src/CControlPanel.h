// ***************************************************************
//  CControlPanel   version:  1.0   ·  date: 06/18/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef CCONTROL_PANEL_H
#define CCONTROL_PANEL_H

#include "wx/wx.h"


#include "IUpdate.h"

#include "CPropertyPanel.h"

class CControlPanel : public wxPanel,
                             IUpdate
{
   //////////////////////////////////////////////////////////////////////////
   // Methods
   //////////////////////////////////////////////////////////////////////////
public:
   CControlPanel(wxWindow* t_parentPtr);
   ~CControlPanel();

   virtual void update();

   //////////////////////////////////////////////////////////////////////////
   // Events
   //////////////////////////////////////////////////////////////////////////
   DECLARE_EVENT_TABLE()
   void onComboObjectSelection(wxCommandEvent& tevent);

   //////////////////////////////////////////////////////////////////////////
   // Variables
   //////////////////////////////////////////////////////////////////////////
   /** Combo Box containing all object graphics */
   wxComboBox*       m_comboObjectSelectionPtr;

   /** Property panel displays all properties of currently selected object */
   CPropertyPanel*   m_panelPropertyPtr;
};

#endif
