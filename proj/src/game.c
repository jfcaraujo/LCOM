#include "game.h"

Game* (start_game)(){
  srand((unsigned) time(NULL));

  Game* game = (Game*) malloc(sizeof(Game));
  game->mouse = (Mouse*) malloc(sizeof(Mouse));
  game->ship = (Ship*) malloc(sizeof(Ship));
  game->playerMov = (PlayerMovement*) malloc(sizeof(PlayerMovement));
  game->gameplay = (GamePlay*) malloc(sizeof(GamePlay));
  game->stats = (Stats*) malloc(sizeof(Stats));
  game->transition = (Transition*) malloc(sizeof(Transition));
  game->name = (Name*) malloc(sizeof(Name));
  game->name->name = (char*) malloc(sizeof(char) * 13);
 
  game->video_mem = vg_init(TRUE_COLOR_MODE);
  game->screenSize = getScreenSize();
  game->type = XPM_8_8_8;

  game->secondBuffer = (uint8_t*) malloc(game->screenSize);
  memset(game->secondBuffer, 0, game->screenSize);
  game->bytesPerPixel = getBytesPerPixel();
  game->IGNORE_COLOR = 0xFFC0CB;
  setSecondBuffer(game->secondBuffer);

  if(write_mouse_command(EN_DATA_RP)) return NULL; //Enables data reporting

  uint8_t bit_no;

  if(mouse_subscribe(&bit_no)) return NULL; //Subscribes mouse interrupts
  game->mouse_irq_set = (uint32_t) (BIT(bit_no));

  if(keyboard_subscribe_int(&bit_no)) return NULL; //Subscribes keyboard interrupts
  game->keyboard_irq_set = (uint32_t) (BIT(bit_no));

  if(timer_subscribe_int(&bit_no)) return NULL; //Subscribes timer interrupts
  game->timer_irq_set = (uint32_t) (BIT(bit_no));

  game->state = MENU; 
  game->transition->doingTransition = false;

  game->ship->firing = false;
  if(xpm_load(ship_xpm, game->type, &game->ship->img) == NULL ) return NULL;

  game->playerMov->speed = 5;
  game->playerMov->xDirection = 0;
  game->playerMov->yDirection = 0;

  game->mouse->x = 400;
  game->mouse->y = 300;
  if(xpm_load(target_xpm, game->type, &game->mouse->img) == NULL) return NULL;

  game->maxNumFireballs = 100;
  game->fireballs = (Fireball*) malloc(sizeof(Fireball) * game->maxNumFireballs);
  memset(game->fireballs, 0, sizeof(Fireball) * game->maxNumFireballs);
  game->maxNumAsteroids = 100;
  game->asteroids = (Asteroid*) malloc(sizeof(Asteroid) * game->maxNumAsteroids);
  memset(game->fireballs, 0, sizeof(Fireball) * game->maxNumFireballs);

  game->highscoresSize = 0;
  game->highscores = (HighScore*) malloc(sizeof(HighScore) * 3);
  game->highscores[0].playerName.name = (char*) malloc(13);
  game->highscores[1].playerName.name = (char*) malloc(13);
  game->highscores[2].playerName.name = (char*) malloc(13);

  //Start of first round
  reset_stats(game);

  if(xpm_load(background_xpm, game->type, &game->background) == NULL) return NULL;
  if(xpm_load(Standard_menu_xpm, game->type, &game->Standard_menu) == NULL) return NULL;
  if(xpm_load(Play_highlight_xpm, game->type, &game->Play_highlight) == NULL) return NULL;
  if(xpm_load(Highscores_highlight_xpm, game->type, &game->Highscores_highlight) == NULL) return NULL;
  if(xpm_load(Exit_highlight_xpm, game->type, &game->Exit_highlight) == NULL) return NULL;
  if(xpm_load(fireball_xpm, game->type, &game->fireball_img) == NULL) return NULL;
  if(xpm_load(asteroid_small_xpm, game->type, &game->asteroid_small) == NULL) return NULL;
  if(xpm_load(asteroid_medium_xpm, game->type, &game->asteroid_medium) == NULL) return NULL;
  if(xpm_load(asteroid_big_xpm, game->type, &game->asteroid_big) == NULL) return NULL;

  if(xpm_load(A_xpm,game->type, &game->A) == NULL) return NULL;
  if(xpm_load(B_xpm,game->type, &game->B) == NULL) return NULL;
  if(xpm_load(C_xpm,game->type, &game->C) == NULL) return NULL;
  if(xpm_load(D_xpm,game->type, &game->D) == NULL) return NULL;
  if(xpm_load(E_xpm,game->type, &game->E) == NULL) return NULL;
  if(xpm_load(F_xpm,game->type, &game->F) == NULL) return NULL;
  if(xpm_load(G_xpm,game->type, &game->G) == NULL) return NULL;
  if(xpm_load(H_xpm,game->type, &game->H) == NULL) return NULL;
  if(xpm_load(I_xpm,game->type, &game->I) == NULL) return NULL;
  if(xpm_load(J_xpm,game->type, &game->J) == NULL) return NULL;
  if(xpm_load(K_xpm,game->type, &game->K) == NULL) return NULL;
  if(xpm_load(L_xpm,game->type, &game->L) == NULL) return NULL;
  if(xpm_load(M_xpm,game->type, &game->M) == NULL) return NULL;
  if(xpm_load(N_xpm,game->type, &game->N) == NULL) return NULL;
  if(xpm_load(O_xpm,game->type, &game->O) == NULL) return NULL;
  if(xpm_load(P_xpm,game->type, &game->P) == NULL) return NULL;
  if(xpm_load(Q_xpm,game->type, &game->Q) == NULL) return NULL;
  if(xpm_load(R_xpm,game->type, &game->R) == NULL) return NULL;
  if(xpm_load(S_xpm,game->type, &game->S) == NULL) return NULL;
  if(xpm_load(T_xpm,game->type, &game->T) == NULL) return NULL;
  if(xpm_load(U_xpm,game->type, &game->U) == NULL) return NULL;
  if(xpm_load(V_xpm,game->type, &game->V) == NULL) return NULL;
  if(xpm_load(W_xpm,game->type, &game->W) == NULL) return NULL;
  if(xpm_load(X_xpm,game->type, &game->X) == NULL) return NULL;
  if(xpm_load(Y_xpm,game->type, &game->Y) == NULL) return NULL;
  if(xpm_load(Z_xpm,game->type, &game->Z) == NULL) return NULL;

  if(xpm_load(n1_xpm,game->type, &game->n1) == NULL) return NULL;
  if(xpm_load(n2_xpm,game->type, &game->n2) == NULL) return NULL;
  if(xpm_load(n3_xpm,game->type, &game->n3) == NULL) return NULL;
  if(xpm_load(n4_xpm,game->type, &game->n4) == NULL) return NULL;
  if(xpm_load(n5_xpm,game->type, &game->n5) == NULL) return NULL;
  if(xpm_load(n6_xpm,game->type, &game->n6) == NULL) return NULL;
  if(xpm_load(n7_xpm,game->type, &game->n7) == NULL) return NULL;
  if(xpm_load(n8_xpm,game->type, &game->n8) == NULL) return NULL;
  if(xpm_load(n9_xpm,game->type, &game->n9) == NULL) return NULL;
  if(xpm_load(n0_xpm,game->type, &game->n0) == NULL) return NULL;
  if(xpm_load(n_two_dots_xpm,game->type, &game->n_two_dots) == NULL) return NULL;
  if(xpm_load(Slash_xpm,game->type, &game->slash) == NULL) return NULL;
  if(xpm_load(minus_xpm,game->type, &game->minus) == NULL) return NULL;
  if(xpm_load(Space_xpm,game->type, &game->Space) == NULL) return NULL;

  if(xpm_load(Faster_xpm,game->type, &game->Faster) == NULL) return NULL;
  if(xpm_load(Game_over_xpm,game->type, &game->Game_over) == NULL) return NULL;
  if(xpm_load(New_highscore_xpm,game->type, &game->New_highscore) == NULL) return NULL;
  if(xpm_load(New_round_xpm,game->type, &game->New_round) == NULL) return NULL;
  if(xpm_load(highscores_xpm, game->type, &game->HighScores) == NULL) return NULL;

  return game;
}

