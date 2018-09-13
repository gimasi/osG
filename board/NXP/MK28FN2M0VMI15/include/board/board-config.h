// This file is part of osG.
//
// osG is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// osG is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Foobar.  If not, see <https://www.gnu.org/licenses/>.
//
// osG is also available under a commercial license.
// Please contact GIMASI at info@gimasi.ch for further information.
//
#ifndef OSG_BOARD_CONFIG_H
#define OSG_BOARD_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CPU_MK28FN2M0VMI15
#define CPU_MK28FN2M0VMI15
#endif
#ifndef FRDM_K28F
#define FRDM_K28F
#endif
#ifndef FREEDOM
#define FREEDOM
#endif


#include <stdint.h>
#include <fsl_device_registers.h>
#include <MK28F15.h>

#if defined ( __GNUC__ ) && !defined (__CC_ARM) /* GNU Compiler */
  #ifndef __weak
    #define __weak   __attribute__((weak))
  #endif /* __weak */
  #ifndef __packed
    #define __packed __attribute__((__packed__))
  #endif /* __packed */
#endif /* __GNUC__ */

#ifdef __cplusplus
}
#endif

#endif
