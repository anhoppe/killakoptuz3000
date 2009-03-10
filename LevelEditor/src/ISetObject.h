/********************************************************************
	created:	2009/03/03
	created:	3:3:2009   22:46
	filename: 	c:\hoppe\develop\KK3000\LevelEditor\src\ISetObject.h
	file path:	c:\hoppe\develop\KK3000\LevelEditor\src
	file base:	ISetObject
	file ext:	h
	author:		anhoppe
	
	purpose:	Interface for object selection
*********************************************************************/
#ifndef I_SET_OBJECT_H
#define I_SET_OBJECT_H

class ISetObject
{
public:
   virtual void setObject(int t_index) = 0;
};
#endif
