/***********************************************************************
 * Source File:
 *    User Interface Draw : put pixels on the screen
 * Author:
 *    Br. Helfrich
 * Summary:
 *    This is the code necessary to draw on the screen. We have a collection
 *    of procedural functions here because each draw function does not
 *    retain state. In other words, they are verbs (functions), not nouns
 *    (variables) or a mixture (objects)
 ************************************************************************/

#include <string>     // need you ask?
#include <sstream>    // convert an integer into text
#include <cassert>    // I feel the need... the need for asserts
#include <GL/gl.h>    // Main OpenGL library
#include <GL/glut.h>  // Second OpenGL library
#include <time.h>     // for clock

#include "point.h"
#include "uiDraw.h"

using namespace std;

#define deg2rad(value) ((M_PI / 180) * (value))

/*********************************************
 * NUMBER OUTLINES
 * We are drawing the text for score and things
 * like that by hand to make it look "old school."
 * These are how we render each individual character.
 * Note how -1 indicates "done".  These are paired
 * coordinates where the even are the x and the odd
 * are the y and every 2 pairs represents a point
 ********************************************/
const char NUMBER_OUTLINES[10][20] =
{
  {0, 0,  7, 0,   7, 0,  7,10,   7,10,  0,10,   0,10,  0, 0,  -1,-1, -1,-1},//0
  {7, 0,  7,10,  -1,-1, -1,-1,  -1,-1, -1,-1,  -1,-1, -1,-1,  -1,-1, -1,-1},//1
  {0, 0,  7, 0,   7, 0,  7, 5,   7, 5,  0, 5,   0, 5,  0,10,   0,10,  7,10},//2
  {0, 0,  7, 0,   7, 0,  7,10,   7,10,  0,10,   4, 5,  7, 5,  -1,-1, -1,-1},//3
  {0, 0,  0, 5,   0, 5,  7, 5,   7, 0,  7,10,  -1,-1, -1,-1,  -1,-1, -1,-1},//4
  {7, 0,  0, 0,   0, 0,  0, 5,   0, 5,  7, 5,   7, 5,  7,10,   7,10,  0,10},//5
  {7, 0,  0, 0,   0, 0,  0,10,   0,10,  7,10,   7,10,  7, 5,   7, 5,  0, 5},//6
  {0, 0,  7, 0,   7, 0,  7,10,  -1,-1, -1,-1,  -1,-1, -1,-1,  -1,-1, -1,-1},//7
  {0, 0,  7, 0,   0, 5,  7, 5,   0,10,  7,10,   0, 0,  0,10,   7, 0,  7,10},//8
  {0, 0,  7, 0,   7, 0,  7,10,   0, 0,  0, 5,   0, 5,  7, 5,  -1,-1, -1,-1} //9
};

/************************************************************************
 * DRAW DIGIT
 * Draw a single digit in the old school line drawing style.  The
 * size of the glyph is 8x11 or x+(0..7), y+(0..10)
 *   INPUT  topLeft   The top left corner of the character
 *          digit     The digit we are rendering: '0' .. '9'
 *************************************************************************/
void drawDigit(const Point &topLeft, char digit)
{
   // we better be only drawing digits
   assert(isdigit(digit));
   if (!isdigit(digit))
      return;

   // compute the row as specified by the digit
   int r = digit - '0';
   assert(r >= 0 && r <= 9);

   // go through each segment.
   for (int c = 0; c < 20 && NUMBER_OUTLINES[r][c] != -1; c += 4)
   {
      assert(NUMBER_OUTLINES[r][c    ] != -1 &&
             NUMBER_OUTLINES[r][c + 1] != -1 &&
             NUMBER_OUTLINES[r][c + 2] != -1 &&
             NUMBER_OUTLINES[r][c + 3] != -1);

      //Draw a line based off of the num structure for each number
      Point start;
      start.setX(topLeft.getX() + NUMBER_OUTLINES[r][c]);
      start.setY(topLeft.getY() - NUMBER_OUTLINES[r][c + 1]);
      Point end;
      end.setX(topLeft.getX() + NUMBER_OUTLINES[r][c + 2]);
      end.setY(topLeft.getY() - NUMBER_OUTLINES[r][c + 3]);
         
      drawLine(start, end);
   }
}

