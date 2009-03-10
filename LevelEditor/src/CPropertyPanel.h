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

#include "ISetObject.h"

class CObjectPropertyPanel;
class CWeaponPropertyPanel;

//////////////////////////////////////////////////////////////////////////
// Enumeration for property type
//////////////////////////////////////////////////////////////////////////
enum EPropertyType
{
   e_boolean = 0,
   e_int     = 1,
   e_float   = 2,
   e_string  = 3,
   e_combo   = 4
};


//////////////////////////////////////////////////////////////////////////
// Struct for property Description
//////////////////////////////////////////////////////////////////////////
struct SPropDesc
{
   /** Name of the property */
   wxString          m_name;

   /** Type of the property */
   EPropertyType     m_type;

   /** row index in property sheet */
   int               m_rowIndex;

   /** Point to represented value. Only the pointer with the correct type is valid */
   float*            m_floatPtr;
   int*              m_intPtr;
   bool*             m_booleanPtr;
   std::string*      m_stringPtr;
};

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
