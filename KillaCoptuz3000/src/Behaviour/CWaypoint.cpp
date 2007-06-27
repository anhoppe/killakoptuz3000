// ***************************************************************
//  CWaypoint   version:  1.0   ·  date: 05/06/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "KillaCoptuz3000/src/Behaviour/CWaypoint.h"
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <math.h>
#include <float.h>
#include <string>
#include "KillaCoptuz3000/src/Functions.h"

CWaypoint::CWaypoint()
{
   m_waitCount = 0;
}

CWaypoint::~CWaypoint()
{

}

bool CWaypoint::load(TiXmlNode* t_nodePtr)
{
   TiXmlNode*     a_nodePtr   = 0;
   TiXmlElement*  a_elemPtr   = 0;

   std::string    a_str;   
   bool           r_ret = true;

   a_elemPtr = t_nodePtr->ToElement();

   if (getAttributeStr(a_elemPtr, "x", a_str))
      m_point.x = static_cast<float>(atof(a_str.c_str()));   
   else
      r_ret = false;

   if (getAttributeStr(a_elemPtr, "y", a_str))
      m_point.y = static_cast<float>(atof(a_str.c_str()));   
   else
      r_ret = false;

   if (getAttributeStr(a_elemPtr, "waittime", a_str))
      m_waitTime = atoi(a_str.c_str());   
   else
      r_ret = false;

   if (getAttributeStr(a_elemPtr, "v", a_str))
      m_v = static_cast<float>(atof(a_str.c_str()));   
   else
      r_ret = false;

   m_oldDist = FLT_MAX;
  
   return r_ret;
}

bool CWaypoint::update(SBehaviorData& t_data)
{   
   bool     r_ret = false;

   float    a_dx  = m_point.x - *t_data.m_xPosPtr; 
   float    a_dy  = m_point.y - *t_data.m_yPosPtr;

   if (a_dx > 0)
   {
      *t_data.m_direction = true;
   }
   else
   {
      *t_data.m_direction = false;
   }

   float    a_dist = sqrt(a_dx*a_dx + a_dy*a_dy);

   *t_data.m_dxPtr = 0.;
   *t_data.m_dyPtr = 0.;

   if(a_dist > m_oldDist || a_dist == 0)
   {
      m_oldDist = FLT_MAX;

      // Wait for m_waitTime
      if (m_waitCount < m_waitTime)
      {
         m_waitCount++;         
      }
      else
      {
         m_waitCount = 0;
         r_ret       = true;
      }           
   }
   else
   {
      // Position
      *t_data.m_dxPtr = a_dx / a_dist * m_v;
      *t_data.m_xPosPtr += *t_data.m_dxPtr;

      *t_data.m_dyPtr = a_dy / a_dist * m_v;
      *t_data.m_yPosPtr += *t_data.m_dyPtr;

      // Translate hull polygon
      // t_data.m_hullPolygonPtr->translate(*t_data.m_dxPtr, *t_data.m_dyPtr);

      m_oldDist = a_dist;
   }
   
   return r_ret;
}