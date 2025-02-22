/*********************************************************************
 * File: game.cpp
 * Description: Contains the implementaiton of the game class
 *  methods.
 *
 *********************************************************************/

#include "game.h"
#include <vector>

// These are needed for the getClosestDistance function...
#include <limits>
#include <algorithm>
#include "uiDraw.h"
#include "uiInteract.h"
#include "point.h"
#include "velocity.h"
#include "bullet.h"
#include "rocks.h"
#include "ship.h"
#include "bullet.h"

#define OFF_SCREEN_BORDER_AMOUNT 5
#define LARGE_ASTEROID_SPEED     1
#define BULLET_SPEED             5
#define BULLET_MAX_FRAME_COUNT   40

using namespace std;


// You may find this function helpful...

/**********************************************************
 * Function: getClosestDistance
 * Description: Determine how close these two objects will
 *   get in between the frames.
 **********************************************************/
float getClosestDistance(const FlyingObject &obj1, const FlyingObject &obj2)
{
   // find the maximum distance traveled
   float dMax = max(abs(obj1.getVelocity().getDx()), abs(obj1.getVelocity().getDy()));
   dMax = max(dMax, abs(obj2.getVelocity().getDx()));
   dMax = max(dMax, abs(obj2.getVelocity().getDy()));
   dMax = max(dMax, 0.1f); // when dx and dy are 0.0. Go through the loop once.
   
   float distMin = std::numeric_limits<float>::max();
   for (float i = 0.0; i <= dMax; i++)
   {
      Point point1(obj1.getPoint().getX() + (obj1.getVelocity().getDx() * i / dMax),
                     obj1.getPoint().getY() + (obj1.getVelocity().getDy() * i / dMax));
      Point point2(obj2.getPoint().getX() + (obj2.getVelocity().getDx() * i / dMax),
                     obj2.getPoint().getY() + (obj2.getVelocity().getDy() * i / dMax));
      
      float xDiff = point1.getX() - point2.getX();
      float yDiff = point1.getY() - point2.getY();
      
      float distSquared = (xDiff * xDiff) +(yDiff * yDiff);
      
      distMin = min(distMin, distSquared);
   }
   
   return sqrt(distMin);
}

/***************************************
 * GAME CONSTRUCTOR
 ***************************************/
Game :: Game(Point tl, Point br)
 : topLeft(tl), bottomRight(br)
{
   // Set up the initial conditions of the game
   // TODO: Set your asteroid pointer to a good initial value (e.g., NULL) 
   for (int i = 0; i < 5; i++)
   {
      rock.push_back(createBigRock());
   }
   createShip();
}
 
/****************************************
 * GAME DESTRUCTOR
 ****************************************/
Game :: ~Game()
{
   // TODO: Check to see if there is currently a asteroid allocated
   //       and if so, delete it.
   std::vector <Rock*> :: iterator it = rock.begin();
   while (it != rock.end())
   {
      if ((*it) != NULL)
      {
         delete *it;
         *it = NULL;
      }
      rock.erase(it);
   }
}

/***************************************
 * GAME :: ADVANCE
 * advance the game one unit of time
 ***************************************/
void Game :: advance()
{
   advanceBullets();
   advanceAsteroid();
   advanceShip();

   handleCollisions();
   cleanUpZombies();
}

/***************************************
 * GAME :: ADVANCE BULLETS
 * Go through each bullet and advance it.
 ***************************************/
void Game :: advanceBullets()
{
   // Move each of the bullets forward if it is alive
   for (std::vector <Bullet*> :: iterator it = bullet.begin(); it != bullet.end(); it++)
   {
      if ((*it)->isAlive())
      {
         // this bullet is alive, so tell it to move forward
         (*it)->screenWrap();
         (*it)->advance(); 

         if ((*it)->counter() == BULLET_MAX_FRAME_COUNT)
         {
            (*it)->kill();
         } 
      }
   }
}

/**************************************************************************
 * GAME :: ADVANCE ASTEROID
 *
 * 1. If there is no bird, create one with some probability
 * 2. If there is a bird, and it's alive, advance it
 * 3. Check if the bird has gone of the screen, and if so, "kill" it
 **************************************************************************/