/*************************************************************************
 * DRAW NUMBER
 * Display an positive integer on the screen using the 7-segment method
 *   INPUT  topLeft   The top left corner of the character
 *          digit     The digit we are rendering: '0' .. '9'
 *************************************************************************/
void drawNumber(const Point &topLeft, unsigned int number)
{
   // render the number as text
   ostringstream sout;
   sout << number;
   string text = sout.str();

   // walk through the text one digit at a time
   Point point = topLeft;
   for (const char *p = text.c_str(); *p; p++)
   {
      assert(isdigit(*p));
      drawDigit(point, *p);
      point.addX(11);
   }
}


/************************************************************************
 * ROTATE
 * Rotate a given point (point) around a given origin (center) by a given
 * number of degrees (angle).
 *    INPUT  point    The point to be moved
 *           center   The center point we will rotate around
 *           rotation Rotation in degrees
 *    OUTPUT point    The new position
 *************************************************************************/
void rotate(Point &point, const Point &origin, int rotation)
{
   // because sine and cosine are expensive, we want to call them only once
   double cosA = cos(deg2rad(rotation));
   double sinA = sin(deg2rad(rotation));

   // remember our original point
   Point tmp(true /*fNoCheck*/);
   tmp.setX(point.getX() - origin.getX());
   tmp.setY(point.getY() - origin.getY());

   // find the new values
   point.setX(static_cast<int> (tmp.getX() * cosA -
                                tmp.getY() * sinA) +
              origin.getX());
   point.setY(static_cast<int> (tmp.getX() * sinA +
                                tmp.getY() * cosA) +
              origin.getY());
}

/************************************************************************
 * DRAW RECTANGLE
 * Draw a rectangle on the screen centered on a given point (center) of
 * a given size (width, height), and at a given orientation (rotation)
 *  INPUT  center    Center of the rectangle
 *         width     Horizontal size
 *         height    Vertical size
 *         rotation  Orientation
 *************************************************************************/
void drawRect(const Point &center, char width, char height, int rotation)
{
   Point tl(true /*fNoCheck*/); // top left
   Point tr(true /*fNoCheck*/); // top right 
   Point bl(true /*fNoCheck*/); // bottom left
   Point br(true /*fNocheck*/); // bottom right

   //Top Left point
   tl.setX(center.getX() - (width  / 2));
   tl.setY(center.getY() + (height / 2));

   //Top right point
   tr.setX(center.getX() + (width  / 2));
   tr.setY(center.getY() + (height / 2));

   //Bottom left point
   bl.setX(center.getX() - (width  / 2));
   bl.setY(center.getY() - (height / 2));

   //Bottom right point
   br.setX(center.getX() + (width  / 2));
   br.setY(center.getY() - (height / 2));

   //Rotate all points the given degrees
   rotate(tl, center, rotation);
   rotate(tr, center, rotation);
   rotate(bl, center, rotation);
   rotate(br, center, rotation);

   //Finally draw the rectangle
   glBegin(GL_LINE_STRIP);
   glVertex2f(tl.getX(), tl.getY());
   glVertex2f(tr.getX(), tr.getY());
   glVertex2f(br.getX(), br.getY());
   glVertex2f(bl.getX(), bl.getY());
   glVertex2f(tl.getX(), tl.getY());
   glEnd();
}

/************************************************************************
 * DRAW CIRCLE
 * Draw a circle from a given location (center) of a given size (radius).
 *  INPUT   center   Center of the circle
 *          radius   Size of the circle
 *          points   How many points will we draw it.  Larger the number,
 *                   the more line segments we will use
 *          rotation True circles are rotation independent.  However, if you
 *                   are drawing a 3-sided circle (triangle), this matters!
 *************************************************************************/
