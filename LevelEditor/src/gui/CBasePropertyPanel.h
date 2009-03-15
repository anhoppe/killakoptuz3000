/********************************************************************
	created:	2009/03/15
	created:	15:3:2009   21:34
	filename: 	c:\hoppe\develop\KK3000\LevelEditor\src\gui\CBasePropertyPanel.h
	file path:	c:\hoppe\develop\KK3000\LevelEditor\src\gui
	file base:	CBasePropertyPanel
	file ext:	h
	author:		anhoppe
	
	purpose:	Base class for object / weapon property panel on notebook
*********************************************************************/
#ifndef CBASE_PROPERTY_PANEL_H
#define CBASE_PROPERTY_PANEL_H

#include <vector>

#include "wx/wx.h"
#include "wx/grid.h"

//////////////////////////////////////////////////////////////////////////
// Enumeration for property type
//////////////////////////////////////////////////////////////////////////
enum EPropertyType
{
   e_boolean = 0,
   e_int     = 1,
   e_uint    = 2,
   e_float   = 3,
   e_string  = 4,
   e_combo   = 5
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
   unsigned int*     m_uintPtr;
   bool*             m_booleanPtr;
   std::string*      m_stringPtr;
};

class CBasePropertyPanel : public wxPanel
{
public:
   //////////////////////////////////////////////////////////////////////////
   // Public Methods
   //////////////////////////////////////////////////////////////////////////
   CBasePropertyPanel(wxWindow* t_parentPtr);
   ~CBasePropertyPanel();

   //////////////////////////////////////////////////////////////////////////
   // Protected Methods
   //////////////////////////////////////////////////////////////////////////
protected:
   /** Must be overwritten, fills the property list */
   virtual void initProperties() = 0;

   /** Fills the property grid */
   void fillPropertyGrid();

   /** This method must be called by the derived class if a grid change event happens */
   void onGridChange(wxGridEvent& t_event);
   
   /** fills a combo box with available textures */
   void fillComboTexture(wxComboBox* t_comboBoxPtr);
   
   //////////////////////////////////////////////////////////////////////////
   // Protected Member Variables
   //////////////////////////////////////////////////////////////////////////
protected:
   /** The grid of the class */
   wxGrid*                 m_gridPropertiesPtr;
   
   /** Vector with available properties */
   std::vector<SPropDesc>  m_properties;
};

#endif
