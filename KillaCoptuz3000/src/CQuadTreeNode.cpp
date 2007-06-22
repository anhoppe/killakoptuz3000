// ***************************************************************
//  CQuadTreeNode   version:  1.0   ·  date: 06/15/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "CQuadTreeNode.h"
#include "CObjectStorage.h"
#include "CEvent.h"

//////////////////////////////////////////////////////////////////////////
// Initialize static members
//////////////////////////////////////////////////////////////////////////
unsigned int CQuadTreeNode::M_maxDepth = 4;

//////////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////////
CQuadTreeNode::CQuadTreeNode(unsigned int t_depth, float t_top, float t_left, float t_right, float t_bottom)
{
   m_depth  = t_depth;

   m_top    = t_top;
   m_left   = t_left;
   m_right  = t_right;
   m_bottom = t_bottom;

   initChildren();
}

CQuadTreeNode::~CQuadTreeNode()
{
   unsigned int a_index = 0;

   for(a_index = 0; a_index <4; a_index++)
   {
      if(0 != m_children[a_index])
      {
         delete m_children[a_index];
      }
   }
}

void CQuadTreeNode::add(CObject* t_objectPtr)
{
   CQuadTreeNode* a_nodePtr   = 0;

   float          a_xMiddle   = 0.;
   float          a_yMiddle   = 0.;

   //////////////////////////////////////////////////////////////////////////
   // check top left square
   if(m_depth < M_maxDepth)
   {
      a_xMiddle = m_left   + (m_right-m_left)/2;
      a_yMiddle = m_bottom + (m_top-m_bottom)/2;

      // top left
      if(t_objectPtr->intersects(t_objectPtr, m_top, m_left, a_xMiddle, a_yMiddle))
      {
         if(0 == m_children[0])
         {
            m_children[0] = new CQuadTreeNode(m_depth+1, m_top, m_left, a_xMiddle, a_yMiddle);
         }
         m_children[0]->add(t_objectPtr);
      }

      // top right
      if(t_objectPtr->intersects(t_objectPtr, m_top, a_xMiddle, m_right, a_yMiddle))
      {
         if(0 == m_children[1])
         {
            m_children[1] = new CQuadTreeNode(m_depth+1, m_top, a_xMiddle, m_right, a_yMiddle);
         }
         m_children[1]->add(t_objectPtr);
      }

      // bottom left
      if(t_objectPtr->intersects(t_objectPtr, a_yMiddle, m_left, a_xMiddle, m_bottom))
      {
         if(0 == m_children[2])
         {
            m_children[2] = new CQuadTreeNode(m_depth+1, a_yMiddle, m_left, a_xMiddle, m_bottom);
         }
         m_children[2]->add(t_objectPtr);
      }

      // bottom right
      if(t_objectPtr->intersects(t_objectPtr, a_yMiddle, a_xMiddle, m_right, m_bottom))
      {
         if(0 == m_children[3])
         {
            m_children[3] = new CQuadTreeNode(m_depth+1, a_yMiddle, a_xMiddle, m_right, m_bottom);
         }
         m_children[3]->add(t_objectPtr);
      }
   }
   else
   {
      m_objectList.push_back(t_objectPtr);
   }

}

bool CQuadTreeNode::remove(CObject* t_nodePtr)
{
   bool r_ret = false;

   // remove object from own list
   if(m_objectList.size() > 0)
   {
      m_objectList.remove(t_nodePtr);
   }

   // remove object from children
   for (unsigned int i = 0; i<4; i++)
   {
      if (0 != m_children[i])
      {
         r_ret = true;

         // remove child if it has no more children and an empty object list
         if(!m_children[i]->remove(t_nodePtr))
         {
            if(m_children[i]->m_objectList.size() == 0)
            {
               delete m_children[i];
               m_children[i] = 0;
            }
         }
      }      
   }

   return r_ret;
}

void CQuadTreeNode::getCollisionEvents()
{
   CEvent* a_eventPtr = 0;
   std::list<CObject*>::iterator a_it;

   std::list<CObject*>::iterator a_firstInnerIt;
   std::list<CObject*>::iterator a_innerIt;   

   if(m_objectList.size() > 1)
   {
      for(a_it = m_objectList.begin(); a_it != m_objectList.end(); a_it++)
      {
         a_firstInnerIt = a_it;
         a_firstInnerIt++;

         for(a_innerIt = a_firstInnerIt; a_innerIt != m_objectList.end(); ++a_innerIt)
         {
            if(CObject::isCollided((*a_it), (*a_innerIt)))
            {
               a_eventPtr = new CEvent(e_collided);
               a_eventPtr->m_objectList.push_back((*a_it)->m_id);
               a_eventPtr->m_objectList.push_back((*a_innerIt)->m_id);

               CObjectStorage::getInstance().m_eventList.push_back(a_eventPtr);
            }
         }
      }
   }
   else
   {
      for (unsigned int i = 0; i<4; i++)
      {
         if (0 != m_children[i])
         {
            m_children[i]->getCollisionEvents();
         }
      }
   }
}

void CQuadTreeNode::draw()
{
   glPushMatrix();
   
   glBegin(GL_LINE_LOOP);

   glColor4f(1.0,1.0,0.0,0.9);

   // Box
   glVertex2f(m_left, m_top);
   glVertex2f(m_right, m_top);
   glVertex2f(m_right, m_bottom);
   glVertex2f(m_left, m_bottom);
   glEnd();

//    // Crossing
//    glBegin(GL_LINES);
//    glColor4f(1.0,1.0,0.0,0.2);
//    glVertex2f(m_left, m_top);
//    glVertex2f(m_right, m_bottom);
//    glVertex2f(m_left, m_bottom);
//    glVertex2f(m_right, m_top);
//    glEnd();

   glColor4f(1.0,1.0,1.0,1.0);

   glPopMatrix();

   for(unsigned int a_index = 0; a_index < 4; a_index++)
   {
      if(0 != m_children[a_index] )
      {
         m_children[a_index]->draw();
      }
   }
   
}
//////////////////////////////////////////////////////////////////////////
// Private Methods
//////////////////////////////////////////////////////////////////////////
void CQuadTreeNode::initChildren()
{
   int a_index = 0;

   for(a_index = 0; a_index < 4; a_index++)
   {
      m_children[a_index] = 0;
   }
}
