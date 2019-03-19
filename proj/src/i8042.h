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

#define AUX_ERR                 BIT(5)

#define MSB                     BIT(7)

#define BYTES_MSB               0xe0

#define DELAY_US                20000

#define INTERRUPT_FREQ          60

#define ESC_BREAKCODE           0x81
#define ENTER_BREAK             0x9C
#define A_MAKE                  0x1E
#define A_BREAK                 0X9E
#define W_MAKE                  0x11
#define W_BREAK                 0X91
#define S_MAKE                  0x1F
#define S_BREAK                 0X9F
#define D_MAKE                  0x20
#define D_BREAK                 0XA0
#define n1_BREAK                0x82
#define n2_BREAK                0x83
#define n3_BREAK                0x84
#define n4_BREAK                0x85
#define n5_BREAK                0x86
#define n6_BREAK                0x87
#define n7_BREAK                0x88
#define n8_BREAK                0x89
#define n9_BREAK                0x8A
#define n0_BREAK                0x8B
#define Q_BREAK                 0x90
#define E_BREAK                 0x92
#define R_BREAK                 0x93
#define T_BREAK                 0x94
#define Y_BREAK                 0x95
#define U_BREAK                 0x96
#define I_BREAK                 0x97
#define O_BREAK                 0x98
#define P_BREAK                 0x99
#define F_BREAK                 0xA1
#define G_BREAK                 0xA2
#define H_BREAK                 0xA3
#define J_BREAK                 0xA4
#define K_BREAK                 0xA5
#define L_BREAK                 0xA6
#define Z_BREAK                 0xAC
#define X_BREAK                 0xAD
#define C_BREAK                 0xAE
#define V_BREAK                 0xAF
#define B_BREAK                 0xB0
#define N_BREAK                 0xB1
#define M_BREAK                 0xB2
#define BACKSPACE_BREAK         0x8E
#define SPACE_BREAK             0xB9

//RTC:
#define RTC_ADDR_REG            0x70
#define RTC_DATA_REG            0x71

#define UIP_FLAG                BIT(7)
#define DM_FLAG                 BIT(2)

#define SECONDS                 0
#define MINUTES                 2
#define HOURS                   4
#define DAY                     7
#define MONTH                   8
#define YEAR                    9

#define REGISTER_A              10
#define REGISTER_B              11
#define REGISTER_C              12
#define REGISTER_D              13


#endif /* _LCOM_I8042_H */
