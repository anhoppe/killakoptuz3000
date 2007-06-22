// ***************************************************************
//  CWaypoint   version:  1.0   ·  date: 05/06/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef CWAYPOINT_H
#define CWAYPOINT_H

#include "KillaCoptuz3000/src/CPoint.h"
#include "KillaCoptuz3000/src/Behaviour/CBehavior.h"

class CWaypoint : public CBehavior
{
public:
   CWaypoint();
   ~CWaypoint();

   virtual bool load(TiXmlNode* t_nodePtr);
   virtual bool update(SBehaviorData& t_data);

   CPoint         m_point;   
   float          m_v;
   float          m_oldDist;

   unsigned int   m_waitTime;
   unsigned int   m_waitCount;
};

#endif