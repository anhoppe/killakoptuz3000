// ***************************************************************
//  CObjectStorage   version:  1.0   ·  date: 06/04/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include <list>
#include "CObjectStorage.h"

#include "tinyxml/tinyxml.h"

#include "Functions.h"

#include "CTexture.h"

#include "Objects/CObject.h"
#include "Objects/CShot.h"
#include "Objects/CPlayer.h"
#include "Objects/CEnemy.h"


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

CPlayer* CObjectStorage::getPlayerPtr()
{
   return static_cast<CPlayer*>(m_objectMap[m_playerId]);
}

/** Process events which were generated during last cycle */
void CObjectStorage::processEvents()
{
   CEvent* a_event;

   // Iterate over events which were generated during the last cycle
   while (m_eventList.size())
   {
      // Get last event
      a_event = m_eventList.back();

      // Process events
      switch (a_event->m_event)
      {
      case e_delete:
         {            
            // Add object to delete list
            m_deleteMap[a_event->m_objectList.back()] = m_objectMap[a_event->m_objectList.back()];

            break;
         }
      case e_dying:
         {
            break;
         }
      case e_collided:
         {
            break;
         }
      }

      // Delete it from list
      m_eventList.pop_back();

      // Delete event object
      delete a_event;

   }
}

/** Process delete objectMap */
void CObjectStorage::processDeleteMap()
{
   CObject* a_objectPtr;
   std::map<unsigned int, CObject*>::iterator a_it;

   for (a_it = m_deleteMap.begin(); a_it != m_deleteMap.end(); a_it++)
   {
      // Remove object from hast table
      m_objectMap.erase(a_it->first);

      // Remove object from draw list
      m_drawList.remove(a_it->first);

      // Delete the object
      a_objectPtr = a_it->second;

      delete a_objectPtr;
   }

   // Delete the deleteMap
   m_deleteMap.clear();
}

/** Add object read from xml node */
unsigned int CObjectStorage::add(TiXmlNode* t_nodePtr, VeObjectType t_type, unsigned int t_parentId)
{
   TiXmlElement*  a_elemPtr   = t_nodePtr->ToElement();
   CObject*       a_objectPtr = 0;   
   int            r_ret       = 0;   


   if(t_type == e_player)
   {
      a_objectPtr = new CPlayer();
   }
   else if (t_type == e_enemy)
   {
      a_objectPtr = new CEnemy();                  
   }
   else if(t_type == e_weapon)
   {
      a_objectPtr = new CWeapon();
   }
   else
   {
      a_objectPtr = new CObject();
   }

   // Add the id of the parent to object
   a_objectPtr->m_parentId = t_parentId;

   // Add own id to object
   r_ret = m_objectIdCount++;
   a_objectPtr->m_id       = r_ret;

   // remember player ID
   if(t_type == e_player)
   {
      m_playerId = a_objectPtr->m_id;
   }

   // Add the object to the map, increment the id counter
   m_objectMap.add(r_ret, a_objectPtr);

   // load enemy content
   a_objectPtr->load(t_nodePtr);

   // add the object to the draw list
   addToDrawList(a_objectPtr);

   return r_ret;   
}

/** Add object from pattern object*/
unsigned int CObjectStorage::add(CObject* t_objectPtr, unsigned int t_parentId, std::list<unsigned int>* t_friendObjectsListPtr)
{
   CObject*       a_objectPtr = 0;
   unsigned int   r_ret       = 0;


   switch (t_objectPtr->getType())
   {
      case e_shot:
         a_objectPtr = new CShot(static_cast<CShot*>(t_objectPtr));
         break;
   }

   // Add the id of the parent to object
   a_objectPtr->m_parentId = t_parentId;   

   // Add own id to object
   r_ret = m_objectIdCount++;
   a_objectPtr->m_id = r_ret;

   // Add the object to the map, increment the id counter
   m_objectMap.add(r_ret, a_objectPtr);

   // add the object to the draw list
   addToDrawList(a_objectPtr);

   return r_ret;
}

void CObjectStorage::addToDrawList(CObject* t_objectPtr)
{
   std::list<unsigned int>::iterator   a_it;
   bool                                a_isInserted   = false;
   CObject*                            a_objectPtr    = 0;


   for(a_it = m_drawList.begin(); a_it != m_drawList.end(); ++a_it)
   {
      a_objectPtr = m_objectMap[*a_it];
      assert(a_objectPtr);

      if(0 != a_objectPtr)
      {
         if(a_objectPtr->m_drawLayer >= t_objectPtr->m_drawLayer)
         {
            m_drawList.insert(a_it, t_objectPtr->m_id);
            a_isInserted= true;
            break;
         }
      }
   }

   if(!a_isInserted)
   {
      m_drawList.push_back(t_objectPtr->m_id);
   }

}

bool CObjectStorage::addTextureMap(TiXmlNode* t_nodePtr)
{
   TiXmlNode*     a_nodePtr   = 0;
   TiXmlElement*  a_elemPtr   = 0;

   std::string    a_key       = "";

   bool           r_ret       = true;


   for(a_nodePtr = t_nodePtr->FirstChild("texture"); a_nodePtr; a_nodePtr = t_nodePtr->IterateChildren("texture", a_nodePtr))
   {
      a_elemPtr = a_nodePtr->ToElement();

      if (0 != a_elemPtr)
      {        

         // read 
         if(!getAttributeStr(a_elemPtr, "key", a_key))
         {
            r_ret = false;
         }

         if(r_ret)
         {
            if(m_textureMap[a_key] == 0)
            {
               m_textureMap[a_key] = new CTexture(a_elemPtr);
            }
         }
      }      
   }

   return r_ret;
}
