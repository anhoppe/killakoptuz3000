// ***************************************************************
//  CObjectStorage   version:  1.0   ·  date: 06/04/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "src/CObjectStorage.h"

CObjectStorage::CObjectStorage()
{

}

CObjectStorage::~CObjectStorage()
{

}

CObjectStorage& CObjectStorage::getInstance()
{
   static CObjectStorage* r_inst = 0;

   if (r_inst == 0)
   {
      r_inst = new CObjectStorage;
   }

   return *r_inst;
}

/** Process the next time step (movement, collisions, event processing)*/
void CObjectStorage::update()
{
   // Iterate over all objects
   // and update them (events are generated)

   // Detect collisions with quad tree (events are generated)

   // Process events
}