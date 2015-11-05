#include "game.h"
#include "uiInteract.h"
#include "uiDraw.h"

#define ALIEN_SIZE 10.0
#define CYCLE_LENGTH 200
#define MOVE_DISTANCE 8
#define MOVE_INTERVAL 20

#ifndef NMEPRO_H
#define NMEPRO_H

/***********************************************
 * PLAYERPROJECTILE
 * All projectiles that are on the player's side
 *    inherit from this class
 ***********************************************/
class EnemyProjectile : public Projectile
{
public:
   TYPE getType() { return ENEMY; };
};

/*******************************************
 * ENEMYSHIP
 * The enemy ships
 *******************************************/
class EnemyShip : public EnemyProjectile
{
  private:
   int rotation;

  public:
   static int time;

   EnemyShip();
   virtual float getSize() { return ALIEN_SIZE; };
   void operator ++ (int postfix);
   virtual void draw();
   virtual void event(const Interface * pUI, Game *) {};
};

#endif // NMEPRO_H
