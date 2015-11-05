#include "game.h"
#include "uiInteract.h"
#include "uiDraw.h"

#define SHIP_SIZE 10.0
#define BULLET_LIFE 45

#ifndef PLAYPRO_H
#define PLAYPRO_H

/***********************************************
 * PLAYERPROJECTILE
 * All projectiles that are on the player's side
 *    inherit from this class
 ***********************************************/
class PlayerProjectile : public Projectile
{
public:
   // Constructors
   //PlayerProjectile(                ) : dead(false), v(    ) {};
   //PlayerProjectile(Point & pt      ) : dead(false), v(pt  ) {};
   //PlayerProjectile(float x, float y) : dead(false), v(x, y) {};
   
   TYPE getType() { return PLAYER; };
};

/*******************************************
 * PLAYERSHIP
 * The player's ship
 *******************************************/
class PlayerShip : public PlayerProjectile
{
  private:
   int rotation;
  public:
   PlayerShip();
   virtual float getSize() { return SHIP_SIZE; };
   void operator ++ (int postfix) { v++; };
   virtual void draw();
   virtual void event(const Interface * pUI, Game *);
};

/*********************************************
 * PLAYERBULLET
 * The player's bullet
 *********************************************/
class PlayerBullet : public PlayerProjectile
{
  public:
   PlayerBullet();
   PlayerBullet(float dx, float dy) : life(BULLET_LIFE) {};

   virtual void operator ++ (int postfix);
   virtual void draw();
   virtual float getSize() { return 0; };
   virtual void event(const Interface * pUI, Game *)  { }; /* Do nothing */
//   virtual void hit(Projectile * rhs, Game *) { };
  private:
   int life;
};

#endif // PLAYPRO_H
