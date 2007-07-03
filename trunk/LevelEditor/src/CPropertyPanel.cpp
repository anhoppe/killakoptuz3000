// ***************************************************************
//  CPropertyPanel   version:  1.0   ·  date: 06/24/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "CPropertyPanel.h"

#include "CDataStorage.h"
#include "CUpdateContainer.h"

//////////////////////////////////////////////////////////////////////////
// Definitions
//////////////////////////////////////////////////////////////////////////
#define ID_GRID_PROPERTY   1001

//////////////////////////////////////////////////////////////////////////
// global variables
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// Event Table
//////////////////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(CPropertyPanel, wxPanel)
   EVT_GRID_CMD_CELL_CHANGE(ID_GRID_PROPERTY, onGridChange)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////
CPropertyPanel::CPropertyPanel(wxWindow* t_parentPtr)
: wxPanel(t_parentPtr, wxID_ANY)
{
   m_selectedObjectIndex = -1;
   m_startIndexObject    = 0;

   wxBoxSizer* a_sizerPtr = new wxBoxSizer(wxVERTICAL);

   m_gridPropertyPtr = new wxGrid(this, ID_GRID_PROPERTY);
   m_gridPropertyPtr->CreateGrid(0, 2);

   a_sizerPtr->Add(m_gridPropertyPtr, 1, wxALIGN_CENTER|wxGROW);

   SetSizer(a_sizerPtr);
}

CPropertyPanel::~CPropertyPanel()
{

}

void CPropertyPanel::setData(int t_index)
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
      // append sprite properties
      if(a_objectPtr->getType() == e_enemy)
      {
         
      }
   }
}

//////////////////////////////////////////////////////////////////////////
// Private Methods
//////////////////////////////////////////////////////////////////////////
void CPropertyPanel::initObjectProperties()
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

   a_desc.m_name      = "textureKeys";
   a_desc.m_type      = e_combo;
   a_desc.m_stringPtr = &a_objectPtr->m_texture;
   a_desc.m_rowIndex = m_selectedObjectIndex+11;
   a_desc.m_editorPtr = 0;
   m_objectProperties.push_back(a_desc);


}

void CPropertyPanel::appendProperties(CObject* t_objectPtr, std::vector<SPropDesc>& t_properties)
{
   std::vector<SPropDesc>::iterator a_it;

   int                              a_numProp   = 0;
   int                              a_index     = 0;


   //////////////////////////////////////////////////////////////////////////
   // get start index for inserting in property table
   a_index = m_gridPropertyPtr->GetNumberRows();

   //////////////////////////////////////////////////////////////////////////
   // Append rows
   m_gridPropertyPtr->AppendRows(t_properties.size());

   //////////////////////////////////////////////////////////////////////////
   // enter values
   for(a_it = t_properties.begin(); a_it != t_properties.end(); a_it++)
   {
      wxString a_str = (*a_it).m_name;

      m_gridPropertyPtr->SetCellValue(a_index, 0, (*a_it).m_name);
      
      a_index++;
   }

   m_gridPropertyPtr->Show();
}

void CPropertyPanel::setProperties()
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
            int                                          a_count     = 0;
            int                                          a_index     = 0;
            wxString*                                    a_keys;
            wxGridCellChoiceEditor*                      a_editorPtr = 0;
            std::map<wxString, CTexture*>::iterator   a_mapIt;

            if(0 != (*a_it).m_editorPtr)
            {
               (*a_it).m_editorPtr->Destroy();
            }

            a_count = CDataStorage::getInstance().m_textureMap.size();
            a_keys = new wxString[a_count];

            for(a_mapIt = CDataStorage::getInstance().m_textureMap.begin(); a_mapIt != CDataStorage::getInstance().m_textureMap.end(); a_mapIt++)
            {
               a_keys[a_index] = a_mapIt->first;
               a_index++;
            }

            a_editorPtr = new wxGridCellChoiceEditor(a_count, a_keys);
            m_gridPropertyPtr->SetCellEditor((*a_it).m_rowIndex, 1, a_editorPtr);
         }
         break;
      }

      m_gridPropertyPtr->SetCellValue((*a_it).m_rowIndex, 1, a_str);
   }


}


//////////////////////////////////////////////////////////////////////////
// Events
//////////////////////////////////////////////////////////////////////////
void CPropertyPanel::onGridChange(wxGridEvent& t_event)
{
   std::vector<SPropDesc>::iterator a_it;
   SPropDesc*                       a_descPtr   = 0;
   int                              a_rowIndex  = t_event.GetRow();
   wxString                         a_str       = "";


   // get entry by row index
   for(a_it = m_objectProperties.begin(); a_it != m_objectProperties.end(); a_it++)
   {
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
