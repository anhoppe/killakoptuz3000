// ***************************************************************
//  CDlgTypeSelection   version:  1.0   ·  date: 06/18/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef CDLG_TYPE_SELECTION_H
#define CDLG_TYPE_SELECTION_H

#include "wx/wx.h"

class CDlgTypeSelection : public wxDialog
{
   //////////////////////////////////////////////////////////////////////////
   // Methods
   //////////////////////////////////////////////////////////////////////////
public:
   CDlgTypeSelection(wxWindow* t_parentPtr);
   ~CDlgTypeSelection();

   //////////////////////////////////////////////////////////////////////////
   // Events
   //////////////////////////////////////////////////////////////////////////
   DECLARE_EVENT_TABLE()
   void onButtonOk(wxCommandEvent& t_event);
   void onButtonCancel(wxCommandEvent& t_event);

   //////////////////////////////////////////////////////////////////////////
   // Variables
   //////////////////////////////////////////////////////////////////////////
public:
   int         m_selection;

private:
   wxComboBox* m_comboSelectotPtr;
};

#endif
