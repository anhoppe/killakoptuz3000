// ***************************************************************
//  CGLCanvas   version:  1.0   ·  date: 06/18/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "CGLCanvas.h"

#include "CDataStorage.h"
#include "CObjectControl.h"

#include "CUpdateContainer.h"

#include "glut/glut.h"

//////////////////////////////////////////////////////////////////////////
// Static variables
//////////////////////////////////////////////////////////////////////////
int g_glinit[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, 0};

//////////////////////////////////////////////////////////////////////////
// Event table
//////////////////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(CGLCanvas, wxGLCanvas)
   EVT_LEFT_DOWN(onLButtonDown)
   EVT_LEFT_UP(onLButtonUp)
   EVT_MOTION(onMouseMove)
   EVT_SIZE(onSize)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////
CGLCanvas::CGLCanvas(wxWindow* t_parentPtr)
: wxGLCanvas(t_parentPtr, ((wxWindowID)wxID_ANY), g_glinit)
{
   CUpdateContainer::getInstance().add(this);

   m_lButtonDown = false;
}

CGLCanvas::~CGLCanvas()
{
   CUpdateContainer::getInstance().remove(this);
}

void CGLCanvas::update()
{
   /*m_glCanvasPtr->*//*SetCurrent();*/

   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   // trigger draw method in data storage
   CDataStorage::getInstance().draw();

   SwapBuffers();
}

//////////////////////////////////////////////////////////////////////////
// Private Methods
//////////////////////////////////////////////////////////////////////////
void CGLCanvas::getCoordinates(double& t_wx, double& t_wy, int t_x, int t_y)
{
   int   a_width  = 0;
   int   a_height = 0;

   this->GetSize(&a_width, &a_height);

   t_wx = (((double)t_x / (double)a_width) * 2.0) - 1.;
   t_wy = (((double)(a_height - t_y) / (double)a_height) * 2.0) - 1.;

   t_wx -= CDataStorage::getInstance().m_xPos;
   t_wy -= CDataStorage::getInstance().m_yPos;
}

//////////////////////////////////////////////////////////////////////////
// Events
//////////////////////////////////////////////////////////////////////////
void CGLCanvas::onLButtonDown(wxMouseEvent& t_event)
{
   if(!CDataStorage::getInstance().isAddObjectModeEnabled())
   {
      double   a_glX    = 0.;
      double   a_glY    = 0.;

      getCoordinates(a_glX, a_glY, t_event.m_x, t_event.m_y);

      // first we must find out if the user uses sizers...
      if(!CObjectControl::getInstance().selectSizer(a_glX, a_glY))
      {
         // if that is not the case, we have to check if another object was selected
         if(!CDataStorage::getInstance().selectObjectByCoord(a_glX, a_glY))
         {
            // and if that is not the case either, the selected object might be translated
            CObjectControl::getInstance().moveControl(a_glX, a_glY);
         }
      }

      m_lButtonDown = true;
   }
}

void CGLCanvas::onLButtonUp(wxMouseEvent& t_event)
{
   m_lButtonDown = false;

   if(!CDataStorage::getInstance().isAddObjectModeEnabled())
   {
      CObjectControl::getInstance().releaseControl();
   }

   // add a new object
   else
   {
      double a_x  = 0.;
      double a_y  = 0.;

      getCoordinates(a_x, a_y, t_event.m_x, t_event.m_y);

      CDataStorage::getInstance().add(OBJ_OBJECT, a_x, a_y);
      CUpdateContainer::getInstance().update();

   }
}

void CGLCanvas::onMouseMove(wxMouseEvent& t_event)
{
   if(m_lButtonDown)
   {
      double   a_glX    = 0.;
      double   a_glY    = 0.;

      getCoordinates(a_glX, a_glY, t_event.m_x, t_event.m_y);

      CObjectControl::getInstance().processMouseMove(a_glX, a_glY);

      update();
   }
}

void CGLCanvas::onSize(wxSizeEvent& t_event)
{
   if(IsShown())
   {
      wxGLContext* a_contextPtr = new wxGLContext(this);
      a_contextPtr->SetCurrent(*this);
      SetCurrent(*a_contextPtr);
   }
//    int      w        = 0;
//    int      h        = 0;
//    
//    wxSize   a_size   = t_event.GetSize();
// 
//    w = a_size.GetWidth();
//    h = a_size.GetHeight();
// 
//    // Prevent a divide by zero, when window is too short
//    // (you cant make a window of zero width).
//    if(h == 0)
//    {
//       h = 1;
//    }
// 
//    float ratio = 1.0* w / h;
// 
//    // Reset the coordinate system before modifying
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
// 
//    // Set the view port to be the entire window
//    glViewport(0, 0, w, h);
// 
//    // Set the correct perspective.
//    gluPerspective(45,ratio,1,1000);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    gluLookAt(0.0,0.0,5.0, 
//       0.0,0.0,-1.0,
//       0.0f,1.0f,0.0f);

}