// ***************************************************************
//  CScript   version:  1.0   ·  date: 05/06/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************
#ifndef CSCIPT_H
#define CSCIPT_H

#include <vector>
#include "KillaCoptuz3000/src/behaviour/CBehavior.h"

class CScript
{
public:
   CScript(const char* t_scriptFileName);
   ~CScript();

   std::vector<CBehavior*> m_behavior;

   void update(SBehaviorData& t_data);

   unsigned int m_activeBehavior;
};

#endif

