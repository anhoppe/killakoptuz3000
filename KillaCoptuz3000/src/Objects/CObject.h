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
#include "glut/glut.h"
#include "KillaCoptuz3000/src/CPolygon.h"
#include "tinyxml/tinyxml.h"

class CLevel;

enum VeObjectType
{
   e_object    = 1,
   e_sprite    = 2,
   e_combatant = 3,
   e_enemy     = 4,
   e_shot      = 5,
   e_weapon    = 6,
   e_player    = 7
};

struct CTextureInfo
{
public:
   CTextureInfo() {};
   CTextureInfo(CTextureInfo* t_srcPtr)
   {
      m_textureKey = t_srcPtr->m_textureKey;
      m_polygonPtr = new CPolygon(t_srcPtr->m_polygonPtr);
   };

   std::string m_textureKey;
   CPolygon*   m_polygonPtr;
};


class CObject
{
   //////////////////////////////////////////////////////////////////////////
   // Methods
   //////////////////////////////////////////////////////////////////////////
public:
   CObject();

   ~CObject();

   /** returns the type of the object */
   virtual VeObjectType getType() { return e_object; };

   /** loads object content from XML file */
   virtual bool load(TiXmlNode* t_nodePtr);    

   /** Draws the object via OpenGL */
   virtual void draw();

   /** updates itself (scripted behavior) and all children */
   virtual void update(CLevel* m_levelPtr);

   /** Cycles the textures */
   void nextTexture();

   /** Initiate dying sequence for the object */
   virtual void startDying();

   /** Return: number of textures in m_textureIdVector */
   size_t getTextureCount();   

   /** Collision functions */
   static bool    segmentsIntersect(CLine& l1, CLine& l2);   
   static int     ccw(CPoint& p0, CPoint& p1, CPoint& p2);
   static bool    isCollided(CObject* t_firstPtr, CObject* t_secondPtr);
   virtual void   collisionImpact(CObject* t_objectPtr, bool t_checkOther = true);
   static bool    intersects(CObject* t_objectPtr, float t_top, float t_left, float t_right, float t_bottom);

protected:


   //////////////////////////////////////////////////////////////////////////
   // Variables
   //////////////////////////////////////////////////////////////////////////
public:
   bool                 m_isDying;

   /** Absolute position */
   float                m_xPos;
   float                m_yPos;
   float                m_width;
   float                m_height;

   int                  m_cycleInterval;

   bool                 m_isBackground;

   std::string          m_gfxType;

   /** Flag, indicates that object is dying (death animation, no more damage) /*
   bool                 m_isDying;

   /** Index of the explosion (death) sequence */
   int                  m_explosionIndex;

   // Active texture (zero based index)
   unsigned int         m_activeTexture;

   // 
   unsigned int         m_activeAnimationPhase;

   // Angle of the sprite
   float                m_angle;

   /** Start angle (only valid for child objects */
   float                m_startAngle;

   /** direction*/
   bool                 m_direction;

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

   /** Objects unique ID for storage class map key. 0 is illegal */
   unsigned int         m_id;

   /** Id of the parent. If it equals 0, object has no parent */
   unsigned int         m_parentId;

   /** layer position determines draw order */
   unsigned int         m_drawLayer;

   // list of children
   std::list<unsigned int>      m_children;

protected:
   // List of textures for the object
//    std::vector <GLuint> m_textureIdVector;
   std::vector<CTextureInfo*>   m_textureKeys;

   // Time counter for animation
   int                          m_timeCounter;
};

#endif
