// ***************************************************************
//  CObject   version:  1.0   ·  date: 04/07/2007
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************


#include "Objects/CObject.h"
#include "CTgaLoader.h"
#include "Functions.h"
#include "lodepng.h"
#include "CLevel.h"

#include "CTexture.h"

#include <stdio.h>
#include <string>


extern bool g_showHull;

CObject::CObject()
{
   m_activeTexture         = 0;
   m_timeCounter           = 0;      
   m_angle                 = 0.;   
   m_dx                    = 0.;
   m_dy                    = 0.;
   m_hitPoints             = 1;
   m_invincible            = false;
   m_damagePoints          = 1;
   m_isDeleted             = false;
   m_activeAnimationPhase  = 0;
}

CObject::CObject(TiXmlNode* t_nodePtr)
{
   m_activeTexture         = 0;
   m_timeCounter           = 0;   
   m_angle                 = 0.;   
   m_dx                    = 0.;
   m_hitPoints             = 1;
   m_invincible            = false;
   m_damagePoints          = 1;
   m_isDeleted             = false;
   m_activeAnimationPhase  = 0;

   load(t_nodePtr);
}

CObject::~CObject()
{   
}

void CObject::nextTexture()
{
   m_timeCounter++;
   if (m_timeCounter >= m_cycleInterval)
   {
      m_activeTexture++;
      if (m_activeTexture >= CLevel::M_textureMap[m_textureKeys[m_activeAnimationPhase]]->m_textureIdVector.size())
      {
         m_activeTexture = 0;
      }

      m_timeCounter = 0;
   }
}

// Return: number of textures in m_textureIdVector
unsigned int CObject::getTextureCount()
{
   return CLevel::M_textureMap[m_textureKeys[m_activeAnimationPhase]]->m_textureIdVector.size();
}

bool CObject::load(TiXmlNode* t_nodePtr)
{
   TiXmlNode*     a_nodePtr      = 0;
   TiXmlElement*  a_elemPtr      = 0;

   TiXmlNode*     a_subNodePtr   = 0;
   TiXmlElement*  a_subElemPtr   = 0;


   std::string    a_str;
   bool           r_ret          = true;
   

   a_elemPtr = t_nodePtr->ToElement();
   
   r_ret = r_ret & getAttributeStr(a_elemPtr, "xpos", a_str);
   m_xPos         = atof(a_str.c_str());

   r_ret = r_ret & getAttributeStr(a_elemPtr, "ypos", a_str);
   m_yPos         = atof(a_str.c_str());

   r_ret = r_ret & getAttributeStr(a_elemPtr, "width", a_str);
   m_width        = atof(a_str.c_str());

   r_ret = r_ret & getAttributeStr(a_elemPtr, "height", a_str);
   m_height       = atof(a_str.c_str());

   r_ret = r_ret & getAttributeStr(a_elemPtr, "cycleInterval", a_str);
   m_cycleInterval= atoi(a_str.c_str());  

   // iterate over all texture tags in a texture list
   TiXmlNode*  a_textureNodePtr = 0;

   a_textureNodePtr = t_nodePtr->FirstChild("texturelist");

   if(0 != a_textureNodePtr)
   {
      for(a_subNodePtr = a_textureNodePtr->FirstChild("texture"); a_subNodePtr; a_subNodePtr = a_textureNodePtr->IterateChildren("texture", a_subNodePtr))
      {
         a_subElemPtr = a_subNodePtr->ToElement();

         r_ret &= getAttributeStr(a_subElemPtr, "key", a_str);

         if(r_ret)
         {
            m_textureKeys.push_back(a_str);
         }
      }
   }

   if (getAttributeStr(a_elemPtr, "hitpoints", a_str))
   {
      m_hitPoints = atoi(a_str.c_str());
   }
   else
   {
      m_invincible = true;
   }

   if (getAttributeStr(a_elemPtr, "damagepoints", a_str))
   {
      m_damagePoints = atoi(a_str.c_str());
   }   

   if (getAttributeStr(a_elemPtr, "background", a_str))
   {
      m_isBackground = atoi(a_str.c_str()) == 1;
   }
   else
   {
      m_isBackground = false;
   }   

   // Loop over all children of <object>
//    for(a_nodePtr = t_nodePtr->FirstChild(); a_nodePtr; a_nodePtr = t_nodePtr->IterateChildren(a_nodePtr))
//    {
//       a_elemPtr = a_nodePtr->ToElement();
// 
//       if(!strcmp("texturelist", a_elemPtr->Value()))
//       {
//          loadTextureList(a_nodePtr);
//       }      
//    }

   return r_ret;
}

