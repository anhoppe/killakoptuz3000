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
#include "KillaCoptuz3000/src/CPolygon.h"
#include "tinyxml/tinyxml.h"
#include "glut/glut.h"

#include "KillaCoptuz3000/src/globals.h"

class CTexture
{
public:
   CTexture();
   CTexture(TiXmlElement* t_elemPtr, const std::string& t_texturePath);
   ~CTexture();

#if(PRODUCT == LE3000)
   /**
   *  Loads the texture content without XML file
   *  @param t_texturePath directory of the GFX files
   *  @param t_baseFileName file name
   *  @param t_gfxType extension of the file
   *  @param t_hullPoints hull points for the file
   */
   void loadFromBaseFile(const std::string& t_texturePath, const std::string& t_baseFileName, const std::string& t_gfxType, int t_hullPoints);
   std::string m_baseFileName;
   std::string m_gfxType;
   int         m_hullPoints;
#endif

private:
   // Loads a texture via TGA loader
   static GLuint loadTexture(CTexture* t_texturePtr, const char* t_filename, const std::string& t_gfxType, unsigned int t_nPolyPoints);

   // Constructs all files from a base file name and loads textures
   static bool loadTextureBase(CTexture* t_texturePtr, const std::string& t_texturePath, const std::string& t_baseFileName, const std::string& t_gfxType, unsigned int t_nPolyPoints = 4);   

public:
   // List of textures for the object
   std::vector <GLuint> m_textureIdVector;

   /** hull polygon description for collision */
   CPolygon*            m_hullPolygonPtr;
};

#endif
