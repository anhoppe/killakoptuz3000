/********************************************************************
	created:	2009/03/06
	created:	6:3:2009   23:52
	filename: 	c:\hoppe\develop\KK3000\LevelEditor\src\CWeaponPropertyPanel.h
	file path:	c:\hoppe\develop\KK3000\LevelEditor\src
	file base:	CWeaponPropertyPanel
	file ext:	h
	author:		anhoppe
	
	purpose:	Property panel for weapons (only available for enemy objects)
*********************************************************************/
#ifndef CWEAPON_PROPERTY_PANEL_H
#define CWEAPON_PROPERTY_PANEL_H

#include "wx/wx.h"
#include "wx/grid.h"

#include "CBasePropertyPanel.h"

#include "../data/IUpdate.h"

#include "../KillaCoptuz3000/src/Objects/CWeapon.h"


class CWeaponPropertyPanel : public CBasePropertyPanel, IUpdate
{
   //////////////////////////////////////////////////////////////////////////
   // Enumeration for weapon properties
   //////////////////////////////////////////////////////////////////////////
   enum EWeaponProperties
   {
      e_xPos               = 0,
      e_yPos               = 1,
      e_width              = 2,
      e_height             = 3,
      e_minAngle           = 4,
      e_maxAngle           = 5,
      e_cycleInterval      = 6,
      e_maxShots           = 7,
      e_framesPerShot      = 8,
      e_shotRadius         = 9,
      e_shotX              = 10,
      e_shotY              = 11,
      e_shotWidth          = 12,
      e_shotHeight         = 13,
      e_shotCycleInterval  = 14,
      e_shotVelocity       = 15,
      e_shotHitPoints      = 16
   };

   //////////////////////////////////////////////////////////////////////////
   // Public Methods
   //////////////////////////////////////////////////////////////////////////
public:
   CWeaponPropertyPanel(wxWindow* t_parentPtr);
   ~CWeaponPropertyPanel();

   virtual void update();

   //////////////////////////////////////////////////////////////////////////
   // Private Methods
   //////////////////////////////////////////////////////////////////////////
private:
   void createControls();

   /** Retrieves pointer to the currently selected weapon */
   CWeapon* getActiveWeapon();

   /** Initializes the property grid with the properties of the selected weapon */
   virtual void initProperties();

   //////////////////////////////////////////////////////////////////////////
   // Events
   //////////////////////////////////////////////////////////////////////////
private:
   DECLARE_EVENT_TABLE()
   
   void onListWeapons(wxCommandEvent& t_event);
   void onButtonAddWeapon(wxCommandEvent& t_event);
   void onButtonDelWeapon(wxCommandEvent& t_event);

   //////////////////////////////////////////////////////////////////////////
   // Private Variables
   //////////////////////////////////////////////////////////////////////////
private:
   /** All weapons of an enemy are listed in this box */
   wxListBox*              m_listWeaponsPtr;

   /** The name of the texture of the weapon */
   wxComboBox*             m_comboWeaponTexturePtr;

   /** The texture of the selected shot */
   wxComboBox*             m_comboShotTexturePtr;

   /** The explosion texture of the selected shot */
   wxComboBox*             m_comboShotExplosionTexturePtr;
};

#endif
