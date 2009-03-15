/********************************************************************
	created:	2009/03/06
	created:	6:3:2009   23:20
	filename: 	c:\hoppe\develop\KK3000\LevelEditor\src\CObjectPropertyPanel.cpp
	file path:	c:\hoppe\develop\KK3000\LevelEditor\src
	file base:	CObjectPropertyPanel
	file ext:	cpp
	author:		anhoppe
	
	purpose:	Property panel for objects
*********************************************************************/
#include "CObjectPropertyPanel.h"

#include "../data/CDataStorage.h"
#include "../data/CUpdateContainer.h"

//////////////////////////////////////////////////////////////////////////
// Definitions
//////////////////////////////////////////////////////////////////////////
#define ID_GRID_PROPERTY      1001
#define ID_BUTTON_ADD_TEXTURE 1002
#define ID_BUTTON_DEL_TEXTURE 1003

#define ADD_BUTTON_SIZE       wxSize(25, 20)

//////////////////////////////////////////////////////////////////////////
// global variables
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// Event Table
//////////////////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(CObjectPropertyPanel, wxPanel)
   EVT_GRID_CMD_CELL_CHANGE(ID_GRID_PROPERTY, onGridChange)
   EVT_BUTTON(ID_BUTTON_ADD_TEXTURE, onButtonAddTexture)
   EVT_BUTTON(ID_BUTTON_DEL_TEXTURE, onButtonDelTexture)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////
CObjectPropertyPanel::CObjectPropertyPanel(wxWindow* t_parentPtr)
: CBasePropertyPanel(t_parentPtr)
{
   m_selectedObjectIndex = -1;
   m_startIndexObject    = 0;

   wxBoxSizer* a_sizerPtr = new wxBoxSizer(wxVERTICAL);

   //////////////////////////////////////////////////////////////////////////
   // add grid
   m_gridPropertiesPtr = new wxGrid(this, ID_GRID_PROPERTY);
   m_gridPropertiesPtr->CreateGrid(0, 2);
   m_gridPropertiesPtr->SetRowLabelSize(0);
   m_gridPropertiesPtr->SetColLabelSize(25);
   m_gridPropertiesPtr->SetColLabelValue(0, "Key");
   m_gridPropertiesPtr->SetColLabelValue(1, "Value");

   a_sizerPtr->Add(m_gridPropertiesPtr, 3, wxEXPAND|wxALIGN_CENTER|wxGROW);

   //    m_gridChoiceTexturePtr = 0;

   //////////////////////////////////////////////////////////////////////////
   // add texture keys list
   m_listTextureKeys = new wxListBox(this, wxID_ANY);
   a_sizerPtr->Add(m_listTextureKeys, 1, wxEXPAND|wxGROW|wxTOP, 1);

   //////////////////////////////////////////////////////////////////////////
   // add combo box with for texture keys and add / remove buttons
   wxBoxSizer* a_selectSizerPtr = new wxBoxSizer(wxHORIZONTAL);

   // combo box
   m_comboTextureKeysPtr = new wxComboBox(this, wxID_ANY);
   a_selectSizerPtr->Add(m_comboTextureKeysPtr, 1, wxGROW|wxEXPAND, 0);

   // add button
   a_selectSizerPtr->Add(new wxButton(this, ID_BUTTON_ADD_TEXTURE, "Add", wxDefaultPosition, ADD_BUTTON_SIZE), 0, wxLEFT|wxRIGHT, 1);
   a_selectSizerPtr->Add(new wxButton(this, ID_BUTTON_DEL_TEXTURE, "Del", wxDefaultPosition, ADD_BUTTON_SIZE), 0, wxLEFT|wxRIGHT, 1);

   a_sizerPtr->Add(a_selectSizerPtr);

   SetSizer(a_sizerPtr);

   CUpdateContainer::getInstance().add((IUpdate*)this);
   CUpdateContainer::getInstance().add((ISetObject*)this);
}

CObjectPropertyPanel::~CObjectPropertyPanel()
{

}

void CObjectPropertyPanel::update()
{
   fillComboTexture(m_comboTextureKeysPtr);
}

void CObjectPropertyPanel::setObject(int t_index)
{
   CObject* a_objectPtr = 0;

   if(CDataStorage::getInstance().m_objects.size() > 0)
   {
      m_selectedObjectIndex = t_index;

      a_objectPtr = CDataStorage::getInstance().m_objects[t_index];

      //////////////////////////////////////////////////////////////////////////
      // append object properties
      initProperties();
      fillPropertyGrid();

      //////////////////////////////////////////////////////////////////////////
      // append texture keys
      m_listTextureKeys->Clear();
      std::vector<std::string>::iterator  a_it;
      for(a_it = a_objectPtr->m_textureKeys.begin(); a_it != a_objectPtr->m_textureKeys.end(); a_it++)
      {
         m_listTextureKeys->Append((*a_it));
      }
   }
}

//////////////////////////////////////////////////////////////////////////
// Private Methods
//////////////////////////////////////////////////////////////////////////
void CObjectPropertyPanel::initProperties()
{
   SPropDesc   a_desc;
   CObject*    a_objectPtr = CDataStorage::getInstance().getActiveObject();

   if(0 != a_objectPtr)
   {
      m_properties.clear();

      a_desc.m_name  = "X-Pos";
      a_desc.m_type  = e_float;
      a_desc.m_floatPtr = &a_objectPtr->m_xPos;
      a_desc.m_rowIndex = m_startIndexObject;
      m_properties.push_back(a_desc);

      a_desc.m_name = "Y-Pos";
      a_desc.m_type = e_float;
      a_desc.m_floatPtr = &a_objectPtr->m_yPos;
      a_desc.m_rowIndex = m_startIndexObject+1;
      m_properties.push_back(a_desc);

      a_desc.m_name = "Width";
      a_desc.m_type = e_float;
      a_desc.m_floatPtr = &a_objectPtr->m_width;
      a_desc.m_rowIndex = m_startIndexObject+2;
      m_properties.push_back(a_desc);

      a_desc.m_name = "Height";
      a_desc.m_type = e_float;
      a_desc.m_floatPtr = &a_objectPtr->m_height;
      a_desc.m_rowIndex = m_startIndexObject+3;
      m_properties.push_back(a_desc);

      a_desc.m_name = "Cycle Interval";
      a_desc.m_type = e_int;
      a_desc.m_intPtr = &a_objectPtr->m_cycleInterval;
      a_desc.m_rowIndex = m_startIndexObject+4;
      m_properties.push_back(a_desc);

      a_desc.m_name = "Is Background";
      a_desc.m_type = e_boolean;
      a_desc.m_booleanPtr = &a_objectPtr->m_isBackground;
      a_desc.m_rowIndex = m_startIndexObject+5;
      m_properties.push_back(a_desc);

      a_desc.m_name = "GFX Type";
      a_desc.m_type = e_string;
      a_desc.m_stringPtr = &a_objectPtr->m_gfxType;
      a_desc.m_rowIndex = m_startIndexObject+6;
      m_properties.push_back(a_desc);

      a_desc.m_name = "Exp. index";
      a_desc.m_type = e_int;
      a_desc.m_intPtr = &a_objectPtr->m_explosionIndex;
      a_desc.m_rowIndex = m_startIndexObject+7;
      m_properties.push_back(a_desc);

      a_desc.m_name = "Start angle";
      a_desc.m_type = e_float;
      a_desc.m_floatPtr = &a_objectPtr->m_startAngle;
      a_desc.m_rowIndex = m_startIndexObject+8;
      m_properties.push_back(a_desc);

      a_desc.m_name = "max Hitpoints";
      a_desc.m_type = e_int;
      a_desc.m_intPtr = &a_objectPtr->m_maxHitPoints;
      a_desc.m_rowIndex = m_startIndexObject+9;
      m_properties.push_back(a_desc);

      a_desc.m_name = "Damage Points";
      a_desc.m_type = e_int;
      a_desc.m_intPtr = &a_objectPtr->m_damagePoints;
      a_desc.m_rowIndex = m_startIndexObject+10;
      m_properties.push_back(a_desc);
   }
}


