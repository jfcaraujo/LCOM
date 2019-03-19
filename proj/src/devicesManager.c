#include "devicesManager.h"

extern uint32_t scanCode;

void (keyboard_manager)(keyboard_event_t* evt, Game* game)
{
  static bool up, down, left, right;
  kbc_ih(); //To do: Use assembly

  int xDirection = 0, yDirection = 0;

  switch (scanCode){
      case ENTER_BREAK: 
      game->name->letter = ',';
        *evt = ENTER;
        break;
      case ESC_BREAKCODE: 
        *evt = ESC;
        break;
      case A_MAKE:
        left = true;
        break;
      case D_MAKE:
        right = true;
        break;
      case W_MAKE:
        up = true;
        break;
      case S_MAKE:
        down = true;
        break;
      case W_BREAK:
        *evt = UP;
        game->name->letter='w';
        up = false;
        break;
      case S_BREAK:
        *evt = DOWN;
        game->name->letter='s';
        down = false;
        break;
      case A_BREAK:
        left = false;
        game->name->letter='a';
        break;
      case D_BREAK:
        right = false;
        game->name->letter='d';
        break;
      case Q_BREAK:
        game->name->letter='q';
        break;
      case E_BREAK:
        game->name->letter='e';
        break;
      case R_BREAK:
        game->name->letter='r';
        break;
      case T_BREAK:
        game->name->letter='t';
        break;
      case Y_BREAK:
        game->name->letter='y';
        break;
      case U_BREAK:
        game->name->letter='u';
        break;
      case I_BREAK:
        game->name->letter='i';
        break;
      case O_BREAK:
        game->name->letter='o';
        break;
      case P_BREAK:
        game->name->letter='p';
        break;
      case F_BREAK:
        game->name->letter='f';
        break;
      case G_BREAK:
        game->name->letter='g';
        break;
      case H_BREAK:
        game->name->letter='h';
        break;
      case J_BREAK:
        game->name->letter='j';
        break;
      case K_BREAK:
        game->name->letter='k';
        break;
      case L_BREAK:
        game->name->letter='l';
        break;
      case Z_BREAK:
        game->name->letter='z';
        break;
      case X_BREAK:
        game->name->letter='x';
        break;
      case C_BREAK:
        game->name->letter='c';
        break;
      case V_BREAK:
        game->name->letter='v';
        break;
      case B_BREAK:
        game->name->letter='b';
        break;
      case N_BREAK:
        game->name->letter='n';
        break;
      case M_BREAK:
        game->name->letter='m';
        break;
      case SPACE_BREAK:
        game->name->letter=' ';
        break;
      case BACKSPACE_BREAK:
        game->name->letter='-';
        break;
      default:
        *evt = STOP;
        game->name->letter='.';
        break;
  }

  if(up) yDirection--;
  if(down) yDirection++;
  if(left) xDirection--;
  if(right) xDirection++;

  game->playerMov->xDirection = xDirection;
  game->playerMov->yDirection = yDirection;

  return;
}

int (mouse_manager)(Game* game)
{
  static uint8_t bytes[3];
  static uint8_t size = 0; //Stores the bytes of each packet and saves its current size

  mouse_ih(); //Reads output buffer if there was not any error
  store_packet(bytes, &size); //Stores in the array the bytes from the packet
  if(size != 3) {
    return 1;    
  }
  else {
    size = 0;
    game->ship->firing = (bytes[0] & LB);

    if (bytes[0] & X_SIGN)
		  game->mouse->x = game->mouse->x - ((bytes[1] ^= 0xFF) + 1);
	  else
		  game->mouse->x = game->mouse->x +  bytes[1];

	  if (bytes[0] & Y_SIGN)
		  game->mouse->y = game->mouse->y + ((bytes[2] ^= 0xFF) + 1);
	  else  
		  game->mouse->y = game->mouse->y - bytes[2];

  }

  return 0;
}
