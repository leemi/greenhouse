#ifdef SENSOR_MODULE

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#include <avr/power.h>

// DHT Setup
#define DHTPIN            8
#define DHTTYPE           DHT22

DHT_Unified dht(DHTPIN, DHTTYPE);

// DHT Vars
float tempVal = 0;
float humidityVal = 0;

int readDelayMs = 1000;
unsigned long lastRead = 0;

// Display Vars
#define latchPin 3
#define clockPin 4
#define dataPin 2
int digitPin[] = {10, 11, 12, 13};

#define displayFreq 5000 // Frequency to change between C and RH
int showTemp = true; 
unsigned long lastSwitched = 0;

void setup() {

  powerSaving();

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  for (int i = 0; i < 4; i++) {
    pinMode(digitPin[i], OUTPUT);
    digitalWrite(digitPin[i], HIGH);
  }

  digitalWrite(latchPin, LOW);

  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  readDelayMs = sensor.min_delay / 1000;

  // Initial read
  updateDHT();
}

void loop() {

  if (millis() - lastSwitched > displayFreq) {
    // updateDHT takes about 271ms to run,
    // so we hide the lag in the transition
    // between C and H.
    updateDHT();
    showTemp = !showTemp;
    lastSwitched = millis();
  }
  
  if (showTemp) {
    writeVal(tempVal, 'C');
  } else {
    writeVal(humidityVal, 'H');
  }
  
}

void updateDHT() {
  if (millis() - lastRead > readDelayMs) {
    sensors_event_t event;  

    dht.temperature().getEvent(&event);
    if (!isnan(event.temperature)) {
      tempVal = event.temperature;
    }

    dht.humidity().getEvent(&event);
    if (!isnan(event.relative_humidity)) {
      humidityVal = event.relative_humidity;
      // 4 Digit Display
      if (humidityVal >= 100) { humidityVal = 99.9; }
    } 
    
    lastRead = millis();
  }
}

void writeDigit(int val) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, myfnNumToBits(val)); 
    digitalWrite(latchPin, HIGH);
}

void writeVal(float val, char type) {

    const int DELAY = 6;

    writeDigit(int(val / 10) % 10);
    digitalWrite(digitPin[0], LOW);
    delay(DELAY);
    digitalWrite(digitPin[0], HIGH);

    writeDigit(int(val) % 10);
    digitalWrite(digitPin[1], LOW);
    delay(DELAY);
    digitalWrite(digitPin[1], HIGH);

    writeDigit(int(val * 10) % 10);
    digitalWrite(digitPin[2], LOW);
    delay(DELAY);
    digitalWrite(digitPin[2], HIGH);

    writeDigit(type == 'C' ? 10 : 11 );
    digitalWrite(digitPin[3], LOW);
    delay(DELAY);
    digitalWrite(digitPin[3], HIGH);
}

void powerSaving() {
  // disable ADC
  ADCSRA = 0;  

  power_adc_disable(); // ADC converter
  power_spi_disable(); // SPI
  power_usart0_disable();// Serial (USART) 
  //  power_timer0_disable();// Timer 0 // This would disable millis()
  power_timer1_disable();// Timer 1
  power_timer2_disable();// Timer 2
  power_twi_disable(); // TWI (I2C)
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

#endif
