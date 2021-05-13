/**
 * Mupen64 - cop1_s.c
 * Copyright (C) 2002 Hacktarux
 *
 * Mupen64 homepage: http://mupen64.emulation64.com
 * email address: hacktarux@yahoo.fr
 * 
 * If you want to contribute to the project please contact
 * me first (maybe someone is already making what you are
 * planning to do).
 *
 *
 * This program is free software; you can redistribute it and/
 * or modify it under the terms of the GNU General Public Li-
 * cence as published by the Free Software Foundation; either
 * version 2 of the Licence, or any later version.
 *
 * This program is distributed in the hope that it will be use-
 * ful, but WITHOUT ANY WARRANTY; without even the implied war-
 * ranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public Licence for more details.
 *
 * You should have received a copy of the GNU General Public
 * Licence along with this program; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139,
 * USA.
 *
**/

#include "r4300.h"
#include "ops.h"
#include "macros.h"
#include <math.h>
#include <float.h>

#define CHECK_INPUT(x) \
    do { if (emulate_float_crashes && !isnormal(x) && (x) != 0 && !isinf(x)) { \
        printf("Operation on denormal/nan: %lf; PC = 0x%lx\n", x, PC->addr); \
        stop=1; \
        PC++; \
        return; \
    } } while (0)

#define CHECK_OUTPUT(x) \
    do { if (emulate_float_crashes && !isnormal(x) && !isinf(x)) { \
        if (isnan(x)) { \
            printf("Invalid float operation; PC = 0x%lx\n", PC->addr); \
            stop=1; \
            PC++; \
            return; \
        } else { \
            /* Flush denormals to zero manually, since x87 doesn't have a built-in */ \
            /* way to do it. Typically this doesn't matter, because denormals are */ \
            /* too small to cause visible console/emu divergences, but since we */ \
            /* check for them on entry to each operation this becomes important.. */ \
            x = 0; \
        } \
    } } while (0)

void ADD_S()
{
   if (check_cop1_unusable()) return;
   set_rounding();
   CHECK_INPUT(*reg_cop1_simple[cffs]);
   CHECK_INPUT(*reg_cop1_simple[cfft]);
   *reg_cop1_simple[cffd] = *reg_cop1_simple[cffs] +
     *reg_cop1_simple[cfft];
   CHECK_OUTPUT(*reg_cop1_simple[cffd]);
   PC++;
}

void SUB_S()
{
   if (check_cop1_unusable()) return;
   set_rounding();
   CHECK_INPUT(*reg_cop1_simple[cffs]);
   CHECK_INPUT(*reg_cop1_simple[cfft]);
   *reg_cop1_simple[cffd] = *reg_cop1_simple[cffs] -
     *reg_cop1_simple[cfft];
   CHECK_OUTPUT(*reg_cop1_simple[cffd]);
   PC++;
}

void MUL_S()
{
   if (check_cop1_unusable()) return;
   set_rounding();
   CHECK_INPUT(*reg_cop1_simple[cffs]);
   CHECK_INPUT(*reg_cop1_simple[cfft]);
   *reg_cop1_simple[cffd] = *reg_cop1_simple[cffs] *
     *reg_cop1_simple[cfft];
   CHECK_OUTPUT(*reg_cop1_simple[cffd]);
   PC++;
}

void DIV_S()
{  
   if (check_cop1_unusable()) return;
   set_rounding();
   CHECK_INPUT(*reg_cop1_simple[cffs]);
   CHECK_INPUT(*reg_cop1_simple[cfft]);
   *reg_cop1_simple[cffd] = *reg_cop1_simple[cffs] /
     *reg_cop1_simple[cfft];
   CHECK_OUTPUT(*reg_cop1_simple[cffd]);
   PC++;
}

void SQRT_S()
{
   if (check_cop1_unusable()) return;
   set_rounding();
   CHECK_INPUT(*reg_cop1_simple[cffs]);
   *reg_cop1_simple[cffd] = sqrt(*reg_cop1_simple[cffs]);
   CHECK_OUTPUT(*reg_cop1_simple[cffd]);
   PC++;
}

void ABS_S()
{
   if (check_cop1_unusable()) return;
   CHECK_INPUT(*reg_cop1_simple[cffs]);
   *reg_cop1_simple[cffd] = fabs(*reg_cop1_simple[cffs]);
   // ABS cannot fail
   PC++;
}

