/*
 * BramaAuthClient.cpp
 *
 *  Created on: 20 lip 2018
 *      Author: Andrzej
 */

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "BramaAuthClient.h"
#include "Arduino.h"
#include "esp_system.h"
#include "aes.h"

#define BLE_BRAMA_PKT_LEN   ((size_t)20)
#define SERVICE_UUID        "84bb818a-27a1-4b33-a956-cfaf6a918571"
#define CHARACTERISTIC_UUID "da549571-29b9-40fc-b724-afa78b26e34d"

#define PSK                 "7fbaea34992446709345b0bef4c666db"

static bool clientConnected = false;
static uint32_t clientUID = 0;

ConnectionCBs connCb;
CharacteristicCbs charCb;
BLEService *pService;
BLECharacteristic *pCharacteristic;
BramaToken_t currToken;

void ConnectionCBs::onConnect(BLEServer* pServer)
{
  Serial.println ("New client connection received!");
  if (clientConnected == false)
  {
    clientConnected = true;
    BramaAuthClient::SetNewToken ();
    pCharacteristic->indicate ();
  }
  else
  {
    Serial.println ("Another connection in progress! Dropping new connection!");
  }
}

void ConnectionCBs::onDisconnect(BLEServer* pServer)
{
  Serial.println ("Client disconnected!");

  BramaAuthClient::SetNewToken ();
  clientConnected = false;
}

void CharacteristicCbs::onRead(BLECharacteristic* pCharacteristic)
{

}

void CharacteristicCbs::onWrite(BLECharacteristic* pCharacteristic)
{
  BLEValue val = pCharacteristic->getBLEValue ();

  Serial.print ("Data written:");

  if (val.getLength () != BLE_BRAMA_PKT_LEN)
  {
    Serial.print ("invalid pkt len!");
  }
  else
  {
    BramaPacket_t pkt;

    memcpy(&pkt, val.getData(), sizeof(pkt));

    switch (pkt.cmd)
    {
      case PacketFlight_Rand1:
        //unexpected
        break;
      case PacketFlight_Rand12:
        memcpy(currToken, pkt.token, sizeof(currToken));
        break;
      case PacketFlight_AuthHello:
        break;
      case PacketFlight_AuthHelloRsp:
        break;
      case PacketFlight_Cmd:
        break;
      default:
        break;
    }
  }
}

void BramaAuthClient::generateToken(BramaToken_t token)
{
  uint32_t rand_val;

  rand_val = esp_random ();
  memcpy (token, &rand_val, sizeof(rand_val));

  rand_val = esp_random ();
  memcpy (&token[4], &rand_val, sizeof(rand_val));

  rand_val = esp_random ();
  memcpy (&token[8], &rand_val, sizeof(rand_val));

  rand_val = esp_random ();
  memcpy (&token[12], &rand_val, sizeof(rand_val));
}

BramaAuthClient::BramaAuthClient() :
    m_state (PacketFlight_Rand1)
{
  BLEServer *pServer = BLEDevice::createServer ();
  pServer->setCallbacks (&connCb);

  pService = pServer->createService (SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic (
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
          | BLECharacteristic::PROPERTY_INDICATE);

  SetNewToken ();
  pService->start ();

  BLEAdvertising *pAdvertising = pServer->getAdvertising ();
  pAdvertising->start ();
}

BramaAuthClient::~BramaAuthClient()
{
}

void BramaAuthClient::SetNewToken()
{
  BramaAuthClient::generateToken (currToken);
  if (pCharacteristic != NULL)
  {
    BramaPacket_t pkt;

    *(uint32_t*)&pkt = esp_random ();
    pkt.cmd = PacketFlight_Rand1;
    memcpy(&pkt.token, currToken, sizeof(currToken));
    pCharacteristic->setValue((unsigned char *) &pkt, sizeof(BramaPacket_t));
  }
}

void BramaAuthClient::indicate()
{
  if (clientConnected)
  {
    SetNewToken ();

    pCharacteristic->indicate ();
  }
}

