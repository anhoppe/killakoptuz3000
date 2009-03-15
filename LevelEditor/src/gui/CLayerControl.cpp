/********************************************************************
	created:	2009/03/04
	created:	4:3:2009   19:56
	filename: 	c:\hoppe\develop\KK3000\LevelEditor\src\CLayerControl.cpp
	file path:	c:\hoppe\develop\KK3000\LevelEditor\src
	file base:	CLayerControl
	file ext:	cpp
	author:		anhoppe
	
	purpose:	
*********************************************************************/
#include "CLayerControl.h"

#include "../data/CDataStorage.h"
#include "../data/CUpdateContainer.h"

//////////////////////////////////////////////////////////////////////////
// Definitions
//////////////////////////////////////////////////////////////////////////
#define ID_LIST_OBJECTS          1000
#define ID_BUTTON_ADD_OBJECT     1001
#define ID_BUTTON_ADD_ENEMY      1002
#define ID_BUTTON_INSERT_LAYER   1003
#define ID_BUTTON_DELETE_LAYER   1004
#define ID_BUTTON_OBJECT_UP      1005
#define ID_BUTTON_OBJECT_DOWN    1006

#define BUTTON_SIZE              wxSize(35, 20)

//////////////////////////////////////////////////////////////////////////
// Event Table
//////////////////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(CLayerControl, wxPanel)
   EVT_TOGGLEBUTTON(ID_BUTTON_ADD_OBJECT, onButtonAddObject)
   EVT_TOGGLEBUTTON(ID_BUTTON_ADD_ENEMY, onButtonAddEnemy)
   EVT_BUTTON(ID_BUTTON_INSERT_LAYER, onButtonInsertLayer)
   EVT_BUTTON(ID_BUTTON_DELETE_LAYER, onButtonDeleteLayer)
   EVT_BUTTON(ID_BUTTON_OBJECT_UP, onButtonObjectUp)
   EVT_BUTTON(ID_BUTTON_OBJECT_DOWN, onButtonObjectDown)

   EVT_LISTBOX(ID_LIST_OBJECTS, onListObjects)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////
CLayerControl::CLayerControl(wxWindow* t_parentPtr)
: wxPanel(t_parentPtr, wxID_ANY, wxDefaultPosition, wxSize(200, 360))
{
   m_listLayersPtr = 0;

   createControls();

   // register as update listener
   CUpdateContainer::getInstance().add((IUpdate*)this);
   CUpdateContainer::getInstance().add((ISetObject*)this);
}

CLayerControl::~CLayerControl()
{

}

void CLayerControl::update()
{
   if(0 != m_listLayersPtr)
   {
      std::vector<std::string>            a_objectList;
      std::vector<std::string>::iterator  a_strIt;
      std::vector<int>::iterator          a_posIt;
      int                                 a_count        = 0;
      char                                a_str[1024];
      int                                 a_index        = 0;

      a_index = m_listLayersPtr->GetSelection();
      m_listLayersPtr->Clear();
      

      // get the object names
      CDataStorage::getInstance().getObjectsAsStrings(a_objectList);

      // put the object names into the list
      for(a_strIt = a_objectList.begin(); a_strIt != a_objectList.end(); a_strIt++)
      {
         m_listLayersPtr->Append((*a_strIt).c_str());
      }

      // insert the layer number
      for(a_posIt = m_layerPositions.begin(); a_posIt != m_layerPositions.end(); a_posIt++)
      {
         sprintf(a_str, "---Layer%d", a_count++);
         m_listLayersPtr->Insert(a_str, (*a_posIt));
      }

      m_listLayersPtr->SetSelection(a_index);

      // assign the layer position
      assignLayerPosition();

   }
}

void CLayerControl::setObject(int t_index)
{
   int a_rowIndex = getObjectRow(t_index);

   m_listLayersPtr->SetSelection(a_rowIndex);
}


