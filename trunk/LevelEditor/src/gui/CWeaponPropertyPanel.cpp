/********************************************************************
	created:	2009/03/06
	created:	6:3:2009   23:56
	filename: 	c:\hoppe\develop\KK3000\LevelEditor\src\CWeaponPropertyPanel.cpp
	file path:	c:\hoppe\develop\KK3000\LevelEditor\src
	file base:	CWeaponPropertyPanel
	file ext:	cpp
	author:		anhoppe
	
	purpose:	
*********************************************************************/

#include "CWeaponPropertyPanel.h"

#include "../data/CUpdateContainer.h"
#include "../data/CDataStorage.h"

#include "../KillaCoptuz3000/src/Objects/CEnemy.h"

//////////////////////////////////////////////////////////////////////////
// Definitions
//////////////////////////////////////////////////////////////////////////
#define ID_LIST_WEAPONS             1000
#define ID_BUTTON_ADD_WEAPON        1001
#define ID_BUTTON_DEL_WEAPON        1002
#define ID_GRID_WEAPON_PROPERTIES   1003

#define BUTTON_SIZE                 wxSize(50, 20)

//////////////////////////////////////////////////////////////////////////
// Event table
//////////////////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(CWeaponPropertyPanel, wxPanel)
   EVT_LISTBOX(ID_LIST_WEAPONS, onListWeapons)
   EVT_BUTTON(ID_BUTTON_ADD_WEAPON, onButtonAddWeapon)
   EVT_BUTTON(ID_BUTTON_DEL_WEAPON, onButtonDelWeapon)
   EVT_GRID_CMD_CELL_CHANGE(ID_GRID_WEAPON_PROPERTIES, onGridChange)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////
CWeaponPropertyPanel::CWeaponPropertyPanel(wxWindow* t_parentPtr)
: CBasePropertyPanel(t_parentPtr)
{
    createControls();

    CUpdateContainer::getInstance().add(this);
}

CWeaponPropertyPanel::~CWeaponPropertyPanel()
{

}

void CWeaponPropertyPanel::update()
{
   CObject* a_objectPtr = CDataStorage::getInstance().getActiveObject();

   m_listWeaponsPtr->Clear();

   if(0 != a_objectPtr)
   {
      if(a_objectPtr->getType() == e_enemy)
      {
         CEnemy*                          a_enemyPtr  = static_cast<CEnemy*>(a_objectPtr);
         std::vector<CWeapon*>::iterator  a_it;
         int                              a_count     = 0;
         char                             a_str[1024];

         for(a_it = a_enemyPtr->m_weaponList.begin(); a_it != a_enemyPtr->m_weaponList.end(); a_it++)
         {
            sprintf_s(a_str, "weapon%d", a_count++);
            m_listWeaponsPtr->Append(a_str);
         }
      }
   }
}

