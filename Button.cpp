/*
 * Button.cpp
 *
 *  Created on: 17 wrz 2018
 *      Author: Andrzej
 */

#include "Button.h"
#include "Arduino.h"
#include "systime.h"

#define BTN_STATE_WAIT_FOR_SHORT_PRESS  0
#define BTN_STATE_SHORT_PRESS_DB        1
#define BTN_STATE_WAIT_FOR_SHORT_PRESS_FINISH 2
#define BTN_STATE_WAIT_FOR_LONG_PRESS_FINISH  3
#define BTN_STATE_WAIT_FOR_RELEASE 4
#define BTN_STATE_RELEASE_DB 5

#define BTN_DB_LEN 50
#define BTN_SHORT_PRESS_TIMEOUT 500
#define BTN_LONG_PRESS_TIMEOUT  1000

Button::Button(gpio_num_t i_pin) : m_state(BTN_STATE_WAIT_FOR_SHORT_PRESS),
  m_currEv(ButtonEv_None), m_lastTime(0), m_pin(i_pin)
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
  ButtonEvType_t ret = m_currEv;

  if (ret != ButtonEv_None)
  {
    m_currEv = ButtonEv_None;
  }
  return ret;
}

void Button::DoWork()
{
  int level = gpio_get_level(m_pin);
  SysTime_t t = SysTimeGetCurent();

  switch(m_state)
  {
    default:
    case BTN_STATE_WAIT_FOR_SHORT_PRESS:
      if (level == 0)
      {
        m_state = BTN_STATE_SHORT_PRESS_DB;
        m_lastTime = t;
      }
      break;
    case BTN_STATE_SHORT_PRESS_DB:
      if (SysTimeCompare(t, m_lastTime + BTN_DB_LEN) > 0)
      {
        m_state = BTN_STATE_WAIT_FOR_SHORT_PRESS_FINISH;
        m_lastTime = t;
      }
      break;
    case BTN_STATE_WAIT_FOR_SHORT_PRESS_FINISH:
      if (level == 0)
      {
        if (SysTimeCompare(t, m_lastTime + BTN_SHORT_PRESS_TIMEOUT) > 0)
        {
          m_state = BTN_STATE_WAIT_FOR_LONG_PRESS_FINISH;
        }
      }
      else
      {
        m_currEv = ButtonEv_ShortPress;
        m_state = BTN_STATE_RELEASE_DB;
        m_lastTime = t;
      }
      break;
    case BTN_STATE_WAIT_FOR_LONG_PRESS_FINISH:
      if (level == 0)
      {
        if (SysTimeCompare(t, m_lastTime + BTN_LONG_PRESS_TIMEOUT) > 0)
        {
          m_state = BTN_STATE_WAIT_FOR_RELEASE;
          m_currEv = ButtonEv_LongPress;
        }
      }
      else
      {
        m_currEv = ButtonEv_ShortPress;
        m_state = BTN_STATE_RELEASE_DB;
        m_lastTime = t;
      }
      break;
    case BTN_STATE_WAIT_FOR_RELEASE:
      if (level != 0)
      {
        m_state = BTN_STATE_RELEASE_DB;
        m_lastTime = t;
      }
      break;
    case BTN_STATE_RELEASE_DB:
      if (SysTimeCompare(t, m_lastTime + BTN_DB_LEN) > 0)
      {
        m_state = BTN_STATE_WAIT_FOR_SHORT_PRESS;
      }
      break;
  }
}
