/********************************************************************
	created:	2007/05/08
	created:	8:5:2007   20:43
	filename: 	\Data\Header\Functions.h
	file path:	\Data\Header
	file base:	Functions
	file ext:	h
	author:		Fabian Heinemann
	
	purpose:	
*********************************************************************/
#ifndef KILLER_COPTUZ_3000_FUNCTIONS_H
#define KILLER_COPTUZ_3000_FUNCTIONS_H

#include <string>
#include "tinyxml/tinyxml.h"

bool getAttributeStr(TiXmlElement* t_elemPtr, std::string t_attributeStr, std::string& t_resultStr);

void swap(int& t_a, int& t_b);

#endif