/*
 * LedIndicator.h
 *
 *  Created on: 17 wrz 2018
 *      Author: Andrzej
 */

#ifndef LEDINDICATOR_H_
#define LEDINDICATOR_H_

#include <stdint.h>
#include "esp_system.h"

typedef enum
{
  LedIndicatorType_Off,
  LedIndicatorType_BlinkSlow,
  LedIndicatorType_BlinkFast,
  LedIndicatorType_On,
  LedIndicatorType_Smooth
} LedIndicatorType_t;

class LedIndicator
{
  gpio_num_t m_pin;
  LedIndicatorType_t m_mode;
  uint32_t m_timeToBlink;
  int m_state;

  void setLedState(int state);

public:
  LedIndicator(gpio_num_t i_pin);
  virtual ~LedIndicator();

  void Set(LedIndicatorType_t i_mode);
  void DoWork();
};

#endif /* LEDINDICATOR_H_ */
