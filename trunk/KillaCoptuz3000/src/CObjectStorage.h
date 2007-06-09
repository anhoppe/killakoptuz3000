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

class CObject;
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
   
   /** Add object read from xml node */
   void add(TiXmlNode* t_nodePtr, VeObjectType t_type, unsigned int t_parentId = 0);

   /** Add object from pattern object */
   void add(CObject* t_objectPtr, unsigned int t_parentId = 0, std::list<unsigned int>* m_friendObjectsListPtr = 0);

   /** Add textures into texture map */
   bool addTextureMap(TiXmlNode* t_nodePtr);

   /** map of all objects */
   std::map<unsigned int, CObject*> m_objectMap;
   
   /** map of all game textures (player and level) */
   static std::map<std::string, CTexture*> m_textureMap;

private:
   /** Object Id counter */ 
   unsigned int m_objectIdCount;

   unsigned int m_playerId;
};

#endif