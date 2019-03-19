#include "gamePlay.h"

bool (detectCollision)(int16_t x1, int16_t y1, uint16_t width1, uint16_t height1, int16_t x2, int16_t y2, uint16_t width2, uint16_t height2)
{
  return !((x1 + width1) < x2 || (x2 + width2) < x1 || (y1 + height1) < y2 || (y2 + height2) < y1);
}

void (checkFireballCollision)(Game* game)
{
  Fireball* fireball;
  Asteroid* asteroid;
  uint16_t asteroid_width;
  uint16_t asteroid_height;

  for(unsigned int i = 0; i < game->numFireballs; i++) {
    fireball = &game->fireballs[i];

    for(unsigned int j = 0; j < game->numAsteroids; j++) {
      asteroid = &game->asteroids[j];

      switch(asteroid->size) {
        case SMALL: {
          asteroid_width = game->asteroid_small.width;
          asteroid_height = game->asteroid_small.height;
          break; 
          }
        case MEDIUM: {
          asteroid_width = game->asteroid_medium.width;
          asteroid_height = game->asteroid_medium.height;
          break;
        }
        case BIG: {
          asteroid_width = game->asteroid_big.width;
          asteroid_height = game->asteroid_big.height;
          break;
        }       
      }

      if(detectCollision(fireball->mov.x, fireball->mov.y, game->fireball_img.width, game->fireball_img.height, asteroid->mov.x, asteroid->mov.y, asteroid_width, asteroid_height)){
        switch(asteroid->size) {
          case SMALL:
            game->stats->score += 10;
            break;
          case MEDIUM:
            game->stats->score += 20;
            break;
          case BIG:
            game->stats->score += 30;
            break;
          default:
            break;}
        game->stats->asteroids_destroyed++;

        destroy_fireball(game, i);
        split_asteroid(game, j);
        
        break;
      }
        
    }
  }
}

void (checkShipCollision)(Game* game)
{
  Asteroid* asteroid;
  uint16_t asteroid_radius, asteroid_center_x, asteroid_center_y;
  int x, y, z;
  bool hit = false;

  for(unsigned int i = 0; i < game->numAsteroids; i++) {
    asteroid = &game->asteroids[i];

    switch(asteroid->size) {
      case SMALL: {
        asteroid_radius = 18;
        asteroid_center_x = 26;
        asteroid_center_y = 22;
        break;
      }
      case MEDIUM: {
        asteroid_radius = 30;
        asteroid_center_x = 31;
        asteroid_center_y = 34;
        break;
      }
      case BIG: {
        asteroid_radius = 48;
        asteroid_center_x = 52;
        asteroid_center_y = 54;
        break;
      }       
    }

    x = abs(asteroid->mov.x + asteroid_center_x - 5 - game->ship->x);
    y = abs(asteroid->mov.y + asteroid_center_y - 35 - game->ship->y);
    z = sqrt(pow(x,2) + pow(y,2));
    if (z < asteroid_radius) hit = true;

    x = abs(asteroid->mov.x + asteroid_center_x - 39-game->ship->x);
    y = abs(asteroid->mov.y + asteroid_center_y - 35-game->ship->y);
    z = sqrt(pow(x,2) + pow(y,2)); 
    if (z < asteroid_radius) hit = true;

    x = abs(asteroid->mov.x + asteroid_center_x - 21 - game->ship->x);
    y = abs(asteroid->mov.y + asteroid_center_y - 7 - game->ship->y);
    z = sqrt(pow(x, 2) + pow(y, 2)); 
    if (z < asteroid_radius) hit = true;

    if (hit) {
      if(check_new_highscore(game) < 3) {
        game->state = NEW_HIGHSCORE;
      }
      else game->state = GAME_OVER;

      break;
    }

  } 
}

void (end_round)(Game* game)
{
  game->numAsteroids = 0;
  game->numFireballs = 0;

  game->ship->x = 300;
  game->ship->y = 300;
}

void(next_round)(Game* game)
{
  end_round(game);

  game->stats->asteroids_destroyed = 0;
  game->stats->current_round++;
  game->stats->score += 100;
  
  game->gameplay->asteroids_per_round =(((game->stats->current_round-1)%5)+1)*10;
  game->gameplay->asteroids_spawn_rate = 1 + ((game->stats->current_round - 1) / 5);
  game->gameplay->asteroids_spawned = 0;
  game->gameplay->asteroids_speed = 2 + ((game->stats->current_round - 1) / 5);

  if ( (game->stats->current_round % 5) == 1) print_sprite(0, 0, game->Faster);
  else print_sprite(0, 0, game->New_round);

  game->transition->doingTransition = true;
  
  uint32_t transition_time = 2;
  game->transition->finalTime = game->stats->time_seconds + transition_time;
}

int (get_number_length)(int number)
{
  if(number == 0) return 1;
  else 
    return (floor(log10(number)) + 1);
}

void (print_HUD)(Game* game)
{
  char score[11];
  sprintf(score, "%d", game->stats->score);
  print_string(game, 130, 565, score, get_number_length(game->stats->score));

  char current_round[3];
  sprintf(current_round, "%d", game->stats->current_round);
  print_string(game, 700, 565, current_round, get_number_length(game->stats->current_round));
}
