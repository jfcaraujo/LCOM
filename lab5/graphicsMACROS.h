#ifndef _LCOM_GRAPHICS_MACROS_H
#define _LCOM_GRAPHICS_MACROS_H

#define VBE_F02				          0x4F02 //Function 0x02 - Set mode

#define VBE_F00                 0x4F00 

#define SUCCESS                 0x00 //Return from function 0x02 -Set mode. Means the function                                           completed successfully

#define INDEXED_MODE            0x105 //Mode 0x105, indexed mode

#define BLACK                   0x00 // Color black

#define MAX_COLOR_SIZE          255 // Max color size for the indexed mode 0x105

#define BIT(n)                  (0x01<<(n))//selects bit n

#define LINEAR_FB			          BIT(14) //Bit 14 set to set the linear frame buffer model, 

#define INT_INSTRUCTION         0X10; //INT 0x10 software interrupt instruction,

#define BYTE_N(n)                 (0x0F<<(n*8))//selects byte n

#endif
