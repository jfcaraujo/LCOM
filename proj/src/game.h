#ifndef GAME_H
#define GAME_H

#include <lcom/lcf.h>
#include <minix/syslib.h>
#include <stdint.h>
#include <minix/sysutil.h>
#include <math.h>
#include <string.h>

#include "keyboard.h"
#include "mouse.h"
#include "graphics.h"
#include "rtc.h"

//XPMs
#include "xpm.h"

/** @defgroup Game Game
 * @{
*/

typedef enum {MENU, OPTION_1, OPTION_2, OPTION_3, RUNNING_GAME, GAME_OVER, NEW_HIGHSCORE, HIGHSCORES} state_t;
typedef enum {UP, DOWN,/* LEFT, RIGHT, */STOP, ENTER, ESC/*, Q, E, R, T, Y, U, I, O, P, A, D, F, G, H, J, K, L, Z, X, C, V, B, N, M, SPACE, BACKSPACE*/} keyboard_event_t;
typedef enum {TOP, LEFT_SIDE, RIGHT_SIDE, BOTTOM} asteroid_spawn_t;
typedef enum {SMALL, MEDIUM, BIG} asteroid_size_t;

typedef struct {
  int16_t x;
  int16_t y;
  xpm_image_t img;
  bool firing;
  double rotation_angle;
} Ship;

typedef struct {
  int8_t speed;
  int8_t xDirection;
  int8_t yDirection;
} PlayerMovement;

typedef struct {
  int16_t x;
  int16_t y;
  xpm_image_t img;
} Mouse;

typedef struct {
  int16_t x;
  int16_t y;
  double xSpeed;
  double ySpeed;
} ObjectMovement;

typedef struct {
  ObjectMovement mov;
  double angle;
} Fireball;

typedef struct {
  ObjectMovement mov;
  asteroid_size_t size;
} Asteroid;

typedef struct {
  uint32_t current_round;
  int32_t score;
  uint32_t asteroids_destroyed;
  uint32_t time_seconds;
} Stats;

typedef struct {
  char* name;
  int size;
  char letter;
} Name;

typedef struct {
  uint16_t asteroids_speed;
  uint16_t asteroids_spawn_rate;
  uint16_t asteroids_spawned;
  uint16_t asteroids_per_round;
  uint16_t fire_rate;
} GamePlay;

typedef struct {
  Date date;
  Stats stats;
  Name playerName;
} HighScore;

typedef struct {
  bool doingTransition;
  uint32_t finalTime;
} Transition;

typedef struct {
  uint8_t* secondBuffer;
  uint8_t* video_mem;
  size_t screenSize;
  int bytesPerPixel;
  enum xpm_image_type type;
  uint32_t IGNORE_COLOR;

  uint32_t keyboard_irq_set;
  uint32_t timer_irq_set;
  uint32_t mouse_irq_set;

  state_t state;

  Ship* ship;
  PlayerMovement* playerMov;
  Mouse* mouse;
  Stats* stats;
  GamePlay* gameplay;
  Transition* transition;
  Name* name;

  unsigned int highscoresSize;
  HighScore* highscores; 

  unsigned int maxNumFireballs;
  Fireball* fireballs;
  unsigned int numFireballs;

  unsigned int maxNumAsteroids;
  Asteroid* asteroids;
  unsigned int numAsteroids;

  xpm_image_t background;
  xpm_image_t Standard_menu;
  xpm_image_t Play_highlight;
  xpm_image_t Highscores_highlight;
  xpm_image_t Exit_highlight;
  xpm_image_t fireball_img;
  xpm_image_t asteroid_small;
  xpm_image_t asteroid_medium;
  xpm_image_t asteroid_big;
  xpm_image_t A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, n1, n2, n3, n4, n5, n6, n7, n8, n9, n0;
  xpm_image_t n_two_dots;
  xpm_image_t minus;
  xpm_image_t slash;
  xpm_image_t Space;
  xpm_image_t Faster;
  xpm_image_t Game_over;
  xpm_image_t New_highscore;
  xpm_image_t New_round;
  xpm_image_t HighScores;
  
} Game;

/**
* @brief Allocates all memory needed for the game, subscribes interrupts, initializes struct values
*
* @return returns a Game struct pointer
*/
Game* (start_game)();

/**
* @brief Handles the event received from the keyboard and checks the position of the mouse if the game is in a menu state, changing the current state of the game
*
* @param game Pointer to Game struct with the game information
* @param kbd_event Event that tells wich key was pressed/released
* @return returns false if exit option was chosen. True otherwise
*/
bool (game_handler)(Game* game, keyboard_event_t kbd_event);

/**
 * @brief Auxiliary function for game_handler that indicates in wich option the mouse is at
 * 
 * @param x X position of the mouse
 * @param y Y position of the mouse
 * @return 0 if the mouse is not at any option
 * @return 1 if the mouse is at play option
 * @return 2 if the mouse is at highscores option
 * @return 3 if the mouse is at exit option
 */
int (which_menu)(int x, int y);

/**
 * @brief Initializes variables related to the stats of the game
 * 
 * @param game Pointer to Game struct with the game information
 */
void (reset_stats)(Game* game);

void (writing_name)(Game* game);

/**
 * @brief Exits the game, freeing all the memoy alocated, unsubscribes interrupts and exits graphics mode
 * 
 * @param game Pointer to Game struct with the game information
 */
int (exit_game)(Game* game);

/** @} end of Game */
#endif /* GAME_H */

