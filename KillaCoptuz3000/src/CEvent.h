// ***************************************************************
//  CEvent   version:  1.0   ·  date: 06/11/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef CEVENT_H
#define CEVENT_H

#include <list>

enum EEventType {e_dying, e_delete, e_collided};

class CEvent
{
public:
   CEvent();
   CEvent(EEventType t_type);

   ~CEvent();

  /** List of participating object id's*/
  std::list<unsigned int> m_objectList;

  /** Event type*/
  EEventType m_event;  
};

#endif
