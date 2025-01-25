#pragma once


#define KEY_ALT                     0xe2
#define KEY_END                     0x4d
#define KEY_F1                      0x3a
#define KEY_F2                      0x3b
#define KEY_F3                      0x3c
#define KEY_F4                      0x3d
#define KEY_F5                      0x3e
#define KEY_F6                      0x3f
#define KEY_F7                      0x40
#define KEY_F8                      0x41
#define KEY_F9                      0x42
#define KEY_F10                     0x43
#define KEY_F11                     0x44
#define KEY_F12                     0x45
#define KEY_F13                     0x68
#define KEY_F14                     0x69
#define KEY_F15                     0x6a
#define KEY_F16                     0x6b
#define KEY_F17                     0x6c
#define KEY_F18                     0x6d
#define KEY_F19                     0x6e
#define KEY_F20                     0x6f
#define KEY_F21                     0x70
#define KEY_F22                     0x71
#define KEY_F23                     0x72
#define KEY_F24                     0x73
#define KEY_GUI                     0xe3
#define KEY_TAB                     0x2b
#define KEY_AKUT                    0x2e
#define KEY_HOME                    0x4a
#define KEY_ALTGR                   0xe6
#define KEY_ENTER                   0x28
#define KEY_PAUSE                   0x48
#define KEY_SHIFT                   0xe1
#define KEY_APPLICATION             0x65
#define KEY_BACKSPACE               0x2a
#define KEY_CAPS_LOCK               0x39
#define KEY_COMMAND                 0xe3
#define KEY_CONTROL                 0xe0
#define KEY_DELETE                  0x4c
#define KEY_DOWN_ARROW              0x51
#define KEY_ESCAPE                  0x29
#define KEY_ESZETT                  0x2d
#define KEY_GRAVE_ACCENT            0x33
#define KEY_GRAVIS                  0x2e
#define KEY_INSERT                  0x49
#define KEY_KEYPAD_ASTERISK         0x55
#define KEY_KEYPAD_EIGHT            0x60
#define KEY_KEYPAD_FIVE             0x5d
#define KEY_KEYPAD_FORWARD_SLASH    0x54
#define KEY_KEYPAD_FOUR             0x5c
#define KEY_KEYPAD_MINUS            0x56
#define KEY_KEYPAD_NINE             0x61
#define KEY_KEYPAD_NUMLOCK          0x53
#define KEY_KEYPAD_ONE              0x59
#define KEY_KEYPAD_PERIOD           0x63
#define KEY_KEYPAD_PLUS             0x57
#define KEY_KEYPAD_SEVEN            0x5f
#define KEY_KEYPAD_SIX              0x5e
#define KEY_KEYPAD_THREE            0x5b
#define KEY_KEYPAD_TWO              0x5a
#define KEY_KEYPAD_ZERO             0x62
#define KEY_LEFT_ALT                0xe2
#define KEY_LEFT_ARROW              0x50
#define KEY_LEFT_CONTROL            0xe0
#define KEY_LEFT_GUI                0xe3
#define KEY_LEFT_SHIFT              0xe1
#define KEY_OEM_102                 0x64
#define KEY_OPTION                  0xe2
#define KEY_PAGE_DOWN               0x4e
#define KEY_PAGE_UP                 0x4b
#define KEY_PERIOD                  0x37
#define KEY_PRINT_SCREEN            0x46
#define KEY_RETURN                  0x28
#define KEY_RIGHT_ALT               0xe6
#define KEY_RIGHT_ARROW             0x4f
#define KEY_RIGHT_BRACKET           0x2e
#define KEY_RIGHT_CONTROL           0xe4
#define KEY_RIGHT_GUI               0xe7
#define KEY_RIGHT_SHIFT             0xe5
#define KEY_SCROLL_LOCK             0x47
#define KEY_UP_ARROW                0x52
#define KEY_WINDOWS                 0xe3
#define KEY_ZIRKUMFLEX              0x35


typedef struct {
	unsigned char usage;
	unsigned char modifier;
} KEYMAP;

#define KEYMAP_SIZE (152)
const KEYMAP keymap[KEYMAP_SIZE] = {
    {0, 0},             /* NUL */
    {0, 0},             /* SOH */
    {0, 0},             /* STX */
    {0, 0},             /* ETX */
    {0, 0},             /* EOT */
    {0, 0},             /* ENQ */
    {0, 0},             /* ACK */
    {0, 0},             /* BEL */
    {0x2a, 0},          /* BS  */  /* Keyboard Delete (Backspace) */
    {0x2b, 0},          /* TAB */  /* Keyboard Tab */
    {0x28, 0},          /* LF  */  /* Keyboard Return (Enter) */
    {0, 0},             /* VT  */
    {0, 0},             /* FF  */
    {0, 0},             /* CR  */
    {0, 0},             /* SO  */
    {0, 0},             /* SI  */
    {0, 0},             /* DEL */
    {0, 0},             /* DC1 */
    {0, 0},             /* DC2 */
    {0, 0},             /* DC3 */
    {0, 0},             /* DC4 */
    {0, 0},             /* NAK */
    {0, 0},             /* SYN */
    {0, 0},             /* ETB */
    {0, 0},             /* CAN */
    {0, 0},             /* EM  */
    {0, 0},             /* SUB */
    {0, 0},             /* ESC */
    {0, 0},             /* FS  */
    {0, 0},             /* GS  */
    {0, 0},             /* RS  */
    {0, 0},             /* US  */
    {0x2c, 0},          /*   */
    {0x1e, KEY_SHIFT},  /* ! */
    {0x1f, KEY_SHIFT},  /* " */
    {0x31, 0},          /* # */
    {0x21, KEY_SHIFT},  /* $ */
    {0x22, KEY_SHIFT},  /* % */
    {0x23, KEY_SHIFT},  /* & */
    {0x31, KEY_SHIFT},  /* ' */
    {0x25, KEY_SHIFT},  /* ( */
    {0x26, KEY_SHIFT},  /* ) */
    {0x30, KEY_SHIFT},  /* * */
    {0x30, 0},          /* + */
    {0x36, 0},          /* , */
    {0x38, 0},          /* - */
    {0x37, 0},          /* . */
    {0x24, KEY_SHIFT},  /* / */
    {0x27, 0},          /* 0 */
    {0x1e, 0},          /* 1 */
    {0x1f, 0},          /* 2 */
    {0x20, 0},          /* 3 */
    {0x21, 0},          /* 4 */
    {0x22, 0},          /* 5 */
    {0x23, 0},          /* 6 */
    {0x24, 0},          /* 7 */
    {0x25, 0},          /* 8 */
    {0x26, 0},          /* 9 */
    {0x37, KEY_SHIFT},  /* : */
    {0x36, KEY_SHIFT},  /* ; */
    {0x64, 0},          /* < */
    {0x27, KEY_SHIFT},  /* = */
    {0x64, KEY_SHIFT},  /* > */
    {0x2d, KEY_SHIFT},  /* ? */
    {0x14, KEY_ALTGR},  /* @ */ 
    {0x04, KEY_SHIFT},  /* A */
    {0x05, KEY_SHIFT},  /* B */
    {0x06, KEY_SHIFT},  /* C */
    {0x07, KEY_SHIFT},  /* D */
    {0x08, KEY_SHIFT},  /* E */
    {0x09, KEY_SHIFT},  /* F */
    {0x0a, KEY_SHIFT},  /* G */
    {0x0b, KEY_SHIFT},  /* H */
    {0x0c, KEY_SHIFT},  /* I */
    {0x0d, KEY_SHIFT},  /* J */
    {0x0e, KEY_SHIFT},  /* K */
    {0x0f, KEY_SHIFT},  /* L */
    {0x10, KEY_SHIFT},  /* M */
    {0x11, KEY_SHIFT},  /* N */
    {0x12, KEY_SHIFT},  /* O */
    {0x13, KEY_SHIFT},  /* P */
    {0x14, KEY_SHIFT},  /* Q */
    {0x15, KEY_SHIFT},  /* R */
    {0x16, KEY_SHIFT},  /* S */
    {0x17, KEY_SHIFT},  /* T */
    {0x18, KEY_SHIFT},  /* U */
    {0x19, KEY_SHIFT},  /* V */
    {0x1a, KEY_SHIFT},  /* W */
    {0x1b, KEY_SHIFT},  /* X */
    {0x1d, KEY_SHIFT},  /* Y */
    {0x1c, KEY_SHIFT},  /* Z */
    {0x25, KEY_ALTGR},  /* [ */
    {0x2d, KEY_ALTGR},  /* \ */
    {0x26, KEY_ALTGR},  /* ] */
    {0x00, 0},          /* ^ */
    {0x38, KEY_SHIFT},  /* _ */
    {0x00, 0},          /* ` */
    {0x04, 0},          /* a */
    {0x05, 0},          /* b */
    {0x06, 0},          /* c */
    {0x07, 0},          /* d */
    {0x08, 0},          /* e */
    {0x09, 0},          /* f */
    {0x0a, 0},          /* g */
    {0x0b, 0},          /* h */
    {0x0c, 0},          /* i */
    {0x0d, 0},          /* j */
    {0x0e, 0},          /* k */
    {0x0f, 0},          /* l */
    {0x10, 0},          /* m */
    {0x11, 0},          /* n */
    {0x12, 0},          /* o */
    {0x13, 0},          /* p */
    {0x14, 0},          /* q */
    {0x15, 0},          /* r */
    {0x16, 0},          /* s */
    {0x17, 0},          /* t */
    {0x18, 0},          /* u */
    {0x19, 0},          /* v */
    {0x1a, 0},          /* w */
    {0x1b, 0},          /* x */
    {0x1d, 0},          /* y */
    {0x1c, 0},          /* z */
    {0x24, KEY_ALTGR},  /* { */
    {0x64, KEY_ALTGR},  /* | */
    {0x27, KEY_ALTGR},  /* } */
    {0x30, KEY_ALTGR},  /* ~ */
    {0,0},              /* DEL */

    {0x3a, 0},          /* F1 */
    {0x3b, 0},          /* F2 */
    {0x3c, 0},          /* F3 */
    {0x3d, 0},          /* F4 */
    {0x3e, 0},          /* F5 */
    {0x3f, 0},          /* F6 */
    {0x40, 0},          /* F7 */
    {0x41, 0},          /* F8 */
    {0x42, 0},          /* F9 */
    {0x43, 0},          /* F10 */
    {0x44, 0},          /* F11 */
    {0x45, 0},          /* F12 */

    {0x46, 0},          /* PRINT_SCREEN */
    {0x47, 0},          /* SCROLL_LOCK */
    {0x39, 0},          /* CAPS_LOCK */
    {0x53, 0},          /* NUM_LOCK */
    {0x49, 0},          /* INSERT */
    {0x4a, 0},          /* HOME */
    {0x4b, 0},          /* PAGE_UP */
    {0x4e, 0},          /* PAGE_DOWN */

    {0x4f, 0},          /* RIGHT_ARROW */
    {0x50, 0},          /* LEFT_ARROW */
    {0x51, 0},          /* DOWN_ARROW */
    {0x52, 0},          /* UP_ARROW */
};
