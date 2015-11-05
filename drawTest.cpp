/****************************************
 * Draw Test
 *  tests the custom asteroid shape and
 *  its collision area
 ***************************************/

#include "point.h"
#include "uiInteract.h"
#include "uiDraw.h"
using namespace std;

/*************************************
 * All the interesting work happens here, when
 * I get called back from OpenGL to draw a frame.
 * When I am finished drawing, then the graphics
 * engine will wait until the proper amount of
 * time has passed and put the drawing on the screen.
 **************************************/
void callBack(const Interface *pUI, void *p)
{
   // yes, the rock is a point . . .
   Point *pRock = (Point *)p;

   static int rotation;
   rotation++;

   // draw the rock
   drawAlien(*pRock, rotation);
   // draw the collision area
   //drawCircle(*pRock, 30, 30, rotation);
}



/*********************************
 * Main is pretty sparse.  Just initialize
 * the class call the display engine.
 * That is all!
 *********************************/
int main(int argc, char ** argv)
{
   Interface ui(argc, argv, "Test");
   Point alien;
   ui.run(callBack, &alien);

   return 0;
}
