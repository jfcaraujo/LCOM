// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

// Any header files included below this line should have been created by you
#include "graphics.h"
#include "keyboard.h"
#include "mouse.h"
#include "game.h"
#include "visuals.h"
#include "devicesManager.h"
#include "highscores.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  //lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;


  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

extern uint32_t interruptsCounter;

int (proj_main_loop)(int UNUSED(argc), char *argv[])
{
  (void) argv;
  int ipc_status, r;
  message msg;

  bool running = true; //Becomes false after choosing exit option in menu

  //State machine variables
  keyboard_event_t kbd_evt;

  Game* game = start_game();
  load_highscores(game);
  
  while (running) //Ends when exit option is chosen
  { 
    //Get a request message
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failes with: %d", r);
      continue;
    }

    if(is_ipc_notify(ipc_status)) { //received notification
      switch(_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: //hardware interrupt notification
          
          //MOUSE
          if(msg.m_notify.interrupts & game->mouse_irq_set) { //Mouse interrupt
            mouse_manager(game); 
          }

          //KEYBOARD
          if(msg.m_notify.interrupts & game->keyboard_irq_set) //Keyboard interrupt
            keyboard_manager(&kbd_evt, game);   
            

          //TIMER 
          if(msg.m_notify.interrupts & game->timer_irq_set) { //Timer interrupt
            timer_int_handler();

            running = game_handler(game, kbd_evt);
            kbd_evt = STOP;
            showFrame(game);
            game->name->letter = '.';
            
          } 
          break;

        default:
          break; 
      }
    }
  }

  save_highscores(game);
  if( exit_game(game)) return 1;
  return 0;
}


