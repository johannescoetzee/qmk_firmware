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
enum layers {
    DVK = 0,
    SYM,
    NUM,
    NAV,
    FUN
};

char* get_layer_name(int layer) {
    switch(layer) {
        case DVK:
            return "DVK";
        case SYM:
            return "SYM";
        case NUM:
            return "NUM";
        case NAV:
            return "NAV";
        case FUN:
            return "FUN";
        default:
            return "UNKNOWN";
    }
}

#define GUI_A LGUI_T(KC_A)
#define GUI_S RGUI_T(KC_S)
#define ALT_O LALT_T(KC_O)
#define ALT_N RALT_T(KC_N)

const key_override_t bspc_key_override    = ko_make_basic(MOD_MASK_GUI  , KC_BSPC, KC_DEL);
const key_override_t dot_key_override     = ko_make_basic(MOD_MASK_SHIFT, KC_DOT,  KC_UNDS);
const key_override_t comm_key_override    = ko_make_basic(MOD_MASK_SHIFT, KC_COMM, KC_SLSH);
const key_override_t esc_tilde_override   = ko_make_basic(MOD_MASK_SHIFT, KC_ESC,  KC_TILD);
const key_override_t esc_grave_override   = ko_make_basic(MOD_MASK_GUI, KC_ESC,  KC_GRV);

const key_override_t **key_overrides = (const key_override_t *[]) {
    &bspc_key_override,
    &dot_key_override,
    &comm_key_override,
    &esc_tilde_override,
    &esc_grave_override,
};

enum my_keycodes {
  UPDIR = SAFE_RANGE,
};


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case UPDIR:
            if (record->event.pressed) {
                SEND_STRING("../");
            }
            return false;
        default:
            return true;
    }
}


// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [DVK] = LAYOUT(
     KC_ESC , KC_SCLN, KC_COMM, KC_DOT , KC_P    , KC_Y  ,                      KC_F  , KC_G    , KC_C   , KC_R , KC_L , KC_BSPC,
     KC_LSFT, GUI_A  , ALT_O  , KC_E   , KC_U    , KC_I  ,                      KC_D  , KC_H    , KC_T   , ALT_N, GUI_S, KC_RSFT,
     KC_LCTL, KC_QUOT, KC_Q   , KC_J   , KC_K    , KC_X  ,                      KC_B  , KC_M    , KC_W   , KC_V , KC_Z , KC_TAB ,
                                         OSL(NUM), KC_SPC, KC_ENT,    OSL(SYM), KC_SPC, OSL(NAV)
    ),

    [SYM] = LAYOUT(
      _______, KC_QUOT, KC_LT  , KC_GT  , KC_DQUO, KC_DOT ,                      KC_AMPR, KC_AT  , KC_LCBR, KC_RCBR, KC_GRV , _______,
      _______, KC_EXLM, KC_MINS, KC_PLUS, KC_EQL , KC_HASH,                      KC_PIPE, KC_PERC, KC_LPRN, KC_RPRN, KC_QUES, _______,
      _______, KC_CIRC, KC_SLSH, KC_ASTR, KC_BSLS, UPDIR  ,                      KC_TILD, KC_DLR , KC_LBRC, KC_RBRC, TO(NAV), _______,
                                          _______, _______, _______,    _______, _______, _______
    ),

    [NUM] = LAYOUT(
      _______, KC_1   , KC_2   , KC_3   , KC_4   , KC_5   ,                      KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , _______,
      _______, _______, _______, _______, _______, _______,                      _______, KC_4   , KC_5   , KC_6   , _______, _______,
      _______, _______, _______, _______, _______, _______,                      KC_0   , KC_1   , KC_2   , KC_3   , _______, _______,
                                          _______, _______, _______,    _______, _______, _______
    ),

    [NAV] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                      _______, KC_PGDN, KC_PGUP, KC_DEL , KC_INS , _______,
      _______, KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, _______,                      _______, KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, _______,
      _______, _______, _______, _______, _______, _______,                      _______, KC_HOME, KC_END , _______, _______, _______,
                                          _______, _______, _______,    _______, _______, _______
    ),

    [FUN] = LAYOUT(
      _______,  KC_F9 ,  KC_F10,  KC_F11,  KC_F12, _______,                      _______, _______, _______, _______, _______, _______,
      _______,  KC_F5 ,  KC_F6 ,  KC_F7 ,  KC_F8 , _______,                      _______, KC_RSFT, KC_RCTL, KC_LALT, KC_RGUI, _______,
      _______,  KC_F1 ,  KC_F2 ,  KC_F3 ,  KC_F4 , _______,                      _______, _______, _______, _______, _______, _______,
                                          _______, _______, _______,    _______, _______, _______
    ),

// /*
//  * Layer template
//  */
//     [_LAYERINDEX] = LAYOUT(
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
//                                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
//     ),
};

/* The default OLED and rotary encoder code can be found at the bottom of qmk_firmware/keyboards/splitkb/kyria/rev1/rev1.c
 * These default settings can be overriden by your own settings in your keymap.c
 * For your convenience, here's a copy of those settings so that you can uncomment them if you wish to apply your own modifications.
 * DO NOT edit the rev1.c file; instead override the weakly defined default functions by your own.
 */

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_270; }

bool oled_task_user(void) {
    // QMK Logo and version information

    if (is_keyboard_master()) {
        // clang-format off
        // switch (get_highest_layer(layer_state|default_layer_state)) {
        //     case _OLD_DVORAK:
        //         break;
        //     case _OLD_SYM:
        //         oled_write_P(PSTR("Sym\n"), false);
        //         break;
        //     case FUN:
        //         oled_write_P(PSTR("Function\n"), false);
        //         break;
        //     default:
        //         oled_write_P(PSTR("Undefined\n"), false);
        // }

    } else {
        // oled_write_P(PSTR("Base layer: "), false);
        // oled_write_P(PSTR(base_layer_name), false);
        // oled_write_P(PSTR("\n"), false);

        //int found_base = 0;
        //for (int layer = 0; layer < 32; layer++) {
        //    if (IS_LAYER_ON_STATE(layer_state | default_layer_state, layer)) {
        //        char* layer_name = get_layer_name(layer);
        //        if (found_base) {
        //            oled_write_P(PSTR("- "), false);
        //        } else {
        //            found_base = 1;
        //        }
        //        oled_write_P(PSTR(layer_name), false);
        //        oled_write_P(PSTR("\n"), false);
        //    }
        //}
    }

    if (is_keyboard_master()) {
        for (int i = 0; i <= FUN; i++) {
            if (IS_LAYER_ON_STATE(layer_state|default_layer_state, i)) {
                oled_write_P(PSTR("O "), false);
            } else {
                oled_write_P(PSTR("- "), false);
            }
            char* layer_name = get_layer_name(i);
            oled_write_P(PSTR(layer_name), false);
            oled_write_P(PSTR("\n"), false);
        }
    } else {
        // static const char PROGMEM qmk_logo[] = {
        //     0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
        //     0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
        //     0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0};
        // // clang-format on

        // oled_write_P(qmk_logo, false);

    }
    return false;
}
#endif
