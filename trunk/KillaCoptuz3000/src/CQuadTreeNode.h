// ***************************************************************
//  CQuadTreeNode   version:  1.0   ·  date: 06/15/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef CQUADTREENODE_H
#define CQUADTREENODE_H

#include <list>

#include "Objects/CObject.h"

class CQuadTreeNode
{
   //////////////////////////////////////////////////////////////////////////
   // Method
   //////////////////////////////////////////////////////////////////////////
public:
   CQuadTreeNode(unsigned int t_depth, float t_top, float t_left, float t_right, float t_bottom);
   ~CQuadTreeNode();
   
   void add(CObject* t_nodePtr);
   bool remove(CObject* t_nodePtr);
   void draw();

   void getCollisionEvents();

private:
   void initChildren();

   //////////////////////////////////////////////////////////////////////////
   // Variable
   //////////////////////////////////////////////////////////////////////////
public:
   /** Maximal depth for the whole tree */
   static unsigned int  M_maxDepth;

private:
   /** depth of this node */
   unsigned int         m_depth;

   /** Array with children */
   CQuadTreeNode*       m_children[4];

   /** List with all objects in this node */
   std::list<CObject*>  m_objectList;

   /** Index of last iterated sub - node*/
   int                  m_lastNodeInedex;

   float                m_top;
   float                m_left;
   float                m_right;
   float                m_bottom;

};    

#endif
