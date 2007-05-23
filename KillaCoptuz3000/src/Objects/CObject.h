// ***************************************************************
//  CObject   version:  1.0   �  date: 04/07/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef COBJECT_H
#define COBJECT_H

#include <tchar.h>
#include <vector>
#include <string>
#include <list>
#include "glut.h"
#include "CPolygon.h"
#include "tinyxml.h"

class CLevel;

class CObject
{
   //////////////////////////////////////////////////////////////////////////
   // Methods
   //////////////////////////////////////////////////////////////////////////
public:
   CObject();
   CObject(TiXmlNode* t_nodePtr);

   ~CObject();

   /** Draws the object via OpenGL */
   virtual void draw();

   /** updates itself (scripted behavior) and all children */
   virtual void update(CLevel* m_levelPtr, std::vector<CObject*>::iterator& t_it, std::vector<CObject*>::iterator& t_endIt);

   // Cycles the textures
   void nextTexture();

   // Return: number of textures in m_textureIdVector
   unsigned int getTextureCount();

   /** Collision functions */
   static bool    segmentsIntersect(CLine l1, CLine l2);
   static int     ccw(CPoint p0, CPoint p1, CPoint p2);
   static bool    isCollided(CObject* t_firstPtr, CObject* t_secondPtr);
   virtual void   collisionImpact(CObject* t_objectPtr, bool t_checkOther = true);

protected:

   virtual bool   load(TiXmlNode* t_nodePtr);    

   //////////////////////////////////////////////////////////////////////////
   // Variables
   //////////////////////////////////////////////////////////////////////////
public:
   /** Flag shows that object is already in the delete list (to avoid double deletion) */
   bool                 m_isDeleted;

   /** Absolute position */
   float                m_xPos;
   float                m_yPos;
   float                m_width;
   float                m_height;   

   int                  m_cycleInterval;

   bool                 m_isBackground;

   std::string          m_gfxType;

   // Active texture (zero based index)
   unsigned int         m_activeTexture;

   unsigned int         m_activeAnimationPhase;

   // Angle of the sprite
   float                m_angle;

   /** Remember movement deltas for children updates */
   float       m_dx;
   float       m_dy;


   /** Hit points */
   int                  m_hitPoints;

   /** Damage points (to others) */
   int                  m_damagePoints;

   /** Flag if object is invincible (set if no hit points are defined in xml file) */
   int                  m_invincible;

protected:
   // List of textures for the object
//    std::vector <GLuint> m_textureIdVector;
   std::vector<std::string>   m_textureKeys;

   // Time counter for animation
   int                  m_timeCounter;   
};

#endif