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

#include <vector>

#include "../data/ISetObject.h"

class CLayerControl;
class CPropertyPanel;

class CControlPanel : public wxPanel,
                             ISetObject
{
   //////////////////////////////////////////////////////////////////////////
   // Methods
   //////////////////////////////////////////////////////////////////////////
public:
   CControlPanel(wxWindow* t_parentPtr);
   ~CControlPanel();

   virtual void setObject(int t_index);

   /** Insert layers into the layer control panel after loading a level */
   void insertLayers(std::vector<int>& t_layerIndices);
   //////////////////////////////////////////////////////////////////////////
   // Events
   //////////////////////////////////////////////////////////////////////////
   DECLARE_EVENT_TABLE()

   //////////////////////////////////////////////////////////////////////////
   // Variables
   //////////////////////////////////////////////////////////////////////////
   /** Layer control allows adding objects and defines layer indices of objects */
   CLayerControl*    m_panelLayerControlPtr;

   /** Property panel displays all properties of currently selected object */
   CPropertyPanel*   m_panelPropertyPtr;
};

#endif
