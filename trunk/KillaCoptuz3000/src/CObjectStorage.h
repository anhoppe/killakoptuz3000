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
#include "CHashMap.h"
#include <list>
#include "tinyxml/tinyxml.h"
#include "Objects/CObject.h"
#include "CEvent.h"
#include "CQuadTreeNode.h"

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
   *  @param t_friendObjectList list of 'friend-objects'. Friend objects are not considered in collision detection
   *  @return ID of newly generated object
   */
   unsigned int add(CObject* t_objectPtr, unsigned int t_parentId = 0, std::list<unsigned int>* t_friendObjectsListPtr = 0);

   /**
   *  Adds an object to the delete map. Do not add it directly, because all children have to be added, too
   *  @param t_objectId object with this ID and all its children will be added to the delete list
   */
   void addToDeleteMap(unsigned int t_objectId);

   /**
   *  Delete all child objects of a given object
   *  @param t_objectId object with this ID and all its children will be added to the delete list
   */
   void deleteChildren(unsigned int t_objectId);

   /**
   *  Inserts an object into the draw list
   *  Called by 'add' methods
   */
   void addToDrawList(CObject* t_objectPtr);

   /** Add textures into texture map */
   bool addTextureMap(TiXmlNode* t_nodePtr);

   /** Updates all objects */
   void updateObjects(CLevel* t_levelPtr);

   /** Process events which were generated during last cycle */
   void processEvents();

   /** Process delete objectMap */
   void processDeleteMap();

   /** Checks if level is over (player won or is dead) */
   bool isGameOver();

   /** Creates the quad tree*/
   void initializeQuadTree(float t_top, float t_left, float t_right, float t_bottom);

   /** map of all objects */
   CHashMap<CObject*>               m_objectMap;

   /** Draw list */
   std::list<unsigned int>          m_drawList;

   /** map of objects to delete */
   CHashMap<CObject*>               m_deleteMap;
   
   /** map of all game textures (player and level) */
   std::map<std::string, CTexture*> m_textureMap;

   unsigned int                     m_playerId;

   /** List of events */
   std::list<CEvent*>               m_eventList;

   /** Quad tree for collision */
   CQuadTreeNode*                   m_quadTreeRootPtr;


private:
   /** Object Id counter */ 
   unsigned int                     m_objectIdCount;
};

#endif