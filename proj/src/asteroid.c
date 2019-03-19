#include "asteroid.h"

void (create_asteroid)(Game* game)
{
  if(game->gameplay->asteroids_spawned >= game->gameplay->asteroids_per_round) return;

  game->gameplay->asteroids_spawned++;

  Asteroid asteroid;

  uint16_t width;
  uint16_t height;

  //Choose asteroid size
  asteroid.size = rand() % 3;
  switch (asteroid.size) {
    case SMALL: {
      width = game->asteroid_small.width;
      height = game->asteroid_small.height;
      break;
    }
    case MEDIUM: {
      width = game->asteroid_medium.width;
      height = game->asteroid_medium.height;
      break;
    }
    case BIG: {
      width = game->asteroid_big.width;
      height = game->asteroid_big.height;
      break;
    }
  }

  //Choose spawn location
  asteroid_spawn_t spawn = rand() % 4;
  switch (spawn) {
    case LEFT_SIDE: {
    asteroid.mov.x = 0;
    asteroid.mov.y = rand() % (getVerRes() - height);
    break;
    }
    case TOP: {
      asteroid.mov.x = rand() % (getHorRes() - width);
      asteroid.mov.y = 0;
      break;
    }
    case RIGHT_SIDE: {
      asteroid.mov.x = getHorRes() - width;
      asteroid.mov.y = rand() % (getVerRes() - height);
      break;
    }
    case BOTTOM: {
      asteroid.mov.x = rand() % (getHorRes() - width);
      asteroid.mov.y = (getVerRes() - height);
      break;
    }
  }

  //Get random angle between 0 and 2*pi radians:
  double PI = acos(-1);
  double random0to1 = rand() / (RAND_MAX + 1.0); 
  double randomAngle = random0to1 * 2 * PI;
  
  asteroid.mov.xSpeed = (double) (cos(randomAngle) * game->gameplay->asteroids_speed);
  asteroid.mov.ySpeed = (double) (sin(randomAngle) * game->gameplay->asteroids_speed);  

  game->numAsteroids++;
  game->asteroids[game->numAsteroids - 1] = asteroid;
}

void (move_asteroids)(Game* game)
{
  Asteroid* asteroid;
  uint16_t width;
  uint16_t height;

  for(size_t i = 0; i < game->numAsteroids; i++) {

    asteroid = &game->asteroids[i];

    switch (asteroid->size) {
      case SMALL: {
        width = game->asteroid_small.width;
        height = game->asteroid_small.height;
        print_sprite(asteroid->mov.x, asteroid->mov.y, game->asteroid_small);
        break;
      }
      case MEDIUM: {
        width = game->asteroid_medium.width;
        height = game->asteroid_medium.height;
        print_sprite(asteroid->mov.x, asteroid->mov.y, game->asteroid_medium);
        break;
      } 
      case BIG: {
        width = game->asteroid_big.width;
        height = game->asteroid_big.height;
        print_sprite(asteroid->mov.x, asteroid->mov.y, game->asteroid_big);
        break;
      }
    }

    asteroid->mov.x += asteroid->mov.xSpeed;
    asteroid->mov.y += asteroid->mov.ySpeed;

    checkCoordinates(width, height, &asteroid->mov.x, &asteroid->mov.y);
  }
}

void (split_asteroid)(Game* game, int index)
{
  Asteroid* asteroid = &game->asteroids[index];
  Asteroid newAsteroid;

  double PI = acos(-1.0);

  switch (asteroid->size) {
    case SMALL: {
      destroy_asteroid(game, index);
      return;
    }
    case MEDIUM: {
      asteroid->size = SMALL;
      newAsteroid.size = SMALL;
      break;
    }
    case BIG: {
      asteroid->size = MEDIUM;
      newAsteroid.size = MEDIUM;
      break;
    }
  }

  double angle = acos(asteroid->mov.xSpeed / game->gameplay->asteroids_speed);

  newAsteroid.mov.xSpeed = (double) (cos(angle + (PI / 2.0)) * game->gameplay->asteroids_speed);
  newAsteroid.mov.ySpeed = (double) (sin(angle + (PI / 2.0)) * game->gameplay->asteroids_speed);
  newAsteroid.mov.x = asteroid->mov.x;
  newAsteroid.mov.y = asteroid->mov.y;

  asteroid->mov.xSpeed = (double) (cos(angle - (PI / 2.0)) * game->gameplay->asteroids_speed);
  asteroid->mov.ySpeed = (double) (sin(angle - (PI / 2.0)) * game->gameplay->asteroids_speed);

  game->numAsteroids++;
  game->asteroids[game->numAsteroids - 1] = newAsteroid;
}

void (destroy_asteroid)(Game* game, int index)
{
  for(unsigned int i = index; i < game->numAsteroids - 1 ; i++) {
    game->asteroids[i] = game->asteroids[i+1];
  }

  game->numAsteroids--;
}

