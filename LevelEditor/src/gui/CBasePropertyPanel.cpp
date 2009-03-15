/********************************************************************
	created:	2009/03/15
	created:	15:3:2009   21:52
	filename: 	c:\hoppe\develop\KK3000\LevelEditor\src\gui\CBasePropertyPanel.cpp
	file path:	c:\hoppe\develop\KK3000\LevelEditor\src\gui
	file base:	CBasePropertyPanel
	file ext:	cpp
	author:		anhoppe
	
	purpose:	
*********************************************************************/

#include "CBasePropertyPanel.h"

#include <map>

#include "../KillaCoptuz3000/src/CTexture.h"

#include "../data/CDataStorage.h"
#include "../data/CUpdateContainer.h"

//////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////
CBasePropertyPanel::CBasePropertyPanel(wxWindow* t_parentPtr)
: wxPanel(t_parentPtr, wxID_ANY)
{

}

CBasePropertyPanel::~CBasePropertyPanel()
{

}

//////////////////////////////////////////////////////////////////////////
// Protected Methods
//////////////////////////////////////////////////////////////////////////
void CBasePropertyPanel::fillPropertyGrid()
{
   if(0 != m_gridPropertiesPtr)
   {
      std::vector<SPropDesc>::iterator a_it;
      int                              a_index  = 0;
      wxString                         a_str    = "";

      m_gridPropertiesPtr->ClearGrid();

      if(m_gridPropertiesPtr->GetNumberRows() > 0)
      {
         m_gridPropertiesPtr->DeleteRows(0, m_gridPropertiesPtr->GetNumberRows());
      }

      //////////////////////////////////////////////////////////////////////////
      // Append rows
      m_gridPropertiesPtr->AppendRows((int)m_properties.size());

      //////////////////////////////////////////////////////////////////////////
      // enter property names
      a_index  = 0;
      for(a_it = m_properties.begin(); a_it != m_properties.end(); a_it++)
      {
         //////////////////////////////////////////////////////////////////////////
         // set property name (first column)
         a_str = (*a_it).m_name;
         m_gridPropertiesPtr->SetCellValue(a_index, 0, (*a_it).m_name);

         //////////////////////////////////////////////////////////////////////////
         // set property value (second column)
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
         case e_uint:
            a_str.sprintf("%d", *((*a_it).m_uintPtr));
            break;
         case e_string:
            a_str = (*a_it).m_stringPtr->c_str();
            break;
         }

         m_gridPropertiesPtr->SetCellValue(a_index, 1, a_str);

         //////////////////////////////////////////////////////////////////////////
         // increment row index
         a_index++;
      }
   }
}

void CBasePropertyPanel::fillComboTexture(wxComboBox* t_comboBoxPtr)
{
   if(0 != t_comboBoxPtr)
   {
      size_t                                    a_count     = 0;
      size_t                                    a_index     = 0;
      wxString                                  a_keys;
      std::map<wxString, CTexture*>::iterator   a_mapIt;


      a_count = CDataStorage::getInstance().m_textureMap.size();
      a_index = 0;

      t_comboBoxPtr->Clear();
      for(a_mapIt = CDataStorage::getInstance().m_textureMap.begin(); a_mapIt != CDataStorage::getInstance().m_textureMap.end(); a_mapIt++)
      {
         t_comboBoxPtr->Append(a_mapIt->first);
      }
   }
}

//////////////////////////////////////////////////////////////////////////
// Event Methods
//////////////////////////////////////////////////////////////////////////
void CBasePropertyPanel::onGridChange(wxGridEvent& t_event)
{
   SPropDesc*                       a_descPtr   = 0;
   int                              a_rowIndex  = t_event.GetRow();
   wxString                         a_str       = "";


   if(wxNOT_FOUND != a_rowIndex)
   {
      if(a_rowIndex < (int)m_properties.size())
      {
         a_descPtr = &m_properties[a_rowIndex];

         if(0 != a_descPtr)
         {
            a_str = m_gridPropertiesPtr->GetCellValue(a_rowIndex, 1);

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
            case e_uint:
               *a_descPtr->m_uintPtr = atoi(a_str.c_str());
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
   }
}
