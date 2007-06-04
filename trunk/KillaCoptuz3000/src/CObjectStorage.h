// ***************************************************************
//  CObjectStorage   version:  1.0   ·  date: 06/04/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef COBJECT_STORAGE_H
#define COBJECT_STORAGE_H

#include <map>
#include "tinyxml/tinyxml.h"
#include "CObject.h"

class CObjectStorage
{
public:
   CObjectStorage();
   ~CObjectStorage();

   /** */
   static CObjectStorage& getInstance();

   /** Process the next time step (movement, collisions, event processing)*/
   update();
   
   /** Add object read from xml node */
   void add(TiXmlNode* t_nodePtr);

   /** Add object from pattern object*/
   void add(CObject* t_objectPtr);

   /** Process events */
   void processEvents();

   /** map of all objects */
   std::map<unsigned int, CObject*> m_objectMap;
};

#endif