//////////////////////////////////////////////////////////////////////////
// Private Methods
//////////////////////////////////////////////////////////////////////////
void CWeaponPropertyPanel::createControls()
{
   wxBoxSizer* a_sizerPtr     = new wxBoxSizer(wxVERTICAL);
   wxBoxSizer* a_subSizerPtr  = 0;

   //////////////////////////////////////////////////////////////////////////
   // add the list with all weapons + add and delete button
   m_listWeaponsPtr = new wxListBox(this, ID_LIST_WEAPONS);
   a_sizerPtr->Add(m_listWeaponsPtr, 1, wxEXPAND);

   // create a sub sizer for add and delete buttons
   a_subSizerPtr = new wxBoxSizer(wxHORIZONTAL);
   a_subSizerPtr->Add(new wxButton(this, ID_BUTTON_ADD_WEAPON, "Add", wxDefaultPosition, BUTTON_SIZE), 0, wxALIGN_LEFT);
   a_subSizerPtr->Add(new wxButton(this, ID_BUTTON_DEL_WEAPON, "Del", wxDefaultPosition, BUTTON_SIZE), 0, wxALIGN_LEFT);
   a_sizerPtr->Add(a_subSizerPtr);

   //////////////////////////////////////////////////////////////////////////
   // Add the grid for the properties of the selected weapon
   m_gridPropertiesPtr = new wxGrid(this, ID_GRID_WEAPON_PROPERTIES);
   m_gridPropertiesPtr->CreateGrid(0, 2);
   m_gridPropertiesPtr->SetRowLabelSize(0);
   m_gridPropertiesPtr->SetColLabelSize(25);
   m_gridPropertiesPtr->SetColLabelValue(0, "Key");
   m_gridPropertiesPtr->SetColLabelValue(1, "Value");
   
   a_sizerPtr->Add(m_gridPropertiesPtr, 4, wxEXPAND|wxTOP, 2);

   //////////////////////////////////////////////////////////////////////////
   // Texture configuration placed in a flex grid sizer
   wxFlexGridSizer*  a_gridSizer = new wxFlexGridSizer(2, 3);
   a_gridSizer->AddGrowableCol(1);

   // add the combo box for weapon texture selection
   a_gridSizer->Add(new wxStaticText(this, wxID_ANY, "Weapon Gfx:"));
   m_comboWeaponTexturePtr = new wxComboBox(this, wxID_ANY);
   a_gridSizer->Add(m_comboWeaponTexturePtr, 1, wxEXPAND);

   // add the combo box for the shot texture
   a_gridSizer->Add(new wxStaticText(this, wxID_ANY, "Shot Gfx:"));
   m_comboShotTexturePtr = new wxComboBox(this, wxID_ANY);
   a_gridSizer->Add(m_comboShotTexturePtr, 1, wxEXPAND);

   // add the combo box for shot explosion texture
   a_gridSizer->Add(new wxStaticText(this, wxID_ANY, "Shot Exp Gfx:"));
   m_comboShotExplosionTexturePtr = new wxComboBox(this, wxID_ANY);
   a_gridSizer->Add(m_comboShotExplosionTexturePtr, 1, wxEXPAND);

   a_sizerPtr->Add(a_gridSizer, 0, wxGROW);
   
   SetSizer(a_sizerPtr);
}

CWeapon* CWeaponPropertyPanel::getActiveWeapon()
{
   CObject* a_objectPtr = CDataStorage::getInstance().getActiveObject();
   CWeapon* r_weaponPtr = 0;


   if(0 != a_objectPtr && e_enemy == a_objectPtr->getType())
   {
      CEnemy*  a_enemyPtr  = static_cast<CEnemy*>(a_objectPtr);
      int      a_index     = m_listWeaponsPtr->GetSelection();

      if(wxNOT_FOUND != a_index)
      {
         if(a_index < (int)a_enemyPtr->m_weaponList.size())
         {
            r_weaponPtr = a_enemyPtr->m_weaponList[a_index];
         }
      }
   }

   return r_weaponPtr;
}

