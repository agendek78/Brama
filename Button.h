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
#include "systime.h"

typedef enum
{
  ButtonEv_None,
  ButtonEv_ShortPress,
  ButtonEv_LongPress
} ButtonEvType_t;

class Button
{
  uint8_t        m_state;
  ButtonEvType_t m_currEv;
  SysTime_t      m_lastTime;
  gpio_num_t     m_pin;

public:
  Button(gpio_num_t i_pin);
  virtual ~Button();

  ButtonEvType_t Get();

  void DoWork();
};

#endif /* BUTTON_H_ */
