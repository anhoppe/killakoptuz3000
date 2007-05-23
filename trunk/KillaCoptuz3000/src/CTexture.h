// ***************************************************************
//  CTexture   version:  1.0   ·  date: 05/20/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef CTEXTURE_H
#define CTEXTURE_H

#include <vector>
#include "CPolygon.h"
#include "tinyxml.h"
#include "glut.h"

class CTexture
{
public:
   CTexture();
   CTexture(TiXmlElement* t_elemPtr);
   ~CTexture();

private:
   // Loads a texture via TGA loader
   static GLuint loadTexture(CTexture* t_texturePtr, const char* t_filename, const std::string& t_gfxType, unsigned int t_nPolyPoints);

   // Constructs all files from a base file name and loads textures
   static bool loadTextureBase(CTexture* t_texturePtr, const std::string& t_baseFileName, const std::string& t_gfxType, unsigned int t_nPolyPoints = 4);   

public:
   // List of textures for the object
   std::vector <GLuint> m_textureIdVector;

   /** hull polygon description for collision */
   CPolygon*            m_hullPolygonPtr;
};

#endif
