// ***************************************************************
//  CUpdateContainer   version:  1.0   ·  date: 06/25/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef CUPDATE_CONTAINER_H
#define CUPDATE_CONTAINER_H

#include <list>

#include "IUpdate.h"

class CUpdateContainer
{
   //////////////////////////////////////////////////////////////////////////
   // Methods
   //////////////////////////////////////////////////////////////////////////

public:
   CUpdateContainer();
   ~CUpdateContainer();

   static CUpdateContainer& getInstance();

   /** Registers a listener */
   void add(IUpdate* t_listenerPtr);

   /** Removes a listener */
   void remove(IUpdate* t_listenerPtr);

   /** Updates all listeners */
   void update();

   //////////////////////////////////////////////////////////////////////////
   // Variables
   //////////////////////////////////////////////////////////////////////////
public:
private:
   std::list<IUpdate*>  m_container;
};

#endif