void MOV_S()
{
   if (check_cop1_unusable()) return;
   // MOV is not an arithmetic instruction, no check needed
   *reg_cop1_simple[cffd] = *reg_cop1_simple[cffs];
   PC++;
}

void NEG_S()
{
   if (check_cop1_unusable()) return;
   CHECK_INPUT(*reg_cop1_simple[cffs]);
   *reg_cop1_simple[cffd] = -(*reg_cop1_simple[cffs]);
   // NEG cannot fail
   PC++;
}

void ROUND_L_S()
{
   if (check_cop1_unusable()) return;
   set_round();
   *((long long*)(reg_cop1_double[cffd])) = *reg_cop1_simple[cffs];
   PC++;
}

void TRUNC_L_S()
{
   if (check_cop1_unusable()) return;
   set_trunc();
   *((long long*)(reg_cop1_double[cffd])) = *reg_cop1_simple[cffs];
   PC++;
}

void CEIL_L_S()
{
   if (check_cop1_unusable()) return;
   set_ceil();
   *((long long*)(reg_cop1_double[cffd])) = *reg_cop1_simple[cffs];
   PC++;
}

void FLOOR_L_S()
{
   if (check_cop1_unusable()) return;
   set_floor();
   *((long long*)(reg_cop1_double[cffd])) = *reg_cop1_simple[cffs];
   PC++;
}

void ROUND_W_S()
{
   if (check_cop1_unusable()) return;
   set_round();
   *((long*)reg_cop1_simple[cffd]) = *reg_cop1_simple[cffs];
   PC++;
}

void TRUNC_W_S()
{
   if (check_cop1_unusable()) return;
   set_trunc();
   *((long*)reg_cop1_simple[cffd]) = *reg_cop1_simple[cffs];
   PC++;
}

void CEIL_W_S()
{
   if (check_cop1_unusable()) return;
   set_ceil();
   *((long*)reg_cop1_simple[cffd]) = *reg_cop1_simple[cffs];
   PC++;
}

void FLOOR_W_S()
{
   if (check_cop1_unusable()) return;
   set_floor();
   *((long*)reg_cop1_simple[cffd]) = *reg_cop1_simple[cffs];
   PC++;
}

void CVT_D_S()
{
   if (check_cop1_unusable()) return;
   *reg_cop1_double[cffd] = *reg_cop1_simple[cffs];
   PC++;
}

void CVT_W_S()
{
   if (check_cop1_unusable()) return;
   set_rounding();
   *((long*)reg_cop1_simple[cffd]) = *reg_cop1_simple[cffs];
   PC++;
}

void CVT_L_S()
{
   if (check_cop1_unusable()) return;
   set_rounding();
   *((long long*)(reg_cop1_double[cffd])) = *reg_cop1_simple[cffs];
   PC++;
}

void C_F_S()
{
   if (check_cop1_unusable()) return;
   FCR31 &= ~0x800000;
   PC++;
}

void C_UN_S()
{
   if (check_cop1_unusable()) return;
   if (isnan(*reg_cop1_simple[cffs]) || isnan(*reg_cop1_simple[cfft]))
     FCR31 |= 0x800000;
   else FCR31 &= ~0x800000;
   PC++;
}

void C_EQ_S()
{
   if (check_cop1_unusable()) return;
   if (!isnan(*reg_cop1_simple[cffs]) && !isnan(*reg_cop1_simple[cfft]) &&
       *reg_cop1_simple[cffs] == *reg_cop1_simple[cfft])
     FCR31 |= 0x800000;
   else FCR31 &= ~0x800000;
   PC++;
}

void C_UEQ_S()
{
   if (check_cop1_unusable()) return;
   if (isnan(*reg_cop1_simple[cffs]) || isnan(*reg_cop1_simple[cfft]) ||
       *reg_cop1_simple[cffs] == *reg_cop1_simple[cfft])
     FCR31 |= 0x800000;
   else FCR31 &= ~0x800000;
   PC++;
}

void C_OLT_S()
{
   if (check_cop1_unusable()) return;
   if (!isnan(*reg_cop1_simple[cffs]) && !isnan(*reg_cop1_simple[cfft]) &&
       *reg_cop1_simple[cffs] < *reg_cop1_simple[cfft])
     FCR31 |= 0x800000;
   else FCR31 &= ~0x800000;
   PC++;
}