//////////////////////////////////////////////////////////////////////////
// Private Methods
//////////////////////////////////////////////////////////////////////////
void CLayerControl::createControls()
{
   wxBoxSizer* a_sizerPtr  = new wxBoxSizer(wxVERTICAL);

   // add the list box to the sizer
   m_listLayersPtr = new wxListBox(this, ID_LIST_OBJECTS);
   a_sizerPtr->Add(m_listLayersPtr, 1, wxEXPAND|wxGROW|wxLEFT|wxTOP|wxRIGHT|wxBOTTOM|wxALIGN_CENTER, 1);

   //////////////////////////////////////////////////////////////////////////
   // create a sizer for the buttons
   wxBoxSizer* a_buttonSizerPtr  = new wxBoxSizer(wxHORIZONTAL);
   wxBitmap*   a_bitmapPtr       = 0;

   // create the add object mode button
   m_buttonAddObjectPtr = new wxToggleButton(this, ID_BUTTON_ADD_OBJECT, "Obj", wxDefaultPosition, BUTTON_SIZE);
   a_buttonSizerPtr->Add(m_buttonAddObjectPtr, 0, wxALIGN_LEFT, 0);

   // create the add enemy mode button
   m_buttonAddEnemyPtr = new wxToggleButton(this, ID_BUTTON_ADD_ENEMY, "Enemy", wxDefaultPosition, BUTTON_SIZE);
   a_buttonSizerPtr->Add(m_buttonAddEnemyPtr, 0, wxALIGN_LEFT, 0);

   // create the add layer button
   a_buttonSizerPtr->Add(new wxButton(this, ID_BUTTON_INSERT_LAYER, "Layer", wxDefaultPosition, BUTTON_SIZE), 0, wxALIGN_LEFT, 0);

   // create the delete button
   a_buttonSizerPtr->Add(new wxButton(this, ID_BUTTON_DELETE_LAYER, "Del", wxDefaultPosition, BUTTON_SIZE), 0, wxALIGN_LEFT, 0);

   a_buttonSizerPtr->AddStretchSpacer(1);


   // create the object up button
   a_buttonSizerPtr->Add(new wxButton(this, ID_BUTTON_OBJECT_UP, "Up", wxDefaultPosition, BUTTON_SIZE), 0, wxALIGN_RIGHT, 0);

   // create the delete button
   a_buttonSizerPtr->Add(new wxButton(this, ID_BUTTON_OBJECT_DOWN, "Down", wxDefaultPosition, BUTTON_SIZE), 0, wxALIGN_RIGHT, 0);

   a_sizerPtr->Add(a_buttonSizerPtr, 0, wxALIGN_BOTTOM);

   SetSizer(a_sizerPtr);
}

void CLayerControl::assignLayerPosition()
{
   unsigned int   a_index        = 0;
   int            a_objectIndex  = 0;

   for(a_index = 0; a_index < m_listLayersPtr->GetCount(); a_index++)
   {
      a_objectIndex = getObjectPosition((int)a_index);

      if(-1 != a_objectIndex)
      {
         assignLayerPosition(a_objectIndex, (int)a_index);
      }
   }
}

void CLayerControl::assignLayerPosition(int t_objectIndex, int t_index)
{
   if(m_layerPositions.size() > 0)
   {
      std::vector<int>::iterator a_it;
      int                        a_layerNumber     = 0;
      bool                       a_numberAssigned  = false;

      for(a_it = m_layerPositions.begin(); a_it != m_layerPositions.end(); a_it++)
      {
         if(t_index < (*a_it))
         {
            CDataStorage::getInstance().assignLayerNumber(t_objectIndex, a_layerNumber);
            a_numberAssigned = true;
            break;
         }
         a_layerNumber++;
      }

      if(!a_numberAssigned)
      {
         CDataStorage::getInstance().assignLayerNumber(t_objectIndex, a_layerNumber);
      }
   }
}

void CLayerControl::insertLayers(std::vector<int>& t_layerPositions)
{
   std::vector<int>::iterator a_it;
   int                        a_count = 0;

   m_layerPositions.clear();
   for(a_it = t_layerPositions.begin(); a_it != t_layerPositions.end(); a_it++)
   {
      m_layerPositions.push_back((*a_it)+a_count++);
   }
}

bool CLayerControl::isObject(int t_index)
{
   std::vector<int>::iterator a_it;
   bool                       r_isObject  = true;


   for(a_it = m_layerPositions.begin(); a_it < m_layerPositions.end(); a_it++)
   {
      if((*a_it) == t_index)
      {
         r_isObject = false;
         break;
      }
   }
   
   return r_isObject;
}

int CLayerControl::getObjectPosition(int t_index)
{
   int   r_position  = -1;


   if(isObject(t_index))
   {
      int   a_index  = 0;

      for(a_index = 0; a_index <= t_index; a_index++)
      {
         if(isObject(a_index))
         {
            r_position++;
         }
      }
   }

   return r_position;
}

int CLayerControl::getObjectRow(int t_index)
{
   int   r_result = 0;
   int   a_count  = 0;
   int   a_index  = 0;


   if(t_index < CDataStorage::getInstance().getNumObjects())
   {
      for(a_index = 0; a_index < (int)m_listLayersPtr->GetCount(); a_index++)
      {
         if(isObject(a_index))
         {
            if(a_count == t_index)
            {
               r_result = a_index;
               break;
            }
            a_count++;
         }
      }
   }

   return r_result;
}

int CLayerControl::getLayerIndex(int t_rowIndex)
{
   std::vector<int>::iterator a_it;
   int                        a_count  = 0;

   int                        r_index  = -1;

   for(a_it = m_layerPositions.begin(); a_it != m_layerPositions.end(); a_it++)
   {
      if((*a_it) == t_rowIndex)
      {
         r_index = a_count;
         break;
      }
      a_count++;
   }

   return r_index;
}

