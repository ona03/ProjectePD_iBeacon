#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <BLEBeacon.h>

#define DEVICE_NAME_1            "ESP32_1"
#define SERVICE_UUID_1           "7A0247E7-8E88-409B-A959-AB5092DDB03E"
#define BEACON_UUID_1            "2D7A9F0C-E0E8-4CC9-A71B-A21DB2D034A1"
#define BEACON_UUID_REV_1        "A134D0B2-1DA2-1BA7-C94C-E8E00C9F7A2D"
#define CHARACTERISTIC_UUID_1    "82258BAA-DF72-47E8-99BC-B73D7ECD08A5"
#define MAJOR_1                  1001
#define MINOR_1                  2001

#define DEVICE_NAME_2            "ESP32_2"
#define SERVICE_UUID_2           "A1B2C3D4-E5F6-1234-5678-ABCDEFABCDEF"
#define BEACON_UUID_2            "A1B2C3D4-E5F6-1234-5678-ABCDEFABCDEF"
#define BEACON_UUID_REV_2        "FEDCBA9876543210-FEDC-BA98-7654-3210FEDCBA98"
#define CHARACTERISTIC_UUID_2    "A1B2C3D4-E5F6-1234-5678-ABCDEFABCDEF"
#define MAJOR_2                  1002
#define MINOR_2                  2002

BLEServer *pServer_1;
BLECharacteristic *pCharacteristic_1;
bool deviceConnected_1 = false;

BLEServer *pServer_2;
BLECharacteristic *pCharacteristic_2;
bool deviceConnected_2 = false;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      if (pServer == pServer_1) {
        deviceConnected_1 = true;
        Serial.println("deviceConnected_1 = true");
      }
      else if (pServer == pServer_2) {
        deviceConnected_2 = true;
        Serial.println("deviceConnected_2 = true");
      }
    }

    void onDisconnect(BLEServer* pServer) {
      if (pServer == pServer_1) {
        deviceConnected_1 = false;
        Serial.println("deviceConnected_1 = false");
      }
      else if (pServer == pServer_2) {
        deviceConnected_2 = false;
        Serial.println("deviceConnected_2 = false");
      }

      // Reiniciar la publicidad para que vuelva a ser visible y conectable
      pServer->getAdvertising()->start();
      Serial.println("iBeacon advertising restarted");
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();

      if (rxValue.length() > 0) {
        Serial.println("*********");
        Serial.print("Received Value: ");
        for (int i = 0; i < rxValue.length(); i++) {
          Serial.print(rxValue[i]);
        }
        Serial.println();
        Serial.println("*********");
      }
    }
};

void initService(BLECharacteristic *pCharacteristic, BLEServer *pServer, const char *serviceUUID, const char *characteristicUUID) {
  BLEService *pService = pServer->createService(BLEUUID(serviceUUID));

  pCharacteristic = pService->createCharacteristic(
                    BLEUUID(characteristicUUID),
                    BLECharacteristic::PROPERTY_READ   |
                    BLECharacteristic::PROPERTY_WRITE  |
                    BLECharacteristic::PROPERTY_NOTIFY
                  );
  pCharacteristic->setCallbacks(new MyCallbacks());
  pCharacteristic->addDescriptor(new BLE2902());

  pServer->getAdvertising()->addServiceUUID(BLEUUID(serviceUUID));

  pService->start();
}

void initBeacon(BLEServer *pServer, const char *beaconUUIDRev, uint16_t MId, uint8_t Flag) {
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->stop();

  BLEBeacon myBeacon;
  myBeacon.setManufacturerId(MId);
  //myBeacon.setMajor(1001);
  //myBeacon.setMinor(2001);
  myBeacon.setSignalPower(0xc5);
  myBeacon.setProximityUUID(BLEUUID(beaconUUIDRev));

  BLEAdvertisementData advertisementData;
  advertisementData.setFlags(Flag);
  advertisementData.setManufacturerData(myBeacon.getData());
  pAdvertising->setAdvertisementData(advertisementData);

  pAdvertising->start();
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Initializing...");
  Serial.flush();
  Serial.println("pas 1");

  BLEDevice::init(DEVICE_NAME_1);
  pServer_1 = BLEDevice::createServer();
  pServer_1->setCallbacks(new MyServerCallbacks());

  Serial.println("pas2");

  BLEDevice::init(DEVICE_NAME_2);
  pServer_2 = BLEDevice::createServer();
  pServer_2->setCallbacks(new MyServerCallbacks());

  Serial.println("pas2");

  uint16_t MId1=0x1111, MId2=0x2222;
  uint8_t Flag1=0x1A, Flag2=0x1F;

  initService(pCharacteristic_1, pServer_1, SERVICE_UUID_1, CHARACTERISTIC_UUID_1);
  Serial.println("service1 started");
  initService(pCharacteristic_2, pServer_2, SERVICE_UUID_2, CHARACTERISTIC_UUID_2);

  Serial.println("service2 started");

  initBeacon(pServer_1, BEACON_UUID_REV_1, MId1, Flag1);
  Serial.println("iBeacon1 funciona");
  initBeacon(pServer_2, BEACON_UUID_REV_2, MId2, Flag2);

  Serial.println("iBeacon2 funciona");
}

void loop() {
  if (deviceConnected_1) {
    //Serial.printf("*** NOTIFY: %d ***\n", value);
    pCharacteristic_1->setValue("dispositiu1");
    //pCharacteristic->notify();
    //value++;
  }
  if (deviceConnected_2) {
    //Serial.printf("*** NOTIFY: %d ***\n", value);
    pCharacteristic_2->setValue("dispositiu2");
    //pCharacteristic->notify();
    //value++;
  }
  delay(2000);
}