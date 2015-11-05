#include "playerProjectile.h"
#include "vector.h"

#define SHIP_MOVE 10
#define BULLET_SPEED 10

/******************
** PLAYER BULLET **
*******************/

/*****************************************
 * Default constructor
 *****************************************/
PlayerBullet :: PlayerBullet()
{
   dead = false;
   life = BULLET_LIFE;
}

/****************************************
 * BULLET::DRAW
 ***************************************/
void PlayerBullet :: draw()
{
   Point point(v.getX(), v.getY());
   drawDot(point);
}

/****************************************
 * BULLET::ADVAVCE
 ***************************************/
void PlayerBullet :: operator ++ (int postfix)
{
   v++;

   if (life <= 0)
      dead = true;
   else
      life--;
}

/******************
**  PLAYER SHIP  **
*******************/

/*****************************************
 * Default constructor
 *****************************************/
PlayerShip :: PlayerShip()
{
   rotation = 270;
   v.setX(0);
   v.setY(v.getYMin() + 20);
}

/************************************************
 * SHIP :: DRAW
 ***********************************************/
void PlayerShip :: draw()
{
   Point pt(v.getX(), v.getY());

   drawPlayerShip(pt);

   return;
}

/**********************************************************
 * SHIP :: EVENT
 * INPUT:  Pointer to interface, pointer to game
 *
 *   handles the user input and moves the ship or fires
 *   a bullet
 *********************************************************/
void PlayerShip :: event(const Interface * pUI, Game * pGame)
{
   if (pUI->isRight() && v.getX() + SHIP_MOVE <= v.getXMax())
      v.addX(SHIP_MOVE);
   if (pUI->isLeft()  && v.getX() - SHIP_MOVE >= v.getXMin())
      v.addX(-SHIP_MOVE);


   if (pUI->isSpace())
   {
      PlayerBullet * p = new PlayerBullet;

      // Set the position to be in front of the ship
      p->getVector().setX(v.getX());
      p->getVector().setY(v.getYMin() + 25);

      // Set the speed of the new bullet
      p->setDx(0/*-5 * cos(deg2rad(rotation)) + v.getDx()*/);
      p->setDy(BULLET_SPEED);

      // Add the bullet to the game
      pGame->addItem(p);
   }

   return;
}

