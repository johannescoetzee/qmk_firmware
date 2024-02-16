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
#include <stdint.h>
#include <stdio.h>
#include "action_layer.h"
#include QMK_KEYBOARD_H
#include "xtaps.h"
#include "layernames.h"
#include <stdlib.h>

enum my_keycodes {
    GRP_NXT = SAFE_RANGE,
    GRP_PRV,
    GRP_RST
};

const key_override_t delete_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_BSPC, KC_DEL);

// This globally defines all key overrides to be used
const key_override_t **key_overrides = (const key_override_t *[]){
    &delete_key_override,
    NULL // Null terminate the array of overrides!
};

const char* layer_names[] = {
    [DVK] = "Dvorak Base",
    [GDV] = "Gaming Dvorak",
    [GDN] = "Gaming Numbers",
    [GDM] = "Gaming Mirrored",
    [NAV] = "Navigation",
    [NUM] = "Number",
    [LSM] = "Left Symbols",
    [RSM] = "Right Symbols",
    [FUN] = "Function",
    [CTL] = "Control",
    [RGB] = "RTS Base",
    [SC2] = "SC2",
    [LAST] = "LAST"
};

GroupRingBuffer groups = {
    /* current */ 0,
    /* size */ GROUP_COUNT,
    /* groups */ {
        {
            "Base",
            { DVK, NAV, NUM, LSM, RSM, FUN, CTL }
        },
        {
            "General Gaming",
            { GDV, GDM, GDN, CTL, -1, -1, -1 }
        },
        {
            "RTS Grid",
            { RGB, GDN, CTL, -1, -1, -1, -1 }
        },
        {
            "SC2",
            { SC2, -1, -1, -1, -1, -1, -1 }
        }
    }
};

char* get_current_group_name(void) {
    return groups.groups[groups.current].name;
}

int get_current_group_base(void) {
    return groups.groups[groups.current].layers[0];
}

int get_group_layer(int index) {
    return groups.groups[groups.current].layers[index];
}

int to_layer_mask(int layer) {
    return 1 << layer;
}

void set_base_to_group(void) {
    int new_base = get_current_group_base();
    layer_move(new_base);
    layer_on(CTL);
    #ifdef OLED_ENABLE
    oled_clear();
    #endif
}

void update_group(uint16_t keycode) {
    switch (keycode) {
        case GRP_NXT:
            groups.current = (groups.current + 1) % GROUP_COUNT;
            break;
        case GRP_PRV:
            {
                int prv_grp = groups.current - 1;
                groups.current = prv_grp >= 0 ? prv_grp : GROUP_COUNT - 1;
            }
            break;
        default:
            break;
    }

    set_base_to_group();
}

