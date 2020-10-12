#include QMK_KEYBOARD_H
#include "mouse_timer.h"
#include "mousekey.h"
#include "keymap_extras/keymap_jp.h"

extern keymap_config_t keymap_config;

#define BASE 0
#define META 1
#define SYMB 2
#define MOUSE 3

// Fillers to make layering more clear
#define _______ KC_TRNS
#define XXXXXXX KC_NO

enum custom_keycodes {
  MY_COMMA = SAFE_RANGE,
  MY_UU,
  MY_AI,
  MY_OU,
  MY_DOT,
  MY_EI,
  MY_UN,
  MY_AN,
  MY_ON,
  MY_IN,
  MY_EN,
  MY_LBRC,
  MY_UAUA,
  MY_EA,
  MY_AH,
  MY_BCB,
  MY_BPB,
  MY_BCF,
  MY_BPF,
  MY_SCLN,
  MY_LSFT,
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

// Change KC_ and JP_ for ANSI and JP.
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [BASE] = LAYOUT( \
      KC_TAB,   KC_Q,      KC_W,     KC_E,    KC_R,     KC_T,    KC_LANG2, JP_COLN, KC_Y,   KC_U,   KC_I,    KC_O,   KC_P,    JP_MINS, \
      KC_LCTL,  KC_A,      KC_S,     KC_D,    KC_F,     KC_G,    JP_DLR,   KC_BSPC, KC_H,   KC_J,   KC_K,    KC_L,   MY_SCLN, JP_LBRC, \
      KC_GT,    KC_Z,      KC_X,     KC_C,    KC_V,     KC_B,    KC_ESC,   JP_COLN, KC_N,   KC_M,   JP_COMM, JP_DOT, JP_SLSH, JP_RBRC, \
      JP_GRV,   JP_CIRC,   KC_LALT,  KC_LGUI, MO(SYMB), KC_SPC,  MO(META), JP_EQL , MY_LSFT,JP_UNDS,_______, _______,JP_AT,  JP_YEN, \
      _______,  MO(MOUSE), _______ , KC_ENT  ,_______,  _______, _______,  _______, _______,  _______, _______, _______, _______, _______    \
      ),

  [META] = LAYOUT( \
      KC_ESC,  JP_EXLM, JP_DQT , JP_HASH, JP_DLR,  JP_PERC, _______, _______, JP_AMPR, JP_QUOT, JP_LPRN, JP_RPRN, JP_MINS, MY_AH,  \
      _______, _______, _______, KC_MS_U, _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT, MY_BCB, MY_BPB, \
      _______, _______, KC_MS_L, KC_MS_D, KC_MS_R, _______, _______, _______, JP_MINS, KC_HOME, KC_END,  KC_MS_R,  MY_BCF, MY_BPF,  \
      _______, _______, _______, KC_BTN1, KC_BTN2, _______, _______, MY_EA,  _______, MY_UAUA, _______, _______, _______, _______,  \
      _______, _______, _______, KC_BTN1, KC_BTN2, _______, _______, JP_MINS, _______, KC_BTN1,  KC_BTN2, _______, _______, _______   \
      ),

  [SYMB] = LAYOUT( \
      KC_TAB,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,    JP_RPRN,  JP_AMPR, KC_7,    KC_8,    KC_9,    JP_ASTR, JP_MINS, \
      KC_LCTL, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,   KC_BSPC,  JP_DQT , KC_4,    KC_5,    KC_6,    JP_PLUS, JP_LBRC, \
      KC_LSFT, JP_PERC, JP_CIRC, JP_LBRC, JP_RBRC, JP_TILD, KC_ESC,    KC_B   , KC_0   , KC_1,    KC_2,    KC_3,    KC_0   , JP_RBRC, \
      _______, _______, _______, _______, _______, _______, _______,   JP_UNDS, _______, _______, _______, _______, JP_EQL , _______,\
      _______, _______, _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, _______,  _______ \
      ),

