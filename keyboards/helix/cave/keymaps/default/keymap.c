#include QMK_KEYBOARD_H

extern keymap_config_t keymap_config;

#define BASE 0
#define META 1
#define SYMB 2
#define SKY 3

// Fillers to make layering more clear
#define _______ KC_TRNS
#define XXXXXXX KC_NO

enum custom_keycodes {
  KCK_COMMA = SAFE_RANGE,
  KCK_UU,
  KCK_AI,
  KCK_OU,
  KCK_DOT,
  KCK_EI,
  KCK_UN,
  KCK_AN,
  KCK_ON,
  KCK_IN,
  KCK_EN,
  KCK_LBRC
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [BASE] = LAYOUT( \
    KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_RPRN,  KC_AMPR, KC_Y,   KC_U,    KC_I,    KC_O,   KC_P,    KC_MINS , \
    KC_LCTL,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_DLR,   KC_BSPC, KC_H,   KC_J,    KC_K,    KC_L,   KC_SCLN, KC_LBRC , \
    KC_LSFT,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_ESC,   KC_B   , KC_N,   KC_M,    KC_COMM, KC_DOT, KC_SLSH, KC_RBRC , \
    KC_GRV,   KC_CIRC, KC_LALT, KC_LGUI, MO(SYMB),KC_SPC,  MO(META), KC_EQL , KC_LSFT, KC_UNDS, KC_BSPC, KC_EQL, KC_AT,   KC_BSLS,   \
    _______, _______,  _______, KC_ESC , KC_ENT  ,KC_SPC,  _______,  _______, KC_ENT,  KC_UNDS, KC_BSPC, KC_EQL, _______, _______    \
  ),

  [META] = LAYOUT( \
    KC_ESC,  KC_EXLM, KC_DQUO, KC_HASH, KC_DLR,  KC_PERC, _______, _______, KC_AMPR, KC_QUOT, KC_LPRN, KC_RPRN, KC_MINS, KC_MINS,  \
    _______, _______, _______, KC_MS_U, _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT, _______, _______, \
    _______, _______, KC_MS_L, KC_MS_D, KC_MS_R, _______, _______, _______, KC_MINS, KC_MS_D, KC_MS_U, KC_MS_R, _______, _______,  \
    _______, _______, _______, KC_BTN1, KC_BTN2, _______, _______, KC_MINS, _______, KC_BTN1, KC_BTN2, _______, _______, TG(SKY) ,  \
    _______, _______, _______, KC_BTN1, KC_BTN2, _______, _______, KC_MINS, _______, KC_BTN1, KC_BTN2, _______, _______, _______   \
  ),

  [SYMB] = LAYOUT( \
    KC_TAB,  KC_EXLM, KC_AT,   KC_LCBR, KC_RCBR, KC_PIPE,KC_LPRN,   KC_RPRN, KC_AMPR, KC_7,    KC_8,    KC_9,    KC_ASTR, KC_MINS, \
    KC_LCTL, KC_HASH, KC_DLR,  KC_LPRN, KC_RPRN, KC_GRV, KC_DLR ,   KC_BSPC, KC_DQUO, KC_4,    KC_5,    KC_6,    KC_PLUS, KC_LBRC, \
    KC_LSFT, KC_PERC, KC_CIRC, KC_LBRC, KC_RBRC, KC_TILD,KC_ESC,    KC_B   , KC_QUOT, KC_1,    KC_2,    KC_3,    KC_SLSH, KC_RBRC, \
    _______, _______, _______, _______, _______, _______, _______,   KC_UNDS, _______, _______, KC_DOT,  KC_0,    KC_EQL,  _______,\
    _______, _______, _______, _______, _______, _______, _______,   KC_UNDS, _______, _______, KC_DOT,  KC_0,    KC_EQL,  _______ \
  ),


  [SKY] = LAYOUT( \
    _______, KCK_COMMA,KC_W,   KC_R   , KC_M   , KC_H   , _______,   _______, KCK_UU,  KCK_AI,  KCK_OU,  KCK_DOT, KCK_EI,   _______,\
    _______, KC_N,     KC_T,   KC_S   , KC_K   , KC_Y   , _______,   _______, KC_U,    KC_A,    KC_O,    KC_I,    KC_E,     KCK_LBRC,\
    _______, KC_P,     KC_D,   KC_Z   , KC_G   , KC_B   , _______,   _______, KCK_UN,  KCK_AN,  KCK_ON,  KCK_IN,  KCK_EN,   _______,\
    _______, _______, _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, _______,  _______,\
    _______, _______, _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, _______,  _______ \
  )
};

static int l_bracket_prefix_on = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  #ifdef SSD1306OLED
    if (record->event.pressed) {
      set_keylog(keycode, record);
    }
  #endif

  switch (keycode) {
    case KCK_COMMA:
      if (record->event.pressed) {
        if (l_bracket_prefix_on) {
          SEND_STRING(",");
        } else {
          SEND_STRING("nn");
        }
      }
      break;
    case KCK_UU:
      if (record->event.pressed) {
        SEND_STRING("uu");
      }
      break;
    case KCK_AI:
      if (record->event.pressed) {
        SEND_STRING("ai");
      }
      break;
    case KCK_OU:
      if (record->event.pressed) {
        SEND_STRING("ou");
      }
      break;
    case KCK_DOT:
      if (record->event.pressed) {
        if (l_bracket_prefix_on) {
          SEND_STRING(".");
        } else {
          SEND_STRING("nn");
        }
      }
      break;
    case KCK_EI:
      if (record->event.pressed) {
        SEND_STRING("ei");
      }
      break;
    case KCK_UN:
      if (record->event.pressed) {
        SEND_STRING("un");
      }
      break;
    case KCK_AN:
      if (record->event.pressed) {
        SEND_STRING("an");
      }
      break;
    case KCK_ON:
      if (record->event.pressed) {
        SEND_STRING("on");
      }
      break;
    case KCK_IN:
      if (record->event.pressed) {
        SEND_STRING("in");
      }
      break;
    case KCK_EN:
      if (record->event.pressed) {
        SEND_STRING("en");
      }
      break;
    case KCK_LBRC:
      if (record->event.pressed) {
        l_bracket_prefix_on = 1;
      }
      return false;
    default:
      l_bracket_prefix_on = 0;
      return true;
  }
  l_bracket_prefix_on = 0;

  return false;
}