bool (game_handler)(Game* game, keyboard_event_t kbd_evt)
{
  if (game->state == MENU || game->state == OPTION_1 || game->state == OPTION_2 || game->state == OPTION_3 ) {
    double mouse_center_x = (game->mouse->x + game->mouse->img.width / 2.0);
    double mouse_center_y = (game->mouse->y + game->mouse->img.height / 2.0);
    int choice = which_menu(mouse_center_x,mouse_center_y);
    switch(choice){
      case 1: {
        game->state = OPTION_1;
        break;
      }
      case 2: {
        game->state = OPTION_2;
        break;
      }
      case 3: {
        game->state = OPTION_3;
        break;
      }
      default:
        break;  
    }
  }

  switch (game->state) {
    case MENU: {
      if(kbd_evt == UP) 
        game->state = OPTION_1;
      else if(kbd_evt == DOWN) 
        game->state = OPTION_2;
      break;
      }
    case OPTION_1: {
      if(kbd_evt == UP)
        game->state = OPTION_3;
      else if(kbd_evt == DOWN)
        game->state = OPTION_2;
      else if(kbd_evt == ENTER || game->ship->firing) {
        game->state = RUNNING_GAME;}   
      break;
    }
    case OPTION_2: {
      if(kbd_evt == UP)
        game->state = OPTION_1;
      else if(kbd_evt == DOWN)
        game->state = OPTION_3;
      else if(kbd_evt == ENTER || game->ship->firing)
        game->state = HIGHSCORES;
      break;
    } 
    case OPTION_3: {
      if(kbd_evt == UP)
        game->state = OPTION_2;
      else if(kbd_evt == DOWN)
        game->state = OPTION_1;
      else if(kbd_evt == ENTER || game->ship->firing)
        return false;
      break;  
    }       
    case RUNNING_GAME: {
      if(kbd_evt == ESC)
        game->state = MENU;  
      break;
    }
    case GAME_OVER: {
      if (kbd_evt == ENTER) {
        reset_stats(game);
        game->state = HIGHSCORES;
      }
      else if (kbd_evt == ESC) {
        reset_stats(game);
        game->state = MENU;
      }
      break;
    }
    case NEW_HIGHSCORE:{
      writing_name(game);
      if (kbd_evt == ESC) {
        reset_stats(game);
        game->state = MENU;
      }
      break;
    }
    case HIGHSCORES:{
      if(kbd_evt == ESC)
        game->state = MENU;
      if(kbd_evt == ENTER)
        game->state = RUNNING_GAME;
      break;
    }
      
  }
  return true;
}