  [MOUSE] = LAYOUT( \
      _______, _______, _______, _______, _______, _______, _______,   JP_AMPR, _______, KC_WH_L, KC_WH_D, KC_WH_R, _______, JP_MINS , \
      _______, _______, _______, _______, _______, _______, _______,   KC_BSPC, _______, KC_BTN1, KC_WH_U, KC_BTN2, JP_SCLN, JP_LBRC , \
      _______, _______, _______, _______, _______, _______, _______,   KC_ESC,  _______, _______, KC_BTN3, JP_DOT,  JP_SLSH, JP_RBRC , \
      _______, _______, _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, _______,  _______,\
      _______, _______, _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, _______,  _______ \
      )
};

static int l_bracket_prefix_on = 0;

static uint8_t mouse_buttons = 0;
static bool lshiftp = false;

static bool right_after_mouse_control(void);
static bool intelligent_mouse_key(keyrecord_t *record, enum mouse_button btn, uint8_t kc_btn);

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef SSD1306OLED
  if (record->event.pressed) {
    set_keylog(keycode, record);
  }
#endif

  switch (keycode) {
    case MY_COMMA:
      if (record->event.pressed) {
        if (l_bracket_prefix_on) {
          SEND_STRING(",");
        } else {
          SEND_STRING("nn");
        }
      }
      break;
    case MY_UU:
      if (record->event.pressed) {
        SEND_STRING("uu");
      }
      break;
    case MY_AI:
      if (record->event.pressed) {
        SEND_STRING("ai");
      }
      break;
    case MY_OU:
      if (record->event.pressed) {
        SEND_STRING("ou");
      }
      break;
    case MY_DOT:
      if (record->event.pressed) {
        if (l_bracket_prefix_on) {
          SEND_STRING(".");
        } else {
          SEND_STRING("nn");
        }
      }
      break;
    case MY_EI:
      if (record->event.pressed) {
        SEND_STRING("ei");
      }
      break;
    case MY_UN:
      if (record->event.pressed) {
        SEND_STRING("un");
      }
      break;
    case MY_AN:
      if (record->event.pressed) {
        SEND_STRING("an");
      }
      break;
    case MY_ON:
      if (record->event.pressed) {
        SEND_STRING("on");
      }
      break;
    case MY_IN:
      if (record->event.pressed) {
        SEND_STRING("in");
      }
      break;
    case MY_EN:
      if (record->event.pressed) {
        SEND_STRING("en");
      }
      break;
    case MY_LBRC:
      if (record->event.pressed) {
        l_bracket_prefix_on = 1;
      }
      return false;
    case MY_UAUA:
      if (record->event.pressed) {
        SEND_STRING("_@_*");
      }
      break;
    case MY_EA:
      if (record->event.pressed) {
        SEND_STRING("=>");
      }
      break;
    case MY_AH:
      if (record->event.pressed) {
        SEND_STRING("<-");
      }
      break;
    case MY_BCB:
      if (record->event.pressed) {
        SEND_STRING("[{");
      }
      break;
    case MY_BPB:
      if (record->event.pressed) {
        SEND_STRING("[(");
      }
      break;
    case MY_BCF:
      if (record->event.pressed) {
        SEND_STRING("]}");
      }
      break;
    case MY_BPF:
      if (record->event.pressed) {
        SEND_STRING("])");
      }
      break;
    case MY_SCLN:
      // ref https://github.com/qmk/qmk_firmware/blob/master/users/spacebarracecar/spacebarracecar.cb
      if (record->event.pressed) {
        // timer_timeout();
        if (lshiftp) {
          unregister_code(KC_LSFT);
          unregister_code(JP_COLN);
          register_code(JP_COLN);
          // add_to_prev(kc2);
        } else {
          unregister_code(KC_LSFT);
          unregister_code(JP_SCLN);
          register_code(JP_SCLN);
        }
      } else {
        unregister_code(JP_SCLN);
        unregister_code(JP_COLN);
      }
      break;
    case MY_LSFT:
      if(record->event.pressed) {
        lshiftp = true;
        unregister_code(KC_LSFT);
        register_code(KC_LSFT);
        lshiftp = true;
      } else {
        unregister_code(KC_LSFT);
        lshiftp = false;
      }
      return false;
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
    case KC_COMM:
      return intelligent_mouse_key(record, BUTTON_BTN3, KC_MS_BTN3);
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