bool CLayerControl::canMoveLayerUp(int t_index)
{
   int   a_index = 0;
   bool  r_result = false;

   if(t_index > 0)
   {
      if(m_layerPositions[t_index] > m_layerPositions[t_index-1]+1)
      {
         r_result = true;
      }
   }
   else
   {
      if(m_layerPositions[t_index] > 0)
      {
         r_result = true;
      }
   }

   return r_result;
}

bool CLayerControl::canMoveLayerDown(int t_index)
{
   size_t   a_index  = 0;
   bool     r_result = false;


   if(t_index < (int)m_layerPositions.size()-1)
   {
      if(m_layerPositions[t_index] < m_layerPositions[t_index+1]-1)
      {
         r_result = true;
      }
   }
   else
   {
      if(m_layerPositions[t_index] < (int)m_listLayersPtr->GetCount()-1)
      {
         r_result = true;
      }
   }

   return r_result;
}

void CLayerControl::deleteLayerByIndex(int t_index)
{
   if(t_index >= 0 && t_index < (int)m_layerPositions.size())
   {
      std::vector<int>::iterator a_it;
      int                        a_count  = 0;

      for(a_it = m_layerPositions.begin(); a_it != m_layerPositions.end(); a_it++)
      {
         if(a_count == t_index)
         {
            m_layerPositions.erase(a_it);
            break;
         }
         a_count++;
      }
      
   }
}

//////////////////////////////////////////////////////////////////////////
// Events
//////////////////////////////////////////////////////////////////////////
void CLayerControl::onButtonAddObject(wxCommandEvent& t_event)
{
   bool a_state = m_buttonAddObjectPtr->GetValue();

   CDataStorage::getInstance().setAddObjectMode(a_state);
   if(a_state)
   {
      m_buttonAddEnemyPtr->SetValue(false);
   }
}

void CLayerControl::onButtonAddEnemy(wxCommandEvent& t_event)
{
   bool a_state = m_buttonAddEnemyPtr->GetValue();

   CDataStorage::getInstance().setAddEnemyMode(a_state);
   if(a_state)
   {
      m_buttonAddObjectPtr->SetValue(false);
   }
}

void CLayerControl::onButtonInsertLayer(wxCommandEvent& t_event)
{
   m_layerPositions.push_back(m_listLayersPtr->GetCount());
   update();
}

void CLayerControl::onButtonDeleteLayer(wxCommandEvent& t_event)
{
   int a_index = m_listLayersPtr->GetSelection();

   if(wxNOT_FOUND != a_index)
   {
      if(isObject(a_index))
      {
         CDataStorage::getInstance().deleteObjectByIndex(getObjectPosition(a_index));
      }
      else
      {
         deleteLayerByIndex(getLayerIndex(a_index));
      }
      CUpdateContainer::getInstance().update();
   }
}

void CLayerControl::onButtonObjectUp(wxCommandEvent& t_event)
{
   int   a_sel = m_listLayersPtr->GetSelection();

   if(a_sel != -1)
   {
      bool  a_moved           = false;
      int   a_objectPosition  = getObjectPosition(a_sel);
      
      // move either object ...
      if(-1 != a_objectPosition)
      {
         a_moved = CDataStorage::getInstance().moveObject(a_objectPosition, a_objectPosition-1);
         CUpdateContainer::getInstance().update();
      }
      // ... or layer
      else
      {
         int a_indexPosition = getLayerIndex(a_sel);

         if(canMoveLayerUp(a_indexPosition))
         {
            m_layerPositions[a_indexPosition] -= 1;
            CUpdateContainer::getInstance().update();
            a_moved = true;
         }
      }

      if(a_moved)
      {
         m_listLayersPtr->SetSelection(a_sel-1);
      }
   }
}

void CLayerControl::onButtonObjectDown(wxCommandEvent& t_event)
{
   int a_sel = m_listLayersPtr->GetSelection();

   if(a_sel != -1)
   {
      bool  a_moved           = false;
      int   a_objectPosition  = getObjectPosition(a_sel);

      if(-1 != a_objectPosition)
      {
         a_moved = CDataStorage::getInstance().moveObject(a_objectPosition, a_objectPosition+1);
         CUpdateContainer::getInstance().update();
      }
      // ... or layer
      else
      {
         int a_indexPosition = getLayerIndex(a_sel);

         if(canMoveLayerDown(a_indexPosition))
         {
            m_layerPositions[a_indexPosition] += 1;
            CUpdateContainer::getInstance().update();
            a_moved = true;
         }
      }

      if(a_moved)
      {
         m_listLayersPtr->SetSelection(a_sel+1);
      }
   }
}

void CLayerControl::onListObjects(wxCommandEvent& t_event)
{
   int a_index = m_listLayersPtr->GetSelection();

   if(a_index != -1)
   {
      a_index = getObjectPosition(a_index);
      if(-1 != a_index)
      {
         CUpdateContainer::getInstance().setObject(a_index);
         CUpdateContainer::getInstance().update();
      }
   }
}
