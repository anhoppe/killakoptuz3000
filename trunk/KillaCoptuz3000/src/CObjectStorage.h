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

#include "Objects/CObject.h"

class CPlayer;
class CTexture;

class CObjectStorage
{
public:
   CObjectStorage();
   ~CObjectStorage();

   /** */
   static CObjectStorage& getInstance();

   CPlayer* getPlayerPtr();
   
   /** 
   *  Add object read from xml node 
   *  @param t_nodePtr Pointer to the XML Node that describes the new object
   *  @param t_type caller must know the type og the object. Is not in the XML node per default
   *  @param t_parentId ID of the parent. 0 if object has no parent
   *  @return ID of the newly generated object
   */
   unsigned int add(TiXmlNode* t_nodePtr, VeObjectType t_type, unsigned int t_parentId = 0);

   /** 
   *  Add object from pattern object
   *  @param t_objectPtr Pointer to the source object. A copy of this object will be inserted
   *  @param t_parentId ID of the parent, 0 if it has no parent
   *  @param m_friendObjectList list of 'friend-objects'. Friend objects are not considered in collision detection
   *  @return ID of newly generated object
   */
   unsigned int add(CObject* t_objectPtr, unsigned int t_parentId = 0, std::list<unsigned int>* m_friendObjectsListPtr = 0);

   /** Add textures into texture map */
   bool addTextureMap(TiXmlNode* t_nodePtr);

   /** map of all objects */
   std::map<unsigned int, CObject*> m_objectMap;
   
   /** map of all game textures (player and level) */
   std::map<std::string, CTexture*> m_textureMap;

private:
   /** Object Id counter */ 
   unsigned int m_objectIdCount;

   unsigned int m_playerId;
};

#endif