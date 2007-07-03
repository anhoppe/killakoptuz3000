// ***************************************************************
//  CTexture   version:  1.0   ·  date: 05/20/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#include "KillaCoptuz3000/src/CTexture.h"
#include "KillaCoptuz3000/src/Functions.h"
#include "KillaCoptuz3000/src/CTgaLoader.h "

#include "lodepng/lodepng.h"

#include <string>

#ifndef GFX_PATH
#define GFX_PATH "data/gfx/"
#endif

CTexture::CTexture()
{

}

CTexture::CTexture(TiXmlElement* t_elemPtr, const std::string& t_texturePath)
{
   TiXmlNode*     a_nodePtr      = 0;
   TiXmlElement*  a_elemPtr      = 0;

   std::string    a_baseFileName = "";
   std::string    a_gfxType      = "";

   std::string    a_str          = "";

   bool           r_ret          = true;


   // set hull to 0
   m_hullPolygonPtr = 0;

   // read base file name and hull info
   if(0 != t_elemPtr)
   {
      if(getAttributeStr(t_elemPtr, "basefilename", a_baseFileName))
      {
         if (getAttributeStr(t_elemPtr, "hullpoints", a_str))
         {
            if(getAttributeStr(t_elemPtr, "gfxType", a_gfxType))
            {
               assert(loadTextureBase(this, t_texturePath, a_baseFileName, a_gfxType, atoi(a_str.c_str())));
            }
         }                 
         else
         {
            if(getAttributeStr(t_elemPtr, "gfxType", a_gfxType))
            {
               assert(loadTextureBase(this, t_texturePath, a_baseFileName, a_gfxType));
            }
         }
      }      
   }
}

CTexture::~CTexture()
{
   if(0 != m_hullPolygonPtr)
   {
      delete m_hullPolygonPtr;
   }
}

#if(PRODUCT == LE3000)
void CTexture::loadFromBaseFile(const std::string& t_texturePath, const std::string& t_baseFileName, const std::string& t_gfxType, int t_hullPoints)
{
   m_baseFileName = t_baseFileName;
   m_gfxType      = t_gfxType;
   m_hullPoints   = t_hullPoints;
   
   loadTextureBase(this, t_texturePath, t_baseFileName, t_gfxType, t_hullPoints);
}
#endif

//////////////////////////////////////////////////////////////////////////
// Private members
//////////////////////////////////////////////////////////////////////////
bool CTexture::loadTextureBase(CTexture* t_texturePtr, const std::string& t_texturePath, const std::string& t_baseFileName, const std::string& t_gfxType, unsigned int t_nPolyPoints)
{
   std::string a_fileName     = "";
   int         a_count        = 0;
   bool        a_fileExists   = false;
   char        a_str[1024];
   FILE*       a_fPtr         = 0;
   GLuint      a_textureId;
   bool        r_ret          = true;
   errno_t     a_err;   

   do
   {
      a_fileName = t_texturePath;
      a_fileName += t_baseFileName;

      _itoa_s(a_count, a_str, 1024, 10);
      a_fileName += a_str;

      a_fileName += ".";
      a_fileName += t_gfxType;         

      a_err = fopen_s(&a_fPtr, a_fileName.c_str(), "rb");
      if(0 == a_err)
      {
         a_fileExists = true;
         fclose(a_fPtr);
      }
      else
      {
         a_fileExists = false;
      }

      if(a_fileExists)
      {
         a_textureId = loadTexture(t_texturePtr, a_fileName.c_str(), t_gfxType, t_nPolyPoints);
         if (a_textureId != -1)
            t_texturePtr->m_textureIdVector.push_back(a_textureId);
      }

      a_count++;
   } while(a_fileExists);

   return r_ret;  
}





GLuint CTexture::loadTexture(CTexture* t_texturePtr, const char* t_filename, const std::string& t_gfxType, unsigned int t_nPolyPoints)
{
   CTGALoader     a_tgaImage;
   unsigned char* a_image;
   unsigned int   a_width;
   unsigned int   a_height;
   GLuint         r_texture   = 0;   

   if (t_gfxType == "tga")
   {
      if (!a_tgaImage.LoadTGA(t_filename))
         return -1;

      a_image     = a_tgaImage.image;
      a_width     = a_tgaImage.imageWidth; 
      a_height    = a_tgaImage.imageHeight;
   }
   else if (t_gfxType == "png")
   {
      std::vector<unsigned char> a_buffer, a_tempImage;
      LodePNG::loadFile(a_buffer, t_filename); //load the image file with given filename
      LodePNG::Decoder a_decoder;
      a_decoder.decode(a_tempImage, &a_buffer[0], (unsigned long)a_buffer.size()); //decode the png
      if (a_decoder.hasError())
         return -1;

      a_width  = a_decoder.getWidth();
      a_height = a_decoder.getHeight();
      a_image = new unsigned char[a_width*a_height*4];
      unsigned int x,y;      
      for (y=0; y<a_height; y++)
      {
         for (x=0; x<a_width; x++)
         {
            a_image[4 * y * a_width + 4 * x + 0] = a_tempImage[4 * (a_height-1-y) * a_width + 4 * x + 0]; //red
            a_image[4 * y * a_width + 4 * x + 1] = a_tempImage[4 * (a_height-1-y) * a_width + 4 * x + 1]; //green
            a_image[4 * y * a_width + 4 * x + 2] = a_tempImage[4 * (a_height-1-y) * a_width + 4 * x + 2]; //blue
            a_image[4 * y * a_width + 4 * x + 3] = a_tempImage[4 * (a_height-1-y) * a_width + 4 * x + 3]; //alpha
         }
      }      
   }
   else return -1;   

   // Bind the texture

   // allocate a texture name
   glGenTextures( 1, &r_texture );

   // select our current texture
   glBindTexture( GL_TEXTURE_2D, r_texture );

   // select modulate to mix texture with color for shading
   glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

   // when texture area is small, bilinear filter the closest mipmap
   glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
      GL_LINEAR_MIPMAP_NEAREST );

   // when texture area is large, bilinear filter the first mipmap
   glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

   // if wrap is true, the texture wraps over at the edges (repeat)
   //       ... false, the texture ends at the edges (clamp)
   glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
      1 ? GL_REPEAT : GL_CLAMP );
   glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
      1 ? GL_REPEAT : GL_CLAMP );

   // build our texture mipmaps
   gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, a_width, a_height,
      GL_RGBA, GL_UNSIGNED_BYTE, a_image);   

   // Create hull polygon
    if (!t_texturePtr->m_hullPolygonPtr)
    {
       t_texturePtr->m_hullPolygonPtr = new CPolygon();
       t_texturePtr->m_hullPolygonPtr->createHull(a_image, a_width, a_height, t_nPolyPoints);       
    }

   return r_texture;
}

