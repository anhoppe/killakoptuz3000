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

class CWeaponPropertyPanel : public wxPanel
{
   //////////////////////////////////////////////////////////////////////////
   // Public Methods
   //////////////////////////////////////////////////////////////////////////
public:
   CWeaponPropertyPanel(wxWindow* t_parentPtr);
   ~CWeaponPropertyPanel();
};

#endif