void drawCircle(const Point &center, char radius, int points, int rotation)
{
   // begin drawing
   glBegin(GL_LINE_LOOP);

   //loop around a circle the given number of times drawing a line from
   //one point to the next
   for (double i = 0; i < 2 * M_PI; i += (2 * M_PI) / points)
   {
      Point temp(true /*noCheck*/);
      temp.setX(center.getX() + static_cast<int>(radius * cos(i)));
      temp.setY(center.getY() + static_cast<int>(radius * sin(i)));
      rotate(temp, center, rotation);
      glVertex2f(temp.getX(), temp.getY());
   }
   
   // complete drawing
   glEnd();

}

/************************************************************************
 * DRAW LINE
 * Draw a line on the screen from the beginning to the end.
 *   INPUT  begin     The position of the beginning of the line
 *          end       The position of the end of the line
 *************************************************************************/
void drawLine(const Point &begin, const Point &end)
{
   // Get ready...
   glBegin(GL_LINES);

   // Note how we scale the lines by a factor of two
   glVertex2f(begin.getX(), begin.getY());
   glVertex2f(  end.getX(),   end.getY());

   // complete drawing.
   glEnd();
}

/************************************************************************
 * DRAW DOT
 * Draw a single point on the screen, 2 pixels by 2 pixels
 *  INPUT point   The position of the dow
 *************************************************************************/
void drawDot(const Point &point)
{
   // Get ready, get set...
   glBegin(GL_POINTS);

   // Go...
   glVertex2f(point.getX(),     point.getY()    );
   glVertex2f(point.getX() + 1, point.getY()    );
   glVertex2f(point.getX() + 1, point.getY() + 1);
   glVertex2f(point.getX(),     point.getY() + 1);

   // Done!  OK, that was a bit too dramatic
   glEnd();
}

/************************************************************************
 * DRAW ASTEROID
 * Draw an asteroid on the screen
 *  INPUT point   The position of the ship
 *        radius  The radius of the asteroid
 *        angle   Which direction it is ponted
 *************************************************************************/
void drawAsteroid(const Point &center, float radius, int sides, int rotation)
{
   // create three arrays
   Point p[11];
   float dx[11];
   float dy[11];

   // manually set the dx's
   dx[0]  = -1 * radius / 4;
   dx[1]  =  2 * radius / 4;
   dx[2]  =  2 * radius / 4;
   dx[3]  =  3 * radius / 4;
   dx[4]  =  3 * radius / 4;
   dx[5]  =  0 * radius / 4;
   dx[6]  =  0 * radius / 4;
   dx[7]  = -1 * radius / 4;
   dx[8]  = -3 * radius / 4;
   dx[9]  = -1 * radius / 4;
   dx[10] = -3 * radius / 4;

   // manually set the dy's
   dy[0]  =  4 * radius / 4;
   dy[1]  =  3 * radius / 4;
   dy[2]  =  1 * radius / 4;
   dy[3]  =  2 * radius / 4;
   dy[4]  =  0 * radius / 4;
   dy[5]  = -3 * radius / 4;
   dy[6]  = -1 * radius / 4;
   dy[7]  = -3 * radius / 4;
   dy[8]  =  0 * radius / 4;
   dy[9]  =  0 * radius / 4;
   dy[10] =  2 * radius / 4;

   for (int i = 0; i < 11; i++)
   {
      // set every point to the center
      p[i] = center;
      // add in the dx's
      p[i].addX(dx[i]);
      // add in the dy's
      p[i].addY(dy[i]);

      // rotate all the points
      rotate(p[i], center, rotation);
   }

   // finally, draw
   glBegin(GL_LINE_STRIP);
   for (int i = 0; i < 11; i++)
   {
      glVertex2f(p[i].getX(), p[i].getY());
   }
   glVertex2f(p[0].getX(), p[0].getY());
   glEnd();
}

/************************************************************************
 * DRAW Ship
 * Draw a spaceship on the screen
 *  INPUT point   The position of the ship
 *        angle   Which direction it is ponted
 *************************************************************************/
