#pragma once
#include <Arduino.h>

#include <FileHelper.h>
#include <FS.h>
#include <hidkeyboardX.h>
#include <DuckyScriptSpecialKeys.h>
#include <hardwaredefs.h>
#include <settings.h>

class DuckyScript{
    public:
        DuckyScript();
        void begin(FS &fs);
        void run(String path);
        void setStandartDelay(int delay);
        u_int16_t standartDelay;

    private:
        FS *fs;
        void parseLine(String line);
        
        void _delay(String arg);
        void enter();
        void gui(char arg);
        void ctrl(char arg);
        void alt(char arg);
        void altgr(char arg);
        void specialKey(int keycode, String arg);
        void string(String arg);
        int findSpecialKey(String key);
        void setLED(String arg);
        void pasteFile(String arg);
};

extern DuckyScript duckyScript;
extern bool ledsenabled;