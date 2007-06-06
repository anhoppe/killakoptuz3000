// ***************************************************************
//  CObjectStorage   version:  1.0   ·  date: 06/04/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "CObjectStorage.h"
#include "Objects/CShot.h"
#include "tinyxml/tinyxml.h"

CObjectStorage::CObjectStorage()
{
   m_objectIdCount = 1;
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

/** Add object read from xml node */
void CObjectStorage::add(TiXmlNode* t_nodePtr, unsigned int t_parentId)
{
   TiXmlElement* a_elemPtr          = t_nodePtr->ToElement();
   CObject*        a_objectPtr      = 0;   

   getAttributeStr(a_elemPtr, "type", a_str);               

   if (a_str == "enemy")
   {
      a_objectPtr = new CEnemy(a_nodePtr);                  
   }
   else
   {
      a_objectPtr = new CObject(a_nodePtr);
   }

   // Add the id of the parent to object
   a_objectPtr->m_parentId = t_parentId;

   // Add own id to object
   a_objectPtr->m_id       = m_objectIdCount;

   // Add the object to the map, increment the id counter
   m_objectMap[m_objectIdCount++] = a_objectPtr;
}

/** Add object from pattern object*/
void CObjectStorage::add(CObject* t_objectPtr, unsigned int t_parentId, std::list<unsigned int>* m_friendObjectsListPtr)
{
   CObject*        a_objectPtr      = 0;

   switch (t_objectPtr->getType())
   {
      case e_shot:
         a_objectPtr = new CShot(t_objectPtr);
         break;
   }

   // Add the id of the parent to object
   a_objectPtr->m_parentId = t_parentId;   

   // Add own id to object
   a_objectPtr->m_id       = m_objectIdCount;

   // Add the object to the map, increment the id counter
   m_objectMap[m_objectIdCount++] = a_objectPtr;      
}