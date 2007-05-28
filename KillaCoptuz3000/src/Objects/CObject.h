// ***************************************************************
//  CObject   version:  1.0   ·  date: 04/07/2007
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

enum VeObjectType
{
   e_object = 1,
   e_sprite = 2,
   e_enemy  = 3,
   e_shot   = 4,
   e_weapon = 5,
   e_player = 6
};

class CObject
{
   //////////////////////////////////////////////////////////////////////////
   // Methods
   //////////////////////////////////////////////////////////////////////////
public:
   CObject();
   CObject(TiXmlNode* t_nodePtr);

   ~CObject();

   /** returns the type of the object */
   virtual VeObjectType getType() { return e_object; };

   /** Draws the object via OpenGL */
   virtual void draw();

   /** updates itself (scripted behavior) and all children */
   virtual void update(CLevel* m_levelPtr, std::vector<CObject*>::iterator& t_it, std::vector<CObject*>::iterator& t_endIt);

   /** Cycles the textures */
   void nextTexture();

   /** Return: number of textures in m_textureIdVector */
   size_t getTextureCount();

   /** Puts all children into the CLevel::M_deleteList */
   void deleteChildren();

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

   bool                 m_isDying;

   /** Absolute position */
   float                m_xPos;
   float                m_yPos;
   float                m_width;
   float                m_height;

   int                  m_cycleInterval;

   bool                 m_isBackground;

   std::string          m_gfxType;

   /** Index of the explosion (death) sequence */
   int                  m_explosionIndex;

   // Active texture (zero based index)
   unsigned int         m_activeTexture;

   unsigned int         m_activeAnimationPhase;

   // Angle of the sprite
   float                m_angle;

   /** Remember movement deltas for children updates */
   float                m_dx;
   float                m_dy;


   /** Hit points */
   int                  m_hitPoints;
   int                  m_maxHitPoints;

   /** Damage points (to others) */
   int                  m_damagePoints;

   /** Flag if object is invincible (set if no hit points are defined in xml file) */
   int                  m_invincible;

   /** Pointer to parent object */
   CObject*          m_parentPtr;

protected:
   // List of textures for the object
//    std::vector <GLuint> m_textureIdVector;
   std::vector<std::string>   m_textureKeys;

   // list of children
   std::vector<CObject*>      m_children;

   // Time counter for animation
   int                        m_timeCounter;   
};

#endif
