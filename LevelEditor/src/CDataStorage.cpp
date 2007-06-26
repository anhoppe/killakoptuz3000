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

#include "KillaCoptuz3000/src/Objects/CEnemy.h"

//////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////
CDataStorage::CDataStorage()
{
   m_xPos = 0;
   m_yPos = 0;
}

CDataStorage::~CDataStorage()
{

}

CDataStorage& CDataStorage::getInstance()
{
   static CDataStorage r_inst;

   return r_inst;
}

void CDataStorage::add(int t_objectId)
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
      a_objectPtr->m_xPos     = 0.;
      a_objectPtr->m_yPos     = 0.;

      a_objectPtr->m_width    = 1.;
      a_objectPtr->m_height   = 1.;

      
      m_objects.push_back(a_objectPtr);
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
