
#pragma once
#include "hidusb.h"
#include "hidkeylayoutX.h"


#if CFG_TUD_HID

#include "cdcusb.h"
extern CDCusb CDCUSBSerial;

class HIDkeyboardX : public HIDusb
{
public:
    HIDkeyboardX(uint8_t id = 3);
    bool begin(char* str = nullptr);

    bool sendKey(uint8_t _keycode, uint8_t modifier = 0);
    bool sendChar(uint8_t _keycode);
    bool sendPress(uint8_t _keycode, uint8_t modifier = 0);
    bool sendRelease();
    bool sendString(const char* text);
    bool sendString(String text);

    bool pressKey(uint8_t _keycode);
    bool pressChar(uint8_t _keycode);
    bool releaseAll();
private:
    u_int8_t pressedKeys[6];
    u_int8_t pressedKeysIndex;
};

extern HIDkeyboardX keyboard;

#endif
