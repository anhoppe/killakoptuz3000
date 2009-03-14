/********************************************************************
	created:	2009/03/03
	created:	3:3:2009   23:31
	filename: 	c:\hoppe\develop\KK3000\LevelEditor\src\CObjectControl.h
	file path:	c:\hoppe\develop\KK3000\LevelEditor\src
	file base:	CObjectControl
	file ext:	h
	author:		anhoppe
	
	purpose:	Draws controllers for the objects in the GL window
*********************************************************************/
#ifndef C_OBJECT_CONTROL_H
#define C_OBJECT_CONTROL_H

class CObject;

class CObjectControl
{
   enum EDirection
   {
      e_none   = 0,
      e_left   = 1,
      e_right  = 2,
      e_top    = 3,
      e_bottom = 4,
      e_move   = 5
   };

   //////////////////////////////////////////////////////////////////////////
   // Public Methods
   //////////////////////////////////////////////////////////////////////////
public:
   CObjectControl();
   ~CObjectControl();

   static CObjectControl& getInstance();

   void setObject(CObject* t_objectPtr) { m_activeObjectPtr = t_objectPtr; }
   
   /** Selects a control sizer */
   bool selectSizer(double t_x, double t_y);

   /** selects move control */
   bool moveControl(double t_x, double t_y);

   /** Releases a selected control */
   void releaseControl();

   /** Processes a mouse move event */
   void processMouseMove(double t_x, double t_y);

   /** draws all controls for the active object */
   void draw();

   //////////////////////////////////////////////////////////////////////////
   // Private Methods
   //////////////////////////////////////////////////////////////////////////
private:
   /** Checks if given coordinates are over sizer rect */
   bool isOverRect(EDirection t_dir, double t_x, double t_y);

   void drawSizerRect(EDirection t_dir);
   void getRect(EDirection t_dir, double& t_minX, double& t_minY, double& t_maxX, double& t_maxY);
   
   //////////////////////////////////////////////////////////////////////////
   // Private Variables
   //////////////////////////////////////////////////////////////////////////
private:
   CObject*    m_activeObjectPtr;
   EDirection  m_selectedControl;
   
   /** start coordinates for controlling actions (sizing etc.) */
   double      m_startX;
   double      m_startY;

   double      m_deltaX;
   double      m_deltaY;

   double      m_startWidth;
   double      m_startHeight;
};

#endif
