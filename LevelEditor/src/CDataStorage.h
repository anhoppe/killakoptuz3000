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

   void add(int t_objectId);

   /** Adds textures to the texture map */
   void addTextures(std::list<wxString>& t_fileList);

   void draw();

   /** 
   *  Generates key from file name 
   *  @param t_fileName base file name
   **/
   std::string getKey(const std::string& t_fileName);

private:
   //////////////////////////////////////////////////////////////////////////
   // Variables
   //////////////////////////////////////////////////////////////////////////
public:
   std::vector<CObject*>            m_objects;

   std::map<wxString, CTexture*>    m_textureMap;
   float                            m_xPos;
   float                            m_yPos;
};

#endif
