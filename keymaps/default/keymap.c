/* Copyright 2020 https://mwworks.uk mw@mwworks.uk
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
#include QMK_KEYBOARD_H
#include "noled.c"
#include "display.c"
#include "raw_hid.h"


//hid currently work in progress
void raw_hid_receive(uint8_t *data, uint8_t length) {
    for(uint8_t i=0; i<length; i++){
        noledWriteChar(0, i, (char)data[i]);
    }
}

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
    _FN
};

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
    QMKBEST = SAFE_RANGE,
    DISP_L0,
    DISP_L1
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_BASE] = LAYOUT(
        KC_ESC ,KC_F1  ,KC_F2  ,KC_F3  ,KC_F4  ,KC_F5  ,KC_F6  ,KC_F7  ,KC_F8  ,KC_F9  ,KC_F10 ,KC_F11 ,KC_F12 ,KC_MPLY,KC_MUTE,
        KC_GRV ,KC_1   ,KC_2   ,KC_3   ,KC_4   ,KC_5   ,KC_6   ,KC_7   ,KC_8   ,KC_9   ,KC_0   ,KC_MINS,KC_EQL ,KC_BSPC,KC_DEL ,
        KC_TAB         ,KC_Q   ,KC_W   ,KC_E   ,KC_R   ,KC_T   ,KC_Y   ,KC_U   ,KC_I   ,KC_O   ,KC_P   ,KC_LBRC,KC_RBRC,KC_ENT ,
        KC_CAPS        ,KC_A   ,KC_S   ,KC_D   ,KC_F   ,KC_G   ,KC_H   ,KC_J   ,KC_K   ,KC_L   ,KC_SCLN,KC_QUOT,KC_NUHS,KC_PGUP,
        KC_LSFT,KC_NUBS,KC_Z   ,KC_X   ,KC_C   ,KC_V   ,KC_B   ,KC_N   ,KC_M   ,KC_COMM,KC_DOT ,KC_SLSH,KC_LSFT,KC_UP  ,KC_PGDN,
        KC_LCTL,KC_LWIN,KC_LALT        ,KC_SPC         ,MO(_FN),LT(_FN,KC_SPC) ,KC_RALT,KC_RWIN,KC_RCTL,KC_LEFT,KC_DOWN,KC_RGHT
    ),
    [_FN] = LAYOUT(
        _______,DISP_L0,DISP_L1,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,
        _______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,KC_INS ,
        _______        ,_______,_______,_______,RESET  ,_______,_______,RESET  ,_______,_______,KC_PSCR,_______,_______,KC_PENT,
        _______        ,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,KC_HOME,
        _______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,_______,KC_END ,
        _______,_______,_______        ,_______        ,_______,_______        ,KC_APP ,_______,_______,_______,_______,_______
    )
};

//mw discovered that RHS (slave in this case) doesn't really run any code or do any processing
//so it only needs to be flashed once with the hardware setup
//hence anything fancy i'm doing here only needs to be flashed to LHS
//lucky because i can't then figure out how to flash the slave without pressing reset
//flash the LHS with qmk flash -bl avrdude-split-left -kb mw_split_75 -km default

void keyboard_pre_init_user(void) {
    noledBegin();
    noledClear();
    displaySetLine(0, "Fn-F1 to edit");
    displaySetLine(1, "Fn-F2 to edit");

}

//encoders, remember we have 2 fake ones 0+1 on LHS so 2+3 are the real ones 
//ensure we are handling the right ones - the fake ones may be floating and could trigger?
void encoder_update_user(uint8_t index, bool clockwise) {
    if((index == 2) || (index == 3)){
        //function encoders scroll the display
        if(IS_LAYER_ON(_FN)){
            uint8_t display_line = index-2;
            displayScrollLine(display_line, !clockwise);

        //otherwise they are media keys for vol and transport
        }else{
            if (index == 2) { /* First encoder */
                if (clockwise) {
                    tap_code(KC_MNXT);
                } else {
                    tap_code(KC_MPRV);
                }
            } else if (index == 3) { /* Second encoder */
                if (clockwise) {
                    tap_code(KC_VOLU);
                } else {
                    tap_code(KC_VOLD);
                }
            }
        }
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    bool result = true;

    //we are only interested in doing anything with keydown events
    if (record->event.pressed) {

        if (keycode == DISP_L0) {
            displaySetLineEdit(0);
        }else if(keycode == DISP_L1){
            displaySetLineEdit(1);
        //are we in edit mode? return false if the key was captured
        }else if(display_line_edit<NOLED_LINES){
            if(keycode == KC_ENT){
                displaySetLineEdit(display_line_edit);
                result = false;
            }else{
                result = !displayCaptureChar(keycode);
            }
        }
    }

    return result;
}

/*
void matrix_init_user(void) {

}

void matrix_scan_user(void) {

}

bool led_update_user(led_t led_state) {
    return true;
}
*/
