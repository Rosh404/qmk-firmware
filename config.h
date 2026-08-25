/*
This is the c configuration file for the keymap

Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2015 Jack Humbert

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

#pragma once

// General
/* Select hand configuration */
#define MASTER_LEFT
// #define MASTER_RIGHT
// #define EE_HANDS

// This sends whether or not ctrl, shift, etc. are held to the secondary half of
// the keyboard. Note that it seems to "break" one half entirely unless BOTH
// halves have this enabled.
#define SPLIT_MODS_ENABLE

// Allow the current layer to be synced between the halves.
#define SPLIT_LAYER_STATE_ENABLE

// Allow the state of caps lock, num lock, etc. to be synced between the halves.
#define SPLIT_LED_STATE_ENABLE

// #define RGBLED_SPLIT { 27, 27 }

#ifdef TAP_DANCE_ENABLE
#define TAPPING_TOGGLE 2
#define TAPPING_TERM 175
#define TAPPING_TERM_PER_KEY
#endif

#ifdef CAPS_WORD_ENABLE
//#define DOUBLE_TAP_SHIFT_TURNS_ON_CAPS_WORD
#endif

#ifdef OLED_ENABLE
#define SPLIT_OLED_ENABLE

// WPM-responsive animation stuff here
#define IDLE_FRAMES 5
#define IDLE_SPEED 35 // below this wpm value your animation will idle
#define PREP_FRAMES 2
#define TAP_FRAMES 5
#define TAP_SPEED 50
#define ANIM_FRAME_DURATION 150 // how long each frame lasts in ms
#define ANIM_SIZE 416 // number of bytes in array, minimize for adequate firmware size, max is 1024
#define FAST_TYPE_WPM 45 //Switch to fast animation when over words per minute
//#define ANIM_INVERT false
//#define ANIM_RENDER_WPM true
#endif

#ifdef COMBO_ENABLE
#define COMBO_TERM 50
#define COMBO_SHOULD_TRIGGER
#endif

#ifdef RGB_MATRIX_ENABLE
#define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_NONE
#define RGB_MATRIX_DEFAULT_HUE 0
#undef RGB_MATRIX_MAXIMUM_BRIGHTNESS
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 80 // limits maximum brightness of LEDs to 150 out of 255. Higher may cause the controller to crash.

// Turn off RGB when USB is suspended
#define RGB_MATRIX_SLEEP

// #    define RGBLED_NUM 54  // Number of LEDs
// #    define DRIVER_LED_TOTAL 48
// #    define RGB_MATRIX_SPLIT { 27, 27 }

/* Disable the animations you don't want/need.  You will need to disable a good number of these    *
 * because they take up a lot of space.  Disable until you can successfully compile your firmware. */
// #   define DISABLE_RGB_MATRIX_ALPHAS_MODS
#define DISABLE_RGB_MATRIX_GRADIENT_UP_DOWN
#define DISABLE_RGB_MATRIX_BREATHING
#define DISABLE_RGB_MATRIX_CYCLE_ALL
#define DISABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
#define DISABLE_RGB_MATRIX_CYCLE_UP_DOWN
#define DISABLE_RGB_MATRIX_CYCLE_OUT_IN
#define DISABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
#define DISABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
#define DISABLE_RGB_MATRIX_DUAL_BEACON
#define DISABLE_RGB_MATRIX_RAINBOW_BEACON
#define DISABLE_RGB_MATRIX_RAINBOW_PINWHEELS
#define DISABLE_RGB_MATRIX_RAINDROPS
#define DISABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
#define DISABLE_RGB_MATRIX_TYPING_HEATMAP
#define DISABLE_RGB_MATRIX_DIGITAL_RAIN
#define DISABLE_RGB_MATRIX_SOLID_REACTIVE
#define DISABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#define DISABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
#define DISABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
#define DISABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS
#define DISABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
#define DISABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS
#define DISABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
#define DISABLE_RGB_MATRIX_SPLASH
#define DISABLE_RGB_MATRIX_MULTISPLASH
#define DISABLE_RGB_MATRIX_SOLID_SPLASH
#define DISABLE_RGB_MATRIX_SOLID_MULTISPLASH
#endif
