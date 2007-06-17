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
   m_quadTreeRootPtr = 0;
}

CObjectStorage::~CObjectStorage()
{
   if(0 != m_quadTreeRootPtr)
   {
      delete m_quadTreeRootPtr;
   }
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

void CObjectStorage::initializeQuadTree(float t_top, float t_left, float t_right, float t_bottom)
{
   if(0 != m_quadTreeRootPtr)
   {
      delete m_quadTreeRootPtr;
   }

   m_quadTreeRootPtr = new CQuadTreeNode(0, t_top, t_left, t_right, t_bottom);

   // Iterate over all objects and insert them to quad tree
   if (m_objectMap.iterate(true))
   {
      do
      {
         if (!m_objectMap.m_current.m_value->m_isBackground)
         {
            m_quadTreeRootPtr->add(m_objectMap.m_current.m_value);
         }         
      }
      while (m_objectMap.iterate());
   }
}

CPlayer* CObjectStorage::getPlayerPtr()
{
   return static_cast<CPlayer*>(m_objectMap[m_playerId]);
}

/** Process events which were generated during last cycle */
void CObjectStorage::processEvents()
{
   CEvent*              a_event;   

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
            addToDeleteMap(a_event->m_objectList[0]);
            break;
         }      
      case e_collided:
         {
            CObject* a_object1Ptr = m_objectMap[a_event->m_objectList[0]];
            CObject* a_object2Ptr = m_objectMap[a_event->m_objectList[1]];

            // first for testing:
            if(!a_object1Ptr->m_invincible)
            {
               if(!a_object2Ptr->m_invincible)
               {
                  a_object1Ptr->m_hitPoints -= a_object2Ptr->m_damagePoints;
               }
               else
               {
                  a_object1Ptr->m_hitPoints = 0;
               }

               if(a_object1Ptr->m_hitPoints <= 0)
               {                  
                  a_object1Ptr->startDying();

                  // Fix me: trigger dying sequence
                  // addToDeleteMap(a_object1Ptr->m_id);
               }
            }

            if(!a_object2Ptr->m_invincible)
            {
               if(!a_object1Ptr->m_invincible)
               {
                  a_object2Ptr->m_hitPoints -= a_object1Ptr->m_damagePoints;
               }
               else
               {
                  a_object2Ptr->m_hitPoints = 0;
               }

               if(a_object2Ptr->m_hitPoints <= 0)
               {
                  a_object2Ptr->startDying();

                  // Fix me: trigger dying sequence
                  // addToDeleteMap(a_object2Ptr->m_id);
               }
            }

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
   // Iterate over delete map  
   if (m_deleteMap.iterate(true))
   {
      do
      {         
         // Remove object from object map
         m_objectMap.erase(m_deleteMap.m_current.m_key);

         // Remove object from draw list
         m_drawList.remove(m_deleteMap.m_current.m_key);

         // Remove object from quad tree
         m_quadTreeRootPtr->remove(m_deleteMap.m_current.m_value);

         // Delete the object
         delete m_deleteMap.m_current.m_value;
      }
      while(m_deleteMap.iterate());
   }

   // Delete the deleteMap
   m_deleteMap.clear();
}


/** Add object read from XML node */
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
         a_objectPtr = new CShot(static_cast<CShot*>(t_objectPtr), t_friendObjectsListPtr);
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

   // add the object to the quad tree
   m_quadTreeRootPtr->add(a_objectPtr);

   return r_ret;
}

void CObjectStorage::addToDeleteMap(unsigned int t_objectId)
{
   // delete object
   m_deleteMap.add(t_objectId, m_objectMap[t_objectId]);

   // delete children
   if (m_objectMap.iterate(true))
   {
      do 
      {
         if(m_objectMap.m_current.m_value->m_parentId == t_objectId)
         {
            m_deleteMap.add(m_objectMap.m_current.m_value->m_id, m_objectMap.m_current.m_value);
         }
      } 
      while(m_objectMap.iterate());
   }
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

void CObjectStorage::updateObjects(CLevel* t_levelPtr)
{
   float                                        a_xOld      = 0;
   float                                        a_yOld      = 0;
   CObject*                                     a_objectPtr = 0;

   std::map<unsigned int, CObject*>::iterator   a_it;


   if (m_objectMap.iterate(true))
   {
      do 
      {
         a_objectPtr = m_objectMap.m_current.m_value;

         a_xOld = a_objectPtr->m_xPos;
         a_yOld = a_objectPtr->m_yPos;

         a_objectPtr->update(t_levelPtr);

         if((a_xOld != a_objectPtr->m_xPos) ||
            (a_yOld != a_objectPtr->m_yPos)   )
         {
            // delete from tree
            m_quadTreeRootPtr->remove(a_objectPtr);
            m_quadTreeRootPtr->add(a_objectPtr);
         }

      } 
      while(m_objectMap.iterate());
   }
}
