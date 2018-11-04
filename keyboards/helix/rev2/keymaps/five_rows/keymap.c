#include QMK_KEYBOARD_H
#include "bootloader.h"
#ifdef PROTOCOL_LUFA
#include "lufa.h"
#include "split_util.h"
#endif

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum layer_number {
    _QWERTY = 0,
    SYMB,
    MDIA
};

// Fillers to make layering more clear
#define _______ KC_TRNS
#define XXXXXXX KC_NO

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT( \
      KC_ESC,  KC_EXLM, KC_DQUO, KC_HASH, KC_DLR,  KC_PERC,                   KC_AMPR, KC_QUOT, KC_LPRN, KC_RPRN, KC_MINS, KC_MINS, \
      KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, \
      KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                      KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_RBRC, \
      KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,   KC_QUOT,  KC_B   , KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_BSLS, \
      rC_GRV,  KC_CIRC, KC_LGUI, KC_LALT, MO(SYMB),KC_SPC, MO(MDIA), KC_UNDS, KC_ENT,  KC_UNDS, KC_BSPC, KC_EQL,  KC_RCTL, KC_AT \
      ),
  [SYMB] = LAYOUT( \
      _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                     KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11, \
      _______, KC_EXLM, KC_AT,   KC_LCBR, KC_RCBR, KC_PIPE,                   _______, KC_7,    KC_8,    KC_9,    KC_ASTR,    KC_F12, \
      _______, KC_HASH, KC_DLR,  KC_LPRN, KC_RPRN, KC_GRV,                    _______, KC_4,    KC_5,    KC_6,    KC_PLUS, _______, \
      _______, KC_PERC, KC_CIRC, KC_LBRC, KC_RBRC, KC_TILD, _______, _______, _______, KC_1,    KC_2,    KC_3,    KC_SLSH, _______, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_DOT,  KC_0,    KC_EQL,  _______ \
      ),
  [MDIA] = LAYOUT( \
      _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______, \
      _______, _______, _______, KC_MS_U, _______, _______,                   KC_HOME, KC_PGDN, KC_PGUP, KC_END , _______, _______, \
      _______, _______, KC_MS_L, KC_MS_D, KC_MS_R, _______,                   KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT, _______, _______, \
      _______, _______, _______, KC_BTN1, KC_BTN2, _______, _______, _______, KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, _______, _______, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_BTN1, KC_BTN2, _______, _______, _______ \
      )
};