void Game :: advanceAsteroid()
{
   for (std::vector <Rock*> :: iterator it = rock.begin(); it != rock.end(); it++)
   {
      if ((*it)->isAlive())
      {
         if (isOnScreen((*it)->getPoint()))
         {
            (*it)->advance();
         }

      else
      {
         (*it)->screenWrap();
         (*it)->advance();
      }
      }
   }

}

/**************************************************************************
 * GAME :: advanceShip()
 *
 **************************************************************************/
void Game::advanceShip()
{
   if (ship.isAlive())
   {
      // advance the ship
      if (isOnScreen(ship.getPoint()))
      {
         ship.advance();
      }

      else
      {
         ship.screenWrap();
         ship.advance();
      }
      
      // check for crash
      // if (!ground.isAboveGround(ship.getPoint()))
      // {
      //    ship.setAlive(false);
      // }
   }
}

/**************************************************************************
 * GAME :: CREATE ASTEROID
 * Create a large rock
 **************************************************************************/
Rock* Game :: createBigRock()
{
   Rock * newRock = NULL;
   float angle = random(0, 361);
   // TODO: Fill this in

   Point point;
   Velocity velocity;

   point.setX(random(-199, 199));
   point.setY(random(-199, 199));
   velocity.setDx(LARGE_ASTEROID_SPEED * (-cos(M_PI / 180.0 * angle)));
   velocity.setDy(LARGE_ASTEROID_SPEED * (sin(M_PI / 180.0 * angle)));
   BigRock* bigRock = new BigRock(point, velocity);
   newRock = new BigRock(point, velocity);

   return newRock;
} 


/**************************************************************************
 * GAME :: CREATE ASTEROID
 * Create a large rock
 **************************************************************************/
Rock* Game :: createMediumRock()
{
   Rock * newRock = NULL;
   float angle = random(0, 361);
   // TODO: Fill this in

   Point point;
   Velocity velocity;

   point.setX(random(-199, 199));
   point.setY(random(-199, 199));
   velocity.setDx(LARGE_ASTEROID_SPEED * (-cos(M_PI / 180.0 * angle)));
   velocity.setDy(LARGE_ASTEROID_SPEED * (sin(M_PI / 180.0 * angle)));
   MediumRock* mediumRock = new MediumRock(point, velocity);
   newRock = new MediumRock(point, velocity);

   return newRock;
} 

/**************************************************************************
 * GAME :: CREATE ASTEROID
 * Create a large rock
 **************************************************************************/
Rock* Game :: createSmallRock()
{
   Rock * newRock = NULL;
   float angle = random(0, 361);
   // TODO: Fill this in

   Point point;
   Velocity velocity;

   point.setX(random(-199, 199));
   point.setY(random(-199, 199));
   velocity.setDx(LARGE_ASTEROID_SPEED * (-cos(M_PI / 180.0 * angle)));
   velocity.setDy(LARGE_ASTEROID_SPEED * (sin(M_PI / 180.0 * angle)));
   SmallRock* smallRock = new SmallRock(point, velocity);
   newRock = new SmallRock(point, velocity);

   return newRock;
} 

/**************************************************************************
 * GAME :: createBullet()
 * 
 **************************************************************************/
Bullet* Game::createBullet()
{
   Velocity bulletVelocity = ship.getVelocity();
   bulletVelocity.setDx(bulletVelocity.getDx() + BULLET_SPEED * (-sin(M_PI / 180.0 * ship.getRotation())));
   bulletVelocity.setDy(bulletVelocity.getDy() + BULLET_SPEED * (cos(M_PI / 180.0 * ship.getRotation())));

   Bullet * newBullet = NULL;
   newBullet = new Bullet(ship.getPoint(), bulletVelocity);

   return newBullet;   
}

/**************************************************************************
 * GAME :: IS ON SCREEN
 * Determines if a given point is on the screen.
 **************************************************************************/
bool Game :: isOnScreen(const Point & point)
{
   return (point.getX() >= topLeft.getX() - OFF_SCREEN_BORDER_AMOUNT
      && point.getX() <= bottomRight.getX() + OFF_SCREEN_BORDER_AMOUNT
      && point.getY() >= bottomRight.getY() - OFF_SCREEN_BORDER_AMOUNT
      && point.getY() <= topLeft.getY() + OFF_SCREEN_BORDER_AMOUNT);
}