int (which_menu)(int x, int y){
  if (x < 310 || x > 500)
    return 0;
  if (y > 180 && y < 230)
    return 1;
  if (y > 270 && y < 320)
    return 2;
  if (y > 360 && y < 410)
    return 3;
  else return 0;
}

void(reset_stats)(Game* game)
{
  game->stats->asteroids_destroyed = 0;
  game->stats->current_round = 1;
  game->stats->score = 0;
  game->stats->time_seconds = 0;

  game->ship->x = 300;
  game->ship->y = 300;

  game->gameplay->asteroids_per_round = 10;
  game->gameplay->asteroids_spawn_rate = 1;
  game->gameplay->asteroids_spawned = 0;
  game->gameplay->asteroids_speed = 2;
  game->gameplay->fire_rate = 6;

  game->numAsteroids = 0;
  game->numFireballs = 0;

  game->name->size = 0;
  memset(game->name->name, ' ', 13);
  game->name->letter = '.';
}

void (writing_name)(Game* game){
  if (game->name->letter == '.' || game->name->letter == ',') {
    return;
  }
  else if (game->name->letter == '-' && game->name->size > 0) {
    game->name->size--;
    game->name->name[game->name->size] = ' ';
  }
  else if (game->name->size < 12) {
    game->name->name[game->name->size] = game->name->letter;
    game->name->size++;
  } 
}

int exit_game(Game* game){
  int result = 0;
  if( mouse_unsubscribe() ) result = 1; //Unsubscribes mouse interrupts
  if( write_mouse_command(DIS_DATA_RP)) result = 1; //Disables data reporting
  if( keyboard_unsubscribe_int() ) result = 1; //Unsubscribes keyboard interrupts
  if( timer_unsubscribe_int() ) result = 1; //Unsubscribes timer interrupts

  free(game->mouse);
  free(game->ship);
  free(game->playerMov);
  free(game->gameplay);
  free(game->stats);
  free(game->transition);
  free(game->name);
  free(game->fireballs);
  free(game->asteroids);
  free(game->highscores);

  free(game->background.bytes);
  free(game->Standard_menu.bytes);
  free(game->Play_highlight.bytes);
  free(game->Highscores_highlight.bytes);
  free(game->Exit_highlight.bytes);
  free(game->fireball_img.bytes);
  free(game->asteroid_small.bytes);
  free(game->asteroid_medium.bytes);
  free(game->asteroid_big.bytes);

  free(game->A.bytes);
  free(game->B.bytes);
  free(game->C.bytes);
  free(game->D.bytes);
  free(game->E.bytes);
  free(game->F.bytes);
  free(game->F.bytes);
  free(game->G.bytes);
  free(game->H.bytes);
  free(game->I.bytes);
  free(game->J.bytes);
  free(game->K.bytes);
  free(game->L.bytes);
  free(game->M.bytes);
  free(game->N.bytes);
  free(game->O.bytes);
  free(game->P.bytes);
  free(game->Q.bytes);
  free(game->R.bytes);
  free(game->S.bytes);
  free(game->T.bytes);
  free(game->U.bytes);
  free(game->V.bytes);
  free(game->W.bytes);
  free(game->Y.bytes);
  free(game->X.bytes);
  free(game->Z.bytes);

  free(game->n0.bytes);
  free(game->n1.bytes);
  free(game->n2.bytes);
  free(game->n3.bytes);
  free(game->n4.bytes);
  free(game->n5.bytes);
  free(game->n6.bytes);
  free(game->n7.bytes);
  free(game->n8.bytes);
  free(game->n9.bytes);
  free(game->n_two_dots.bytes);
  free(game->slash.bytes);
  free(game->minus.bytes);
  free(game->Space.bytes);

  free(game->Faster.bytes);
  free(game->New_highscore.bytes);
  free(game->New_round.bytes);
  free(game->Game_over.bytes);

  free(game);

  if( vg_exit()) result = 1; //Returns to text mode

  return result;
}


