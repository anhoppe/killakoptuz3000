// ***************************************************************
//  CLevel   version:  1.0   �  date: 04/07/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "CLevel.h"
#include "Objects/CSprite.h"
#include "Objects/CObject.h"
#include "Objects/CEnemy.h"
#include "Functions.h"
#include "globals.h"

//////////////////////////////////////////////////////////////////////////
// Initialize static variables
//////////////////////////////////////////////////////////////////////////
std::vector<CObject*> CLevel::M_objects;
std::vector<CObject*> CLevel::M_deleteList;
std::vector<CObject*> CLevel::M_addList;

std::map<std::string, CTexture*> CLevel::M_textureMap;
//////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////
CLevel::CLevel()
{
   m_sound = 0;  
   m_levelOver = false;
}

CLevel::~CLevel()
{

}

bool CLevel::playMusic()
{
   if (!m_sound)
      return false;
 
   if (FSOUND_Stream_Play(0, m_sound) == -1)
      return false;

   return true;   
}

bool CLevel::load(const char* t_fileName)
{
   TiXmlDocument  a_doc;
   TiXmlNode*     a_nodePtr      = 0;
   TiXmlNode*     a_rootNodePtr  = 0;
   TiXmlElement*  a_elemPtr      = 0;

   CObject*       a_objectPtr    = 0;

   std::string    a_str;

   bool           r_ret      = true;


   M_objects.clear();

   if (!a_doc.LoadFile(t_fileName))
   {
      r_ret = false;
   }
   else
   {
      a_rootNodePtr = a_doc.FirstChild("level");
      a_elemPtr = a_rootNodePtr->ToElement();

      r_ret = r_ret & getAttributeStr(a_elemPtr, "xmin", a_str);
      m_minX         = atof(a_str.c_str());

      r_ret = r_ret & getAttributeStr(a_elemPtr, "xmax", a_str);
      m_maxX         = atof(a_str.c_str());

      r_ret = r_ret & getAttributeStr(a_elemPtr, "ymin", a_str);
      m_minY         = atof(a_str.c_str());

      r_ret = r_ret & getAttributeStr(a_elemPtr, "ymax", a_str);
      m_maxY         = atof(a_str.c_str());

      if (getAttributeStr(a_elemPtr, "music", a_str))
      {
         m_sound = FSOUND_Stream_Open(a_str.c_str(), FSOUND_LOOP_NORMAL, 0, 0);
      }            
      
      for(a_nodePtr = a_rootNodePtr->FirstChild(); a_nodePtr; a_nodePtr = a_rootNodePtr->IterateChildren(a_nodePtr))
      {
         a_elemPtr = a_nodePtr->ToElement();

         if(0 != a_elemPtr)
         {
            // load object list
            if(!strcmp("objectlist", a_elemPtr->Value()))
            {
               r_ret &= loadObjectList(a_nodePtr);
            }

            // load texture list
            if(!strcmp("texturelist", a_elemPtr->Value()))
            {
               r_ret &= loadTextureMap(a_nodePtr);
            }
         }
      }
   }

   M_objects.push_back(g_playerPtr);

   return r_ret;
}

bool CLevel::loadObjectList(TiXmlNode* t_nodePtr)
{
   TiXmlNode*     a_nodePtr   = 0;
   TiXmlElement*  a_elemPtr   = 0;

   std::string    a_str       = "";

   CObject*       a_objectPtr = 0;

   bool           r_ret       = true;


   for(a_nodePtr = t_nodePtr->FirstChild(); a_nodePtr; a_nodePtr = t_nodePtr->IterateChildren(a_nodePtr))
   {
      a_elemPtr = a_nodePtr->ToElement();

      if(0 != a_elemPtr)
      {
         // Load Objects
         if(!strcmp("object", a_elemPtr->Value()))
         {
            // Get object type
            TiXmlElement*  a_elemPtr   = 0;
            a_elemPtr = a_nodePtr->ToElement();

            getAttributeStr(a_elemPtr, "type", a_str);               

            if (a_str == "enemy")
            {
               a_objectPtr = new CEnemy(a_nodePtr);
            }
            else
            {
               a_objectPtr = new CObject(a_nodePtr);
            }               
            M_objects.push_back(a_objectPtr);
         }
      }
   }

   return r_ret;
}

bool CLevel::loadTextureMap(TiXmlNode* t_nodePtr)
{
   TiXmlNode*     a_nodePtr      = 0;
   TiXmlElement*  a_elemPtr      = 0;

   std::string    a_key          = "";
   std::string    a_baseFileName = "";

   bool           r_ret          = true;


   for(a_nodePtr = t_nodePtr->FirstChild(); a_nodePtr; a_nodePtr = t_nodePtr->IterateChildren(a_nodePtr))
   {
      a_elemPtr = a_nodePtr->ToElement();

      if(0 != a_elemPtr)
      {
         // Load textures
         if(!strcmp("texture", a_elemPtr->Value()))
         {
            // read 
            if(!getAttributeStr(a_elemPtr, "key", a_key))
            {
               r_ret = false;
            }

            if(r_ret)
            {
               M_textureMap[a_key] = new CTexture(a_elemPtr);
            }
         }
      }
   }

   return r_ret;
}

void CLevel::updateLevel()
{
   std::vector<CObject*>::iterator  a_it;
   CObject*                         a_objectPtr = 0;


   M_deleteList.clear();
   M_addList.clear();

   for(a_it = M_objects.begin(); a_it != M_objects.end(); a_it++)
   {
       (*a_it)->update(this, a_it+1, M_objects.end());

      (*a_it)->draw();
   }  

   // remove all objects from the delete list
   while(M_deleteList.size() > 0)
   {
      a_objectPtr = M_deleteList.back();
      M_deleteList.pop_back();

      if(a_objectPtr != g_playerPtr)
      {
         for(a_it = M_objects.begin(); a_it != M_objects.end(); a_it++)
         {
            if((*a_it) == a_objectPtr)
            {            
               M_objects.erase(a_it);
               break;
            }
         }
         delete a_objectPtr;
      }
      else
      {
         m_levelOver = true;
      }

   }

   // add all objects from add list to the object list
   while(M_addList.size() > 0)
   {
      M_objects.push_back(M_addList.back());
      M_addList.pop_back();
   }
}

void CLevel::nextTexture()
{
   std::vector<CObject*>::iterator a_it;

   for(a_it = M_objects.begin(); a_it != M_objects.end(); a_it++)
   {
      (*a_it)->nextTexture();
   }
}


