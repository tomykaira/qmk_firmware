#include QMK_KEYBOARD_H
#include "mouse_timer.h"
#include "mousekey.h"

extern keymap_config_t keymap_config;

#define BASE 0
#define META 1
#define SYMB 2
#define MOUSE 3

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
  KCK_LBRC,
  KCK_UAUA,
  KCK_EA,
  KCK_AH,
  KCK_BCB,
  KCK_BPB,
  KCK_BCF,
  KCK_BPF,
};

enum mouse_button {
  BUTTON_BTN1 = 1 << 0,
  BUTTON_BTN2 = 1 << 1,
  BUTTON_BTN3 = 1 << 2,
  BUTTON_WH_LEFT = 1 << 3,
  BUTTON_WH_RIGHT = 1 << 4,
  BUTTON_WH_UP = 1 << 5,
  BUTTON_WH_DOWN = 1 << 6,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [BASE] = LAYOUT( \
      KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_LANG2, KC_LANG1,KC_Y,   KC_U,   KC_I,    KC_O,   KC_P,    KC_MINS , \
      KC_LCTL,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_DLR,   KC_BSPC, KC_H,   KC_J,   KC_K,    KC_L,   KC_SCLN, KC_LBRC , \
      KC_GT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_ESC,   KC_BTN1, KC_N,   KC_M,   KC_COMM, KC_DOT, KC_SLSH, KC_RBRC , \
      KC_GRV,   KC_CIRC, KC_LALT, KC_LGUI, MO(SYMB),KC_SPC,  MO(META), KC_EQL , KC_LSFT,KC_UNDS, KC_LSFT, KC_EQL, KC_AT,   KC_BSLS,   \
      _______, _______,  MO(MOUSE), KC_ESC , KC_ENT  ,KC_SPC,  _______,  _______, KC_ENT,  KC_UNDS, KC_BSPC, KC_EQL, _______, _______    \
      ),

  [META] = LAYOUT( \
      KC_ESC,  KC_EXLM, KC_DQUO, KC_HASH, KC_DLR,  KC_PERC, _______, _______, KC_AMPR, KC_QUOT, KC_LPRN, KC_RPRN, KC_MINS, KCK_AH,  \
      _______, _______, _______, KC_MS_U, _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT, KCK_BCB, KCK_BPB, \
      _______, _______, KC_MS_L, KC_MS_D, KC_MS_R, _______, _______, _______, KC_MINS, KC_HOME, KC_END, KC_MS_R,   KCK_BCF, KCK_BPF,  \
      _______, _______, _______, KC_BTN1, KC_BTN2, _______, _______, KCK_EA,  _______, KCK_UAUA, KC_BTN2, _______, _______, _______,  \
      _______, _______, _______, KC_BTN1, KC_BTN2, _______, _______, KC_MINS, _______, KC_BTN1, KC_BTN2, _______, _______, _______   \
      ),

  [SYMB] = LAYOUT( \
      KC_TAB,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,    KC_RPRN, KC_AMPR, KC_7,    KC_8,    KC_9,    KC_ASTR, KC_MINS, \
      KC_LCTL, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,   KC_BSPC, KC_DQUO, KC_4,    KC_5,    KC_6,    KC_PLUS, KC_LBRC, \
      KC_LSFT, KC_PERC, KC_CIRC, KC_LBRC, KC_RBRC, KC_TILD,KC_ESC,    KC_B   , KC_QUOT, KC_1,    KC_2,    KC_3,    KC_SLSH, KC_RBRC, \
      _______, _______, _______, _______, _______, _______, _______,   KC_UNDS, _______, _______, KC_DOT,  KC_0,    KC_EQL,  _______,\
      _______, _______, _______, _______, _______, _______, _______,   KC_UNDS, _______, _______, KC_DOT,  KC_0,    KC_EQL,  _______ \
      ),

  [MOUSE] = LAYOUT( \
      _______, _______, _______, _______, _______, _______, _______,   KC_AMPR, KC_Y,   KC_WH_L, KC_WH_D, KC_WH_R,KC_P,    KC_MINS , \
      _______, _______, _______, _______, _______, _______, _______,   KC_BSPC, KC_H,   KC_BTN1, KC_WH_U, KC_BTN2,KC_SCLN, KC_LBRC , \
      _______, _______, _______, _______, _______, _______, _______,   KC_ESC,  KC_B,   KC_N,    KC_BTN3, KC_DOT, KC_SLSH, KC_RBRC , \
      _______, _______, _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, _______,  _______,\
      _______, _______, _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, _______,  _______ \
      )
};

static int l_bracket_prefix_on = 0;

static uint8_t mouse_buttons = 0;

static bool right_after_mouse_control(void);
static bool intelligent_mouse_key(keyrecord_t *record, enum mouse_button btn, uint8_t kc_btn);

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
    case KCK_UAUA:
      if (record->event.pressed) {
        SEND_STRING("_@_*");
      }
      break;
    case KCK_EA:
      if (record->event.pressed) {
        SEND_STRING("=>");
      }
      break;
    case KCK_AH:
      if (record->event.pressed) {
        SEND_STRING("<-");
      }
      break;
    case KCK_BCB:
      if (record->event.pressed) {
        SEND_STRING("[{");
      }
      break;
    case KCK_BPB:
      if (record->event.pressed) {
        SEND_STRING("[(");
      }
      break;
    case KCK_BCF:
      if (record->event.pressed) {
        SEND_STRING("]}");
      }
      break;
    case KCK_BPF:
      if (record->event.pressed) {
        SEND_STRING("])");
      }
      break;
    case KC_U:
      return intelligent_mouse_key(record, BUTTON_WH_LEFT, KC_MS_WH_LEFT);
    case KC_I:
      return intelligent_mouse_key(record, BUTTON_WH_DOWN, KC_MS_WH_DOWN);
    case KC_O:
      return intelligent_mouse_key(record, BUTTON_WH_RIGHT, KC_MS_WH_RIGHT);
    case KC_J:
      return intelligent_mouse_key(record, BUTTON_BTN1, KC_MS_BTN1);
    case KC_K:
      return intelligent_mouse_key(record, BUTTON_WH_UP, KC_MS_WH_UP);
    case KC_L:
      return intelligent_mouse_key(record, BUTTON_BTN2, KC_MS_BTN2);
    default:
      l_bracket_prefix_on = 0;
      host_mouse_send_ms = HOST_MOUSE_SEND_MS_UNSET;
      return true;
  }
  l_bracket_prefix_on = 0;

  return false;
}

bool intelligent_mouse_key(keyrecord_t *record, enum mouse_button btn, uint8_t kc_btn) {
  if (record->event.pressed) {
    if (right_after_mouse_control()) {
      mouse_buttons |= btn;
      mousekey_on(kc_btn);
      mousekey_send();
      return false;
    } else {
      return true;
    }
  } else {
    if (mouse_buttons & btn) {
      mouse_buttons &= ~btn;
      mousekey_off(kc_btn);
      mousekey_send();
      return false;
    } else {
      return true;
    }
  }
}

bool right_after_mouse_control(void) {
  if (host_mouse_send_ms == HOST_MOUSE_SEND_MS_UNSET)
    return false;
  uint16_t now = timer_read();
  if (TIMER_DIFF_16(now, host_mouse_send_ms) > 1000) {
    host_mouse_send_ms = HOST_MOUSE_SEND_MS_UNSET;
    return false;
  } else {
    host_mouse_send_ms = HOST_MOUSE_SEND_MS_UNSET;
    return true;
  }
}

// vim: set expandtab sw=2 ts=2:
