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
: wxPanel(t_parentPtr, wxID_ANY, wxDefaultPosition, wxSize(200, 360))
{
   m_selectedObjectIndex = -1;
   m_startIndexObject    = 0;

   wxBoxSizer* a_sizerPtr = new wxBoxSizer(wxVERTICAL);

   //////////////////////////////////////////////////////////////////////////
   // add grid
   m_gridPropertyPtr = new wxGrid(this, ID_GRID_PROPERTY);
   m_gridPropertyPtr->CreateGrid(0, 2);
   m_gridPropertyPtr->SetRowLabelSize(0);
   m_gridPropertyPtr->SetColLabelSize(25);
   m_gridPropertyPtr->SetColLabelValue(0, "Key");
   m_gridPropertyPtr->SetColLabelValue(1, "Value");

   a_sizerPtr->Add(m_gridPropertyPtr, 3, wxEXPAND|wxALIGN_CENTER|wxGROW);

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
   fillComboTexture();
}

void CObjectPropertyPanel::setObject(int t_index)
{
   CObject* a_objectPtr = 0;

   if(CDataStorage::getInstance().m_objects.size() > 0)
   {
      m_selectedObjectIndex = t_index;

      a_objectPtr = CDataStorage::getInstance().m_objects[t_index];

      //////////////////////////////////////////////////////////////////////////
      // Clear old content and initialize grid
      m_gridPropertyPtr->ClearGrid();

      if(m_gridPropertyPtr->GetNumberRows() > 0)
      {
         m_gridPropertyPtr->DeleteRows(0, m_gridPropertyPtr->GetNumberRows());
      }

      //////////////////////////////////////////////////////////////////////////
      // append object properties
      initObjectProperties();

      m_startIndexObject = m_gridPropertyPtr->GetNumberRows();
      appendProperties(a_objectPtr, m_objectProperties);
      setProperties();

      //////////////////////////////////////////////////////////////////////////
      // append texture keys
      m_listTextureKeys->Clear();
      std::vector<std::string>::iterator  a_it;
      for(a_it = a_objectPtr->m_textureKeys.begin(); a_it != a_objectPtr->m_textureKeys.end(); a_it++)
      {
         m_listTextureKeys->Append((*a_it));
      }

      //////////////////////////////////////////////////////////////////////////
      // append sprite properties
      if(a_objectPtr->getType() == e_enemy)
      {

      }
   }
}

//////////////////////////////////////////////////////////////////////////
// Private Methods
//////////////////////////////////////////////////////////////////////////
void CObjectPropertyPanel::initObjectProperties()
{
   SPropDesc   a_desc;
   CObject*    a_objectPtr = CDataStorage::getInstance().m_objects[m_selectedObjectIndex];

   m_objectProperties.clear();

   a_desc.m_name  = "X-Pos";
   a_desc.m_type  = e_float;
   a_desc.m_floatPtr = &a_objectPtr->m_xPos;
   a_desc.m_rowIndex = m_startIndexObject;
   m_objectProperties.push_back(a_desc);

   a_desc.m_name = "Y-Pos";
   a_desc.m_type = e_float;
   a_desc.m_floatPtr = &a_objectPtr->m_yPos;
   a_desc.m_rowIndex = m_startIndexObject+1;
   m_objectProperties.push_back(a_desc);

   a_desc.m_name = "Width";
   a_desc.m_type = e_float;
   a_desc.m_floatPtr = &a_objectPtr->m_width;
   a_desc.m_rowIndex = m_startIndexObject+2;
   m_objectProperties.push_back(a_desc);

   a_desc.m_name = "Height";
   a_desc.m_type = e_float;
   a_desc.m_floatPtr = &a_objectPtr->m_height;
   a_desc.m_rowIndex = m_startIndexObject+3;
   m_objectProperties.push_back(a_desc);

   a_desc.m_name = "Cycle Interval";
   a_desc.m_type = e_int;
   a_desc.m_intPtr = &a_objectPtr->m_cycleInterval;
   a_desc.m_rowIndex = m_startIndexObject+4;
   m_objectProperties.push_back(a_desc);

   a_desc.m_name = "Is Background";
   a_desc.m_type = e_boolean;
   a_desc.m_booleanPtr = &a_objectPtr->m_isBackground;
   a_desc.m_rowIndex = m_startIndexObject+5;
   m_objectProperties.push_back(a_desc);

   a_desc.m_name = "GFX Type";
   a_desc.m_type = e_string;
   a_desc.m_stringPtr = &a_objectPtr->m_gfxType;
   a_desc.m_rowIndex = m_startIndexObject+6;
   m_objectProperties.push_back(a_desc);

   a_desc.m_name = "Exp. index";
   a_desc.m_type = e_int;
   a_desc.m_intPtr = &a_objectPtr->m_explosionIndex;
   a_desc.m_rowIndex = m_startIndexObject+7;
   m_objectProperties.push_back(a_desc);

   a_desc.m_name = "Start angle";
   a_desc.m_type = e_float;
   a_desc.m_floatPtr = &a_objectPtr->m_startAngle;
   a_desc.m_rowIndex = m_startIndexObject+8;
   m_objectProperties.push_back(a_desc);

   a_desc.m_name = "max Hitpoints";
   a_desc.m_type = e_int;
   a_desc.m_intPtr = &a_objectPtr->m_maxHitPoints;
   a_desc.m_rowIndex = m_startIndexObject+9;
   m_objectProperties.push_back(a_desc);

   a_desc.m_name = "Damage Points";
   a_desc.m_type = e_int;
   a_desc.m_intPtr = &a_objectPtr->m_damagePoints;
   a_desc.m_rowIndex = m_startIndexObject+10;
   m_objectProperties.push_back(a_desc);
}

