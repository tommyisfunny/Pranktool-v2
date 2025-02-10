#include <DuckyScript.h>
#include <debugOut.h>

FileHelper fileHelper;



DuckyScript::DuckyScript(){
    standartDelay = 1;
    currentPayload = "";
    repitions = 1;
}

void DuckyScript::begin(FS &fs, const char* payloadDir){
    this->fs = &fs;
    this->payloadDir = payloadDir;
}

void DuckyScript::setStandartDelay(int delay){
    standartDelay = delay;
}

void DuckyScript::run(String payload){
    String fullPath = payloadDir + "/" + payload + "/" + payload + ".dd";
    debugOutln("Running " + fullPath);
    currentPayload = payload;

    repitions = 1;

    File file = fs->open(fullPath, "r");
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

        arg = line.substring(i + 1);
    }

    String lowerCmd = command;
    lowerCmd.toLowerCase();

    //debugOutln("Command: " + command);
    //debugOutln("Arg: " + arg);

    bool repeat = false;

    for(; repitions != 0; repitions--){
        if     (lowerCmd == "delay") this->_delay(arg);
        else if(lowerCmd == "string") this->string(arg);
        else if(lowerCmd == "stringln") this->string(arg + "\n") ;
        else if(lowerCmd == "led") setLED(arg);
        else if(lowerCmd == "paste") pasteFile(arg);
        else if(lowerCmd == "rem") /* Nothing */;
        else if(lowerCmd == "repeat") {
            repitions = arg.toInt();
            repeat = true;
            break;
        }
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

    if(!repeat) repitions = 1;
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
    if(arg == "on"){
        digitalWrite(L_USER, HIGH);
        debugOutln("User LED ON");
    } 
    if(arg == "off"){
        digitalWrite(L_USER, LOW);
        debugOutln("User LED OFF");
    } 
    if(arg == "toggle"){
        digitalWrite(L_USER, !digitalRead(L_USER));
        debugOutln("User LED toggle");
    } 
}

void DuckyScript::pasteFile(String arg){
    File file = fs->open(payloadDir + "/" + currentPayload + "/" + arg);
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