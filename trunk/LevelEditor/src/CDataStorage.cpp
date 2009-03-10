// ***************************************************************
//  CDataStorage   version:  1.0   ·  date: 06/18/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "CDataStorage.h"
#include "CObjectControl.h"
#include "CLayerControl.h"
#include "CUpdateContainer.h"

#include "KillaCoptuz3000/src/Objects/CEnemy.h"

#include <wx/filename.h>

//////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////
CDataStorage::CDataStorage()
{
   m_xPos = 0;
   m_yPos = 0;
   m_fileName = "";
   m_activeObjectPtr = 0;
   m_addObjectModeActive = false;
}

CDataStorage::~CDataStorage()
{

}

CDataStorage& CDataStorage::getInstance()
{
   static CDataStorage r_inst;

   return r_inst;
}

bool CDataStorage::isEmpty()
{
   bool r_isEmpty = true;

   if(m_textureMap.size() > 0)
   {
      r_isEmpty = false;
   }

   if(m_objects.size() > 0)
   {
      r_isEmpty = false;
   }

   return r_isEmpty;
}


void CDataStorage::clear()
{
   std::vector<CObject*>::iterator           a_objectIt;
   std::map<wxString, CTexture*>::iterator   a_textureIt;


   // delete all objects
   for(a_objectIt = m_objects.begin(); a_objectIt != m_objects.end(); a_objectIt++)
   {
      delete (*a_objectIt);
   }
   m_objects.clear();

   // delete all textures
   for(a_textureIt = m_textureMap.begin(); a_textureIt != m_textureMap.end(); a_textureIt++)
   {
      delete a_textureIt->second;
   }
   m_textureMap.clear();

   // reset file name
   m_fileName = "";
}

bool CDataStorage::moveObject(int t_oldIndex, int t_newIndex)
{
   bool  r_result = false;


   if(t_oldIndex < (int)m_objects.size() && t_newIndex < (int)m_objects.size())
   {
      std::vector<CObject*>::iterator  a_it;
      CObject*                         a_objectPtr = 0;
      int                              a_count  = 0;

      a_objectPtr = m_objects[t_oldIndex];

      for(a_it = m_objects.begin(); a_it != m_objects.end(); a_it++)
      {
         if(a_count == t_oldIndex)
         {
            m_objects.erase(a_it);
            break;
         }
         a_count++;
      }

      a_count = 0;
      if(t_newIndex < (int)m_objects.size())
      {
         for(a_it = m_objects.begin(); a_it != m_objects.end(); a_it++)
         {
            if(a_count == t_newIndex)
            {
               m_objects.insert(a_it, a_objectPtr);
               break;
            }
            a_count++;
         }
      }
      else 
      {
         m_objects.push_back(a_objectPtr);
      }

      r_result = true;
   }

   return r_result;
}

void CDataStorage::assignLayerNumber(int t_objectIndex, int t_layerNumber)
{
   if(t_objectIndex >= 0 && t_objectIndex < (int)m_objects.size())
   {
      m_objects[t_objectIndex]->m_drawLayer = t_layerNumber;
   }
}

void CDataStorage::setActiveObjectByIndex(int t_index)
{
   m_activeObjectPtr = 0;

   if(t_index >= 0 && t_index < (int)m_objects.size())
   {
      m_activeObjectPtr = m_objects[t_index];
      CObjectControl::getInstance().setObject(m_activeObjectPtr);
   }
}

void CDataStorage::getObjectsAsStrings(std::vector<std::string>& t_objects)
{
   std::vector<CObject*>::iterator  a_it;


   for(a_it = m_objects.begin(); a_it != m_objects.end(); a_it++)
   {
      t_objects.push_back((*a_it)->m_name);
   }
}

void CDataStorage::add(int t_objectId, double t_x, double t_y)
{
   CObject* a_objectPtr = 0;

   switch(t_objectId)
   {
   case OBJ_OBJECT:
      a_objectPtr = new CObject();
      break;
   case OBJ_ENEMY:
      a_objectPtr = new CEnemy();
      break;
   }

   //////////////////////////////////////////////////////////////////////////
   // initialize basic object data
   if(0 != a_objectPtr)
   {
      a_objectPtr->m_xPos     = t_x;
      a_objectPtr->m_yPos     = t_y;

      a_objectPtr->m_width    = 1.;
      a_objectPtr->m_height   = 1.;

      char a_str[1024];
      sprintf_s(a_str, "Object%d", m_objects.size());
      a_objectPtr->m_name = a_str;
      
      m_objects.push_back(a_objectPtr);
   }
}

void CDataStorage::deleteObjectByIndex(int t_index)
{
   if(t_index >= 0 && t_index < (int)m_objects.size())
   {
      CObject*                         a_objectPtr = 0;
      std::vector<CObject*>::iterator  a_it;
      int                              a_count  = 0;

      for(a_it = m_objects.begin(); a_it != m_objects.end(); a_it++)
      {
         if(a_count == t_index)
         {
            m_objects.erase(a_it);
            break;
         }
         a_count++;
      }
   }
}

