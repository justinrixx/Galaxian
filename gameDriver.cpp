/********************************************
 * The real deal : Galaxian!
 *
 * Authors : Justin Ricks
 *******************************************/

#include "uiInteract.h"
#include "uiDraw.h"
#include "playerProjectile.h"
#include "enemyProjectile.h"

#include <iostream>

#define deg2rad(value) ((M_PI / 180) * (value))

using namespace std;

/*************************************
 * Take the user interface, when ever the user hits 
 * the space bar fire a bullet.
 **************************************/
void callBack(const Interface * pUI, void * p)
{
   Game * pGame = (Game *)p;

   pGame->advance(pUI);
   
   pGame->draw();
}

/*********************************
 * Main
 *********************************/
int main(int argc, char ** argv)
{
   Interface ui(argc, argv, "Galaxian");

   Game game;

   PlayerShip * ship = new PlayerShip();

   game.addItem(ship);
   
   ui.run(callBack, &game);

   return 0;
}