void C_ULT_S()
{
   if (check_cop1_unusable()) return;
   if (isnan(*reg_cop1_simple[cffs]) || isnan(*reg_cop1_simple[cfft]) ||
       *reg_cop1_simple[cffs] < *reg_cop1_simple[cfft])
     FCR31 |= 0x800000;
   else FCR31 &= ~0x800000;
   PC++;
}

void C_OLE_S()
{
   if (check_cop1_unusable()) return;
   if (!isnan(*reg_cop1_simple[cffs]) && !isnan(*reg_cop1_simple[cfft]) &&
       *reg_cop1_simple[cffs] <= *reg_cop1_simple[cfft])
     FCR31 |= 0x800000;
   else FCR31 &= ~0x800000;
   PC++;
}

void C_ULE_S()
{
   if (check_cop1_unusable()) return;
   if (isnan(*reg_cop1_simple[cffs]) || isnan(*reg_cop1_simple[cfft]) ||
       *reg_cop1_simple[cffs] <= *reg_cop1_simple[cfft])
     FCR31 |= 0x800000;
   else FCR31 &= ~0x800000;
   PC++;
}

void C_SF_S()
{
   if (check_cop1_unusable()) return;
   if (isnan(*reg_cop1_simple[cffs]) || isnan(*reg_cop1_simple[cfft]))
     {
	printf("Invalid operation exception in C opcode\n");
	stop=1;
     }
   FCR31 &= ~0x800000;
   PC++;
}

void C_NGLE_S()
{
   if (check_cop1_unusable()) return;
   if (isnan(*reg_cop1_simple[cffs]) || isnan(*reg_cop1_simple[cfft]))
     {
	printf("Invalid operation exception in C opcode\n");
	stop=1;
     }
   FCR31 &= ~0x800000;
   PC++;
}

void C_SEQ_S()
{
   if (check_cop1_unusable()) return;
   if (isnan(*reg_cop1_simple[cffs]) || isnan(*reg_cop1_simple[cfft]))
     {
	printf("Invalid operation exception in C opcode\n");
	stop=1;
     }
   if (*reg_cop1_simple[cffs] == *reg_cop1_simple[cfft])
     FCR31 |= 0x800000;
   else FCR31 &= ~0x800000;
   PC++;
}

void C_NGL_S()
{
   if (check_cop1_unusable()) return;
   if (isnan(*reg_cop1_simple[cffs]) || isnan(*reg_cop1_simple[cfft]))
     {
	printf("Invalid operation exception in C opcode\n");
	stop=1;
     }
   if (*reg_cop1_simple[cffs] == *reg_cop1_simple[cfft])
     FCR31 |= 0x800000;
   else FCR31 &= ~0x800000;
   PC++;
}

void C_LT_S()
{
   if (check_cop1_unusable()) return;
   if (isnan(*reg_cop1_simple[cffs]) || isnan(*reg_cop1_simple[cfft]))
     {
	printf("Invalid operation exception in C opcode\n");
	stop=1;
     }
   if (*reg_cop1_simple[cffs] < *reg_cop1_simple[cfft])
     FCR31 |= 0x800000;
   else FCR31 &= ~0x800000;
   PC++;
}

void C_NGE_S()
{
   if (check_cop1_unusable()) return;
   if (isnan(*reg_cop1_simple[cffs]) || isnan(*reg_cop1_simple[cfft]))
     {
	printf("Invalid operation exception in C opcode\n");
	stop=1;
     }
   if (*reg_cop1_simple[cffs] < *reg_cop1_simple[cfft])
     FCR31 |= 0x800000;
   else FCR31 &= ~0x800000;
   PC++;
}

void C_LE_S()
{
   if (check_cop1_unusable()) return;
   if (isnan(*reg_cop1_simple[cffs]) || isnan(*reg_cop1_simple[cfft]))
     {
	printf("Invalid operation exception in C opcode\n");
	stop=1;
     }
   if (*reg_cop1_simple[cffs] <= *reg_cop1_simple[cfft])
     FCR31 |= 0x800000;
   else FCR31 &= ~0x800000;
   PC++;
}

void C_NGT_S()
{
   if (check_cop1_unusable()) return;
   if (isnan(*reg_cop1_simple[cffs]) || isnan(*reg_cop1_simple[cfft]))
     {
	printf("Invalid operation exception in C opcode\n");
	stop=1;
     }
   if (*reg_cop1_simple[cffs] <= *reg_cop1_simple[cfft])
     FCR31 |= 0x800000;
   else FCR31 &= ~0x800000;
   PC++;
}
