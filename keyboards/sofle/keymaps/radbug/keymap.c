// Copyright 2026 radbug
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Sofle v2 keymap - baseline transcribed from the keyboard's firmware via the
// VIA protocol (fc32:0287).
// Raw snapshot: docs/firmware-snapshot.json

#include QMK_KEYBOARD_H

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
 * | MUTE | VOL- | VOL+ | PLAY | PREV | NEXT |                    | TRNS | TRNS | TRNS | TRNS | TRNS | DEL  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |                    |  F7  |  F8  |  F9  |  F10 |  F11 | F12  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  ~   |  |   |  "   |  YEN |  $   |  %  |-------.  ,---------|  ^   |  &   |  *   |  (   |  )   |  !   |
 * `------+ CAPS | TRNS | TRNS | TRNS | TRNS | TRNS  |  | TRNS   | TRNS | TRNS | TRNS | TRNS | TRNS |------'
 *        `----------------------------------/       /   \        \-----------------------------------------'
 *                   | ---- | TRNS | TRNS |  / TRNS  /     \ TRNS  \ | TRNS | RGUI | ---- |
 *                   `----------------------------'       \       -----------------+--------------------'*/
[_LOWER] = LAYOUT(
  KC_MUTE    , KC_VOLD    , KC_VOLU    , KC_MPLY    , KC_MRWD    , KC_MFFD, _______    , _______    , _______    , _______    , _______    , KC_DEL,
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
 * | TRNS | TRNS | TRNS | TRNS | TRNS | TRNS |-------.  ,---------| LEFT | DOWN |  UP  | RIGHT| ---- | ---- |
 * `------+ TRNS | TRNS | TRNS | TRNS | TRNS | TRNS  |  | TRNS   |  {   |  +   |  }   |  |   | S(=) |------'
 *        `----------------------------------/       /   \        \-----------------------------------------'
 *                   | ---- | TRNS | TRNS |  / TRNS  /     \ TRNS  \ | TRNS | RGUI | ---- |
 *                   `----------------------------'       \       -----------------+--------------------'*/
[_RAISE] = LAYOUT(
  KC_F1      , KC_F2      , KC_F3      , KC_F4      , KC_F5      , KC_F6, KC_F7      , KC_F8      , KC_F9      , KC_F10     , KC_F11     , KC_F12,
  XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX, XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX    , XXXXXXX,
  _______    , _______    , _______    , _______    , _______    , _______, KC_LEFT    , KC_DOWN    , KC_UP      , KC_RIGHT   , XXXXXXX    , XXXXXXX,
  _______    , _______    , _______    , _______    , _______    , _______, _______, _______, S(KC_EQL)  , KC_EQL     , KC_LBRC    , KC_RBRC    , KC_BSLS    , _______,
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
