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

#include "tinyxml/tinyxml.h"

#include "CTexture.h"

#include "Objects/CObject.h"
#include "Objects/CShot.h"
#include "Objects/CPlayer.h"


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
   return m_objectMap[m_playerId];
}

/** Add object read from xml node */
void CObjectStorage::add(TiXmlNode* t_nodePtr, VeObjectType t_type, unsigned int t_parentId)
{
   TiXmlElement* a_elemPtr          = t_nodePtr->ToElement();
   CObject*        a_objectPtr      = 0;   


   if(t_type == e_player)
   {
      a_objectPtr = new CPlayer();
   }
   if (t_type == e_enemy)
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
   a_objectPtr->m_id       = m_objectIdCount;

   // remember player ID
   if(t_type == e_player)
   {
      m_playerId = a_objectPtr->m_id;
   }

   // Add the object to the map, increment the id counter
   m_objectMap[m_objectIdCount++] = a_objectPtr;

   // load enemy content
   a_objectPtr->load(t_nodePtr);
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

bool CObjectStorage::addTextureMap(TiXmlNode* t_nodePtr)
{
   TiXmlNode*     a_nodePtr   = 0;
   TiXmlElement*  a_elemPtr   = 0;

   std::string    a_key       = "";

   bool           r_ret       = false;


   for(a_nodePtr = a_nodePtr->FirstChild("texture"); a_nodePtr; a_nodePtr = a_nodePtr->IterateChildren("texture", a_nodePtr))
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
