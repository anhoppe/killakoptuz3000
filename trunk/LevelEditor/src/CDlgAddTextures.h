// ****************  ***********************************************
//  CDlgAddTextures   version:  1.0   ·  date: 06/28/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef CDLGADDTEXTURES_H
#define CDLGADDTEXTURES_H

#include <wx/wx.h>

struct STextureDesc
{
   /** Base file */
   wxString m_baseFileName;

   /** File extension */
   wxString m_gfxType;

   /** Number of points in collision polygon */
   int      m_hullPoints;
};

class CDlgAddTextures : public wxDialog
{
   /////////////////////////////////////////////////////////////////////////
   // Methods
   //////////////////////////////////////////////////////////////////////////
public:
   CDlgAddTextures(wxWindow* t_parentPtr);
   ~CDlgAddTextures();

private:
   /** Updates the texture list. Content is read from m_textures */
   void update();

   /** stores already loaded textures into m_textures */
   void getTexturesFromDataStorage();

   //////////////////////////////////////////////////////////////////////////
   // Events
   //////////////////////////////////////////////////////////////////////////
private:
   DECLARE_EVENT_TABLE()

   void onButtonAdd(wxCommandEvent& t_event);
   void onButtonRemove(wxCommandEvent& t_event);
   void onButtonOk(wxCommandEvent& t_event);
   void onButtonCancel(wxCommandEvent& t_event);

   //////////////////////////////////////////////////////////////////////////
   // Variables
   //////////////////////////////////////////////////////////////////////////
public:
   /** Contains descriptors for all textures */
   std::vector<STextureDesc>  m_textures;

private:
   wxListBox*                 m_listTexturePtr;
   
};

#endif
