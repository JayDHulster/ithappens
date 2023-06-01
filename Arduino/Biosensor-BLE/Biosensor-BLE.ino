#include <ArduinoBLE.h>


BLEService pressureService("2781"); // BLE LED Service

// BLE Pressure Characteristic - custom 128-bit UUID, read and writable by central
BLEIntCharacteristic pressurizer("2A57", BLERead | BLENotify); // send data as integer
// BLEFloatCharacteristic pressurizer("2A57", BLERead | BLENotify); // send data as float !!! does not work with app
// BLEStringCharacteristic pressurizer("2A57", BLERead | BLENotify, 6); // send data as a string

short pressZero = 412;        //put the highest raw reading you see here at atmospheric pressure
short pressMax = 30;         //reduce pressure rating by %66 if 3.3v logic but still use 5v input
float resolution = 3686.4; //921.6;      //3686.4 for Due/Zero and add analogReadResolution(12) to void setup
byte analogPin = A1;           //transducer pin number
boolean autoCalibrate = false; //will automatically select highest raw reading in 10sec period

int timeDelay = 2500; //Delay waarin sensor opmeet (en dus ook data uitstuurt)

// https://forum.arduino.cc/t/pressure-transducer-code/362754/36
///////////////////////////////////////////////////////////////////////////////////////////////////////
byte pressStep;
short rawRead[201];
unsigned long timer;
byte calibrate;
boolean autoCalibrateComplete = false;
short pressZeroTemp;
String buf;

void setup()
{
  buf.reserve(40);
  analogReadResolution(12);
  Serial.begin(9600);
  while (!Serial);
  transProp();
    // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy failed!");

    while (1);
  }

    // set advertised local name and service UUID:
  BLE.setLocalName("ItHappens™ Biosense");
  BLE.setAdvertisedService(pressureService);

  // add the characteristic to the service
  pressureService.addCharacteristic(pressurizer);

  // add service
  BLE.addService(pressureService);

  // set the initial value for the characteristic:
  pressurizer.writeValue(0);
  // pressurizer.writeValue("Start"); // string

  // start advertising
  BLE.advertise();

  Serial.println("BLE Pressure Peripheral");
  Serial.println(BLE.address());
}

void loop()
{

    // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

    // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
        if (millis() - timer >= timeDelay)
  {
    float pressPsi;
    unsigned long rawTemp = 0;
    short raw;
    for (byte x = 0; x < 200; x++) rawTemp += rawRead[x];
    raw = rawTemp / 200;
    unsigned short calc1 = raw - pressZero;
    unsigned long calc2 = calc1 * pressMax;
    float calc3 = resolution - float(pressZero);
    if (raw <= pressZero) pressPsi = 0.0;
    else pressPsi = float(calc2) / float(calc3);
    float pressMmhg = pressPsi * 51.7149;
    float pressMeter = pressPsi * .7030695782; // psi to meters of water head
    buf = F("Raw: ");
    buf += raw;
    buf += F(" psi: ");
    buf += pressPsi;
    buf += F(" mmHg: ");
    buf += pressMmhg;
    buf += F(" meters: ");
    buf += pressMeter;
    Serial.println(buf); 
    pressurizer.writeValue(int(pressMmhg*100)); // send raw data  
    // pressurizer.writeValue(String(pressMmhg, 2)); // Stuur data via bluetooth
    if (autoCalibrate == true && calibrate >= 10)
    {
      if (autoCalibrateComplete == false)
      {
        pressZero = pressZeroTemp;
        buf = F("\n\nsensor re-calibrated to : ");
        buf += pressZero;
        buf += F(" (raw)\n");
        Serial.println(buf);
        transProp();
        delay(3*timeDelay);
        autoCalibrateComplete = true;
      }
    }
    else
    {
      if (raw > pressZeroTemp) pressZeroTemp = raw;
      calibrate++;
    }
    timer = millis();
  }
  if (pressStep < 200) pressStep++;
  else pressStep = 0;
  rawRead[pressStep] = analogRead(analogPin);
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  } 
  else 
  {
  if (millis() - timer >= timeDelay)
    {
      float pressPsi;
      unsigned long rawTemp = 0;
      short raw;
      for (byte x = 0; x < 200; x++) rawTemp += rawRead[x];
      raw = rawTemp / 200;
      unsigned short calc1 = raw - pressZero;
      unsigned long calc2 = calc1 * pressMax;
      float calc3 = resolution - float(pressZero);
      if (raw <= pressZero) pressPsi = 0.0;
      else pressPsi = float(calc2) / float(calc3);
      float pressMmhg = pressPsi * 51.7149;
      float pressMeter = pressPsi * .7030695782; // psi to meters of water head
      buf = F("Raw: ");
      buf += raw;
      buf += F(" psi: ");
      buf += pressPsi;
      buf += F(" mmHg: ");
      buf += pressMmhg;
      buf += F(" meters: ");
      buf += pressMeter;
      Serial.println(buf);
      if (autoCalibrate == true && calibrate >= 10)
      {
        if (autoCalibrateComplete == false)
        {
          pressZero = pressZeroTemp;
          buf = F("\n\nsensor re-calibrated to : ");
          buf += pressZero;
          buf += F(" (raw)\n");
          Serial.println(buf);
          transProp();
          delay(3*timeDelay);
          autoCalibrateComplete = true;
        }
      }
      else
      {
        if (raw > pressZeroTemp) pressZeroTemp = raw;
        calibrate++;
      }
      timer = millis();
    }
    if (pressStep < 200) pressStep++;
    else pressStep = 0;
    rawRead[pressStep] = analogRead(analogPin);
  }
}

  void transProp() {
    buf = (F("max depth: "));
    float tDepth = pressMax;
    buf += (round(tDepth * .7030695782));
    buf += (F(" meters\n"));
    buf += (F("max resolution: "));
    float tRes = (tDepth / (resolution - float(pressZero)));
    buf += (tRes * 70.30695782);
    buf += (F("cm  or  "));
    buf += (tRes);
    buf += (F(" psi\n\n"));
    Serial.println(buf);
  }