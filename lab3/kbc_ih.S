#include <machine/asm.h>
//#include <stdint.h>
#include "i8042.h"


.intel_syntax 

.global _C_LABEL(kbc_asm_ih)

.text
_C_LABEL(kbc_asm_ih):
        in      al, STAT_REG
        mov     dl, al
        or      dl, OBF
        xor     ecx, ecx
        cmp     dl, cl
        je      exit
        mov     dl, (PAR_ERR | TO_ERR)
        and     dl, al
        cmp     dl, cl
        jne     exit
        in      al, OUT_BUF
        mov     _C_LABEL(scanCode), al
exit:   ret

