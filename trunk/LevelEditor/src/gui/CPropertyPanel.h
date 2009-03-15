// ***************************************************************
//  CPropertyPanel   version:  1.0   ·  date: 06/24/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef CPROPERTY_PANEL_H
#define CPROPERTY_PANEL_H

#include "wx/wx.h"
#include "wx/notebook.h"

#include "../data/ISetObject.h"

class CObjectPropertyPanel;
class CWeaponPropertyPanel;


//////////////////////////////////////////////////////////////////////////
// Class Declaration
//////////////////////////////////////////////////////////////////////////
class CPropertyPanel : public wxPanel, ISetObject
{
   //////////////////////////////////////////////////////////////////////////
   // Public Methods
   //////////////////////////////////////////////////////////////////////////
public:   
   CPropertyPanel(wxWindow* t_parentPtr);
   ~CPropertyPanel();

   virtual void setObject(int t_index);

   //////////////////////////////////////////////////////////////////////////
   // Member variables
   //////////////////////////////////////////////////////////////////////////
   wxNotebook*             m_notebookPtr;

   CObjectPropertyPanel*   m_objectPanelPtr;
   CWeaponPropertyPanel*   m_weaponPanelPtr;
};
#endif
