#include "graphics.h"

static uint8_t *video_mem;		/* Process (virtual) address to which VRAM is mapped */
static uint8_t *second_buffer; /*Address of the base of the second buffer */

static unsigned h_res;	        /* Horizontal resolution in pixels */
static unsigned v_res;	        /* Vertical resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
int bytesPerPixel; //Calculates the ceiling of the number of bytes per pixel
uint32_t IGNORE_COLOR = 0xFFC0CB;

void* (vg_init)(uint16_t mode)
{
  void* error = NULL;

  if (lm_init(false) == NULL) return error;
  
  vbe_mode_info_t vmi_p;
  if( vbe_get_mode_info	(mode, &vmi_p) ) return error;

  h_res = vmi_p.XResolution; //Saves to a static global variable the horizontal resolution
  v_res = vmi_p.YResolution; //Saves to a static global variable the vertival resolution
  bits_per_pixel = vmi_p.BitsPerPixel; //Saves to a static global variable the number of bits per pixel

  bytesPerPixel = ( (bits_per_pixel + 7) / 8 );

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

  if( sys_int86(&rg) != OK ) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return error;
  }

  return video_mem; //Base address of the VRAM
}

int (vg_draw_hline)(uint16_t x,uint16_t y,uint16_t len, uint32_t color){

  uint8_t colorByte;
  uint32_t colorCopy;

  uint16_t pixelCounter = 0; //Counts the number of pixels changes
  uint8_t *pixel_mem;

  pixel_mem = second_buffer + (x + h_res * y) * bytesPerPixel; //Calculates the address of the first pixel that we want to change

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

  for (unsigned int i = 0; i < height; i++ ) { //Draws a line until it the specified height is achieved
    if (vg_draw_hline(x, y + i, width, color))
      return 1; 
  }
  
  return 0;
}

int (print_sprite)(int16_t x, int16_t y, xpm_image_t xpm)
{
  uint16_t width = xpm.width, height = xpm.height;
  uint8_t* sprite = xpm.bytes;
  uint8_t* pixel_mem;

  uint8_t blue = 0;
  uint8_t green = 0;
  uint8_t red = 0;
  uint32_t color = 0;

  //Copys the sprite to the VRAM
  for(unsigned int line = 0; line < height; line++) {
    pixel_mem = second_buffer + (x + h_res * (y + line)) * bytesPerPixel; //Calculates the VRAM address for the pixel at line and col 
    for(unsigned int col = 0; col < width; col++) {

      blue = *sprite;
      green = *(sprite + 1);
      red = *(sprite + 2);

      color = red<<16 | green<<8 | blue;

      if(color != IGNORE_COLOR) {
        for(int byteNo = 0; byteNo < bytesPerPixel; byteNo++) {
          *pixel_mem = *sprite;
          sprite++;
          pixel_mem++;
        }
      }
      else {
        for(int byteNo = 0; byteNo < bytesPerPixel; byteNo++) {
          sprite++;
          pixel_mem++;
        }
      }
    }
  }
  return 0;
}

size_t (getScreenSize)()
{
  return (size_t) (v_res * h_res * bytesPerPixel);
}

unsigned (getHorRes)() { return h_res; }
unsigned (getVerRes)() { return v_res; }

void (setSecondBuffer)(uint8_t* new_address)
{
  second_buffer = new_address;
}

int (getBytesPerPixel)()
{
  return bytesPerPixel;
}

void (checkCoordinates)(int width, int height, int16_t* x, int16_t* y)
{
  if( (*x) <= 0) {
    (*x) = h_res - (width + 1); 
  }
  else if( ((*x) + width - 1) >= ((int) h_res) ) {
    (*x) = 0;
  }

  if( (*y) <= 0) { 
    (*y)= v_res - (height + 1);
  }
  else if ( ((*y) + height - 1) >= ( (int) v_res) ) {
    (*y) = 0;
  }
}


