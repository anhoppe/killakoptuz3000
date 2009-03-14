/********************************************************************
	created:	2009/03/04
	created:	4:3:2009   19:44
	filename: 	c:\hoppe\develop\KK3000\LevelEditor\src\CLayerControl.h
	file path:	c:\hoppe\develop\KK3000\LevelEditor\src
	file base:	CLayerControl
	file ext:	h
	author:		anhoppe
	
	purpose:	GUI panel for object layer control
*********************************************************************/
#ifndef CLAYER_CONTROL_H
#define CLAYER_CONTROL_H

#include "wx/wx.h"
#include "wx/tglbtn.h"

#include "../data/IUpdate.h"
#include "../data/ISetObject.h"

#include <vector>

class CLayerControl : public wxPanel, IUpdate, ISetObject
{
   //////////////////////////////////////////////////////////////////////////
   // Public Methods
   //////////////////////////////////////////////////////////////////////////
public:
   CLayerControl(wxWindow* t_parentPtr);
   ~CLayerControl();

   virtual void update();
   virtual void setObject(int t_index);
   //////////////////////////////////////////////////////////////////////////
   // Private Methods
   //////////////////////////////////////////////////////////////////////////
   void createControls();

   /** assigns layer numbers to all objects before saving */
   void assignLayerPosition();

   /** assigns layer number to a single object, used by the other assignLayerPosition method */
   void assignLayerPosition(int t_objectIndex, int t_index);

   /** insert layers after loading */
   void insertLayers(std::vector<int>& t_layerPositions);

   /** Checks if at a given index in the list box is an object (return true) or a layer separator (false) */
   bool isObject(int t_index);

   /** 
   *  Retrieves the index in the object list of an item, 
   *  -1 if at t_index is no object but a layer separator 
   *  (list row position to object index)
   */
   int getObjectPosition(int t_index);

   /**
   *  Retrieves the index of the row of an object
   *  (object index to list row index)
   */
   int getObjectRow(int t_index);

   /** Retrieves index in layer position vector of selected row index, -1 if at row index is no layer */
   int getLayerIndex(int t_rowIndex);

   /** Checks if the layer with the given index in the layer position vector can be moved up */
   bool canMoveLayerUp(int t_index);

   /** Checks if the layer with the given index in the layer position vector can be moved down */
   bool canMoveLayerDown(int t_index);

   /** deletes layer at given index */
   void deleteLayerByIndex(int t_index);

   //////////////////////////////////////////////////////////////////////////
   // Events
   //////////////////////////////////////////////////////////////////////////
   DECLARE_EVENT_TABLE()
   void onButtonAddObject(wxCommandEvent& t_event);
   void onButtonInsertLayer(wxCommandEvent& t_event);
   void onButtonDeleteLayer(wxCommandEvent& t_event);
   void onButtonObjectUp(wxCommandEvent& t_event);
   void onButtonObjectDown(wxCommandEvent& t_event);
   void onListObjects(wxCommandEvent& t_event);

   //////////////////////////////////////////////////////////////////////////
   // Private Variables
   //////////////////////////////////////////////////////////////////////////
private:
   wxListBox*        m_listLayersPtr;

   wxToggleButton*   m_buttonAddObjectPtr;

   /** Vector with layer positions in the object list */
   std::vector<int>  m_layerPositions;
};
#endif
