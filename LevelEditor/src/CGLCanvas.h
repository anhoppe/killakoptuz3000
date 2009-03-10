// ***************************************************************
//  CGLCanvas   version:  1.0   ·  date: 06/18/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef C_GL_CANVAS_H
#define C_GL_CANVAS_H

#include "wx/wx.h"
#include "wx/glcanvas.h"

#include "IUpdate.h"

class CGLCanvas : public wxGLCanvas,
                         IUpdate
{
public:
   //////////////////////////////////////////////////////////////////////////
   // Methods
   //////////////////////////////////////////////////////////////////////////
   CGLCanvas(wxWindow* t_parentPtr);
   ~CGLCanvas();

   /** Updates GL context */
   virtual void update();

   //////////////////////////////////////////////////////////////////////////
   // Private Methods
   //////////////////////////////////////////////////////////////////////////
private:
   void getCoordinates(double& t_wx, double& t_wy, int t_x, int t_y);

   //////////////////////////////////////////////////////////////////////////
   // Events
   //////////////////////////////////////////////////////////////////////////
private:
   DECLARE_EVENT_TABLE()

   void onLButtonDown(wxMouseEvent& t_event);
   void onLButtonUp(wxMouseEvent& t_event);
   void onMouseMove(wxMouseEvent& t_event);

   void onSize(wxSizeEvent& t_event);

   //////////////////////////////////////////////////////////////////////////
   // Variables
   //////////////////////////////////////////////////////////////////////////
   
   /** Indicates if the left button is down */
   bool m_lButtonDown;

};

#endif