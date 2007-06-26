// ***************************************************************
//  CApp   version:  1.0   ·  date: 06/18/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef CAPP_H
#define CAPP_H

#include "wxWidgets/include/wx/wx.h"

class CApp : public wxApp
{
public:
   /*
   * WX init method
   */
   virtual bool OnInit();

   /*
   * WX exit method
   */
   virtual int OnExit();
};


#endif
