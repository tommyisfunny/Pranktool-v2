#include <DuckyScript.h>
#include <debugOut.h>

FileHelper fileHelper;



DuckyScript::DuckyScript(){
    this->standartDelay = 1;
}

void DuckyScript::begin(FS &fs){
    this->fs = &fs;
}

void DuckyScript::setStandartDelay(int delay){
    this->standartDelay = delay;
}

void DuckyScript::run(String path){
    debugOutln("Running " + path);
    File file = fs->open(path, "r");
    String line = "";
    char _char = ' ';
    while(file.available()){
        _char = file.read();
        line += _char;
        if(_char != '\n') continue;
        this->parseLine(line);
        line = "";
    }
    if(line != "") this->parseLine(line);

    file.close();
}

void DuckyScript::parseLine(String line){
    line.trim();
    u_int8_t i = line.indexOf(' ');
    String command = "";
    String arg = " ";

    if(i == -1){
        command = line;
    } else {
        command = line.substring(0, i);

        arg = line.substring(i);
        arg.trim();
    }

    String lowerCmd = command;
    lowerCmd.toLowerCase();

    debugOutln("Command: " + command);
    debugOutln("Arg: " + arg);

    if     (lowerCmd == "delay") this->_delay(arg);
    else if(lowerCmd == "string") this->string(arg);
    else if(lowerCmd == "led") setLED(arg);
    else if(lowerCmd == "paste") pasteFile(arg);
    else {
        int keycode = findSpecialKey(command); 
        if(keycode != -1){
            this->specialKey(keycode, arg);
        } else {
            if(settings["LEDSENABLED"]) digitalWrite(L_ERR, HIGH);
            debugOutln("Command: " + command);
            debugOutln("Arg: " + arg);
            debugOutln("Not Found");
        }
    }
}

void DuckyScript::_delay(String arg){
    delay(arg.toInt() * this->standartDelay);
}

void DuckyScript::string(String arg){
    keyboard.sendString(arg);
    delay(standartDelay * 10);
}

void DuckyScript::setLED(String arg){
    arg.toLowerCase();
    if(arg == "on") digitalWrite(L_USER, HIGH);
    if(arg == "off") digitalWrite(L_USER, LOW);
    if(arg == "toggle") digitalWrite(L_USER, !digitalRead(L_USER));
}

void DuckyScript::pasteFile(String arg){
    File file = fs->open("/payloads/" + arg);
    if(!file) return;
    String cnt = file.readString();
    file.close();
    keyboard.sendString(cnt);
}

void DuckyScript::specialKey(int keycode, String arg){
    //debugOut("Arg: \"");debugOut(arg);debugOutln("\"");
    //debugOut("specialKey: (");debugOut(keycode);

    keyboard.pressKey(keycode);
    String additonalKey = "";
    arg += " ";
    for(int i = 0; i < arg.length(); i++){
        if(arg[i] != ' '){
            additonalKey += arg[i];
        } else {
            int keycodeArg = this->findSpecialKey(additonalKey);
            if(keycodeArg == -1) keycodeArg = keymap[additonalKey[0]].usage;
            keyboard.pressKey(keycodeArg);

            //debugOut(") arg: (");debugOut(additonalKey);debugOut("|");debugOut(keycodeArg);
            additonalKey = "";
        }
    }

    //debugOutln(")");

    delay(standartDelay * 10);
    keyboard.releaseAll();
    delay(standartDelay * 10);
}

int DuckyScript::findSpecialKey(String key){
    for(int i = 0; i < sizeof(specialKeys)/sizeof(specialKeys[0]); i++){
        if(key == specialKeys[i].key) return specialKeys[i].keycode;
    }
    return -1;
}

DuckyScript duckyScript;