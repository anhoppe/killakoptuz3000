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

#include "CUpdateContainer.h"

#include "glut/glut.h"


//////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////
CGLCanvas::CGLCanvas(wxWindow* t_parentPtr)
: wxGLCanvas(t_parentPtr, ((wxWindowID)wxID_ANY), wxDefaultPosition, wxSize(400, 300))
{
   CUpdateContainer::getInstance().add(this);
}

CGLCanvas::~CGLCanvas()
{
   CUpdateContainer::getInstance().remove(this);
}

void CGLCanvas::update()
{

   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   // trigger draw method in data storage
   CDataStorage::getInstance().draw();

   SwapBuffers();
}
