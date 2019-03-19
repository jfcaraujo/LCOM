#include "fireball.h"

void (create_fireball)(Game* game)
{
  if(game->numFireballs >= game->maxNumFireballs) return;

  int8_t speed = 10;

  Fireball fireball;

  fireball.mov.xSpeed = cos(game->ship->rotation_angle) * speed;
  fireball.mov.ySpeed = sin(game->ship->rotation_angle) * speed;
  fireball.angle = game->ship->rotation_angle;

  fireball.mov.x = (game->ship->x + game->ship->img.width / 2.0) + fireball.mov.xSpeed * 3 - (game->fireball_img.width/2.0);
  fireball.mov.y = (game->ship->y + game->ship->img.height / 2.0) + fireball.mov.ySpeed * 3 - (game->fireball_img.width/2.0);

  game->numFireballs++;
  game->fireballs[game->numFireballs - 1] = fireball;
}

void (move_Fireballs)(Game* game)
{
  Fireball* fireball;
  xpm_image_t rotated_fireball;
  rotated_fireball.height = game->fireball_img.height;
  rotated_fireball.width = game->fireball_img.width;

  for(size_t i = 0; i < game->numFireballs; i++) {

    fireball = &game->fireballs[i];

    rotated_fireball.bytes = rotate_sprite(game, game->fireball_img, fireball->angle);
    print_sprite(fireball->mov.x, fireball->mov.y, rotated_fireball);
    free(rotated_fireball.bytes);

    fireball->mov.x += fireball->mov.xSpeed;
    fireball->mov.y += fireball->mov.ySpeed;

    if(fireball->mov.x < 0 || (fireball->mov.x + game->fireball_img.width) > (int) getHorRes() || fireball->mov.y < 0 || (fireball->mov.y + game->fireball_img.height) > (int) getVerRes()) {
      destroy_fireball(game, i);
    }
  }
}

void (destroy_fireball)(Game* game, int index)
{
  for(unsigned int i = index; i < game->numFireballs - 1 ; i++) {
    game->fireballs[i] = game->fireballs[i+1];
  }
  game->numFireballs--;
}


