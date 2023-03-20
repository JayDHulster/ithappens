short pressZero = 412;        //put the highest raw reading you see here at atmospheric pressure
short pressMax = 30;         //reduce pressure rating by %66 if 3.3v logic but still use 5v input
float resolution = 3686.4; //921.6;      //3686.4 for Due/Zero and add analogReadResolution(12) to void setup
byte analogPin = A1;           //transducer pin number
boolean autoCalibrate = false; //will automatically select highest raw reading in 10sec period

// https://forum.arduino.cc/t/pressure-transducer-code/362754/36
///////////////////////////////////////////////////////////////////////////////////////////////////////
byte pressStep;
short rawRead[201];
unsigned long timer;
byte calibrate;
boolean autoCalibrateComplete = false;
short pressZeroTemp;
String buf;
int counter = 0;

void setup()
{
  delay(6000);
  buf.reserve(40);
  Serial.begin(9600);
  transProp();
  analogReadResolution(12);
  Serial.println("Time;Raw;PSI;mmHg;meters");
  delay(6000);
  
}

void loop()
{
  if (millis() - timer >= 1000)
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
    buf = F("");
    buf += counter;
    buf += F(";");
    buf += raw;
    buf += F(";");
    buf += pressPsi;
    buf += F(";");
    buf += pressMmhg;
    buf += F(";");
    buf += pressMeter;
    Serial.println(buf);
    counter++;
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
        delay(6000);
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