#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include "pitches.h"

#define I2C_ADDR 0x27  // I2C scanner address
#define BACKLIGHT_PIN 3
#define En_pin 2
#define Rw_pin 1
#define Rs_pin 0
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7
#define led_pin 13
#define BUZZER_PIN 12
bool songPlaying = false;
bool lowVoltageTriggered = false;
LiquidCrystal_I2C lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

int melody[] = {
  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, REST,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REST,
  NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, REST,
  NOTE_A4, NOTE_G4, NOTE_A4, REST,
  
  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, REST,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REST,
  NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, REST,
  NOTE_A4, NOTE_G4, NOTE_A4, REST,
  
  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, REST,
  NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, REST,
  NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, REST,
  NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, REST,
  
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REST,
  NOTE_D5, NOTE_E5, NOTE_A4, REST,
  NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, REST,
  NOTE_C5, NOTE_A4, NOTE_B4, REST,
  
  NOTE_E5, REST, REST, NOTE_F5, REST, REST,
  NOTE_E5, NOTE_E5, REST, NOTE_G5, REST, NOTE_E5, NOTE_D5, REST, REST,
  NOTE_D5, REST, REST, NOTE_C5, REST, REST,
  NOTE_B4, NOTE_C5, REST, NOTE_B4, REST, NOTE_A4,
  
  NOTE_E5, REST, REST, NOTE_F5, REST, REST,
  NOTE_E5, NOTE_E5, REST, NOTE_G5, REST, NOTE_E5, NOTE_D5, REST, REST,
  NOTE_D5, REST, REST, NOTE_C5, REST, REST,
  NOTE_B4, NOTE_C5, REST, NOTE_B4, REST, NOTE_A4
};

int durations[] = {
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8,
  
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8,
  
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 8, 4, 8,
  
  8, 8, 4, 8, 8,
  4, 8, 4, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 4,
  
  4, 8, 4, 4, 8, 4,
  8, 8, 8, 8, 8, 8, 8, 8, 4,
  4, 8, 4, 4, 8, 4,
  8, 8, 8, 8, 8, 2,
  
  4, 8, 4, 4, 8, 4,
  8, 8, 8, 8, 8, 8, 8, 8, 4,
  4, 8, 4, 4, 8, 4,
  8, 8, 8, 8, 8, 2
};

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);  //My LCD was 16x2
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home();  // go home

  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);

  pinMode(2, OUTPUT);  //define arduino pin
  pinMode(4, INPUT);   //define arduino pin
  pinMode(12, OUTPUT);
}

void loop() {
  if (!songPlaying && !lowVoltageTriggered){
    printVolts();
  }
}

void printVolts() {
  int sensorValue = analogRead(A0);                        //read the A1 pin value
  float voltageValue = sensorValue * (5 / 1023.0) * 7.23;  // Convert analog value to voltage (assuming 5V Arduino)

  Serial.println(voltageValue, 2);

  lcd.setCursor(0, 0);
  lcd.print("Voltage = ");
  lcd.print(voltageValue);  //print the voltage to LCD
  lcd.print(" V");
  lcd.setCursor(0, 1);
  lcd.print("              ");

if (voltageValue <= 15.0)
  {
    lcd.setCursor(0, 1);
    lcd.print("Low Voltage!");
    playSong();
    lowVoltageTriggered = true;
  }

  delay(1000);
}


void playSong()
{
  songPlaying = true;
  int size = sizeof(durations) / sizeof(int);

  for (int note = 0; note < size; note++) {

    int duration = 1000 / durations[note];
    tone(BUZZER_PIN, melody[note], duration);

    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);

    noTone(BUZZER_PIN);
  }
  songPlaying = false;
}