void drawShip(const Point &center, int rotation)
{
   //glColor3ub(0, 255, 0);
   
   Point bow(      center); // front
   Point stern(    center); // back
   Point starboard(center); // right
   Point port(     center); // left

   bow.addX(-6);

   stern.addX(2);

   starboard.addY(3);
   starboard.addX(3);

   port.addY(-3);
   port.addX( 3);
   
   //Rotate all points the given degrees
   rotate(bow,       center, rotation);
   rotate(stern,     center, rotation);
   rotate(starboard, center, rotation);
   rotate(port,      center, rotation);

   //Finally draw the rectangle
   glBegin(GL_LINE_STRIP);
   glVertex2f(      bow.getX(),       bow.getY());
   glVertex2f(starboard.getX(), starboard.getY());
   glVertex2f(    stern.getX(),     stern.getY());
   glVertex2f(     port.getX(),      port.getY());
   glVertex2f(      bow.getX(),       bow.getY());
   
   // Done!  OK, that was a bit too dramatic
   glEnd();

   glColor3ub(255, 255, 255);
}

/************************************************************************
 * DRAW PlayerShip
 * Draw a spaceship on the screen
 *  INPUT point   The position of the ship
 *************************************************************************/
void drawPlayerShip(const Point &center)
{
   Point bow(      center); // front
   Point stern(    center); // back
   Point starboard(center); // right
   Point port(     center); // left

   bow.addY(10);

   stern.addY(-6);

   starboard.addX(10);
   starboard.addY(-10);

   port.addX(-10);
   port.addY(-10);

   glBegin(GL_LINE_STRIP);
   glVertex2f(      bow.getX(),       bow.getY());
   glVertex2f(starboard.getX(), starboard.getY());
   glVertex2f(    stern.getX(),     stern.getY());
   glVertex2f(     port.getX(),      port.getY());
   glVertex2f(      bow.getX(),       bow.getY());

   glEnd();
}

/************************************************************************
 * DRAW Alien
 * Draw the spaceship on the screen
 *************************************************************************/
void drawAlien(const Point &center, int rotation)
{
   // create three arrays
      Point p[12];
      float dx[12];
      float dy[12];

      // manually set the dx's
      dx[0]  = 0;
      dx[1]  = 5;
      dx[2]  = 7.5;
      dx[3]  = 10;
      dx[4]  = 7.5;
      dx[5]  = 2.5;
      dx[6]  = 0;
      dx[7]  = -2.5;
      dx[8]  = -7.5;
      dx[9]  = -10;
      dx[10] = -7.5;
      dx[11] = -5;

      // manually set the dy's
      dy[0]  = 10;
      dy[1]  = 7.5;
      dy[2]  = 5;
      dy[3]  = -5;
      dy[4]  = -10;
      dy[5]  = -5;
      dy[6]  = -10;
      dy[7]  = -5;
      dy[8]  = -10;
      dy[9]  = -5;
      dy[10] = 5;
      dy[11] = 7.5;

      for (int i = 0; i < 12; i++)
      {
         // set every point to the center
         p[i] = center;
         // add in the dx's
         p[i].addX(dx[i]);
         // add in the dy's
         p[i].addY(dy[i]);

         // rotate all the points
         rotate(p[i], center, rotation);
      }

      // finally, draw
      glBegin(GL_LINE_STRIP);
      for (int i = 0; i < 11; i++)
      {
         glVertex2f(p[i].getX(), p[i].getY());
      }
      glVertex2f(p[0].getX(), p[0].getY());
      glEnd();

      // Right eye
      Point re = center;

      // Left eye
      Point le = center;

      le.addX(-4);
      le.addY(4);

      re.addX(4);
      re.addY(4);

      // Put the center of the eyes where they should be
      rotate(re, center, rotation);
      rotate(le, center, rotation);

      glColor3ub(255, 0, 0);

      // Draw the eyes
      drawRect(le, 4, 4, rotation);
      drawRect(re, 4, 4, rotation);

      glColor3ub(255, 255, 255);
}

/******************************************************************
 * RANDOM
 * This function generates a random number.  
 *
 *    INPUT:   min, max : The number of values (min <= num <= max)
 *    OUTPUT   <return> : Return the integer
 ****************************************************************/
int random(int min, int max)
{
   assert(min <= max);
   int num = (rand() % (max - min)) + min;
   assert(min <= num && num <= max);

   return num;
}


