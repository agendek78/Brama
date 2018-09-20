/*
 * Button.cpp
 *
 *  Created on: 17 wrz 2018
 *      Author: Andrzej
 */

#include "Button.h"
#include "Arduino.h"

Button::Button(gpio_num_t i_pin)
{
  gpio_config_t cfg;

  cfg.mode = GPIO_MODE_INPUT;
  cfg.pin_bit_mask = bit(i_pin);
  cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
  cfg.pull_up_en = GPIO_PULLUP_ENABLE;
  cfg.intr_type = GPIO_INTR_DISABLE;
  gpio_config(&cfg);
}

Button::~Button()
{
}

ButtonEvType_t Button::Get()
{
  return ButtonEv_None;
}

void Button::DoWork()
{

}