// bool CObject::loadTextureList(TiXmlNode* t_nodePtr)
// {
//    TiXmlNode*     a_nodePtr = 0;
//    TiXmlElement*  a_elemPtr = 0;
//    std::string    a_baseFileName;
//    std::string    a_str;
// 
//    bool r_ret = true;
// 
// 
//    // loop over all base file names in the texture list
//    for(a_nodePtr = t_nodePtr->FirstChild(); a_nodePtr; a_nodePtr = t_nodePtr->IterateChildren(a_nodePtr))
//    {
//       if (!strcmp(a_nodePtr->Value(), "texture"))
//       {      
//          a_elemPtr = a_nodePtr->ToElement();
//          if (getAttributeStr(a_elemPtr, "basefilename", a_baseFileName))
//          {
//             if (getAttributeStr(a_elemPtr, "hullpoints", a_str))
//             {
//                loadTextureBase(a_baseFileName, atoi(a_str.c_str()));
//             }                 
//             else
//             {
//                loadTextureBase(a_baseFileName);
//             }
//          }      
//       }
//    }
// 
//    return r_ret;
// }

void CObject::draw() 
{
   glEnable( GL_TEXTURE_2D );
   glBindTexture(GL_TEXTURE_2D, CLevel::M_textureMap[m_textureKeys[m_activeAnimationPhase]]->m_textureIdVector[m_activeTexture]);
      
   glPushMatrix();

   glBegin( GL_QUADS );
   glTexCoord2d(0.0,0.0); 
   glVertex2d(0.0+m_xPos,0.0+m_yPos);

   glTexCoord2d(1.0,0.0);
   glVertex2d(m_width+m_xPos,0.0+m_yPos);

   glTexCoord2d(1.0,1.0); 
   glVertex2d(m_width+m_xPos,m_height+m_yPos);

   glTexCoord2d(0.0,1.0); 
   glVertex2d(0.0+m_xPos,m_height+m_yPos);

   glEnd();

   glPopMatrix();
   glDisable( GL_TEXTURE_2D );

   CPolygon* a_polygonPtr = CLevel::M_textureMap[m_textureKeys[m_activeAnimationPhase]]->m_hullPolygonPtr;
   if (g_showHull && 0 != a_polygonPtr)
   {      
      // DEBUG: SHOW POLYGON HULL FOR COLLISION DETECTION

      // Scale polygon to correct width and height
      a_polygonPtr->rescale(m_width / a_polygonPtr->m_width, m_height / a_polygonPtr->m_height);

      // Translate polygon to correct position
      a_polygonPtr->translate(m_xPos, m_yPos);

      glBegin(GL_LINE_LOOP);
      glColor3f(1.0,1.0,1.0);
      std::vector<CPoint*>::iterator a_it;
      for (a_it = a_polygonPtr->m_points.begin(); a_it != a_polygonPtr->m_points.end(); a_it++)
      {      
         glVertex2d((*a_it)->x, (*a_it)->y);      
      }
      glEnd();

      // Undo of: Translate polygon to correct position
      a_polygonPtr->translate(-m_xPos, -m_yPos);

      // Undo of: Scale polygon to correct width and height
      a_polygonPtr->rescale(a_polygonPtr->m_width / m_width, a_polygonPtr->m_height / m_height);

      // DEBUG END
   }
}



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Collision stuff
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Test for intersection of the segments
// See: Sedgewick, Algorithms
bool CObject::segmentsIntersect(CLine l1, CLine l2)
{
   bool r_ret;

   r_ret =  ((ccw(l1.p1, l1.p2, l2.p1)*ccw(l1.p1,l1.p2,l2.p2))<=0)&&
      ((ccw(l2.p1, l2.p2, l1.p1)*ccw(l2.p1,l2.p2,l1.p2))<=0);

   return r_ret;
}

