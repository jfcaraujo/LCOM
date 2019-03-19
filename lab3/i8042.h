#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define KEYBOARD_IRQ            1       /**< @brief Keyboard IRQ line */

#define STAT_REG                0x64

#define KBC_CMD_REG             0x64

#define INT_ENABLE              BIT(0)

#define OUT_BUF                 0x60//adress of output buffer

#define BIT(n)                  (0x01<<(n))//selects bit n

#define IBF                     BIT(1)

#define OBF                     BIT(0)

#define PAR_ERR                 BIT(7)

#define TO_ERR                  BIT(6)

#define AUX_ERR                 BIT(5)

#define MSB                     BIT(7)

#define BYTES_MSB               0xe0

#define ESC_BREAKCODE           0x81

#define DELAY_US                20000

#define INTERRUPT_FREQ          60  

#endif /* _LCOM_I8042_H */
