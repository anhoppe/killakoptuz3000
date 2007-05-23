// ***************************************************************
//  CBehavior   version:  1.0   ·  date: 05/06/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef CBEHAVIOR_H
#define CBEHAVIOR_H

#include "CPolygon.h"
#include "tinyxml.h"
#include <vector>

struct SBehaviorData
{
   // Reference pointers to position of object
   float* m_xPosPtr;
   float* m_yPosPtr;

   // Delta positions, since last update
   float* m_dxPtr;
   float* m_dyPtr;

   // Initial positions
   float  m_x0;
   float  m_y0;
}; 

class CBehavior
{
public:
   CBehavior();
   ~CBehavior();

   virtual bool load(TiXmlNode* t_nodePtr) {return true;};
   virtual bool update(SBehaviorData& t_data) {return true;};
};

#endif
