#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "esp_system.h"
#include "BramaAuthClient.h"
#include "LedIndicator.h"

#define OUT1_PIN GPIO_NUM_17
#define OUT2_PIN GPIO_NUM_16
#define LED_PIN  GPIO_NUM_4

BramaAuthClient *BLEAuthServer;
LedIndicator    *led;

void setup ()
{
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  BLEDevice::init("Brama");
  BLEAuthServer = new BramaAuthClient();

  {
    gpio_config_t cfg;

    cfg.mode = GPIO_MODE_OUTPUT;
    cfg.pin_bit_mask = bit(OUT1_PIN) | bit(OUT2_PIN);
    cfg.pull_down_en = GPIO_PULLDOWN_ENABLE;
    cfg.pull_up_en = GPIO_PULLUP_DISABLE;
    cfg.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&cfg);

    gpio_set_level(OUT1_PIN, 1);
  }

  led = new LedIndicator(LED_PIN);
  led->Set(LedIndicatorType_BlinkSlow);
}

void loop ()
{
  // put your main code here, to run repeatedly:
  //Serial.println (esp_random ());
  //delay (2000);
  //BLEAuthServer->indicate();
  led->DoWork();
}
