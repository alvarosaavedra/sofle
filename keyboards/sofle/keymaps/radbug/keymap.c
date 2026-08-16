// Copyright 2026 radbug
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Sofle v2 keymap - transcribed from the original firmware, adapted for Omarchy:
// adds Print Screen (Lower+6) and a nav cluster on Raise (Z/X/C/V -> Home/End/PgUp/PgDn).
// Raw snapshot: docs/firmware-snapshot.json

#include QMK_KEYBOARD_H
#include "print.h"
#include <stdio.h>

enum sofle_layers {
    _QWERTY,
    _LOWER,
    _RAISE,
    _ADJUST,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  | BSPC |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | TAB  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  |  -_  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | LCTL |   A  |   S  |   D  |   F  |   G  |-------.  ,---------|   H  |   J  |   K  |   L  |   ;  |  '"  |
 * `------+   Z  |   X  |   C  |   V  |   B  | MUTE  |  | PLAY   |   N  |   M  |   ,  |   .  |   /  |------'
 *        `----------------------------------/       /   \        \-----------------------------------------'
 *                   | LALT | LGUI |LOWER |  / SPACE /     \ ENTER \  | RAISE| RGUI | RALT |
 *                   `----------------------------'       \       -----------------+--------------------'*/
[_QWERTY] = LAYOUT(
  KC_ESC     , KC_1       , KC_2       , KC_3       , KC_4       , KC_5, KC_6       , KC_7       , KC_8       , KC_9       , KC_0       , KC_BSPC,
  KC_TAB     , KC_Q       , KC_W       , KC_E       , KC_R       , KC_T, KC_Y       , KC_U       , KC_I       , KC_O       , KC_P       , KC_MINS,
  KC_LCTL    , KC_A       , KC_S       , KC_D       , KC_F       , KC_G, KC_H       , KC_J       , KC_K       , KC_L       , KC_SCLN    , KC_QUOT,
  KC_LSFT    , KC_Z       , KC_X       , KC_C       , KC_V       , KC_B, KC_MUTE, KC_MPLY, KC_N       , KC_M       , KC_COMM    , KC_DOT     , KC_SLSH    , KC_RSFT,
  KC_LALT    , KC_LGUI    , MO(1)      , KC_LBRC    , KC_SPACE, KC_ENT     , KC_RBRC    , MO(2)      , KC_RGUI    , KC_RALT
),

/*
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | MUTE | VOL- | VOL+ | PLAY | PREV | NEXT |                    | PSCR  | TRNS | TRNS | TRNS | TRNS | DEL  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |                    |  F7  |  F8  |  F9  |  F10 |  F11 | F12  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  ~   |  |   |  "   |  YEN |  $   |  %  |-------.  ,---------|  ^   |  &   |  *   |  (   |  )   |  !   |
 * `------+ CAPS | TRNS | TRNS | TRNS | TRNS | TRNS  |  | TRNS   | TRNS | TRNS | TRNS | TRNS | TRNS |------'
 *        `----------------------------------/       /   \        \-----------------------------------------'
 *                   | ---- | TRNS | TRNS |  / TRNS  /     \ TRNS  \ | TRNS | RGUI | ---- |
 *                   `----------------------------'       \       -----------------+--------------------'*/
[_LOWER] = LAYOUT(
  KC_MUTE    , KC_VOLD    , KC_VOLU    , KC_MPLY    , KC_MRWD    , KC_MFFD, KC_PSCR    , _______    , _______    , _______    , _______    , KC_DEL,
  KC_F1      , KC_F2      , KC_F3      , KC_F4      , KC_F5      , KC_F6, KC_F7      , KC_F8      , KC_F9      , KC_F10     , KC_F11     , KC_F12,
  S(KC_GRV)  , S(KC_BSLS) , S(KC_QUOT) , KC_INT3    , S(KC_4)    , S(KC_5), S(KC_6)    , S(KC_7)    , S(KC_8)    , S(KC_9)    , S(KC_0)    , S(KC_3),
  KC_CAPS    , _______    , _______    , _______    , _______    , _______, _______, _______, S(KC_BSLS) , KC_GRV     , KC_EQL     , S(KC_LBRC) , S(KC_RBRC) , _______,
  XXXXXXX    , _______    , _______    , _______    , _______, _______    , _______    , _______    , KC_LGUI    , XXXXXXX
),

/*
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |                    |  F7  |  F8  |  F9  |  F10 |  F11 | F12  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | ---- | ---- | ---- | ---- | ---- | ---- |                    | ---- | ---- | ---- | ---- | ---- | ---- |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | TRNS | HOME | END  | PGUP | PGDN | TRNS |-------.  ,---------| LEFT | DOWN |  UP  | RIGHT| ---- | ---- |
 * `------+ TRNS | TRNS | TRNS | TRNS | TRNS | TRNS  |  | TRNS   |  {   |  +   |  }   |  |   | S(=) |------'
 *        `----------------------------------/       /   \        \-----------------------------------------'
 *                   | ---- | TRNS | TRNS |  / TRNS  /     \ TRNS  \ | TRNS | RGUI | ---- |
 *                   `----------------------------'       \       -----------------+--------------------'*/
[_RAISE] = LAYOUT(
  KC_F1      , KC_F2      , KC_F3      , KC_F4      , KC_F5      , KC_F6, KC_F7      , KC_F8      , KC_F9      , KC_F10     , KC_F11     , KC_F12,
  XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX, XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX,
  _______    , _______    , _______    , _______    , _______    , _______, KC_LEFT    , KC_DOWN    , KC_UP      , KC_RIGHT   , XXXXXXX    , XXXXXXX,
  _______    , KC_HOME    , KC_END     , KC_PGUP    , KC_PGDN    , _______, _______, _______, S(KC_EQL)  , KC_EQL     , KC_LBRC    , KC_RBRC    , KC_BSLS    , _______,
  XXXXXXX    , _______    , _______    , _______    , _______, _______    , _______    , _______    , KC_LGUI    , XXXXXXX
),

/*
 * Empty (unused).*/
[_ADJUST] = LAYOUT(
  XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX, XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX,
  XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX, XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX,
  XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX, XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX,
  XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX,
  XXXXXXX    , _______    , _______    , _______    , _______, _______    , _______    , _______    , _______    , XXXXXXX
),

};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [_QWERTY] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT) },
    [_LOWER]  = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT) },
    [_RAISE]  = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT) },
    [_ADJUST] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT) },
};
#endif

