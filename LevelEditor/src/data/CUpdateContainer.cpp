// ***************************************************************
//  CUpdateContainer   version:  1.0   �  date: 06/25/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "CUpdateContainer.h"

//////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////
CUpdateContainer::CUpdateContainer()
{

}

CUpdateContainer::~CUpdateContainer()
{

}

CUpdateContainer& CUpdateContainer::getInstance()
{
   static CUpdateContainer r_container;

   return r_container;
}

void CUpdateContainer::add(IUpdate* t_listenerPtr)
{
   m_container.push_back(t_listenerPtr);
}

void CUpdateContainer::remove(IUpdate* t_listenerPtr)
{
   m_container.remove(t_listenerPtr);
}

void CUpdateContainer::add(ISetObject* t_listenerPtr)
{
   m_setObjectContainer.push_back(t_listenerPtr);
}

void CUpdateContainer::remove(ISetObject* t_listenerPtr)
{
   m_setObjectContainer.remove(t_listenerPtr);
}

void CUpdateContainer::update()
{
   std::list<IUpdate*>::iterator a_it;


   for(a_it = m_container.begin(); a_it != m_container.end(); a_it++)
   {
      (*a_it)->update();
   }
}

void CUpdateContainer::setObject(int t_index)
{
   std::list<ISetObject*>::iterator a_it;


   for(a_it = m_setObjectContainer.begin(); a_it != m_setObjectContainer.end(); a_it++)
   {
      (*a_it)->setObject(t_index);
   }
}