/**************************************************************************
 * GAME :: HANDLE COLLISIONS
 * Check for a collision between a asteroid, ship, and bullet.
 **************************************************************************/
void Game :: handleCollisions()
{
   // create a temp vector to add new rocks 
   
   // now check for a hit (if it is close enough to any live bullets)
   for (std::vector <Rock*> :: iterator itRock = rock.begin(); itRock != rock.end(); itRock++)
   {
      for (std::vector <Bullet*> :: iterator itBullet = bullet.begin(); itBullet != bullet.end(); itBullet++)
      {
         if ((*itBullet)->isAlive())
         {
            // this bullet is alive, see if its too close

            // check if the flyingobject is at this point (in case it was hit)
            if ((*itRock) != NULL && (*itRock)->isAlive())
            {

               if (getClosestDistance(**itBullet, **itRock) < (*itRock)->getRockSize()) // add radius comparison here rock.getSize)
               {
                  //we have a hit!
                  (*itRock)->hit();

                  // the bullet is dead as well
                  (*itBullet)->kill();
               }
            }
         }
      } 
   }
   // add temp vector to rock vector
}

/**************************************************************************
 * GAME :: CLEAN UP ZOMBIES
 * Remove any dead objects (take bullets out of the list, deallocate rock)
 **************************************************************************/
void Game :: cleanUpZombies()
{
   // move dead birds and null pointers last
   auto newend = std::partition(rock.begin(), rock.end(), [](Rock* rock) {
       return rock && rock->isAlive();
   });

   // delete deads
   for(auto it = newend; it != rock.end(); ++it) {
       if(*it != nullptr) delete *it;
   }
   // remove the deads from the vector
   rock.erase(newend, rock.end());

//    // Look for dead bullets
//    vector<Bullet>::iterator bulletIt = bullets.begin();
//    while (bulletIt != bullets.end())
//    {
//       Bullet bullet = *bulletIt;
//       // Asteroids Hint:
//       // If we had a list of pointers, we would need this line instead:
//       //Bullet* pBullet = *bulletIt;
      
//       if (!bullet.isAlive())
//       {
//          // If we had a list of pointers, we would need to delete the memory here...
         
         
//          // remove from list and advance
//          bulletIt = bullets.erase(bulletIt);
//       }
//       else
//       {
//          bulletIt++; // advance
//       }
//    }
   
}

/***************************************
 * GAME :: HANDLE INPUT
 * accept input from the user
 ***************************************/
void Game :: handleInput(const Interface & ui)
{
   if (ship.isAlive())
   {  
      if (ui.isUp())
      {
         ship.isThrust();
         ship.applyThrust();
      }

      else
      {
         ship.isNotThrust();
      }
      

      if (ui.isLeft())
      {
         ship.applyRotationLeft();
      }
      
      if (ui.isRight())
      {
         ship.applyRotationRight();
      }

      if (ui.isSpace())
      {
         bullet.push_back(createBullet());
      }
   }
}

/*********************************************
 * GAME :: DRAW
 * Draw everything on the screen
 *********************************************/
void Game :: draw(const Interface & ui)
{
   // draw the bird

   // TODO: Check if you have a valid bird and if it's alive
   // then call it's draw method
   for (std::vector <Rock*> :: iterator it = rock.begin(); it != rock.end(); it++) 
   {  
      if ((*it) != NULL && (*it)->isAlive())
      {
         (*it)->draw();
      }
   }

   for (std::vector <Bullet*> :: iterator it = bullet.begin(); it != bullet.end(); it++) 
   {  
      if ((*it) != NULL && (*it)->isAlive())
      {
         (*it)->draw();
      }
   }

   // draw the ship
   ship.draw();
   
   // Put the score on the screen
   /*Point scoreLocation;
   scoreLocation.setX(topLeft.getX() + 5);
   scoreLocation.setY(topLeft.getY() - 5);
   
   drawNumber(scoreLocation, score); */

}

/*********************************************
 * GAME :: 
 * 
 *********************************************/
void Game::createShip()
{
   Velocity velocity;
   Point point;

   velocity.setDx(0);
   velocity.setDy(0);   
   point.setX(0);
   point.setY(0);
   ship.setVelocity(velocity);
   ship.setPoint(point);
} 
