#include "enemyProjectile.h"
#include "vector.h"

/******************
**  ENEMY  SHIP  **
*******************/

int EnemyShip::time = 0;

/*****************************************
 * Default constructor
 *****************************************/
EnemyShip :: EnemyShip()
{
   rotation = 0;
   v.setX(0);
   v.setY(v.getYMax() - 50);
}

/************************************************
 * SHIP :: DRAW
 ***********************************************/
void EnemyShip :: draw()
{
   Point pt(v.getX(), v.getY());

   drawAlien(pt, rotation);

   return;
}

/************************************************
 * SHIP :: DRAW
 ***********************************************/
void EnemyShip :: operator ++ (int postfix)
{
   char dx;



   if (EnemyShip::time < CYCLE_LENGTH / 2 || EnemyShip::time == (CYCLE_LENGTH / 2)
         || EnemyShip::time == 0)
      dx = MOVE_DISTANCE;

   else// if (EnemyShip::time > CYCLE_LENGTH / 2)
        dx = -MOVE_DISTANCE;

   if (EnemyShip::time % MOVE_INTERVAL == 0)
      v.addX(dx);
}