bool CDataStorage::selectObjectByCoord(double t_x, double t_y)
{
   std::vector<CObject*>::reverse_iterator   a_it;
   size_t                                    a_index  = 0;
   bool                                      r_found  = false;


   a_index = m_objects.size()-1;

   for(a_it = m_objects.rbegin(); a_it != m_objects.rend(); a_it++)
   {
      if((t_x > (*a_it)->m_xPos) && (t_x < (*a_it)->m_xPos + (*a_it)->m_width) &&
         (t_y > (*a_it)->m_yPos) && (t_y < (*a_it)->m_yPos + (*a_it)->m_height)    )
      {
         if(m_activeObjectPtr != (*a_it))
         {
            CUpdateContainer::getInstance().setObject((int)a_index);
            CUpdateContainer::getInstance().update();
   
            r_found  = true;
         }
         break;
      }
      a_index--;
   }

   return r_found;
}

std::string CDataStorage::getKey(const std::string& t_fileName)
{
   wxFileName  a_fileName(t_fileName);
   wxString    r_ret = "";

   r_ret = a_fileName.GetName(); 
   r_ret = r_ret.Left(r_ret.Length()-1);

   return r_ret.c_str();
}

//////////////////////////////////////////////////////////////////////////
// Load Methods
//////////////////////////////////////////////////////////////////////////
bool CDataStorage::load(std::string t_gfxDir, std::vector<int>& t_layerIndices)
{
   bool  r_result = false;

   if(wxFileExists(m_fileName))
   {
      TiXmlDocument  a_doc(m_fileName.c_str());
      TiXmlNode*     a_nodePtr                  = 0;
      TiXmlElement*  a_elemPtr                  = 0;

      if(a_doc.LoadFile())
      {
         a_nodePtr = a_doc.FirstChild("level");

         if(0 != a_nodePtr)
         {
            r_result = true;

            r_result &= loadResources(a_nodePtr, t_gfxDir);
            r_result &= loadObjects(a_nodePtr);

            // after the resources are loaded the layers can be inserted
            if(r_result)
            {
               getLayerIndices(t_layerIndices);
            }
         }
      }
   }

   return r_result;
}

bool CDataStorage::loadResources(TiXmlNode* t_rootPtr, std::string t_gfxDir)
{
   bool        r_result       = false;

   if(0 != t_rootPtr)
   {
      TiXmlNode*  a_nodePtr      = 0;
      TiXmlNode*  a_subNodePtr   = 0;

      a_nodePtr = t_rootPtr->FirstChild("resources");

      if(0 != a_nodePtr)
      {
         std::string    a_name      = "";
         TiXmlElement*  a_elemPtr   = 0;

         for(a_subNodePtr = a_nodePtr->FirstChild("texture"); a_subNodePtr; a_subNodePtr = a_nodePtr->IterateChildren("texture", a_subNodePtr))
         {
            a_elemPtr = a_subNodePtr->ToElement();

            a_name = a_elemPtr->Attribute("key");

            m_textureMap[a_name] = new CTexture(a_elemPtr, t_gfxDir);
         }

         r_result = true;
      }
   }

   return r_result;
}

bool CDataStorage::loadObjects(TiXmlNode* t_rootPtr)
{
   bool        r_result       = false;

   if(0 != t_rootPtr)
   {
      TiXmlNode*  a_nodePtr      = 0;
      TiXmlNode*  a_subNodePtr   = 0;

      a_nodePtr = t_rootPtr->FirstChild("objectlist");

      if(0 != a_nodePtr)
      {
         std::string    a_name         = "";
         TiXmlElement*  a_elemPtr      = 0;
         CObject*       a_objectPtr    = 0;
         int            a_objectCount  = 0;
         char           a_str[1024];


         r_result = true;
         
         for(a_subNodePtr = a_nodePtr->FirstChild("object"); a_subNodePtr; a_subNodePtr = a_nodePtr->IterateChildren("object", a_subNodePtr))
         {
            a_objectPtr = CObject::create(a_subNodePtr);
            if(0 != a_objectPtr)
            {
               sprintf_s(a_str, "Object%d", a_objectCount++);
               a_objectPtr->m_name = a_str;
               insertObjectByLayerNumber(a_objectPtr);
            }
            else
            {
               r_result = false;
               break;
            }
         }
      }

   }

   return r_result;
}

void CDataStorage::insertObjectByLayerNumber(CObject* t_objectPtr)
{
   bool                             a_inserted  = false;
   std::vector<CObject*>::iterator  a_it;
   

   for(a_it = m_objects.begin(); a_it != m_objects.end(); a_it++)
   {
      if((*a_it)->m_drawLayer > t_objectPtr->m_drawLayer)
      {
         m_objects.insert(a_it, t_objectPtr);
         a_inserted = true;
         break;
      }
   }

   if(!a_inserted)
   {
      m_objects.push_back(t_objectPtr);
   }
}

