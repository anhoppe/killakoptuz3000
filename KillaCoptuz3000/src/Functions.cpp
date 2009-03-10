/********************************************************************
	created:	2007/05/08
	created:	8:5:2007   20:45
	filename: 	\Data\Source\Functions.cpp
	file path:	\Data\Source
	file base:	Functions
	file ext:	cpp
	author:		Fabian Heinemann
	
	purpose:	
*********************************************************************/
#include "Functions.h"

bool getAttributeStr(TiXmlElement* t_elemPtr, std::string t_attributeStr, std::string& t_resultStr)
{
   bool r_ret;

   if(0 != t_elemPtr)
   {
      if (t_elemPtr->Attribute(t_attributeStr.c_str()))
      {
          t_resultStr = t_elemPtr->Attribute(t_attributeStr.c_str());
          r_ret = true;
      }
      else
      {      
          t_resultStr = "";
          r_ret = false;
      }
   }

   return r_ret;
}

void swap(int& t_a, int& t_b)
{
   int a_buf = t_a;
   t_a = t_b;
   t_b = a_buf;
}