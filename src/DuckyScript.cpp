#include <DuckyScript.h>


FileHelper fileHelper;



DuckyScript::DuckyScript(){
    this->standartDelay = 1;
}

void DuckyScript::begin(){
    
}

void DuckyScript::setStandartDelay(int delay){
    this->standartDelay = delay;
}

void DuckyScript::run(String path){
    //debugOutln("Running " + path);
    File fs = SPIFFS.open(path);
    String line = "";
    char _char = ' ';
    while(fs.available()){
        _char = fs.read();
        line += _char;
        if(_char != '\n') continue;
        this->parseLine(line);
        line = "";
    }
    if(line != "") this->parseLine(line);

    fs.close();
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

    //Serial.println("Command: " + command);
    //Serial.println("Arg: " + arg);

    if     (lowerCmd == "delay") this->_delay(arg);
    else if(lowerCmd == "string") this->string(arg);
    else {
        int keycode = findSpecialKey(command); 
        if(keycode != -1){
            this->specialKey(keycode, arg);
        } else {
            //debugOutln("Command: " + command);
            //debugOutln("Arg: " + arg);
            //debugOutln("Not Found");
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

void DuckyScript::specialKey(int keycode, String arg){
    Serial.print("specialKey: (");Serial.print(keycode, HEX);

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

            Serial.print(") arg: (");Serial.print(additonalKey);Serial.print("|");Serial.print(keycodeArg, HEX);
            additonalKey = "";
        }
    }

    Serial.println(")");

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