// Report the active layer over the debug console (CONSOLE_ENABLE) so the host
// daemon can mirror it on the Omarchy bar. Layer keys here are momentary, so
// a press/release pair arrives on every use and the host stays in sync.
layer_state_t layer_state_set_user(layer_state_t state) {
    uprintf("sofle layer=%u\n", (unsigned int)get_highest_layer(state));
    return state;
}

#ifdef OLED_ENABLE
// OLEDs show the same status on both halves, portrait (270°): layer tag,
// held modifiers (letters light up), WPM, caps lock (inverted when on).
// sofle.c only rotates the master, so force the slave to match. WPM and
// caps state reach the slave via the SPLIT_* options in rules.mk.
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return is_keyboard_master() ? rotation : OLED_ROTATION_270;
}

static void render_status(void) {
    oled_write_P(PSTR("\n\n"), false);
    switch (get_highest_layer(layer_state)) {
        case _QWERTY: oled_write_ln_P(PSTR("Qwrt"), false); break;
        case _LOWER:  oled_write_ln_P(PSTR("Lowr"), false); break;
        case _RAISE:  oled_write_ln_P(PSTR("Rais"), false); break;
        default:      oled_write_ln_P(PSTR("Adjt"), false); break;
    }
    oled_write_P(PSTR("\n\n"), false);
    uint8_t mods = get_mods();
    oled_write_P(PSTR("S"), mods & MOD_MASK_SHIFT);
    oled_write_P(PSTR("C"), mods & MOD_MASK_CTRL);
    oled_write_P(PSTR("A"), mods & MOD_MASK_ALT);
    oled_write_ln_P(PSTR("G"), mods & MOD_MASK_GUI);
    oled_write_P(PSTR("\n\n"), false);
    char wpm[6];
    snprintf(wpm, sizeof(wpm), "%3u\n", get_current_wpm());
    oled_write(wpm, false);
    oled_write_ln_P(PSTR("wpm"), false);
    oled_write_P(PSTR("\n\n"), false);
    oled_write_ln_P(PSTR("CPSLK"), host_keyboard_led_state().caps_lock);
}

bool oled_task_user(void) {
    render_status();
    return false; // replace the keyboard-level rendering entirely
}
#endif
