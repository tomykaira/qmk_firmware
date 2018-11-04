#include QMK_KEYBOARD_H
#include "bootloader.h"
#ifdef PROTOCOL_LUFA
#include "lufa.h"
#include "split_util.h"
#endif
#ifdef AUDIO_ENABLE
  #include "audio.h"
#endif
#ifdef SSD1306OLED
  #include "ssd1306.h"
#endif

extern keymap_config_t keymap_config;

#define BASE 0
#define META 1
#define SYMB 2

// Fillers to make layering more clear
#define _______ KC_TRNS
#define XXXXXXX KC_NO

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [BASE] = LAYOUT( \
    KC_ESC,  KC_EXLM, KC_DQUO,KC_HASH, KC_DLR,  KC_PERC, KC_AMPR,KC_QUOT, KC_LPRN, KC_RPRN,KC_MINS, KC_MINS, \
    KC_TAB,  KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,   KC_U,    KC_I,    KC_O,   KC_P,    KC_LBRC, \
    KC_LCTL, KC_A,   KC_S,    KC_D,    KC_F,    KC_G,    KC_H,   KC_J,    KC_K,    KC_L,   KC_SCLN, KC_RBRC, \
    KC_LSFT, KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,    KC_N,   KC_M,    KC_COMM, KC_DOT, KC_SLSH, KC_BSLS, \
    KC_LALT,KC_LGUI, MO(SYMB),KC_SPC,  MO(META),KC_GRV,  KC_BSPC,KC_EQL,  KC_ENT,  KC_UNDS, KC_AT,   KC_AT   \
  ),

  [META] = LAYOUT( \
    KC_ESC,  KC_EXLM, KC_DQUO, KC_HASH, KC_DLR,  KC_PERC, KC_AMPR, KC_QUOT, KC_LPRN, KC_RPRN, KC_MINS, KC_MINS, \
    KC_ESC,  KC_EXLM, KC_DQUO, KC_HASH, KC_DLR,  KC_PERC, KC_AMPR, KC_QUOT, KC_LPRN, KC_RPRN, KC_MINS, KC_MINS, \
    _______, _______, _______, KC_MS_U, _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT, _______, _______, \
    _______, _______, KC_MS_L, KC_MS_D, KC_MS_R, _______, KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, _______, _______, \
    _______, _______, _______, KC_BTN1, KC_BTN2, _______, KC_DEL , KC_BTN1, KC_BTN2, _______, _______, _______ \
  ),

  [SYMB] = LAYOUT( \
    _______, KC_EXLM, KC_AT,   KC_LCBR, KC_RCBR, KC_PIPE, KC_AMPR, KC_7,    KC_8,    KC_9,    KC_ASTR, _______, \
    _______, KC_EXLM, KC_AT,   KC_LCBR, KC_RCBR, KC_PIPE, KC_AMPR, KC_7,    KC_8,    KC_9,    KC_ASTR, _______, \
    _______, KC_HASH, KC_DLR,  KC_LPRN, KC_RPRN, KC_GRV,  KC_DQUO, KC_4,    KC_5,    KC_6,    KC_PLUS, _______, \
    _______, KC_PERC, KC_CIRC, KC_LBRC, KC_RBRC, KC_TILD, KC_QUOT, KC_1,    KC_2,    KC_3,    KC_SLSH, _______, \
    _______, _______, _______, _______, _______, _______, _______, _______, KC_DOT,  KC_0,    KC_0,    KC_0 \
  )

};
