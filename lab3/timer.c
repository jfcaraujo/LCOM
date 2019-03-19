#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <minix/syslib.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  
  uint8_t config, controlWord;
  uint8_t lsb, msb;
  if (timer<0||timer>2) return 1;
  if (freq<MIN_FREQ || freq>TIMER_FREQ) return 1;//because its the minimum and maximum frequency

  if(timer_get_conf(timer, &config)) return 1;

  config &= 0x0f; //because we only want the last 4 bits
  controlWord = config | TIMER_LSB_MSB;

  switch (timer) {//selects the timer
    case 0 : 
      controlWord |= TIMER_SEL0;
      break;
    case 1 : 
      controlWord |= TIMER_SEL1;
      break;
    case 2 : 
      controlWord |= TIMER_SEL2;
      break;
    default: return 1;
  }
  
  if(sys_outb(TIMER_CTRL, controlWord)) return 1;

  //because the interrupt frequency is equal to the clock frequency divided by the value loaded initially by the timer
  uint16_t initialValue = (uint16_t) (TIMER_FREQ / freq);
  
  if(util_get_LSB(initialValue, &lsb)) return 1;
  if(util_get_MSB(initialValue, &msb)) return 1;
  
  if(sys_outb(TIMER_ADDR(timer), lsb)) return 1;
  if(sys_outb(TIMER_ADDR(timer), msb)) return 1;

  return 0;
}

int hook_id = 0;

int (timer_subscribe_int)(uint8_t *bit_no) {

  *bit_no = (uint8_t) hook_id; //bits_no = 0

  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id)) return 1;

  return 0;
}

int (timer_unsubscribe_int)() {

  if(sys_irqrmpolicy(&hook_id)) return 1;

  return 0;
}

uint32_t interruptsCounter = 0; 
//increments a global counter variable.
void (timer_int_handler)() {
  interruptsCounter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (timer<0||timer>2) return 1;
  uint32_t config;
  uint8_t readBackCommand = TIMER_RB_CMD|TIMER_RB_COUNT_|TIMER_RB_SEL(timer);//makes the program read only status (not count)

  if(sys_outb(TIMER_CTRL, readBackCommand)) return 1;
  if(sys_inb(TIMER_ADDR(timer), &config)) return 1; //added TIMER_ADDR(n) to i8254.h file

  *st = (uint8_t) (config);

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  if (timer<0||timer>2) return 1;
  union timer_status_field_val v;
  switch(field) {
    case all : {
      v.byte = st;
      break;
    }
    case initial : {
      st &= 0x30;//selects bit 5 and 4
      st >>= 4; //moves them to the final bits
      v.in_mode = st;//selects option number st from union in_mode
      break;
    } 
    case mode : {
      st &= 0x0E;//selects bit 3, 2, 1
      st >>= 1;//moves them to the final bits

      //these 2 are for the special cases
      if(st == 6) v.count_mode = 2; //6=110
      else if(st == 7) v.count_mode = 3;//7=111
      else 
        v.count_mode = st;
      break;
    }
    case base : {
      if(st & TIMER_BCD) v.bcd = true;//if the last bit is 1
      else  v.bcd = false;//if the last bit is 0
      break;
    }
    default: 
      return 1;
  }

  if(timer_print_config(timer, field, v)) return 1;

  return 0;
}
