/***********************************************************************
* Program:
*    Project 4, Asteroids -- score.cpp
*    Brother Helfrich, CS165
* Author:
*    Devyn Stott, Justin Ricks
* Summary: 
*    Has functions and calls function from uiDraw.h to draw the score on
*    the screen to be used by a number of different games. This code
*    is very reusable ina  number of different situations.
*    We adapted this from the skeet project.
************************************************************************/

#include "uiDraw.h"
#include "score.h"
#include <GL/gl.h>

/*************************************************************************
 * Score::Score()
 * Default Constructor!
 * Sets intial position to top left and right of the screen!
 ***********************************************************************/
Score::Score()
{
   score[HIT]  = 0;
   score[FAIL] = 0;

   failPos.setX(failPos.getXMin() + 5);
   failPos.setY(failPos.getYMax() - 5);

   hitPos.setX(hitPos.getXMax() - 15);
   hitPos.setY(hitPos.getYMax() - 5);
}

/**********************************************************************
 * Score::operator ++ (postfix)
 * Increments the users score after winning a point, gets all ready to be
 * drawn
 ********************************************************************/
void Score::operator ++ (int postfix)
{
   score[HIT]++;
   return;
}

/**********************************************************************
 * Score::operator -- (postfix)
 * Increments the computers score after the user loses, gets all ready to be
 * drawn.
 ********************************************************************/
void Score::operator -- (int postfix)
{
   score[FAIL]++;
   return;
}

/******************************************************************
 * Score::draw()
 * Draws the user and computers scores on the screen! Excellent!
 * Calls a function in uiDraw.h to do this for us.
 *****************************************************************/
void Score::draw()
{
   char multiplier = 0;

   if (score[HIT] > 9 && score[HIT] < 100)
      multiplier = 1;
   else if (score[HIT] > 99 && score[HIT] < 1000)
      multiplier = 2;
   else if (score[HIT] > 999 && score[HIT] < 10000)
      multiplier = 3;
   else if (score[HIT] > 9999 && score[HIT] < 100000)
      multiplier = 4;

   hitPos.setX(hitPos.getXMax() - (12 + (11 * multiplier)));

   glColor3ub(0, 255, 0);
   drawNumber(hitPos, score[HIT]);
   glColor3ub(255, 255, 255);

   return;
}
