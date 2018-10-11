/*
 * BramaAuthClient.h
 *
 *  Created on: 20 lip 2018
 *      Author: Andrzej
 */

#ifndef BRAMAAUTHCLIENT_H_
#define BRAMAAUTHCLIENT_H_

typedef enum
{
  PacketFlight_Rand1,
  PacketFlight_Rand12,
  PacketFlight_AuthHello,
  PacketFlight_AuthHelloRsp,
  PacketFlight_Cmd
} PacketFlight_t;

#define BRAMA_TOKEN_LEN 16

typedef uint8_t BramaToken_t[BRAMA_TOKEN_LEN];

typedef struct __attribute__((packed))
{
  uint8_t       cmd;
  uint8_t       unused[3];
  BramaToken_t  token;
} BramaPacket_t;

class ConnectionCBs : public BLEServerCallbacks
{
public:
  void onConnect (BLEServer* pServer);
  void onDisconnect (BLEServer* pServer);
};

class CharacteristicCbs : public BLECharacteristicCallbacks
{
public:
  void onRead (BLECharacteristic* pCharacteristic);
  void onWrite (BLECharacteristic* pCharacteristic);
};

class BramaAuthClient
{
  PacketFlight_t        m_state;

  static void generateToken(BramaToken_t token);

  friend class ConnectionCBs;

public:
  BramaAuthClient ();
  virtual ~BramaAuthClient ();

  static void SetNewToken();
  void indicate();
};

#endif /* BRAMAAUTHCLIENT_H_ */
