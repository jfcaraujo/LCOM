#include "graphics.h"

static char *video_mem;		/* Process (virtual) address to which VRAM is mapped */

static unsigned h_res;	        /* Horizontal resolution in pixels */
static unsigned v_res;	        /* Vertical resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static uint8_t RedScreeMask, GreenScreeMask,BlueScreeMask; //Size of each color component 
uint16_t g_mode;

void* (vg_init)(uint16_t mode)
{
  void* error = NULL;
  g_mode=mode;

  if (lm_init(false) == NULL) return error;
  
  vbe_mode_info_t vmi_p;
  if( vbe_get_mode_info	(mode, &vmi_p) ) return error;

  h_res = vmi_p.XResolution; //Saves to a static global variable the horizontal resolution
  v_res = vmi_p.YResolution; //Saves to a static global variable the vertival resolution
  bits_per_pixel = vmi_p.BitsPerPixel; //Saves to a static global variable the number of bits per pixel
  RedScreeMask = vmi_p.RedMaskSize; //Saves to a static global variable the size of the red color component
  GreenScreeMask = vmi_p.GreenMaskSize; //Saves to a static global variable the size of the green color component
  BlueScreeMask =vmi_p.BlueMaskSize; //Saves to a static global variable the size of the blue color component

  int r;
  struct minix_mem_range mr; /* physical memory range */
  unsigned int vram_base = vmi_p.PhysBasePtr; /* VRAM’s physical addresss */
  unsigned int vram_size = h_res * v_res * bits_per_pixel; /* VRAM’s size, but you can use the  frame-buffer size, instead */
                                            
  /* Allow memory mapping */

  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;
  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  if(video_mem == MAP_FAILED)
    panic("couldn’t map video memory");


  //Initializes the specified mode by using fucntion 0x02 - Set VBE Mode
  struct reg86u rg;
  memset(&rg, 0, sizeof(rg));	/* zero the structure */

  rg.u.w.ax = VBE_F02; // VBE call, function 02 -- set VBE mode
  rg.u.w.bx = LINEAR_FB | mode; // set bit 14: linear framebuffer
  rg.u.b.intno = INT_INSTRUCTION; //0X10 instruction

  if(rg.u.b.ah != SUCCESS) 
    return error; 

  if( sys_int86(&rg) != OK ) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return error;
  }

  return video_mem; //Base address of the VRAM
}

int (vg_draw_hline)(uint16_t x,uint16_t y,uint16_t len, uint32_t color){

  int bytesPerPixel = ( (bits_per_pixel + 7) / 8 ); //Calculates the ceiling of the number of bytes per pixel

  uint8_t colorByte;
  uint32_t colorCopy;

  uint16_t pixelCounter = 0; //Counts the number of pixels changes
  char *pixel_mem;

  pixel_mem = video_mem + (x + h_res * y) * bytesPerPixel; //Calculates the address of the first pixel that we want to change

  if(bytesPerPixel == 1) 
    if(color > MAX_COLOR_SIZE) return 1; //Max color size for mode 0x105

  while (pixelCounter < len)  
  {
    colorCopy = color; //Saves the original color specified in the argument

    for(int i = 0; i < bytesPerPixel; i++) { //For each pixel, changes the number of bytes necessary
      colorByte = (uint8_t) colorCopy; //Gets the last byte of the color
      *pixel_mem = colorByte; //Copys to VRAM the color byte
      colorCopy >>= 8; //Shift right of 1 byte to get the next byte
      pixel_mem++; //Goes to the next pixel
    }
      pixelCounter++; //After the for loop, the pixel is set in VRAM with the color specified
  }
 
  return 0;
} 	

int (vg_draw_rectangle)(uint16_t x, uint16_t y,uint16_t width, uint16_t height, uint32_t color){
  if (v_res-y < height || v_res <= y)
    return 1;

  for (unsigned int i = 0; i < height; i++ ) { //Draws a line until it the specified height is achiebed
    if (vg_draw_hline(x, y + i, width, color))
      return 1; 
  }
  
  return 0;
}

int (print_sprite)(uint16_t x, uint16_t y, char* sprite, int width, int height)
{
  char* pixel_mem;
  char* spriteCopy;
  spriteCopy = sprite; //Using a copy so that the original variable sprite does not change its value

  //Copys the sprite to the VRAM
  for(int line = 0; line < height; line++) { 
    pixel_mem = video_mem + (x + h_res * (y + line)); //Calculates the VRAM address for the pixel at line and col
    for(int col = 0; col < width; col++) {
      
      *pixel_mem = *spriteCopy; //Copys to VRAM the pixel of the sprite
      pixel_mem++; //Moves to the next pixel in VRAM
      spriteCopy++; //Moves to the nest sprite pixel
    }
  }
  return 0;
}

int (move_sprite)(char* sprite, int width, int height, uint16_t* x, uint16_t* y, int16_t speed, int16_t* framesCounter, int8_t xDirection, int8_t yDirection)
{
  if(vg_draw_rectangle(*x, *y, width, height, 0)) return 1; //Clears previous sprite

  if(speed > 0) { //Moves sprite the speed number of pixels
    *x += speed * xDirection; 
    *y += speed * yDirection;
  }
  else  {
    if(*framesCounter == abs(speed)) { //If number of frames passed is equal to the number required to move a pixel 
      *framesCounter = 0;
      *x += xDirection;
      *y += yDirection;
    }
  }

  if(print_sprite(*x, *y, sprite, width, height)) return 1; //Displays the sprite in the new coordinates
  return 0;
}

int32_t (select_color)(uint16_t row,uint16_t column, uint32_t first, uint8_t step, uint8_t no_rectangles){
  if (g_mode == 0x105){
    uint32_t x;
    x=(uint32_t)(first + (row * no_rectangles + column) * step) % (1 << bits_per_pixel);
    return x;
  }else if (g_mode == 0x115){
    uint8_t red, green, blue;
    red = (uint8_t)((first>>(BlueScreeMask+GreenScreeMask)) & BYTE_N(0));
    green = (uint8_t)((first>>GreenScreeMask) & (BYTE_N(0)>>(8-BlueScreeMask)));
    blue = (uint8_t)(first & (BYTE_N(0) >> (8-GreenScreeMask)));
    return (uint32_t)((((red + column * step) % (1 << RedScreeMask))<< (GreenScreeMask+BlueScreeMask))+ (((green + row * step) % (1 << GreenScreeMask))<< BlueScreeMask)+(blue + (column + row) * step) % (1 << BlueScreeMask) );}
  else return -1;
} 
unsigned (getHeight)(){return v_res;}
unsigned (getWidth)(){return h_res;}
