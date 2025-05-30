#pragma once
#include <Arduino.h>


typedef struct {
	String key;
	unsigned char keycode;
} SPECIAL_KEY;

const SPECIAL_KEY specialKeys[] = {
    {"ALT", 0xe2},
    {"END", 0x4d},
    {"F1", 0x3a},
    {"F2", 0x3b},
    {"F3", 0x3c},
    {"F4", 0x3d},
    {"F5", 0x3e},
    {"F6", 0x3f},
    {"F7", 0x40},
    {"F8", 0x41},
    {"F9", 0x42},
    {"F10", 0x43},
    {"F11", 0x44},
    {"F12", 0x45},
    {"F13", 0x68},
    {"F14", 0x69},
    {"F15", 0x6a},
    {"F16", 0x6b},
    {"F17", 0x6c},
    {"F18", 0x6d},
    {"F19", 0x6e},
    {"F20", 0x6f},
    {"F21", 0x70},
    {"F22", 0x71},
    {"F23", 0x72},
    {"F24", 0x73},
    {"GUI", 0xe3},
    {"TAB", 0x2b},
    {"AKUT", 0x2e},
    {"HOME", 0x4a},
    {"ALTGR", 0xe6},
    {"ENTER", 0x28},
    {"PAUSE", 0x48},
    {"SHIFT", 0xe1},
    {"APPLICATION", 0x65},
    {"BACKSPACE", 0x2a},
    {"CAPS_LOCK", 0x39},
    {"COMMAND", 0xe3},
    {"CTRL", 0xe0},
    {"DEL", 0x4c},
    {"DOWN", 0x51},
    {"ESC", 0x29},
    {"ESZETT", 0x2d},
    {"GRAVE_ACCENT", 0x33},
    {"GRAVIS", 0x2e},
    {"INSERT", 0x49},
    {"KEYPAD_ASTERISK", 0x55},
    {"KEYPAD_EIGHT", 0x60},
    {"KEYPAD_FIVE", 0x5d},
    {"KEYPAD_FORWARD_SLASH", 0x54},
    {"KEYPAD_FOUR", 0x5c},
    {"KEYPAD_MINUS", 0x56},
    {"KEYPAD_NINE", 0x61},
    {"NUMLOCK", 0x53},
    {"KEYPAD_ONE", 0x59},
    {"KEYPAD_PERIOD", 0x63},
    {"KEYPAD_PLUS", 0x57},
    {"KEYPAD_SEVEN", 0x5f},
    {"KEYPAD_SIX", 0x5e},
    {"KEYPAD_THREE", 0x5b},
    {"KEYPAD_TWO", 0x5a},
    {"KEYPAD_ZERO", 0x62},
    {"LEFT_ALT", 0xe2},
    {"LEFT", 0x50},
    {"LEFT_CTRL", 0xe0},
    {"LEFT_GUI", 0xe3},
    {"LEFT_SHIFT", 0xe1},
    {"OEM_102", 0x64},
    {"OPTION", 0xe2},
    {"PAGE_DOWN", 0x4e},
    {"PAGE_UP", 0x4b},
    {"PERIOD", 0x37},
    {"PRINT_SCREEN", 0x46},
    {"RETURN", 0x28},
    {"RIGHT_ALT", 0xe6},
    {"RIGHT", 0x4f},
    {"RIGHT_CTRL", 0xe4},
    {"RIGHT_GUI", 0xe7},
    {"RIGHT_SHIFT", 0xe5},
    {"SCROLL_LOCK", 0x47},
    {"SPACE", 0x2c},
    {"UP", 0x52},
    {"WIN", 0xe3},
    {"ZIRKUMFLEX", 0x35}
};