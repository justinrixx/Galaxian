/***********************************************************************************************
 * Galaxian :: GAME
 *      This class is designed to hold of of the objects and handle the game of Galaxian
 *
 * Authors:
 *      Justin Ricks
 ******************************************************************************************/

#include "vector.h"
#include "uiInteract.h"
#include "score.h"

#ifndef GAME_H
#define GAME_H

enum TYPE { ENEMY, PLAYER };

#include <list>

using namespace std;

// Forward declaration
class Projectile;

/**************************************************
 * The game class is mostly just a container for
 *    the items the game uses
 *************************************************/
class Game
{
  public:
   Game();
   void advance(const Interface * pUI);
   void draw();
   void addItem(Projectile * p);
  private:
   list<Projectile *> items;
   Score score;
   int time;
};

/*************************************************
 * PROJECTILE
 * 
 ************************************************/
class Projectile
{
  public:
   // Constructors
   Projectile(                ) : dead(false), v(    ) {};
   Projectile(Point & pt      ) : dead(false), v(pt  ) {};
   Projectile(float x, float y) : dead(false), v(x, y) {};

   // Methods
   bool isDead()        { return dead; };
   void kill ()         { dead = true; };
   void setDx(float dx) { v.setDx(dx); };
   void setDy(float dy) { v.setDy(dy); };
   Vector & getVector() { return v;    };
   virtual float getSize() { return 0; };

   // Pure virtual functions
   virtual void operator ++ (int postfix) = 0;
   virtual void draw() = 0;
   virtual void event(const Interface * pUI, Game * pGame) = 0;
   virtual TYPE getType() = 0;

  protected:
   bool dead;
   Vector v;
};


#endif //GAME_H