// Test 3 points for being counter clockwise ordered
// See: Sedgewick, Algorithms
int CObject::ccw(CPoint p0, CPoint p1, CPoint p2)
{
   float dx1, dx2, dy1, dy2;
   int   r_ret;

   dx1 = p1.x - p0.x;
   dy1 = p1.y - p0.y;

   dx2 = p2.x - p0.x;
   dy2 = p2.y - p0.y;

   if (dx1*dy2 > dy1*dx2)
      r_ret =  1;
   if (dx1*dy2 < dy1*dx2)
      r_ret =  -1;
   if (dx1*dy2 == dy1*dx2)
   {
      if ((dx1*dx2<0)||(dy1*dy2<0))
         r_ret =  -1;
      else
      {
         if ((dx1*dx1+dy1*dy1)>=(dx2*dx2+dy2*dy2))
            r_ret =  0;
         else
            r_ret =  1;
      }
   }

   return r_ret;
}


// Check for collisions of player with level objects
bool CObject::isCollided(CObject* t_firstPtr, CObject* t_secondPtr)
{   
   bool           r_ret = false;
   unsigned int   a_n;
   CPoint         a_p1, a_p2, a_p3, a_p4;
   CPoint         a_o1, a_o2, a_o3, a_o4;
   CLine          a_pl1, a_pl2, a_pl3, a_pl4;
   CLine          a_ol1, a_ol2, a_ol3, a_ol4;

   // We can only collide with non - background objects
   if(t_firstPtr->m_isBackground || t_secondPtr->m_isBackground)
   {
      r_ret = false;
   }
   else
   {
      // Object 1 square coordinates
      a_o1.x = t_firstPtr->m_xPos;
      a_o1.y = t_firstPtr->m_yPos;

      a_o2.x = t_firstPtr->m_xPos + t_firstPtr->m_width;
      a_o2.y = t_firstPtr->m_yPos;

      a_o3.x = t_firstPtr->m_xPos + t_firstPtr->m_width;
      a_o3.y = t_firstPtr->m_yPos + t_firstPtr->m_height;

      a_o4.x = t_firstPtr->m_xPos;
      a_o4.y = t_firstPtr->m_yPos + t_firstPtr->m_height;

      // Rotate with first object angle   
      CPoint a_middlePoint = CPoint(a_o1.x + t_firstPtr->m_width/2, a_o1.y + t_firstPtr->m_height/2);
      a_o1 = a_o1 - a_middlePoint;
      a_o2 = a_o2 - a_middlePoint;
      a_o3 = a_o3 - a_middlePoint;
      a_o4 = a_o4 - a_middlePoint;

      a_o1.rotate(t_firstPtr->m_angle);   
      a_o2.rotate(t_firstPtr->m_angle);
      a_o3.rotate(t_firstPtr->m_angle);
      a_o4.rotate(t_firstPtr->m_angle);

      a_o1 = a_o1 + a_middlePoint;
      a_o2 = a_o2 + a_middlePoint;
      a_o3 = a_o3 + a_middlePoint;
      a_o4 = a_o4 + a_middlePoint;
     
      a_ol1.p1 = a_o1;
      a_ol1.p2 = a_o2;

      a_ol2.p1 = a_o2;
      a_ol2.p2 = a_o3;

      a_ol3.p1 = a_o3;
      a_ol3.p2 = a_o4;

      a_ol4.p1 = a_o4;
      a_ol4.p2 = a_o1;

      // Object 2 square coordinates  
      a_p1.x = t_secondPtr->m_xPos;
      a_p1.y = t_secondPtr->m_yPos;

      a_p2.x = t_secondPtr->m_xPos + t_secondPtr->m_width;
      a_p2.y = t_secondPtr->m_yPos;

      a_p3.x = t_secondPtr->m_xPos + t_secondPtr->m_width;
      a_p3.y = t_secondPtr->m_yPos + t_secondPtr->m_height;

      a_p4.x = t_secondPtr->m_xPos;        
      a_p4.y = t_secondPtr->m_yPos + t_secondPtr->m_height;

      // Rotate with helicopter angle
      a_middlePoint = CPoint(a_p1.x + t_secondPtr->m_width/2, a_p1.y + t_secondPtr->m_height/2);
      a_p1 = a_p1 - a_middlePoint;
      a_p2 = a_p2 - a_middlePoint;
      a_p3 = a_p3 - a_middlePoint;
      a_p4 = a_p4 - a_middlePoint;

      a_p1.rotate(t_secondPtr->m_angle);   
      a_p2.rotate(t_secondPtr->m_angle);
      a_p3.rotate(t_secondPtr->m_angle);
      a_p4.rotate(t_secondPtr->m_angle);

      a_p1 = a_p1 + a_middlePoint;
      a_p2 = a_p2 + a_middlePoint;
      a_p3 = a_p3 + a_middlePoint;
      a_p4 = a_p4 + a_middlePoint;

      // Lines
      a_pl1.p1 = a_p1;
      a_pl1.p2 = a_p2;

      a_pl2.p1 = a_p2;
      a_pl2.p2 = a_p3;

      a_pl3.p1 = a_p3;
      a_pl3.p2 = a_p4;

      a_pl4.p1 = a_p4;
      a_pl4.p2 = a_p1;           

      //////////////////////////////////////////////////////////////////////////
      // (1.) 
      // Test for intersection of player 
      // square (tilted with m_angle)
      // and object square
      //////////////////////////////////////////////////////////////////////////

      // Test all 4 border lines of player square
      // for intersection with all 4 object square lines

      if (segmentsIntersect(a_pl1, a_ol1))
         r_ret = true;
      if (segmentsIntersect(a_pl1, a_ol2))
         r_ret = true;
      if (segmentsIntersect(a_pl1, a_ol3))
         r_ret = true;
      if (segmentsIntersect(a_pl1, a_ol4))
         r_ret = true;

      if (segmentsIntersect(a_pl2, a_ol1))
         r_ret = true;
      if (segmentsIntersect(a_pl2, a_ol2))
         r_ret = true;
      if (segmentsIntersect(a_pl2, a_ol3))
         r_ret = true;
      if (segmentsIntersect(a_pl2, a_ol4))
         r_ret = true;

      if (segmentsIntersect(a_pl3, a_ol1))
         r_ret = true;
      if (segmentsIntersect(a_pl3, a_ol2))
         r_ret = true;
      if (segmentsIntersect(a_pl3, a_ol3))
         r_ret = true;
      if (segmentsIntersect(a_pl3, a_ol4))
         r_ret = true;

      if (segmentsIntersect(a_pl4, a_ol1))
         r_ret = true;
      if (segmentsIntersect(a_pl4, a_ol2))
         r_ret = true;
      if (segmentsIntersect(a_pl4, a_ol3))
         r_ret = true;
      if (segmentsIntersect(a_pl4, a_ol4))
         r_ret = true;

      //////////////////////////////////////////////////////////////////////////
      // (2.) 
      // We have a collision candidate, look closer
      // Check all polygon points for being inside target polygon
      //////////////////////////////////////////////////////////////////////////
      if (r_ret)
      {         
         r_ret = false;

         CPolygon* a_polygonAPtr = CLevel::M_textureMap[t_firstPtr->m_textureKeys[t_firstPtr->m_activeAnimationPhase]]->m_hullPolygonPtr;
         CPolygon* a_polygonBPtr = CLevel::M_textureMap[t_secondPtr->m_textureKeys[t_secondPtr->m_activeAnimationPhase]]->m_hullPolygonPtr;
         
         if((0 != a_polygonAPtr) &&
            (0 != a_polygonBPtr)    )
         {

            // Scale polygon A to correct width and height
            a_polygonAPtr->rescale(t_firstPtr->m_width / a_polygonAPtr->m_width, t_firstPtr->m_height / a_polygonAPtr->m_height);     

            // Translate polygon A to correct position
            a_polygonAPtr->translate(t_firstPtr->m_xPos, t_firstPtr->m_yPos);

            // Rotate polygon A
            a_polygonAPtr->rotate(-t_firstPtr->m_angle, t_firstPtr->m_xPos + t_firstPtr->m_width/2.0, t_firstPtr->m_yPos + t_firstPtr->m_height/2.0);

            // Scale polygon B to correct width and height
            a_polygonBPtr->rescale(t_secondPtr->m_width / a_polygonBPtr->m_width, t_secondPtr->m_height / a_polygonBPtr->m_height);     

            // Translate polygon B to correct position
            a_polygonBPtr->translate(t_secondPtr->m_xPos, t_secondPtr->m_yPos);

            // Rotate polygon B
            a_polygonBPtr->rotate(-t_secondPtr->m_angle, t_secondPtr->m_xPos + t_secondPtr->m_width/2.0, t_secondPtr->m_yPos + t_secondPtr->m_height/2.0);

            for (a_n = 0; a_n < a_polygonAPtr->m_points.size(); a_n++)
            {
               if (a_polygonBPtr->isInside(a_polygonAPtr->m_points[a_n]))
               {
                  r_ret = true;
                  break;
               }
            }

            // Undo of: Rotate polygon A
            a_polygonAPtr->rotate(t_firstPtr->m_angle, t_firstPtr->m_xPos + t_firstPtr->m_width/2.0, t_firstPtr->m_yPos + t_firstPtr->m_height/2.0);

            // Undo of: Translate polygon A to correct position
            a_polygonAPtr->translate(-t_firstPtr->m_xPos, -t_firstPtr->m_yPos);

            // Undo of: Scale polygon A to correct width and height
            a_polygonAPtr->rescale(a_polygonAPtr->m_width / t_firstPtr->m_width, a_polygonAPtr->m_height / t_firstPtr->m_height);     

            // Undo of: Rotate polygon B
            a_polygonBPtr->rotate(t_secondPtr->m_angle, t_secondPtr->m_xPos + t_secondPtr->m_width/2.0, t_secondPtr->m_yPos + t_secondPtr->m_height/2.0);

            // Undo of: Translate polygon B to correct position
            a_polygonBPtr->translate(- t_secondPtr->m_xPos, - t_secondPtr->m_yPos);

            // Undo of: Scale polygon B to correct width and height
            a_polygonBPtr->rescale(a_polygonBPtr->m_width / t_secondPtr->m_width, a_polygonBPtr->m_height / t_secondPtr->m_height);                 
         }
      }               
   }

   return r_ret;
}

void CObject::update(CLevel* t_levelPtr, std::vector<CObject*>::iterator& t_it, std::vector<CObject*>::iterator& t_endIt)
{
   std::vector<CObject*>::iterator a_it;

   //////////////////////////////////////////////////////////////////////////
   // collision detection
   for(a_it = t_it; a_it != t_endIt; a_it++)
   {
      // do only update if not both are from type object
      if(!((this->getType()==e_object) && 
           ((*a_it)->getType() == e_object)
          )
        )
      {
         if(isCollided(this, *a_it))
         {
            this->collisionImpact((*a_it));
         }
      }
   }
}


void CObject::collisionImpact(CObject* t_objectPtr, bool t_checkOther)
{
   m_hitPoints -= t_objectPtr->m_damagePoints;

   // object deletes itself as soon as it get hit
   if(!this->m_isDeleted && m_hitPoints < 0 && !m_invincible)
   {
      CLevel::M_deleteList.push_back(this);
      this->m_isDeleted = true;
   }

   if(t_checkOther)
   {
      t_objectPtr->collisionImpact(this, false);
   }   
}