void CWeaponPropertyPanel::initProperties()
{
   SPropDesc   a_desc;
   CWeapon*    a_weaponPtr = 0;

   m_properties.clear();

   a_weaponPtr = getActiveWeapon();

   if(0 != a_weaponPtr)
   {
      a_desc.m_name     = "X-Pos";
      a_desc.m_type     = e_float;
      a_desc.m_floatPtr = &a_weaponPtr->m_xPos;
      m_properties.push_back(a_desc);

      a_desc.m_name     = "Y-Pos";
      a_desc.m_type     = e_float;
      a_desc.m_floatPtr = &a_weaponPtr->m_yPos;
      m_properties.push_back(a_desc);

      a_desc.m_name     = "Width";
      a_desc.m_type     = e_float;
      a_desc.m_floatPtr = &a_weaponPtr->m_width;
      m_properties.push_back(a_desc);

      a_desc.m_name     = "Height";
      a_desc.m_type     = e_float;
      a_desc.m_floatPtr = &a_weaponPtr->m_height;
      m_properties.push_back(a_desc);

      a_desc.m_name     = "Min Angle";
      a_desc.m_type     = e_float;
      a_desc.m_floatPtr = &a_weaponPtr->m_minAngle;
      m_properties.push_back(a_desc);

      a_desc.m_name     = "Max Angle";
      a_desc.m_type     = e_float;
      a_desc.m_floatPtr = &a_weaponPtr->m_maxAngle;
      m_properties.push_back(a_desc);

      a_desc.m_name     = "Cycle Interval";
      a_desc.m_type     = e_int;
      a_desc.m_intPtr   = &a_weaponPtr->m_cycleInterval;
      m_properties.push_back(a_desc);

      a_desc.m_name     = "Max Shots";
      a_desc.m_type     = e_uint;
      a_desc.m_uintPtr  = &a_weaponPtr->m_maxShots;
      m_properties.push_back(a_desc);

      a_desc.m_name     = "Frames per shot";
      a_desc.m_type     = e_uint;
      a_desc.m_uintPtr  = &a_weaponPtr->m_framesPerShot;
      m_properties.push_back(a_desc);

      a_desc.m_name     = "Shot Radius";
      a_desc.m_type     = e_float;
      a_desc.m_floatPtr = &a_weaponPtr->m_shotRadius;
      m_properties.push_back(a_desc);

      a_desc.m_name     = "Shot X-Pos";
      a_desc.m_type     = e_float;
      a_desc.m_floatPtr = &a_weaponPtr->m_shotX;
      m_properties.push_back(a_desc);

      a_desc.m_name     = "Shot Y-Pos";
      a_desc.m_type     = e_float;
      a_desc.m_floatPtr = &a_weaponPtr->m_shotY;
      m_properties.push_back(a_desc);

      a_desc.m_name     = "Shot Width";
      a_desc.m_type     = e_float;
      a_desc.m_floatPtr = &a_weaponPtr->m_shotWidth;
      m_properties.push_back(a_desc);

      a_desc.m_name     = "Shot Height";
      a_desc.m_type     = e_float;
      a_desc.m_floatPtr = &a_weaponPtr->m_shotHeight;
      m_properties.push_back(a_desc);

      a_desc.m_name     = "Shot Cycle Int.";
      a_desc.m_type     = e_int;
      a_desc.m_intPtr   = &a_weaponPtr->m_shotCycleInterval;
      m_properties.push_back(a_desc);

      a_desc.m_name     = "Shot Velocity";
      a_desc.m_type     = e_float;
      a_desc.m_floatPtr = &a_weaponPtr->m_shotVelocity;
      m_properties.push_back(a_desc);

      a_desc.m_name     = "Shot Hit Points";
      a_desc.m_type     = e_int;
      a_desc.m_intPtr   = &a_weaponPtr->m_shotHitPoints;
      m_properties.push_back(a_desc);
   }
}


//////////////////////////////////////////////////////////////////////////
// Events
//////////////////////////////////////////////////////////////////////////
void CWeaponPropertyPanel::onListWeapons(wxCommandEvent& t_event)
{
   ////////////////////////////////////////////////////////////////////////
   // initialize weapon values
   initProperties();
   fillPropertyGrid();
}

void CWeaponPropertyPanel::onButtonAddWeapon(wxCommandEvent& t_event)
{
   CObject* a_objectPtr = CDataStorage::getInstance().getActiveObject();

   if(0 != a_objectPtr)
   {
      CEnemy*  a_enemyPtr = static_cast<CEnemy*>(a_objectPtr);

      // the enemy receives a weapon object
      a_enemyPtr->m_weaponList.push_back(new CWeapon());

      CUpdateContainer::getInstance().update();
   }
}

void CWeaponPropertyPanel::onButtonDelWeapon(wxCommandEvent& t_event)
{         
}


