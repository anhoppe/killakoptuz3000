/*
   Class Name:

      CTGALoader.

   Created by:

      Allen Sherrod (Programming Ace of www.UltimateGameProgramming.com).

   Description:

      This class loads a tga texture into this object.
*/


#ifndef CTGALOADER_H
#define CTGALOADER_H

#include<stdio.h>


class CTGALoader
{
   public:
      CTGALoader();
      ~CTGALoader();

      bool LoadTGA(const char *file);
      void FreeImage();

      unsigned int ID;
      int imageWidth;
      int imageHeight;
      unsigned char *image;
      int type;

      char* filename;
};

#endif


// Copyright June 2004
// All Rights Reserved!
// Allen Sherrod
// ProgrammingAce@UltimateGameProgramming.com
// www.UltimateGameProgramming.com
