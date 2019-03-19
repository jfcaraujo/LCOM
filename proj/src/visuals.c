#include "visuals.h"

void (showFrame)(Game* game)
{
  memcpy(game->video_mem, game->secondBuffer, game->screenSize);
  updateFrame(game);
}

extern uint32_t interruptsCounter;

void (updateFrame)(Game* game)
{
  switch (game->state) {
    case MENU: {
      print_sprite(0, 0, game->Standard_menu);
      break;
    }
    case OPTION_1: {
      print_sprite(0, 0, game->Play_highlight);
      break;
    }
    case OPTION_2: {
      print_sprite(0, 0, game->Highscores_highlight);
      break;
    }      
    case OPTION_3: {
      print_sprite(0, 0, game->Exit_highlight);
      break;
    }
    case RUNNING_GAME: {

      if( (interruptsCounter % sys_hz()) == 0)
        game->stats->time_seconds++; 

      if( game->transition->doingTransition ) {

        if(game->transition->finalTime <= game->stats->time_seconds)
          game->transition->doingTransition = false;
        else
          return;
      }

      //BackGround:
      print_sprite(0, 0, game->background);

      //HUD
      print_HUD(game);

      //Ship:
      print_ship(game);

      //Firing:
      if( (interruptsCounter % (sys_hz() / game->gameplay->fire_rate)) == 0) {
        if(game->ship->firing) create_fireball(game); 
      }

      //Spawn asteroid
      if( (interruptsCounter % (sys_hz() / game->gameplay->asteroids_spawn_rate)) == 0) {
        create_asteroid(game);
      }

      //Fireballs:
      move_Fireballs(game);

      //Asteroids:
      move_asteroids(game);

      //Detects collisions between firballs and asteroids:
      checkFireballCollision(game);

      //Detects collision between ship and asteroids:
      checkShipCollision(game);

      if ( (game->numAsteroids == 0) && game->gameplay->asteroids_spawned >= game->gameplay->asteroids_per_round) {
        next_round(game);
      }

      break;
    }
    case GAME_OVER: {
      print_sprite(0, 0, game->Game_over);

      char score[11];
      sprintf(score, "%d", game->stats->score);
      print_string(game, 247, 275, score, get_number_length(game->stats->score));
      break;
    }
    case NEW_HIGHSCORE:{
      print_sprite(0, 0, game->New_highscore);
      print_string(game,237,406,game->name->name,game->name->size);

      char score[11];
      sprintf(score, "%d", game->stats->score);
      print_string(game, 247, 275, score, get_number_length(game->stats->score));

      if (game->name->letter == ',') {
        write_new_highscore(game, check_new_highscore(game));
        reset_stats(game);
        game->state = HIGHSCORES;
      }
      break;
    }
    case HIGHSCORES: {
      print_sprite(0, 0, game->HighScores);
      print_highscores(game);
      break;
    }
  }
  
  print_mouse(game);
}

void (print_ship)(Game* game)
{
  game->ship->x += game->playerMov->xDirection * game->playerMov->speed;
  game->ship->y += game->playerMov->yDirection * game->playerMov->speed;

  checkCoordinates(game->ship->img.width, game->ship->img.height, &game->ship->x, &game->ship->y);

  double mouse_center_x = (game->mouse->x + game->mouse->img.width / 2.0);
  double mouse_center_y = (game->mouse->y + game->mouse->img.height / 2.0);

  double ship_center_x = (game->ship->x + game->ship->img.width / 2.0);
  double ship_center_y = (game->ship->y + game->ship->img.height / 2.0);

  double dx = mouse_center_x - ship_center_x;
  double dy = mouse_center_y - ship_center_y;

  game->ship->rotation_angle = atan2(dy, dx);

  uint8_t* rotated_ship_bytes = rotate_sprite(game, game->ship->img, game->ship->rotation_angle);

  xpm_image_t rotated_ship_xpm;
  rotated_ship_xpm.bytes = rotated_ship_bytes;
  rotated_ship_xpm.width = game->ship->img.width;
  rotated_ship_xpm.height = game->ship->img.height;

  print_sprite(game->ship->x, game->ship->y, rotated_ship_xpm); //Displays the sprite in the new coordinates

  free(rotated_ship_bytes);
}

void (print_mouse)(Game* game)
{
  //Check of mouse coordinates
  if(game->mouse->x < 0) //mouse.x is negative
    game->mouse->x = 0;
  else if(game->mouse->x + game->mouse->img.width >= (int) getHorRes())
    game->mouse->x = getHorRes() - game->mouse->img.width - 1;
  
  if(game->mouse->y < 0) //mouse.y is negative
    game->mouse->y = 0;
  else if(game->mouse->y + game->mouse->img.height >= (int) getVerRes()) 
    game->mouse->y = getVerRes() - game->mouse->img.height - 1;

  print_sprite(game->mouse->x, game->mouse->y, game->mouse->img);
}

