// ***************************************************************
//  CDataStorage   version:  1.0   ·  date: 06/18/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef CDATASTORAGE_H
#define CDATASTORAGE_H

#include "../KillaCoptuz3000/src/Objects/CObject.h"
#include "../KillaCoptuz3000/src/CTexture.h"

#include <wx/wx.h>

#include <map>

//////////////////////////////////////////////////////////////////////////
// Object IDs
//////////////////////////////////////////////////////////////////////////
#define OBJ_OBJECT   0
#define OBJ_ENEMY    1

//////////////////////////////////////////////////////////////////////////
// Declaration
//////////////////////////////////////////////////////////////////////////
class CDataStorage
{
   //////////////////////////////////////////////////////////////////////////
   // Methods
   //////////////////////////////////////////////////////////////////////////
public:
   CDataStorage();
   ~CDataStorage();

   static CDataStorage& getInstance();

   /** Checks if the data storage is empty (no textures and no objects) */
   bool isEmpty();

   /** Deletes all textures and objects */
   void clear();

   /** Moves an item in the object list to a new position */
   bool moveObject(int t_oldIndex, int t_newIndex);

   /** Assigns a value to the draw layer */
   void assignLayerNumber(int t_objectIndex, int t_layerNumber);

   /** Sets the active object by vector index */
   void setActiveObjectByIndex(int t_index);

   /** Checks for active object */
   bool isActiveObject(CObject* t_objectPtr) { return m_activeObjectPtr == t_objectPtr; }

   CObject* getActiveObject() { return m_activeObjectPtr; }

   /** Retrieves the object list as string vector */
   void getObjectsAsStrings(std::vector<std::string>& t_objects);

   /** Checks if the add object mode is on */
   void setAddObjectMode(bool t_enabled);
   bool isAddObjectModeEnabled() { return m_addObjectModeActive; }

   /** Checks if the add enemy mode is on */
   void setAddEnemyMode(bool t_enable);
   bool isAddEnemyModeEnabled() { return m_addEnemyModeActive; }

   /** Adds an object */
   void add(int t_objectId, double t_x, double t_y);

   /** Deletes an object from the object vector by index */
   void deleteObjectByIndex(int t_index);

   /** Retrieves the number of objects in the object list */
   int getNumObjects() { return (int)m_objects.size(); }

   /** Adds textures to the texture map */
   void addTextures(std::list<wxString>& t_fileList);

   /** Selects an object by coordinates */
   bool selectObjectByCoord(double t_x, double t_y);

   /** Loads the content of a file */
   bool load(std::string t_gfxDir, std::vector<int>& t_layerIndices);

   /** Saves the content to a level file */
   void save();

   /** Draw the level */
   void draw();

   /**
   *  Generates key from file name 
   *  @param t_fileName base file name
   **/
   std::string getKey(const std::string& t_fileName);

private:
   bool loadResources(TiXmlNode* t_nodePtr, std::string t_gfxDir);
   bool loadObjects(TiXmlNode* t_nodePtr);
   void insertObjectByLayerNumber(CObject* t_objectPtr);
   void getLayerIndices(std::vector<int>& t_layerIndices);

   void saveResources(TiXmlNode* t_rootPtr);
   void saveObjects(TiXmlNode* t_rootPtr);

   void getLevelBounds(double& t_xMin, double& t_yMin, double& t_xMax, double& t_yMax);

   //////////////////////////////////////////////////////////////////////////
   // Variables
   //////////////////////////////////////////////////////////////////////////
public:
   /** file name of the level */
   wxString                         m_fileName;

   /** Vector with all objects in the level */
   std::vector<CObject*>            m_objects;

   /** name-to-texture map*/
   std::map<wxString, CTexture*>    m_textureMap;

   // base position for all objects ?!?
   float                            m_xPos;
   float                            m_yPos;

   /** Currently active object */
   CObject*                         m_activeObjectPtr;

   /** Indicates if the add object mode is enabled */
   bool                             m_addObjectModeActive;

   /** Indicates if the add weapon mode is enabled (Mutual exclusive with add object mode) */
   bool                             m_addEnemyModeActive;
};

#endif
