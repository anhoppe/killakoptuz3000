// ***************************************************************
//  CScript   version:  1.0   ·  date: 05/06/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include <string>
#include "KillaCoptuz3000/src/Behaviour\CScript.h"
#include "KillaCoptuz3000/src/Behaviour\CWaypoint.h"
#include "KillaCoptuz3000/src/Functions.h"

CScript::CScript(const char* t_scriptFileName)
{
   TiXmlDocument  a_doc;
   TiXmlNode*     a_nodePtr;
   TiXmlElement*  a_elemPtr;
   std::string    a_str;
   CBehavior*     a_behavPtr;

   if (a_doc.LoadFile(t_scriptFileName))
   {
      for(a_nodePtr = a_doc.FirstChild("behavior"); a_nodePtr; a_nodePtr = a_doc.IterateChildren(a_nodePtr))
      {
         a_elemPtr = a_nodePtr->ToElement();
         
         if (getAttributeStr(a_elemPtr, "type",a_str))
         {            
            if (a_str == "waypoint")
            {
               a_behavPtr = new CWaypoint();
               a_behavPtr->load(a_nodePtr);
               m_behavior.push_back(a_behavPtr);
            }
         }
      }
   }

   m_activeBehavior = 0;
}

CScript::~CScript()
{
   std::vector<CBehavior*>::iterator a_it;

   for (a_it = m_behavior.begin(); a_it != m_behavior.end(); a_it++)
   {
      delete *a_it;
   }
   m_behavior.clear();
}

void CScript::update(SBehaviorData& t_data)
{
   int tol = 0;
   if(m_behavior.size() > 0)
   {
      if (m_behavior[m_activeBehavior]->update(t_data))
      {                  
         m_activeBehavior++;
         
         if (m_activeBehavior >= m_behavior.size())
         {
            m_activeBehavior = 0;
         }
      }
   }
}