//////////////////////////////////////////////////////////////////////////
// Events
//////////////////////////////////////////////////////////////////////////
// void CObjectPropertyPanel::onGridChange(wxGridEvent& t_event)
// {
//    std::vector<SPropDesc>::iterator a_it;
//    SPropDesc*                       a_descPtr   = 0;
//    int                              a_rowIndex  = t_event.GetRow();
//    wxString                         a_str       = "";
// 
// 
//    // get entry by row index
//    for(a_it = m_properties.begin(); a_it != m_properties.end(); a_it++)
//    {
//       int a_i = (*a_it).m_rowIndex;
//       if((*a_it).m_rowIndex == a_rowIndex)
//       {
//          a_descPtr = &(*(a_it));
//          break;
//       }
//    }
// 
//    if(0 != a_descPtr)
//    {
//       a_str = m_gridPropertiesPtr->GetCellValue(a_rowIndex, 1);
// 
//       switch(a_descPtr->m_type)
//       {
//       case e_boolean:
//          if((a_str == "true") || (a_str == "1"))
//          {
//             *a_descPtr->m_booleanPtr = true;
//          }
//          else
//          {
//             *a_descPtr->m_booleanPtr = false;
//          }
//          break;
//       case e_int:
//          *a_descPtr->m_intPtr = atoi(a_str.c_str());
//          break;
//       case e_float:
//          *a_descPtr->m_floatPtr = atof(a_str.c_str());
//          break;
//       case e_string:
//          *a_descPtr->m_stringPtr = a_str.c_str();
//          break;
//       case e_combo:
//          *a_descPtr->m_stringPtr = a_str.c_str();
//          break;
// 
//       }
// 
//       CUpdateContainer::getInstance().update();
//    }
// }

void CObjectPropertyPanel::onButtonAddTexture(wxCommandEvent& t_event)
{
   int a_index = m_comboTextureKeysPtr->GetSelection();

   if(wxNOT_FOUND != a_index)
   {
      std::string a_key = m_comboTextureKeysPtr->GetValue().c_str();
      CObject*    a_objectPtr = CDataStorage::getInstance().m_objects[m_selectedObjectIndex];

      if(0 != a_objectPtr)
      {
         std::vector<std::string>::iterator  a_it;
         bool                                a_found  = false;

         for(a_it = a_objectPtr->m_textureKeys.begin(); a_it != a_objectPtr->m_textureKeys.end(); a_it++)
         {
            if((*a_it) == a_key)
            {
               a_found = true;
               break;
            }
         }
         if(!a_found)
         {
            a_objectPtr->m_textureKeys.push_back(a_key);
            CUpdateContainer::getInstance().setObject(m_selectedObjectIndex);
            CUpdateContainer::getInstance().update();
         }
      }
   }
}

void CObjectPropertyPanel::onButtonDelTexture(wxCommandEvent& t_event)
{
   int   a_index  = m_listTextureKeys->GetSelection();

   if(wxNOT_FOUND != a_index)
   {
      std::string a_str       = m_listTextureKeys->GetString((unsigned int) a_index);
      CObject*    a_objectPtr = CDataStorage::getInstance().m_objects[m_selectedObjectIndex];

      if(0 != a_objectPtr)
      {
         std::vector<std::string>::iterator  a_it;

         for(a_it = a_objectPtr->m_textureKeys.begin(); a_it != a_objectPtr->m_textureKeys.end(); a_it++)
         {
            if((*a_it) == a_str)
            {
               a_objectPtr->m_textureKeys.erase(a_it);
               break;
            }
         }
         CUpdateContainer::getInstance().setObject(m_selectedObjectIndex);
         CUpdateContainer::getInstance().update();
      }
   }
}