void CObjectPropertyPanel::appendProperties(CObject* t_objectPtr, std::vector<SPropDesc>& t_properties)
{
   std::vector<SPropDesc>::iterator a_it;

   int                              a_numProp   = 0;
   int                              a_index     = 0;


   //////////////////////////////////////////////////////////////////////////
   // get start index for inserting in property table
   a_index = m_gridPropertyPtr->GetNumberRows();

   //////////////////////////////////////////////////////////////////////////
   // Append rows
   m_gridPropertyPtr->AppendRows((int)t_properties.size());

   //////////////////////////////////////////////////////////////////////////
   // enter property names
   for(a_it = t_properties.begin(); a_it != t_properties.end(); a_it++)
   {
      wxString a_str = (*a_it).m_name;

      m_gridPropertyPtr->SetCellValue(a_index, 0, (*a_it).m_name);

      a_index++;
   }

   m_gridPropertyPtr->Show();
}

void CObjectPropertyPanel::setProperties()
{
   std::vector<SPropDesc>::iterator a_it;
   wxString                         a_str = "";

   for(a_it = m_objectProperties.begin(); a_it != m_objectProperties.end(); a_it++)
   {
      switch((*a_it).m_type)
      {
      case e_boolean:
         if(*((*a_it).m_booleanPtr))
         {
            a_str = "true";
         }
         else
         {
            a_str = "false";
         }
         break;
      case e_float:
         a_str.sprintf("%f", *((*a_it).m_floatPtr));
         break;
      case e_int:
         a_str.sprintf("%d", *((*a_it).m_intPtr));
         break;
      case e_string:
         a_str = (*a_it).m_stringPtr->c_str();
         break;
      case e_combo:
         if((*a_it).m_name == "textureKeys")
         {
            a_str = (*a_it).m_stringPtr->c_str();
            continue;
         }
         break;
      }

      m_gridPropertyPtr->SetCellValue((*a_it).m_rowIndex, 1, a_str);
   }
}

void CObjectPropertyPanel::fillComboTexture()
{
   size_t                                    a_count     = 0;
   size_t                                    a_index     = 0;
   wxString                                  a_keys;
   std::map<wxString, CTexture*>::iterator   a_mapIt;


   a_count = CDataStorage::getInstance().m_textureMap.size();
   a_index = 0;

   m_comboTextureKeysPtr->Clear();
   for(a_mapIt = CDataStorage::getInstance().m_textureMap.begin(); a_mapIt != CDataStorage::getInstance().m_textureMap.end(); a_mapIt++)
   {
      //          a_keys += a_mapIt->first;
      // 
      //          if(a_index < (a_count-1))
      //          {
      //             a_keys += ",";
      //          }
      //          a_index++;
      m_comboTextureKeysPtr->Append(a_mapIt->first);
   }
}

//////////////////////////////////////////////////////////////////////////
// Events
//////////////////////////////////////////////////////////////////////////
void CObjectPropertyPanel::onGridChange(wxGridEvent& t_event)
{
   std::vector<SPropDesc>::iterator a_it;
   SPropDesc*                       a_descPtr   = 0;
   int                              a_rowIndex  = t_event.GetRow();
   wxString                         a_str       = "";


   // get entry by row index
   for(a_it = m_objectProperties.begin(); a_it != m_objectProperties.end(); a_it++)
   {
      int a_i = (*a_it).m_rowIndex;
      if((*a_it).m_rowIndex == a_rowIndex)
      {
         a_descPtr = &(*(a_it));
         break;
      }
   }

   if(0 != a_descPtr)
   {
      a_str = m_gridPropertyPtr->GetCellValue(a_rowIndex, 1);

      switch(a_descPtr->m_type)
      {
      case e_boolean:
         if((a_str == "true") || (a_str == "1"))
         {
            *a_descPtr->m_booleanPtr = true;
         }
         else
         {
            *a_descPtr->m_booleanPtr = false;
         }
         break;
      case e_int:
         *a_descPtr->m_intPtr = atoi(a_str.c_str());
         break;
      case e_float:
         *a_descPtr->m_floatPtr = atof(a_str.c_str());
         break;
      case e_string:
         *a_descPtr->m_stringPtr = a_str.c_str();
         break;
      case e_combo:
         *a_descPtr->m_stringPtr = a_str.c_str();
         break;

      }

      CUpdateContainer::getInstance().update();
   }
}

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
