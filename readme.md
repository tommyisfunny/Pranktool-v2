# Pranktool
## Introduction
Insert yepping here

## Hardware
The Pranktool uses a ESP32-S2 module on a custom PCB.
It has 4 LEDs and 8 buttons:

### LEDS
| Name | Color | Purpose |
|----|----|----|
| USER | Orange | Controlable by the user |
| OK | Green | Indicates that a script is running
| WLAN | Blue | Indicates if WLAN is active|
| ERR | Red | Indikates an error while executing a payload |

### Buttons
| Name | Purpose |
|----|----|
| RST | Restarts the Pranktool |
| BOOT | Is used to upload new firmware through the serial interface. Normaly not used. |
| SETUP | Activates / Deactivates WLAN |
| DARUN | When pressed disables automatic payload execution after the pranktool is pluged in |
| B1 - B4 | Executes the corresponding payload as configured in the settings |

## Software
### How to access the configuration website
1. Insert Pranktool
2. Press the **SETUP** button if **WLAN on boot** isn't enabled
3. The blue LED should ligth up if **Enable LEDs** is enabled
4. Connect to the WLAN Network. Default name is "*Pranktool*"
5. Enter the Pranktools IP-address *192.168.4.1* into your browser. There can be problems accessing the website if your device is also connected to a difrent network, for example through an ethernet cable

### Settings
The settings can be saved clicking the "*Save settings*" Button.
| Setting | Description |
|----|----|
| Autostart | Selects a payload to be automaticly executed after the Pranktool is inserted |
| Button 1 - 4 | Selects a payload to be executed if one of the corresponding Buttons is pressed |
| Delay multiplier | Multiplies all delays in payloads with the given factor. Usefull for slow computers |
| WLAN name | The SSID of the WLAN network. Applies after reactivating WLAN. |
| WLAN password | The password of the WLAN network. At least 8 letters. Leave blank for an open network. Applies after reactivating WLAN. |
| Hide WLAN | Specifies if the WLAN network shoul be hidden. Applies after reactivating WLAN. |
| WLAN on boot | When enabled automaticly activates WLAN after power up |
| Enable LEDS | LEDs wont light up when disabled. Usefull for more stealthy operation |

lol