/*
   Class Name:

      CTGALoader.

   Created by:

      Allen Sherrod (Programming Ace of www.UltimateGameProgramming.com).

   Description:

      This class loads a tga texture into this object.
*/


#include "CTgaLoader.h"
#include <string.h>

#define ALPHA_RED    255
#define ALPHA_GREEN  255
#define ALPHA_BLUE   255

CTGALoader::CTGALoader()
{
   // Give everything default values.
  image = 0;
  type = 0;
  ID = -1;
  imageWidth = 0;
  imageHeight = 0;

  filename = NULL;
}


CTGALoader::~CTGALoader()
{
  if (filename)
    delete[] filename;


  FreeImage();
}


bool CTGALoader::LoadTGA(const char* file)
{
   FILE*          pfile;

   unsigned char  bitCount;

   int            colorMode;
   long           tgaSize;

   unsigned char  unCompressHeader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
   unsigned char  tgaHeader[12];
   unsigned char  header[6];
   unsigned char* tempBuffer = 0;

   unsigned char  red;
   unsigned char  green;
   unsigned char  blue;


   // Open the file.
   if(!file) return false;
   pfile = fopen(file, "rb");
   if(!pfile) return false;

   // Read the header.
   fread(tgaHeader, 1, sizeof(tgaHeader), pfile);

   // We only want to read uncompressed tga's.  Compare the header
   // with data that would be in a uncompressed header.
   if(memcmp(unCompressHeader, tgaHeader, sizeof(unCompressHeader)) != 0)
   {
	   fclose(pfile);
	   return false;
   }

   // Read image info.
   fread(header, 1, sizeof(header), pfile);

   // Calculate and save the width and height.
   imageWidth = header[1] * 256 + header[0];
   imageHeight = header[3] * 256 + header[2];

   bitCount = header[4];

   // calculate the color mode and image size.
   colorMode = bitCount / 8;
   tgaSize = imageWidth * imageHeight * colorMode;

   // Allocate data for the temp buffer and load it.
   tempBuffer = new unsigned char[sizeof(unsigned char) * tgaSize];
   fread(tempBuffer, sizeof(unsigned char), tgaSize, pfile);

   // allocate buffer for the image data with space for alpha channel
   image = new unsigned char[sizeof(unsigned char) * (tgaSize+tgaSize/colorMode)];

   // Convert from BGR to RGB format.
   for(long index = 0; index < tgaSize; index += colorMode)
   {
      // read red
      red = tempBuffer[index+2];

      // read green
      green = tempBuffer[index+1];

      // read blue
      blue = tempBuffer[index];

      image[index + index / colorMode]       = red;
      image[index + 1 + index / colorMode]   = green;
      image[index + 2 + index / colorMode]   = blue;
   
      // set alpha
      if((red == ALPHA_RED) && 
         (green == ALPHA_GREEN) && 
         (blue == ALPHA_BLUE))
      {
         image[index + 3 + index / colorMode] = 0;
      } 
      else
      {
         image[index + 3 + index / colorMode] = 255;
      }
   }

   delete[] tempBuffer;

   fclose(pfile);

   if(colorMode == 3) type = 3;
   else type = 4;

   // Filenamen als Member speichern
   if (filename)
     delete filename;
   filename = new char[strlen(file)+1];
   strcpy(filename, file);

   return true;
}


void CTGALoader::FreeImage()
{
   // When the application is done delete all memory.
   if(image)
      {
         delete[] image;
         image = NULL;
      }
}

 
// Copyright June 2004
// All Rights Reserved!
// Allen Sherrod
// ProgrammingAce@UltimateGameProgramming.com
// www.UltimateGameProgramming.com