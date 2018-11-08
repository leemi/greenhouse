#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN            8
#define DHTTYPE           DHT22

DHT_Unified dht(DHTPIN, DHTTYPE);

// Display Vars
int latchPin = 3;
int clockPin = 4;
int dataPin = 2;

int displayFreq = 10000; // Frequency to change between C and RH

// DHT Vars
int showTemp = true; 
float tempVal = 0;
float humidityVal = 0;

int readDelayMs = 1000;
unsigned long lastRead = 0;

void setup() {
  Serial.begin(9600);

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);

  digitalWrite(latchPin, LOW);

  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  readDelayMs = sensor.min_delay / 1000;
}

void loop() {

  showTemp = millis() % displayFreq * 2 > displayFreq;

  if (millis() - lastRead > readDelayMs) {
    sensors_event_t event;  

    dht.temperature().getEvent(&event);
    if (!isnan(event.temperature)) {
      tempVal = event.temperature;
      Serial.print("Temp: ");
      Serial.println(event.temperature);
    }

    dht.humidity().getEvent(&event);
    if (!isnan(event.relative_humidity)) {
      humidityVal = event.relative_humidity;
      Serial.print("RH: ");
      Serial.println(event.relative_humidity);
      // 4 Digit Display
      if (humidityVal >= 100) { humidityVal = 99.9; }
    } 
    
    lastRead = millis();
  }

  if (showTemp) {
    writeVal(tempVal, 'C');
  } else {
    writeVal(humidityVal, 'H');
  }
}

void writeDigit(int val) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, myfnNumToBits(val)); 
    digitalWrite(latchPin, HIGH);
}

void writeVal(float val, char type) {
    digitalWrite(10, LOW);
    writeDigit(int(val / 10) % 10);
    delay(4);
    digitalWrite(10, HIGH);

    digitalWrite(11, LOW);
    writeDigit(int(val) % 10);
    delay(4);
    digitalWrite(11, HIGH);

    digitalWrite(12, LOW);
    writeDigit(int(val * 10) % 10);
    delay(4);
    digitalWrite(12, HIGH);

    digitalWrite(13, LOW);
    writeDigit(type == 'C' ? 10 : 11 );
    delay(4);
    digitalWrite(13, HIGH);
}

byte myfnNumToBits(int someNumber) {
  switch (someNumber) {
    case 0:
      return B11111110;
      break;
    case 1:
      return B00001100;
      break;
    case 2:
      return B00110111;
      break;
    case 3:
      return B00011111;
      break;
    case 4:
      return B01001101;
      break;
    case 5:
      return B01011011;
      break;
    case 6:
      return B01111011;
      break;
    case 7:
      return B00001110;
      break;
    case 8:
      return B11111111;
      break;
    case 9:
      return B11011111;
      break;
    case 10:
      return B01110010; // Hexidecimal C
      break;
    case 11:
      return B01101101; // Hexidecimal H
      break;
    default:
      return B10010010; // Error condition, displays three vertical bars
      break;   
  }
}
