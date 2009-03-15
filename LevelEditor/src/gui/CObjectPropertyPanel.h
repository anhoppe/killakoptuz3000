/********************************************************************
	created:	2009/03/06
	created:	6:3:2009   23:22
	filename: 	c:\hoppe\develop\KK3000\LevelEditor\src\CObjectPropertyPanel.h
	file path:	c:\hoppe\develop\KK3000\LevelEditor\src
	file base:	CObjectPropertyPanel
	file ext:	h
	author:		anhoppe
	
	purpose:	Property panel for CObject parameters
*********************************************************************/
#ifndef COBJECT_PROPERTY_PANEL_H
#define COBJECT_PROPERTY_PANEL_H

#include "..//KillaCoptuz3000/src/Objects/CObject.h"

#include <wx/wx.h>
#include <wx/grid.h>

#include "../data/IUpdate.h"
#include "../data/ISetObject.h"

#include "CBasePropertyPanel.h"


//////////////////////////////////////////////////////////////////////////
// Class Declaration
//////////////////////////////////////////////////////////////////////////
class CObjectPropertyPanel : public CBasePropertyPanel, 
   IUpdate, 
   ISetObject
{
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
      e_damagePoints    = 10,
   };

   //////////////////////////////////////////////////////////////////////////
   // Methods
   //////////////////////////////////////////////////////////////////////////
public:
   CObjectPropertyPanel(wxWindow* t_parentPtr);
   ~CObjectPropertyPanel();

   virtual void update();
   virtual void setObject(int t_index);
   

private:

   /**
   *  !! Caution !! If ever attributes of an CObject changes in KK3000,
   *     T-H-I-S    function must be altered
   */
   virtual void initProperties();

   //////////////////////////////////////////////////////////////////////////
   // Events
   //////////////////////////////////////////////////////////////////////////
   DECLARE_EVENT_TABLE()
   void onButtonAddTexture(wxCommandEvent& t_event);
   void onButtonDelTexture(wxCommandEvent& t_event);

   //////////////////////////////////////////////////////////////////////////
   // Variables
   //////////////////////////////////////////////////////////////////////////
public:
private:
   /** Index of currently selected object in CDataStorage */
   int                                    m_selectedObjectIndex;

   /** Start index of object properties */
   int                                    m_startIndexObject;

   /** List Box for all added textures (selection depended) */
   wxListBox*                             m_listTextureKeys;

   /** Combo Box for all textures (NOT selection depended) */
   wxComboBox*                            m_comboTextureKeysPtr;
};

#endif
