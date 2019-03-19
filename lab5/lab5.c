// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "graphics.h"
#include "keyboard.h"
#include "i8042.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

extern uint32_t interruptsCounter; //Counter for timer interrupts

int (video_test_init)(uint16_t mode, uint8_t delay) {

  if( vg_init(mode) == NULL ) return 1; //Initializes de specified mode

  sleep(delay); //Waits delay seconds

  if (vg_exit()) return 1; //Returns to text mode

  return 0;
}

int (video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {

  int ipc_status, r;
  message msg;
  uint8_t bit_no;
  uint32_t irq_set;

  bool receiving = true; //Becomes false when esc key is released

  if(keyboard_subscribe_int(&bit_no)) return 1; //Subscbribes keyboard interrupts
  irq_set = (uint32_t) (BIT(bit_no));

  if( vg_init(mode) == NULL) return 1; //Initializes the specified mode

  if( vg_draw_rectangle(x, y, width, height, color)) { //Prints on the screen a rectangle with the dimentions specified in width and length

  if ( vg_exit()) return 1; //Returns to text mode
    return 1;
  }

  while (receiving) //End when esc key is released
  {
    //Get a request message
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failes with: %d", r);
      continue;
    }

    if(is_ipc_notify(ipc_status)) { //received notification
      switch(_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: //hardware interrupt notification
          if(msg.m_notify.interrupts & irq_set) { //subscribed keyboard interrupt

            kbc_ih(); //Reads scan code

            if(escReleased()) receiving = false; //Esc key released

          break;

        default:
          break; 
        }
      }
    }
  }

  if( keyboard_unsubscribe_int()) return 1; //Unsubscribes keyboard interrupts
  if ( vg_exit()) return 1; //Returns to text mode
  return 0;
}

int (video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {

  int ipc_status, r;
  message msg;
  uint8_t bit_no;
  uint32_t irq_set;
  int height,width,color;
  bool receiving = true;

  if( vg_init(mode)==NULL ) return 1;
  height=getHeight()/no_rectangles;
  width=getWidth()/no_rectangles;
  for (unsigned int i=0;i<no_rectangles;i++)
    for (unsigned int j=0;j<no_rectangles;j++){
      color=select_color(i,j,first,step,no_rectangles);
      if (color==-1)
        return 1;
      if (vg_draw_rectangle(j*width,i*height,width,height,color))
        return 1;
    }
  if(keyboard_subscribe_int(&bit_no)) return 1; 
  irq_set = (uint32_t) (BIT(bit_no));

  while (receiving) //End when esc key is released
  {
    //Get a request message
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failes with: %d", r);
      continue;
    }

    if(is_ipc_notify(ipc_status)) { //received notification
      switch(_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: //hardware interrupt notification
          if(msg.m_notify.interrupts & irq_set) { //subscribed keyboard interrupt

            kbc_ih();

            if(escReleased()) receiving = false;

          break;

        default:
          break; 
        }
      }
    }
  }

  if( keyboard_unsubscribe_int()) return 1;
  if ( vg_exit()) return 1;
  return 0;
}

int (video_test_xpm)(const char * xpm[], uint16_t x, uint16_t y)
{
  int ipc_status, r;
  message msg;
  uint8_t bit_no;
  uint32_t irq_set;

  bool receiving = true; //Becomes false when the esc key is released

  if( vg_init(INDEXED_MODE) == NULL) return 1; //Initializes the specified mode

  if(keyboard_subscribe_int(&bit_no)) return 1; //Subscribes keyboard interrupts
  irq_set = (uint32_t) (BIT(bit_no));

  int wd, hg;
  char *sprite = read_xpm(xpm, &wd, &hg);
  if(print_sprite(x, y, sprite, wd, hg)) return 1; //Copys to the VRAM the sprite obtained

  while (receiving) //End when esc key is released
  {
      //Get a request message
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failes with: %d", r);
      continue;
    }

    if(is_ipc_notify(ipc_status)) { //received notification
      switch(_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: //hardware interrupt notification
          if(msg.m_notify.interrupts & irq_set) { //subscribed keyboard interrupt

            kbc_ih(); //Reads the scan code

            if(escReleased()) receiving = false; //Esc key released

          break;

        default:
          break; 
        }
      }
    }
  }

  if( keyboard_unsubscribe_int()) return 1; //Unsubscribes keyboard interrupts
  if ( vg_exit()) return 1; //Returns to text mode
  return 0;
}

extern uint32_t interruptsCounter; //Gets global variable interrupts Counter from timer.c file
int (video_test_move)(const char *xpm[], uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,                                  int16_t speed, uint8_t fr_rate)
{
  int ipc_status, r;
  message msg;
  uint8_t bit_no;
  uint32_t keyboard_irq_set, timer_irq_set;

  bool receiving = true; //Becomes false when the esc key is released
  int8_t xDirection, yDirection;

  if(xi == xf && yi != yf) { //Horizontal movement
    xDirection = 0; 
    yDirection = 1;
  }
  else if(xi != xf && yi == yf) { //Vertical movement
    xDirection = 1;
    yDirection = 0;
  } 
  else return 1;
 
  uint32_t ticksPerFrame = sys_hz() / fr_rate;
  int16_t framesCounter = 0;

  if( vg_init(INDEXED_MODE) == NULL) return 1; //Inisitalizes the 0x105 indexed mode

  if(keyboard_subscribe_int(&bit_no)) return 1; //Subscribes keyboard interrupts
  keyboard_irq_set = (uint32_t) (BIT(bit_no));

  if(timer_subscribe_int(&bit_no)) return 1; //Subscribes timer interrupts
  timer_irq_set = (uint32_t) (BIT(bit_no));

  int wd, hg;
  char *sprite = read_xpm(xpm, &wd, &hg);
  if(print_sprite(xi, yi, sprite, wd, hg)) return 1; //Displays the pixmap provided as an XPM image

  while (receiving) //End when esc key is released
  {
    //Get a request message
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failes with: %d", r);
      continue;
    }

    if(is_ipc_notify(ipc_status)) { //received notification
      switch(_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: //hardware interrupt notification
          if(msg.m_notify.interrupts & keyboard_irq_set) { //subscribed keyboard interrupt

            kbc_ih();
            if(escReleased()) receiving = false;
          }

          if(msg.m_notify.interrupts & timer_irq_set) { //Subscrbied timer interrupt
            timer_int_handler();
            framesCounter++;

            if(interruptsCounter == ticksPerFrame) { //Do: Update frame
              interruptsCounter = 0;

              //Does an animation. Cleards the previously displayed sprite and prints a new one at the new coordinates
              if(move_sprite(sprite, wd, hg, &xi, &yi, speed, &framesCounter, xDirection, yDirection)) 
                return  1;
            }

          }
          break;

        default:
          break; 
      }
    }
  }

  if( keyboard_unsubscribe_int()) return 1; //Unsubscribes keyboard interrupts
  if ( vg_exit()) return 1; //Returns to text mode
  return 0;
}

int  (video_test_controller)()
{
  struct reg86u rg;
  memset(&rg, 0, sizeof(rg));	/* zero the structure */

  rg.u.w.ax = VBE_F00; // VBE call, function 02 -- set VBE mode
  rg.u.w.bx = LINEAR_FB | mode; // set bit 14: linear framebuffer
  rg.u.b.intno = INT_INSTRUCTION; //0X10 instruction

  if( sys_int86(&rg) != OK ) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return 1;
  }


  return 0;
}

