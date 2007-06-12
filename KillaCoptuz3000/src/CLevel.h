/***************************************************************
 CLevel   version:  1.0   ·  date: 04/07/2007
 -------------------------------------------------------------
 
 -------------------------------------------------------------
 Copyright (C) 2007 - All Rights Reserved
***************************************************************

***************************************************************/

#ifndef CLEVEL_H
#define CLEVEL_H

#include <string>

#include "tinyxml/tinyxml.h"
#include "fmod.h"


class CLevel
{
   //////////////////////////////////////////////////////////////////////////
   // Methods
   //////////////////////////////////////////////////////////////////////////
public:
   CLevel();
   ~CLevel();

   /** Load level file */
   bool load(TiXmlNode* t_nodePtr);

   /** Check if position is inside level boundary*/
   bool positionAllowed(const float& t_xPos, const float& t_yPos);

private:

   /** Process events */
   void processEvents();

   //////////////////////////////////////////////////////////////////////////
   // Member variables
   //////////////////////////////////////////////////////////////////////////
public:
   double               m_minX;
   double               m_minY;
   double               m_maxX;
   double               m_maxY;

private:
   FSOUND_STREAM*      m_sound;

};

#endif

// #include <list>
// #include <map>
// #include "fmod.h"
// #include "objects/CPlayer.h"
// #include "CPoint.h"
// #include "CTexture.h"
// 
// class CObject;
// class CPlayer;
// 
// class CLevel
// {
// public:
//    CLevel();
//    ~CLevel();
// 
//    bool load(const char* t_fileName);
// 
//    /** Loads object list from level xml */
//    bool loadObjectList(TiXmlNode* t_nodePtr);
// 
//    /** loads texture list from level xml and stores it in std::map object */
//    bool loadTextureMap(TiXmlNode* t_nodePtr);
// 
//    /** Updates all individuals in the level */
//    void updateLevel();
// 
//    /** Draws all individuals */
//    void drawLevel();
// 
//    // Cycle textures of all objects
//    void nextTexture();
// 
//    // Start playing the level music
//    bool playMusic();
// 
//    // Check for collisions of player with level objects
//    bool isCollided(CPlayer* t_playerPtr);   
// 
//    double               m_minX;
//    double               m_minY;
//    double               m_maxX;
//    double               m_maxY;
// 
//    // Pointer to the player
//    //CPlayer*             m_playerPtr;
// 
//    /** Object list */
//    static std::vector<CObject*> M_objects;
// 
//    /** Objects deleted from M_objects list in update function */
//    static std::vector<CObject*> M_deleteList;
// 
//    static std::vector<CObject*> M_addList; 
// 
//    /** texturemap - map ;-) */
//    static std::map<std::string, CTexture*> M_textureMap;
//    
//    /** Indicates that the player won or lost the level */
//    bool                          m_levelOver;
// 
// private:
//    // Test for intersection of the segments ab and cd
//    bool segmentsIntersect(CLine l1, CLine l2);
// 
//    // Test 3 points for being counter clockwise ordered
//    int ccw(CPoint p0, CPoint p1, CPoint p2);
// 
//    FSOUND_STREAM*      m_sound;
// 
// };
// 