void reset_group(void) {
    groups.current = 0;
    set_base_to_group();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case GRP_NXT:
        case GRP_PRV:
            if (record->event.pressed) {
                update_group(keycode);
            }
            return false;
        case GRP_RST:
            if (record->event.pressed) {
                reset_group();
            }
        default:
            return true;

    }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [DVK] = LAYOUT(
      _______, KC_SCLN, KC_COMM, KC_DOT , KC_P   , KC_Y    ,                                            KC_F    , KC_G    , KC_C   , KC_R   , KC_L   , _______,
      _______, MT_LG_A, MT_LA_O, MT_LC_E, MT_LS_U, KC_I    ,                                            KC_D    , MT_RS_H , MT_RC_T, MT_RA_N, MT_RG_S, _______,
      _______, KC_QUOT, KC_Q   , KC_J   , KC_K   , KC_X    , _______ , _______,      MO(CTL), _______, KC_B    , KC_M    , KC_W   , KC_V   , KC_Z   , _______,
                                 _______, KC_ESC , LT_NV_SP, LT_RS_TB, _______,      _______, LT_LS_BS, LT_NM_EN, LT_FN_DL, _______
    ),

    [NAV] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                         KC_CAPS, _______, _______, _______, _______, _______,
      _______, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, _______,                                         CW_TOGG, KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, _______,
      _______, _______, _______, _______, _______, _______, _______, _______,     MO(CTL), _______, KC_INS , KC_HOME, KC_PGDN, KC_PGUP, KC_END , _______,
                                 _______, _______, _______, _______, _______,     _______, KC_BSPC, KC_ENT , KC_DEL , _______
    ),

    [NUM] = LAYOUT(
      _______, _______, KC_7, KC_8, KC_9, _______,                                                  _______, _______, _______, _______, _______, _______,
     _______, KC_0   , KC_4, KC_5, KC_6, _______,                                                  _______, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI, _______,
      _______, _______, KC_1, KC_2, KC_3, _______, _______, _______, _______,     MO(CTL), _______, _______, _______, _______, _______, _______,
                                 _______, _______, KC_SPC , KC_TAB , _______,     _______, _______, _______, _______, _______
    ),

    [LSM] = LAYOUT(
      _______, KC_QUOT, KC_LT  , KC_GT  , KC_MINS, _______,                                         _______, _______, _______, _______, _______, _______,
      _______, KC_EXLM, KC_MINS, KC_PLUS, KC_EQL , KC_HASH,                                         _______, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI, _______,
      _______, KC_CIRC, KC_BSLS, KC_ASTR, KC_SLSH, _______, _______, _______,     MO(CTL), _______, _______, _______, _______, _______, _______, _______,
                                 _______, _______, KC_UNDS, KC_TILD, _______,     _______, _______, _______, _______, _______
    ),

    [RSM] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                         KC_AMPR, KC_AT  , KC_LCBR, KC_RCBR, KC_GRV , _______,
      _______, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, _______,                                         KC_PIPE, KC_PERC, KC_LPRN, KC_RPRN, KC_QUES, _______,
      _______, _______, _______, _______, _______, _______, _______, _______,     MO(CTL), _______, KC_TILD, KC_DLR , KC_LBRC, KC_RBRC, _______, _______,
                                 _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______
    ),

    [FUN] = LAYOUT(
      _______, KC_F9, KC_F10, KC_F11 , KC_F12 , _______,                                         _______, _______, _______, _______, _______, _______,
      _______, KC_F5, KC_F6 , KC_F7  , KC_F8  , _______,                                         _______, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI, _______,
      _______, KC_F1, KC_F2 , KC_F3  , KC_F4  , _______, _______, _______,     MO(CTL), _______, _______, _______, _______, _______, _______, _______,
                              _______, _______, KC_SPC , KC_TAB , _______,     _______, _______, _______, _______, _______
    ),

    [GDV] = LAYOUT(
      KC_ESC , KC_SCLN, KC_COMM, KC_DOT , KC_P   , KC_Y  ,                                          KC_F   , KC_G   , KC_C   , KC_R   , KC_L   , KC_GRV ,
      KC_LSFT, KC_A   , KC_O   , KC_E   , KC_U   , KC_I  ,                                          KC_D   , KC_H   , KC_T   , KC_N   , KC_S   , KC_RSFT,
      KC_LCTL, KC_QUOT, KC_Q   , KC_J   , KC_K   , KC_X  , _______, _______,      MO(CTL), _______, KC_B   , KC_M   , KC_W   , KC_V   , KC_Z   , KC_RCTL,
                                 _______, MO(GDM), KC_SPC, MO(GDN), KC_TAB ,      KC_LALT, KC_BSPC, KC_ENT , KC_DEL , _______
    ),

    [RGB] = LAYOUT(
      KC_ESC , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   ,                                         KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , _______,
      KC_LSFT, KC_A   , KC_S   , KC_D   , KC_F   , KC_G   ,                                         KC_H   , KC_J   , KC_K   , KC_L   , KC_SCLN, _______,
      KC_LCTL, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   , A_GDN  , _______,     MO(CTL), _______, KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, _______,
                                 _______, S_GDN  , MO(GDN), C_GDN  , CS_GDN ,     _______, _______, _______, _______, _______
    ),

    [SC2] = LAYOUT(
      KC_ESC , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   ,                                         KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , _______,
      KC_LSFT, KC_A   , KC_S   , KC_D   , KC_F   , KC_G   ,                                         KC_H   , KC_J   , KC_K   , KC_L   , KC_SCLN, _______,
      KC_LCTL, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   , KC_LSFT, KC_LSFT,     MO(CTL), _______, KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, _______,
                                 _______, MO(GDN), KC_SPC , KC_LCTL, KC_LALT,     _______, _______, _______, _______, _______
    ),

    [GDM] = LAYOUT(
      KC_ESC , KC_L   , KC_R   , KC_C   , KC_G   , KC_F   ,                                          KC_Y   , KC_P   , KC_DOT , KC_COMM, KC_SCLN, KC_GRV ,
      KC_LSFT, KC_S   , KC_N   , KC_T   , KC_H   , KC_D   ,                                          KC_I   , KC_U   , KC_E   , KC_O   , KC_A   , KC_RSFT,
      KC_LCTL, KC_Z   , KC_V   , KC_W   , KC_M   , KC_B   , _______, _______,      MO(CTL), _______, KC_X   , KC_K   , KC_J   , KC_Q   , KC_QUOT, KC_RCTL,
                                 _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______
    ),

    [GDN] = LAYOUT(
      KC_ESC , KC_1   , KC_2   , KC_3   , KC_4   , KC_9   ,                                          _______, KC_F9  , KC_F10 , KC_F11 , KC_F12 , KC_GRV ,
      KC_LSFT, KC_5   , KC_6   , KC_7   , KC_8   , KC_0   ,                                          _______, KC_F5  , KC_F6  , KC_F7  , KC_F8  , KC_RSFT,
      KC_LCTL, KC_F19 , KC_F20 , KC_F21 , KC_F22 , KC_F23 , KC_F24 , _______,      MO(CTL), _______, _______, KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_RCTL,
                                 _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______
    ),

    [CTL] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                         _______, _______, _______, _______, _______, _______,
      _______, GRP_PRV, _______, _______, GRP_NXT, GRP_RST,                                         GRP_RST, GRP_PRV, _______, _______, GRP_NXT, _______,
      _______, _______, _______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______, _______, _______,
                                 _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______
    ),

//     [_LAYERINDEX] = LAYOUT(
//       _______, _______, _______, _______, _______, _______,                                         _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______,                                         _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______, _______, _______,
//                                  _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______
//     ),
};

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_180; }

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        char out_buffer[50] = { 0 };
        sprintf(out_buffer, "## %s (%d) ##\n", get_current_group_name(), get_current_group_base());
        oled_write_P(PSTR(out_buffer), false);

        int layer;
        for (int i = 0; i < GROUP_SIZE && (layer = get_group_layer(i)) != -1; i++) {
            char *status = IS_LAYER_ON_STATE(layer_state|default_layer_state, layer) ? "ON" : "--";
            sprintf(out_buffer, "%s %s\n", status, layer_names[get_group_layer(i)]);
            oled_write_P(PSTR(out_buffer), false);
        }
    } else {
        oled_write_P(PSTR("\n"), false);
        static const char PROGMEM qmk_logo[] = {
            0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
            0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
            0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0};
        // clang-format on

        oled_write_P(qmk_logo, false);
        oled_write_P(PSTR("Kyria Rev3.1\n\n"), false);

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
