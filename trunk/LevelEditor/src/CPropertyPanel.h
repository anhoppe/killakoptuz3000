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

#include "..//KillaCoptuz3000/src/Objects/CObject.h"

#include <wx/wx.h>
#include <wx/grid.h>

#include <vector>

//////////////////////////////////////////////////////////////////////////
// Enumerations for object properties
//////////////////////////////////////////////////////////////////////////
enum EObjectProperties
{
   e_xPos            = 0,
   e_yPos            = 1,
   e_width           = 2,
   e_height          = 3,
   e_cycleInterval   = 4,
   e_isBackground    = 5,
   e_gfxType         = 6,
   e_explosionIndex  = 7,
   e_startAngle      = 8,
   e_maxHitpoints    = 9,
   e_damagePoints    = 10
};

//////////////////////////////////////////////////////////////////////////
// Enumeration for property type
//////////////////////////////////////////////////////////////////////////
enum EPropertyType
{
   e_boolean = 0,
   e_int     = 1,
   e_float   = 2,
   e_string  = 3
};


//////////////////////////////////////////////////////////////////////////
// Struct for property Description
//////////////////////////////////////////////////////////////////////////
struct SPropDesc
{
   /** Name of the property */
   wxString       m_name;

   /** Type of the property */
   EPropertyType  m_type;

   /** row index in property sheet */
   int            m_rowIndex;

   /** Point to represented value. Only the pointer with the correct type is valid */
   float*         m_floatPtr;
   int*           m_intPtr;
   bool*          m_booleanPtr;
   std::string*   m_stringPtr;
};


//////////////////////////////////////////////////////////////////////////
// Class Declaration
//////////////////////////////////////////////////////////////////////////
class CPropertyPanel : public wxPanel
{
   //////////////////////////////////////////////////////////////////////////
   // Methods
   //////////////////////////////////////////////////////////////////////////
public:
   CPropertyPanel(wxWindow* t_parentPtr);
   ~CPropertyPanel();

   void setData(int t_index);

private:

   /**
   *  !! Caution !! If ever attributes of an CObject changes in KK3000,
   *     T-H-I-S    function must be altered
   */
   void initObjectProperties();

   /**
   *  Appends object properties to the property sheet
   */
   void appendProperties(CObject* t_objectPtr, std::vector<SPropDesc>& t_popertyMap);

   /**
   *  Sets object properties according to the currently selected object
   */
   void setProperties();

   //////////////////////////////////////////////////////////////////////////
   // Events
   //////////////////////////////////////////////////////////////////////////
   DECLARE_EVENT_TABLE()
   void onGridChange(wxGridEvent& t_event);

   //////////////////////////////////////////////////////////////////////////
   // Variables
   //////////////////////////////////////////////////////////////////////////
public:
private:
   /** A wxGrid contains the properties */
   wxGrid*                                m_gridPropertyPtr;

   /** Index of currently selected object in CDataStorage */
   int                                    m_selectedObjectIndex;

   /** Map with property description */
   std::vector<SPropDesc>                 m_objectProperties;

   /** Start index of object properties */
   int                                    m_startIndexObject;

};

#endif
