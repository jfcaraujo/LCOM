#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define MOUSE_IRQ               12

#define RB                      BIT(1)

#define LB                      BIT(0)

#define MB                      BIT(2)

#define Y_SIGN                  BIT(5)

#define X_SIGN                  BIT(4)

#define Y_OV                    BIT(7)

#define X_OV                    BIT(6)

#define KEYBOARD_IRQ            1       /**< @brief Keyboard IRQ line */

#define STAT_REG                0x64

#define KBC_CMD_REG             0x64

#define WRT_BYTE_MS             0xD4

#define WRT_CMD_BYTE            0x60

#define INT_ENABLE              BIT(0)

#define IN_BUF                  0x60

#define OUT_BUF                 0x60

#define EN_DATA_RP              0xF4

#define DIS_DATA_RP             0xF5

#define ACK                     0xFA

#define ERROR                   0xFC

#define SET_REM_MODE            0xF0

#define SET_STREAM_MODE         0xEA

#define READ_DATA               0xEB

#define BIT(n)                  (0x01<<(n))

#define IBF                     BIT(1)

#define OBF                     BIT(0)

#define PAR_ERR                 BIT(7)

#define TO_ERR                  BIT(6)

#define AUX                     BIT(5)

#define MSB                     BIT(7)

#define BYTES_MSB               0xe0

#define ESC_BREAKCODE           0x81

#define DELAY_US                20000

#define INTERRUPT_FREQ          60  

#endif /* _LCOM_I8042_H */
