/**********************************************************************
 * Project 4 :: Asteroids
 *      This file conatinas asteroids Member functions
 * Authors:
 *      Devyn Stott
 ********************************************************************/

#include "enemyProjectile.h"
#include "playerProjectile.h"
#include "game.h"
#include "point.h"
#include "score.h"

#define NUMNME 48
#define ROW_SIZE 12
#define TOP_MARGIN 40
#define LEFT_MARGIN 28

using namespace std;

/********************************************************************
 * ASTEROIDS :: DRAW
 *    loops through every item in the list and tells it to draw itself
 *******************************************************************/
void Game :: draw()
{
   // Loop through every item
   for (list<Projectile *>::iterator it = items.begin();
        it != items.end(); ++it)
      (*it)->draw();

   score.draw();
}

/********************************************************************
 * ASTEROIDS :: ADVANCE
 *    loops through every item in the list and tells it to advance itself
 *******************************************************************/
void Game :: advance(const Interface * pUI)
{
   time %= CYCLE_LENGTH;

   time++;

   // Loop through every item
   for (list<Projectile *>::iterator it = items.begin();
        it != items.end(); ++it)
   {
      // Loop through all the other items
      for (list<Projectile *>::iterator iter = items.begin();
           iter != items.end(); ++iter)
      {
         // Check if the projectiles are on different sides, and if they collide
         if ((*it)->getType() != (*iter)->getType()
               && minDistance((*it)->getVector(), (*iter)->getVector()) <
                     (*it)->getSize() + (*iter)->getSize())
         {
            (*it)->kill();
            (*iter)->kill();
            score.addScore(50);
         }
      }
      
      // Advance the object
      (*it)->operator++(1);
      (*it)->event(pUI, this);
      
      // If it's dead, bury it
      if ((*it)->isDead())
      {
         // Erase the pointer from the list
         items.erase(it);
         // Free the memory
         delete (*it);
         // Decrement the iterator so that it does not
         //   advance this iteration
         --it;
      }
   }

   // Set the time
   EnemyShip::time = this->time;
}

/********************************************************************
 * GAME :: ADDITEM
 *    adds a new projectile to the list
 *******************************************************************/
void Game :: addItem(Projectile * p)
{
   // Add to the list
   items.push_back(p);
}

/************************
 * Default Constructor
 ************************/
Game :: Game()
{
   // Time starts at zero
   time = (CYCLE_LENGTH / 2);

   // Create an array of pointers to EnemyShips
   EnemyShip ** enemies = new EnemyShip * [NUMNME];

   // Allocate each ship in the array
   for (int i = 0; i < NUMNME; i++)
      enemies[i] = new EnemyShip;

   // Set the positions and add them to the game
   for (int i = 0; i < NUMNME; i++)
   {
      enemies[i]->getVector().setX(-POINT_MAX + (((i % ROW_SIZE) + 1) * 35) + LEFT_MARGIN);
      enemies[i]->getVector().setY(POINT_MAX - (((i / ROW_SIZE) + 1) * 30) - TOP_MARGIN);

      addItem(enemies[i]);
   }
};
