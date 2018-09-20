/*
 * Button.h
 *
 *  Created on: 17 wrz 2018
 *      Author: Andrzej
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <stdint.h>
#include "esp_system.h"

typedef enum
{
  ButtonEv_None,
  ButtonEv_ShortPress,
  ButtonEv_LongPress
} ButtonEvType_t;

class Button
{

public:
  Button(gpio_num_t i_pin);
  virtual ~Button();

  ButtonEvType_t Get();

  void DoWork();
};

#endif /* BUTTON_H_ */
