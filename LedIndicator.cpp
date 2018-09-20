/*
 * LedIndicator.cpp
 *
 *  Created on: 17 wrz 2018
 *      Author: Andrzej
 */

#include "LedIndicator.h"
#include "Arduino.h"

#define LED_ON_STATE    0
#define LED_OFF_STATE   (1u)
#define LED_BLINK_FAST  (250u)
#define LED_BLINK_SLOW  (750u)

LedIndicator::LedIndicator(gpio_num_t i_pin) : m_pin(i_pin), m_mode(LedIndicatorType_Off), m_timeToBlink(0)
{
  // TODO Auto-generated constructor stub
  gpio_config_t cfg;

  cfg.mode = GPIO_MODE_OUTPUT;
  cfg.pin_bit_mask = bit(i_pin);
  cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
  cfg.pull_up_en = GPIO_PULLUP_DISABLE;
  cfg.intr_type = GPIO_INTR_DISABLE;
  gpio_config(&cfg);

  setLedState(0);
}

LedIndicator::~LedIndicator()
{
  setLedState(0);
}

void LedIndicator::setLedState(int state)
{
  if (state == 0)
  {
    gpio_set_level(m_pin, LED_OFF_STATE);
    m_state = 0;
  }
  else
  {
    gpio_set_level(m_pin, LED_ON_STATE);
    m_state = 1;
  }
}

void LedIndicator::Set(LedIndicatorType_t i_mode)
{
  m_mode = i_mode;
  switch(i_mode)
  {
    case   LedIndicatorType_Off:
      setLedState(0);
      break;
    case LedIndicatorType_BlinkSlow:
      break;
    case LedIndicatorType_BlinkFast:
      break;
    case LedIndicatorType_On:
      setLedState(1);
      break;
    case LedIndicatorType_Smooth:
      break;
  }
}

#define IS_LED_TIMEOUT(t, p)    ((t) > (m_timeToBlink + (p))) || ((m_timeToBlink - (t)) > (2*(p)))

void LedIndicator::DoWork()
{
  if (m_mode == LedIndicatorType_BlinkSlow)
  {
    uint32_t currTime = millis();

    if (IS_LED_TIMEOUT(currTime, LED_BLINK_SLOW))
    {
      m_timeToBlink = currTime + LED_BLINK_SLOW;
      setLedState(m_state == 0 ? 1 : 0);
    }
  }
  else if (m_mode == LedIndicatorType_BlinkFast)
  {
    uint32_t currTime = millis();

    if (IS_LED_TIMEOUT(currTime, LED_BLINK_FAST))
    {
      m_timeToBlink = currTime + LED_BLINK_FAST;
      setLedState(m_state == 0 ? 1 : 0);
    }
  }
}
