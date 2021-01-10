/* Copyright 2020 Matthew Whyte
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#define NOLED_LINES 2
#define NOLED_COLS 16
#define LINE_BUFFER 64

//probably a neater way to do this! need 2 each for mode and value
#define NOLED_PORT_SCK  PORTD
#define NOLED_PIN_SCK      PD2
#define NOLED_DDR_SCK    DDRD
#define NOLED_DPIN_SCK    DDD2

#define NOLED_PORT_MOSI PORTD
#define NOLED_PIN_MOSI     PD3
#define NOLED_DDR_MOSI   DDRD
#define NOLED_DPIN_MOSI   DDD3


#include "quantum.h"

/* This is a shortcut to help you visually see your layout.
 *
 * The first section contains all of the arguments representing the physical
 * layout of the board and position of the keys.
 *
 * The second converts the arguments into a two-dimensional array which
 * represents the switch matrix.
 */

//we need same no of cols in each side, so we have dummy col on lhs
//see config for using unused pin
#define LAYOUT( \
      k00, k01, k02, k03, k04, k05, k06,      k07, k08, k09, k0a, k0b, k0c, k0d, k0e, \
      k10, k11, k12, k13, k14, k15, k16,      k17, k18, k19, k1a, k1b, k1c, k1d, k1e, \
      k20,      k22, k23, k24, k25, k26,      k27, k28, k29, k2a, k2b, k2c, k2d, k2e, \
      k30,      k32, k33, k34, k35, k36,      k37, k38, k39, k3a, k3b, k3c, k3d, k3e, \
      k40, k41, k42, k43, k44, k45, k46,      k47, k48, k49, k4a, k4b, k4c, k4d, k4e, \
      k50, k51, k52,      k54,      k56,      k57     , k59, k5a, k5b, k5c, k5d, k5e  \
) \
{ \
    { k00, k01, k02, k03, k04, k05, k06,KC_NO }, \
    { k10, k11, k12, k13, k14, k15, k16,KC_NO }, \
    { k20,KC_NO,k22, k23, k24, k25, k26,KC_NO }, \
    { k30,KC_NO,k32, k33, k34, k35, k36,KC_NO }, \
    { k40, k41, k42, k43, k44, k45, k46,KC_NO }, \
    { k50, k51, k52,KC_NO,k54,KC_NO,k56,KC_NO }, \
                                            { k07, k08, k09, k0a, k0b, k0c, k0d, k0e }, \
                                            { k17, k18, k19, k1a, k1b, k1c, k1d, k1e }, \
                                            { k27, k28, k29, k2a, k2b, k2c, k2d, k2e }, \
                                            { k37, k38, k39, k3a, k3b, k3c, k3d, k3e }, \
                                            { k47, k48, k49, k4a, k4b, k4c, k4d, k4e }, \
                                            { k57,KC_NO,k59, k5a, k5b, k5c, k5d, k5e }  \
}
