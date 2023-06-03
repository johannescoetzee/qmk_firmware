/* Copyright 2019 Thomas Baart <thomas@splitkb.com>
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
#include "xtaps.h"
#include "layernames.h"
#include <stdlib.h>
#define LAYER_GROUP_COUNT 2

// GroupRingBuffer *get_group_buffer(void) {
//     int myryoku_layers[] = {DVK, LAST};
//     LayerGroup myryoku = { "myryoku", myryoku_layers};
//
//     int dvkgaming_layers[] = {GDV, GDN, GDM, LAST};
//     LayerGroup dvkgaming = { "dvkgaming", dvkgaming_layers};
//
//     LayerGroup *layer_groups = calloc((size_t) LAYER_GROUP_COUNT, (size_t) sizeof(LayerGroup));
//     layer_groups[0] = myryoku;
//     GroupRingBuffer *group_buffer = malloc(sizeof(GroupRingBuffer));
//     group_buffer->current = 0;
//     group_buffer->size = LAYER_GROUP_COUNT;
//     group_buffer->groups = layer_groups;
//
//     return group_buffer;
// }

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [DVK] = LAYOUT(
      _______, KC_SCLN, KC_COMM, KC_DOT , KC_P   , KC_Y    ,                          KC_F    , KC_G    , KC_C   , KC_R   , KC_L   , _______,
      _______, MT_LG_A, MT_LA_O, MT_LC_E, MT_LS_U, KC_I    ,                          KC_D    , MT_RS_H , MT_RC_T, MT_RA_N, MT_RG_S, _______,
      _______, KC_QUOT, KC_Q   , KC_J   , KC_K   , KC_X    ,                          KC_B    , KC_M    , KC_W   , KC_V   , KC_Z   , _______,
                                          KC_ESC , LT_NV_SP, LT_SM_TB,      LT_SM_BS, LT_NM_EN, LT_FN_DL
    ),

    [NAV] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                           _______, _______, _______, _______, _______, _______,
      _______, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, _______,                           KC_CAPS, KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, _______,
      _______, _______, _______, _______, _______, _______,                           KC_INS , KC_HOME, KC_PGDN, KC_PGUP, KC_END , _______,
                                          _______, _______, _______,         KC_BSPC, KC_ENT , KC_DEL
    ),

    [NUM] = LAYOUT(
      _______, _______, KC_7, KC_8, KC_9   , _______,                           _______, _______, _______, _______, _______, _______,
      _______, _______, KC_4, KC_5, KC_6   , _______,                           _______, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI, _______,
      _______, _______, KC_1, KC_2, KC_3   , _______,                           _______, _______, _______, _______, _______, _______,
                                    KC_DOT , KC_0   , KC_MINS,         _______, _______, _______
    ),

    [SYM] = LAYOUT(
      _______, KC_QUOT, KC_LT  , KC_GT  , KC_MINS, KC_DQT ,                          KC_AMPR, KC_AT  , KC_LCBR, KC_RCBR, _______, _______,
      _______, KC_EXLM, KC_MINS, KC_PLUS, KC_EQL , KC_HASH,                          KC_PIPE, KC_PERC, KC_LPRN, KC_RPRN, KC_QUES, _______,
      _______, KC_CIRC, KC_BSLS, KC_ASTR, KC_SLSH, _______,                          KC_TILD, KC_DLR , KC_LBRC, KC_RBRC, _______, _______,
                                          _______, KC_UNDS, KC_TILD,        _______, _______, _______
    ),

    [FUN] = LAYOUT(
      _______, KC_F9, KC_F10, KC_F11 , KC_F12 , _______,                          _______, _______, _______, _______, _______, _______,
      _______, KC_F5, KC_F6 , KC_F7  , KC_F8  , _______,                          _______, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI, _______,
      _______, KC_F1, KC_F2 , KC_F3  , KC_F4  , _______,                          _______, _______, _______, _______, _______, _______,
                                       _______, KC_SPC , KC_TAB ,        _______, _______, _______
    ),

//     [_LAYERINDEX] = LAYOUT(
//       _______, _______, _______, _______, _______, _______,                                         _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______,                                         _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______, _______, _______,
//                                  _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______
//     ),
};

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_270; }

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        for (int i = 0; i < 7; i++) {
            if (IS_LAYER_ON_STATE(layer_state|default_layer_state, i)) {
                oled_write_P(PSTR("X "), false);
            } else {
                oled_write_P(PSTR("- "), false);
            }
            const char* layer_name = layer_names[i];
            oled_write_P(PSTR(layer_name), false);
            oled_write_P(PSTR("\n"), false);
        }
    } else {
        // Write host Keyboard LED Status to OLEDs
        led_t led_usb_state = host_keyboard_led_state();
        oled_write_P(led_usb_state.num_lock    ? PSTR("NUMLCK ") : PSTR("       "), false);
        oled_write_P(led_usb_state.caps_lock   ? PSTR("CAPLCK ") : PSTR("       "), false);
        oled_write_P(led_usb_state.scroll_lock ? PSTR("SCRLCK ") : PSTR("       "), false);
    }
    return false;
}
#endif

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {

    if (index == 0) {
        // Volume control
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    } else if (index == 1) {
        // Page up/Page down
        if (clockwise) {
            tap_code(KC_PGDN);
        } else {
            tap_code(KC_PGUP);
        }
    }
    return false;
}
#endif