void CDataStorage::getLayerIndices(std::vector<int>& t_layerIndices)
{
   unsigned int                     a_layerNumber  = 0;
   std::vector<CObject*>::iterator  a_it;
   int                              a_count        = 0;

   if(m_objects.size() > 0)
   {
      a_layerNumber = m_objects[0]->m_drawLayer;

      for(a_it = m_objects.begin(); a_it != m_objects.end(); a_it++)
      {
         if((*a_it)->m_drawLayer > a_layerNumber)
         {
            t_layerIndices.push_back(a_count);
            a_layerNumber = (*a_it)->m_drawLayer;
         }
         a_count++;
      }
   }
}

//////////////////////////////////////////////////////////////////////////
// Save Methods
//////////////////////////////////////////////////////////////////////////
void CDataStorage::save()
{
   if(m_fileName != "")
   {
      TiXmlDocument  a_doc(m_fileName.c_str());
      TiXmlNode*     a_nodePtr                  = 0;
      TiXmlElement*  a_elemPtr                  = 0;
      double         a_xMin                     = 0.;
      double         a_yMin                     = 0.;
      double         a_xMax                     = 0.;
      double         a_yMax                     = 0.;
      char           a_str[1024];


      a_elemPtr = new TiXmlElement("level");

      // store level bounds
      getLevelBounds(a_xMin, a_yMin, a_xMax, a_yMax);
      
      sprintf_s(a_str, "%f", a_xMin);
      a_elemPtr->SetAttribute("xmin", a_str);

      sprintf_s(a_str, "%f", a_yMin);
      a_elemPtr->SetAttribute("ymin", a_str);

      sprintf_s(a_str, "%f", a_xMax);
      a_elemPtr->SetAttribute("xmax", a_str);

      sprintf_s(a_str, "%f", a_yMax);
      a_elemPtr->SetAttribute("ymax", a_str);


      saveResources(a_elemPtr);
      saveObjects(a_elemPtr);

      a_doc.InsertEndChild(*a_elemPtr);
      delete a_elemPtr;

      a_doc.SaveFile();
   }
}

void CDataStorage::saveResources(TiXmlNode* t_rootPtr)
{
   std::map<wxString, CTexture*>::iterator   a_textureIt;
   TiXmlElement*                             a_elemPtr   = 0;

   
   a_elemPtr = new TiXmlElement("resources");

   for(a_textureIt = m_textureMap.begin(); a_textureIt != m_textureMap.end(); a_textureIt++)
   {
      (a_textureIt->second)->save(a_elemPtr, (a_textureIt->first).c_str());
   }

   t_rootPtr->InsertEndChild(*a_elemPtr);
   delete a_elemPtr;
}

void CDataStorage::saveObjects(TiXmlNode* t_rootPtr)
{
   std::vector<CObject*>::iterator  a_objectIt;
   TiXmlElement*                    a_elemPtr   = 0;


   a_elemPtr = new TiXmlElement("objectlist");

   for(a_objectIt = m_objects.begin(); a_objectIt != m_objects.end(); a_objectIt++)
   {
      (*a_objectIt)->save(a_elemPtr);
   }

   t_rootPtr->InsertEndChild(*a_elemPtr);
   delete a_elemPtr;
}

void CDataStorage::getLevelBounds(double& t_xMin, double& t_yMin, double& t_xMax, double& t_yMax)
{
   std::vector<CObject*>::iterator  a_it;


   t_xMin = DBL_MAX;
   t_yMin = DBL_MAX;
   t_xMax = -DBL_MAX;
   t_yMax = -DBL_MAX;

   for(a_it = m_objects.begin(); a_it != m_objects.end(); a_it++)
   {
      if((*a_it)->m_xPos < t_xMin)
      {
         t_xMin = (*a_it)->m_xPos;
      }
      if((*a_it)->m_yPos < t_yMin)
      {
         t_yMin = (*a_it)->m_yPos;
      }
      if((*a_it)->m_xPos + (*a_it)->m_width > t_xMax)
      {
         t_xMax = (*a_it)->m_xPos + (*a_it)->m_width;
      }
      if((*a_it)->m_yPos + (*a_it)->m_height > t_yMax)
      {
         t_yMax = (*a_it)->m_yPos + (*a_it)->m_height;
      }
   }
}

void CDataStorage::draw()
{
   std::vector<CObject*>::iterator  a_it;

   glTranslatef(m_xPos, m_yPos, 0.);
   glPushMatrix();

   for(a_it = m_objects.begin(); a_it != m_objects.end(); a_it++)
   {
      (*a_it)->draw();
   }
   glPopMatrix();
   glTranslatef(-m_xPos, -m_yPos, 0.);
}
