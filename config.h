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
#define TAPPING_TOGGLE 2

#ifdef TAP_DANCE_ENABLE
#define TAPPING_TERM 175
#define TAPPING_TERM_PER_KEY
#endif

#ifdef CAPS_WORD_ENABLE
//#define DOUBLE_TAP_SHIFT_TURNS_ON_CAPS_WORD
#endif

#ifdef OLED_ENABLE
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
#endif
