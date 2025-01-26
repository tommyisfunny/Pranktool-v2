#pragma once

enum Buttons{
  B_1 =     GPIO_NUM_1,
  B_2 =     GPIO_NUM_2,
  B_3 =     GPIO_NUM_3,
  B_4 =     GPIO_NUM_4,
  B_DARUN = GPIO_NUM_7,
  B_SETUP = GPIO_NUM_8
};

enum LEDs{
  L_ERR =  GPIO_NUM_11,
  L_WLAN = GPIO_NUM_12,
  L_OK =   GPIO_NUM_13,
  L_USER = GPIO_NUM_14
};