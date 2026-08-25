/* Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include "action_tapping.h"
#include "swapper.c"

// ====================
// LAYERS
// ====================
enum layers {
    _QWERTY,
    _SYM_NUM,
    _EXTEND,
    _FN,
    _GAME,
    _GAME_FN,
};


#ifdef OLED_ENABLE

static uint32_t scroll_timer = 0;

// =====================================================
// OLED SETTINGS
// =====================================================

#define SCROLL_SPEED 120
#define OLED_COLS    21

// =====================================================
// LEFT OLED TEXT
// =====================================================

#define LEFT_TEXT     "PING PONG!"
#define LEFT_TEXT_LEN 10

// =====================================================
// RIGHT OLED TEXT
// =====================================================

#define RIGHT_TEXT     "HELLO WORLD!"
#define RIGHT_TEXT_LEN 12

// =====================================================
// PING-PONG POSITIONS
// =====================================================

static int8_t left_x = 0;
static int8_t left_dir = 1;

static int8_t right_x = 0;
static int8_t right_dir = 1;


bool oled_task_user(void) {

    // =====================================================
    // UPDATE TEXT ANIMATION
    // =====================================================

    if (timer_elapsed32(scroll_timer) > SCROLL_SPEED) {
        scroll_timer = timer_read32();

        // -------------------------------------------------
        // LEFT: PING PONG!
        // -------------------------------------------------

        left_x += left_dir;

        if (left_x <= 0) {
            left_x = 0;
            left_dir = 1;
        }

        if (left_x >= OLED_COLS - LEFT_TEXT_LEN) {
            left_x = OLED_COLS - LEFT_TEXT_LEN;
            left_dir = -1;
        }


        // -------------------------------------------------
        // RIGHT: HELLO WORLD!
        // -------------------------------------------------

        right_x += right_dir;

        if (right_x <= 0) {
            right_x = 0;
            right_dir = 1;
        }

        if (right_x >= OLED_COLS - RIGHT_TEXT_LEN) {
            right_x = OLED_COLS - RIGHT_TEXT_LEN;
            right_dir = -1;
        }
    }


    oled_clear();


    if (is_keyboard_master()) {

        // -----------------------------
        // PING PONG!
        // -----------------------------

        oled_set_cursor(0, 1);

        for (uint8_t i = 0; i < OLED_COLS; i++) {

            if (i >= left_x &&
                i < left_x + LEFT_TEXT_LEN) {

                oled_write_char(
                    LEFT_TEXT[i - left_x],
                    false
                );

            } else {

                oled_write_char(' ', false);
            }
        }


        // -----------------------------
        // LAYER INFORMATION
        // -----------------------------

        oled_set_cursor(0, 3);

        switch (get_highest_layer(layer_state)) {

            case _QWERTY:
                oled_write_P(PSTR("QWERTY"), false);
                break;

            case _SYM_NUM:
                oled_write_P(PSTR("SYM/NUM"), false);
                break;

            case _EXTEND:
                oled_write_P(PSTR("EXTEND"), false);
                break;

            case _FN:
                oled_write_P(PSTR("FN"), false);
                break;

            case _GAME:
                oled_write_P(PSTR("GAME"), false);
                break;

            case _GAME_FN:
                oled_write_P(PSTR("GAME FN"), false);
                break;

            default:
                oled_write_P(PSTR("UNKNOWN"), false);
                break;
        }


    // =====================================================
    // RIGHT / SLAVE OLED
    // HELLO WORLD!
    // =====================================================

    } else {

        oled_set_cursor(0, 1);

        for (uint8_t i = 0; i < OLED_COLS; i++) {

            if (i >= right_x &&
                i < right_x + RIGHT_TEXT_LEN) {

                oled_write_char(
                    RIGHT_TEXT[i - right_x],
                    false
                );

            } else {

                oled_write_char(' ', false);
            }
        }
    }

    return false;
}

#endif

// ====================
// MACROS
// ====================
enum custom_keycodes {
    CTRL_CMD_L = SAFE_RANGE,
    CTRL_CMD_R,
    CTRL_TAB_L,
    CTRL_TAB_R,
    CTRL_BACKSPACE,
    SEL_WORD_L,
    SEL_WORD_R,
    SKIP_WORD_L,
    SKIP_WORD_R,
    CTL_ALT_DEL,
    ALT_F4,
    // EXT_ENTER, // Enter back to _QWERTY/Base layer
    UNDO,
    REDO,
    CUT,
    COPY,
    PASTE,
    SW_WIN,  // Switch to next window         (alt-tab)
    SW_TAB,  // Switch to next browser tab    (ctrl-tab)
    OSM_CTL_TOGG,
    OSM_ALT_TOGG,
    OSM_SFT_TOGG,
    OSM_GUI_TOGG,
};

bool sw_win_active = false;
bool sw_tab_active = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    update_swapper(&sw_win_active, KC_LALT, KC_TAB, SW_WIN, KC_LSFT, keycode, record);
    update_swapper(&sw_tab_active, KC_LCTL, KC_TAB, SW_TAB, KC_LSFT, keycode, record);

    if (!record->event.pressed) {
        return true;
    }

    uint8_t mods = get_oneshot_mods();

    switch (keycode) {
        case CTRL_CMD_L:
            register_mods(MOD_BIT(KC_LCTL) | MOD_BIT(KC_LGUI));
            tap_code(KC_LEFT);
            unregister_mods(MOD_BIT(KC_LCTL) | MOD_BIT(KC_LGUI));
            return false;
        case CTRL_CMD_R:
            register_mods(MOD_BIT(KC_LCTL) | MOD_BIT(KC_LGUI));
            tap_code(KC_RGHT);
            unregister_mods(MOD_BIT(KC_LCTL) | MOD_BIT(KC_LGUI));
            return false;
        case CTRL_TAB_L:
            tap_code16(C(S(KC_TAB)));
            return false;
        case CTRL_TAB_R:
            tap_code16(C(KC_TAB));
            return false;
        case CTRL_BACKSPACE:
            tap_code16(C(KC_BSPC));
            return false;
        case SEL_WORD_L:
            tap_code16(C(S(KC_LEFT)));
            return false;
        case SEL_WORD_R:
            tap_code16(C(S(KC_RGHT)));
            return false;
        case SKIP_WORD_L:
            tap_code16(C(KC_LEFT));
            return false;
        case SKIP_WORD_R:
            tap_code16(C(KC_RGHT));
            return false;
        case CTL_ALT_DEL:
            register_code(KC_LCTL);
            register_code(KC_LALT);
            tap_code(KC_DEL);
            unregister_code(KC_LALT);
            unregister_code(KC_LCTL);
            return false;
        case ALT_F4:
            register_code(KC_LALT);
            tap_code(KC_F4);
            unregister_code(KC_LALT);
            return false;
        // case EXT_ENTER:
        //     tap_code(KC_ENT);
        //     layer_move(_QWERTY);
        //     return false;
        case UNDO:
            register_code(KC_LCTL);
            tap_code(KC_Z);
            unregister_code(KC_LCTL);
            return false;
        case REDO:
            register_code(KC_LCTL);
            tap_code(KC_Y);
            unregister_code(KC_LCTL);
            return false;
        case CUT:
            register_code(KC_LCTL);
            tap_code(KC_X);
            unregister_code(KC_LCTL);
            return false;
        case COPY:
            register_code(KC_LCTL);
            tap_code(KC_C);
            unregister_code(KC_LCTL);
            return false;
        case PASTE:
            register_code(KC_LCTL);
            tap_code(KC_V);
            unregister_code(KC_LCTL);
            return false;
        case OSM_CTL_TOGG:
            mods ^= MOD_BIT(KC_LCTL);
            set_oneshot_mods(mods);
            return false;
        case OSM_ALT_TOGG:
            mods ^= MOD_BIT(KC_LALT);
            set_oneshot_mods(mods);
            return false;
        case OSM_SFT_TOGG:
            mods ^= MOD_BIT(KC_LSFT);
            set_oneshot_mods(mods);
            return false;
        case OSM_GUI_TOGG:
            mods ^= MOD_BIT(KC_LGUI);
            set_oneshot_mods(mods);
            return false;
    }

    return true;
}

// Tap Dance declarations
enum {
    TD_QUOTE,
    TD_SLASH,
};
#ifdef TAP_DANCE_ENABLE

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    [TD_QUOTE] = ACTION_TAP_DANCE_DOUBLE(KC_QUOT, S(KC_QUOT)),
    [TD_SLASH] = ACTION_TAP_DANCE_DOUBLE(KC_SLSH, S(KC_SLSH)),
};

// Add tap dance item to your keymap in place of a keycode
// const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
//     TD(TD_ESC_CAPS)
// };
#endif

// ====================
// LAYERS
// ====================

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_QWERTY] = LAYOUT_split_3x5_3(
        //|------------+------------+------------+------------+------------|        |------------+------------+------------+------------+------------|
            KC_Q,        KC_W,        KC_E,        KC_R,        KC_T,                 KC_Y,        KC_U,        KC_I,        KC_O,        KC_P,
        //|------------+------------+------------+------------+------------|        |------------+------------+------------+------------+------------|
            KC_A,        KC_S,        KC_D,        KC_F,        KC_G,                 KC_H,        KC_J,        KC_K,        KC_L,        KC_QUOT,
        //|------------+------------+------------+------------+------------|        |------------+------------+------------+------------+------------|
            KC_Z,        KC_X,        KC_C,        KC_V,        KC_B,                 KC_N,        KC_M,        KC_COMM,     KC_DOT,      KC_SLSH,
        //|------------+------------+------------+------------+------------|        |------------+------------+------------+------------+------------|
                                           TO(_GAME),  MO(_EXTEND), KC_SPC,           OSM(MOD_LSFT),  MO(_SYM_NUM),   MO(_FN)

    ),

    [_SYM_NUM] = LAYOUT_split_3x5_3(
        //|------------+------------+------------+------------+------------|        |------------+------------+------------+------------+------------|
            KC_GRV,      S(KC_8),     S(KC_1),     S(KC_7),     S(KC_BSLS),           KC_7,        KC_8,        KC_9,        KC_MINS,     S(KC_EQL),
        //|------------+------------+------------+------------+------------|        |------------+------------+------------+------------+------------|
            S(KC_GRV),   S(KC_MINS),  KC_EQL,      S(KC_SCLN),  S(KC_2),              KC_4,        KC_5,        KC_6,        KC_0,        KC_EQL,
        //|------------+------------+------------+------------+------------|        |------------+------------+------------+------------+------------|
            KC_BSLS,     S(KC_6),     S(KC_4),     S(KC_5),     S(KC_3),              KC_1,        KC_2,        KC_3,        KC_DOT,      KC_SLSH,
        //|------------+------------+------------+------------+------------|        |------------+------------+------------+------------+------------|
                                      KC_NO,       MO(_EXTEND), KC_SPC,               KC_NO,       KC_NO,       KC_NO
    ),

    [_EXTEND] = LAYOUT_split_3x5_3(
        //|------------+------------+------------+------------+------------|        |------------+------------+------------+------------+------------|
            KC_ESC,      CTRL_CMD_L,  CTRL_CMD_R,  SW_WIN,      SW_TAB,               KC_HOME,     KC_PGDN,     KC_PGUP,     KC_END,      KC_INSERT,
        //|------------+------------+------------+------------+------------|        |------------+------------+------------+------------+------------|
            OSM_CTL_TOGG, OSM_ALT_TOGG, OSM_GUI_TOGG, OSM_SFT_TOGG, KC_TAB,           KC_LEFT,     KC_DOWN,     KC_UP,       KC_RGHT,     KC_BSPC,
        //|------------+------------+------------+------------+------------|        |------------+------------+------------+------------+------------|
            UNDO,        CUT,         COPY,        PASTE,       REDO,                 SKIP_WORD_L, SEL_WORD_L,  SEL_WORD_R,  SKIP_WORD_R, KC_DEL,
        //|------------+------------+------------+------------+------------|        |------------+------------+------------+------------+------------|
                                      KC_NO,       KC_NO,       KC_NO,                KC_ENT,      KC_NO,       KC_NO
    ),

    [_FN] = LAYOUT_split_3x5_3(
        //|------------+------------+------------+------------+------------|        |------------+------------+------------+------------+------------|
            KC_VOLD,     KC_VOLU,     KC_BRID,     KC_BRIU,     UG_TOGG,              KC_F7,       KC_F8,       KC_F9,       KC_F12,      KC_PSCR,
        //|------------+------------+------------+------------+------------|        |------------+------------+------------+------------+------------|
            OSM_CTL_TOGG, OSM_ALT_TOGG, OSM_GUI_TOGG, OSM_SFT_TOGG, KC_NO,            KC_F4,       KC_F5,       KC_F6,       KC_F11,      ALT_F4,
        //|------------+------------+------------+------------+------------|        |------------+------------+------------+------------+------------|
            KC_MPRV,     KC_MNXT,     KC_MPLY,     KC_MUTE,     KC_NO,                KC_F1,       KC_F2,       KC_F3,       KC_F10,      CTL_ALT_DEL,
        //|------------+------------+------------+------------+------------|        |------------+------------+------------+------------+------------|
                                      KC_NO,       KC_NO,       KC_NO,                KC_NO,       KC_NO,       KC_NO
    ),

    [_GAME] = LAYOUT_split_3x5_3(
        //|------------+------------+------------+------------+------------|        |------------+------------+------------+------------+------------|
            KC_TAB,      KC_Q,        KC_W,       KC_E,       KC_R,                   KC_NO,       KC_NO,       KC_NO,       KC_NO,      KC_PSCR,
        //|------------+------------+------------+------------+------------|        |------------+------------+------------+------------+------------|
            KC_LCTL,     KC_A,        KC_S,       KC_D,       KC_F,                   KC_NO,       KC_NO,       KC_NO,       KC_NO,      KC_NO,
        //|------------+------------+------------+------------+------------|        |------------+------------+------------+------------+------------|
            KC_LSFT,     KC_Z,        KC_X,       KC_C,       KC_V,                   KC_NO,       KC_NO,       KC_NO,       KC_NO,      KC_NO,
        //|------------+------------+------------+------------+------------|        |------------+------------+------------+------------+------------|
                                    TO(_QWERTY), MO(_GAME_FN), KC_SPC,                KC_NO,       TO(_QWERTY), KC_NO
    ),

    [_GAME_FN] = LAYOUT_split_3x5_3(
        //|------------+------------+------------+------------+------------|        |------------+------------+------------+------------+------------|
            KC_ESC,      KC_1,        KC_2,        KC_3,        KC_4,                 KC_NO,       KC_NO,       KC_NO,       KC_NO,      KC_NO,
        //|------------+------------+------------+------------+------------|        |------------+------------+------------+------------+------------|
            KC_LALT,     KC_G,        KC_M,        KC_T,        KC_I,                 KC_NO,       KC_NO,       KC_NO,       KC_NO,      KC_NO,
        //|------------+------------+------------+------------+------------|        |------------+------------+------------+------------+------------|
            KC_GRV,      KC_NO,       KC_NO,       KC_NO,       KC_NO,                KC_NO,       KC_NO,       KC_NO,       KC_NO,      KC_NO,
        //|------------+------------+------------+------------+------------|        |------------+------------+------------+------------+------------|
                                      KC_NO,       KC_NO,       KC_SPC,               KC_NO,       TO(_QWERTY), KC_NO
    ),
};

// ====================
// COMBOS
// ====================
#ifdef COMBO_ENABLE
enum combos {
    _COMBO_CW_TOGGLE,
    _COMBO_CTRL_BSPC,
    _COMBO_CTRL_DEL,
    _COMBO_FN,
    _COMBO_CMD,
    _COMBO_CTRL,
    _COMBO_ALT,
    _COMBO_CMD_SYM_NUM_LAYER,
    _COMBO_CTRL_SYM_NUM_LAYER,
    _COMBO_ALT_SYM_NUM_LAYER,
    _COMBO_ESC,
    _COMBO_SEMI_COLON,
    _COMBO_PARANTHESES_LEFT,
    _COMBO_PARANTHESES_RIGHT,
    _COMBO_CURLY_BRACES_LEFT,
    _COMBO_CURLY_BRACES_RIGHT,
    _COMBO_SQUARE_BRACKET_LEFT,
    _COMBO_SQUARE_BRACKET_RIGHT,
    _COMBO_ARROW_BRACKET_LEFT,
    _COMBO_ARROW_BRACKET_RIGHT,
};

const uint16_t PROGMEM combo_cw_toggle[] = {KC_X, KC_C, COMBO_END};
const uint16_t PROGMEM combo_ctrl_bspc[] = {KC_RGHT, KC_BSPC, COMBO_END};
const uint16_t PROGMEM combo_ctrl_del[] = {SKIP_WORD_R, KC_DEL, COMBO_END};
const uint16_t PROGMEM combo_fn[] = {TO(_EXTEND), TO(_SYM_NUM), COMBO_END};
const uint16_t PROGMEM combo_ctrl[] = {KC_A, KC_S, COMBO_END};
const uint16_t PROGMEM combo_alt[] = {KC_S, KC_D, COMBO_END};
const uint16_t PROGMEM combo_cmd[] = {KC_D, KC_F, COMBO_END};
const uint16_t PROGMEM combo_ctrl_sym_layer[] = {KC_GRV, S(KC_MINS), COMBO_END};
const uint16_t PROGMEM combo_alt_sym_layer[] = {S(KC_MINS), KC_EQL, COMBO_END};
const uint16_t PROGMEM combo_esc[] = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM combo_semi_colon[] = {KC_COMM, KC_DOT, COMBO_END};
const uint16_t PROGMEM combo_parenteses_left[] = {S(KC_SCLN), S(KC_2), COMBO_END};
const uint16_t PROGMEM combo_parenteses_right[] = {KC_4, KC_5, COMBO_END};
const uint16_t PROGMEM combo_curly_braces_left[] = {S(KC_5), S(KC_3), COMBO_END};
const uint16_t PROGMEM combo_curly_braces_right[] = {KC_1, KC_2, COMBO_END};
const uint16_t PROGMEM combo_square_bracket_left[] = {S(KC_7), S(KC_BSLS), COMBO_END};
const uint16_t PROGMEM combo_square_bracket_right[] = {KC_7, KC_8, COMBO_END};
const uint16_t PROGMEM combo_arrow_bracket_left[] = {KC_EQL, S(KC_SCLN), COMBO_END};
const uint16_t PROGMEM combo_arrow_bracket_right[] = {KC_5, KC_6, COMBO_END};

combo_t key_combos[] = {
    [_COMBO_CW_TOGGLE] = COMBO(combo_cw_toggle, CW_TOGG),
    [_COMBO_CTRL_BSPC] = COMBO(combo_ctrl_bspc, C(KC_BSPC)),
    [_COMBO_CTRL_DEL] = COMBO(combo_ctrl_del, C(KC_DEL)),
    [_COMBO_FN] = COMBO(combo_fn, TO(_FN)),
    [_COMBO_CTRL] = COMBO(combo_ctrl, OSM_CTL_TOGG),
    [_COMBO_ALT] = COMBO(combo_alt, OSM_ALT_TOGG),
    [_COMBO_CMD] = COMBO(combo_cmd, OSM_GUI_TOGG),
    [_COMBO_ESC] = COMBO(combo_esc, KC_ESC),
    [_COMBO_SEMI_COLON] = COMBO(combo_semi_colon, KC_SCLN),
    [_COMBO_PARANTHESES_LEFT] = COMBO(combo_parenteses_left, S(KC_9)),
    [_COMBO_PARANTHESES_RIGHT] = COMBO(combo_parenteses_right, S(KC_0)),
    [_COMBO_CURLY_BRACES_LEFT] = COMBO(combo_curly_braces_left, S(KC_LBRC)),
    [_COMBO_CURLY_BRACES_RIGHT] = COMBO(combo_curly_braces_right, S(KC_RBRC)),
    [_COMBO_SQUARE_BRACKET_LEFT] = COMBO(combo_square_bracket_left, KC_LBRC),
    [_COMBO_SQUARE_BRACKET_RIGHT] = COMBO(combo_square_bracket_right, KC_RBRC),
    [_COMBO_ARROW_BRACKET_LEFT] = COMBO(combo_arrow_bracket_left, S(KC_COMM)),
    [_COMBO_ARROW_BRACKET_RIGHT] = COMBO(combo_arrow_bracket_right, S(KC_DOT)),
};

#endif


#ifdef RGB_MATRIX_ENABLE

// Diagram of per-key LEDs when viewed from above:
//  23  18  17  10  9       36  37  44  45  50
//  22  19  16  11  8       35  38  43  46  49
//  21  20  15  12  7       34  39  42  47  48
//           14  13  6       33  40  41


// Disable initial rgb light when plugged in for the first time (default is red)
void keyboard_post_init_user(void) {
    rgb_matrix_sethsv_noeeprom(0, 0, 0);
}

// 1. Set the base color
// layer_state_t layer_state_set_user(layer_state_t state) {
//     switch (get_highest_layer(state)) {
//         case _QWERTY:
//             rgb_matrix_sethsv_noeeprom(0, 0, 0);                                  // no color
//             break;
//         case _SYM_NUM:
//             rgb_matrix_sethsv_noeeprom(128, 255, RGB_MATRIX_MAXIMUM_BRIGHTNESS);  // cyan
//             break;
//         case _EXTEND:
//             rgb_matrix_sethsv_noeeprom(11, 255, RGB_MATRIX_MAXIMUM_BRIGHTNESS);   // orange
//             break;
//         case _FN:
//             rgb_matrix_sethsv_noeeprom(43, 255, RGB_MATRIX_MAXIMUM_BRIGHTNESS);    // red
//             break;
//         case _GAME:
//             rgb_matrix_sethsv_noeeprom(85, 255, RGB_MATRIX_MAXIMUM_BRIGHTNESS);   // green
//             break;
//         case _GAME_FN:
//             rgb_matrix_sethsv_noeeprom(213, 255, RGB_MATRIX_MAXIMUM_BRIGHTNESS);  // purple
//             break;
//     }
//
//     return state;
// }

// 2. Set per key rgb
bool rgb_matrix_indicators_user(void) {
    switch (get_highest_layer(layer_state)) {
        case _QWERTY:
            break;
        case _SYM_NUM:
            rgb_matrix_set_color(40, 0, 255, 255);
            break;
        case _EXTEND:
            rgb_matrix_set_color(13, 255, 128, 0); // LAYER key
            rgb_matrix_set_color(23, 80, 0, 0); // ESC key
            rgb_matrix_set_color(33, 80, 255, 0); // ENTER key
            rgb_matrix_set_color(49, 255, 100, 0); // BACKSPACE key
            rgb_matrix_set_color(48, 80, 0, 0); // DEL key
            rgb_matrix_set_color(8, 0, 255, 255); // TAB key
            break;
        case _FN:
            rgb_matrix_set_color(41, 255, 20, 147);
            break;
        case _GAME:
            rgb_matrix_set_color(14, 80, 255, 0);
            break;
        case _GAME_FN:
            rgb_matrix_set_color(13, 255, 0, 255);
            break;
    }


    // Blinking rbg light to indicate that caps word will be toggled off in few seconds.
    if (is_caps_word_on()) {
        if ((timer_read() / 500) % 2) {
            rgb_matrix_set_color(20, 0, RGB_MATRIX_MAXIMUM_BRIGHTNESS, 0);
            rgb_matrix_set_color(15, 0, RGB_MATRIX_MAXIMUM_BRIGHTNESS, 0);
        } else {
            rgb_matrix_set_color(20, 0, 0, 0);
            rgb_matrix_set_color(15, 0, 0, 0);
        }
    }

    uint8_t oneshot_mods = get_oneshot_mods();

    if (oneshot_mods & MOD_BIT(KC_LCTL)) {
        rgb_matrix_set_color(22, 0, RGB_MATRIX_MAXIMUM_BRIGHTNESS, 0);
    }

    if (oneshot_mods & MOD_BIT(KC_LALT)) {
        rgb_matrix_set_color(19, 0, RGB_MATRIX_MAXIMUM_BRIGHTNESS, 0);
    }

    if (oneshot_mods & MOD_BIT(KC_LGUI)) {
        rgb_matrix_set_color(16, 0, RGB_MATRIX_MAXIMUM_BRIGHTNESS, 0);
    }

    if (oneshot_mods & MOD_BIT(KC_LSFT)) {
        rgb_matrix_set_color(11, 0, RGB_MATRIX_MAXIMUM_BRIGHTNESS, 0);
    }

    return false;
}

#endif
