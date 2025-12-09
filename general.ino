#include <LiquidCrystal.h>
#include <SoftwareSerial.h> 
const int pin_RS = 8; 
const int pin_EN = 9; 
const int pin_d4 = 5; 
const int pin_d5 = 12; 
const int pin_d6 = 13; 
const int pin_d7 = 7; 
const int pin_BL = 10; 
LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);
SoftwareSerial bluetooth(1, 0); // RX, TX
float voltage[4]; // Масив для напруги з п'яти датчиків
float R1 = 30000.0; // Опір резистора R1
float R2 = 7500.0; // Опір резистора R2
int lowb = 2.7;
int displayState = 0; // Стан для відображення інформації на LCD
int lastButtonState = HIGH; // Зберігає останній стан кнопки

float readVoltage(int pin) {
  int value = analogRead(pin);
  float vout = (value * 5.0) / 1024.0; // Перетворення аналогового сигналу в напругу
  return vout / (R2 / (R1 + R2)); // Розрахунок реальної напруги
}

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  Serial.begin(9600);
  bluetooth.begin(9600); // Ініціалізація Bluetooth-модуля
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("System Ready");
}

void loop() {
  for (int i = 0; i < 4; i++) {
    voltage[i] = readVoltage(i);
  }
  bool isPowerAvailable = digitalRead(6); // Перевірка наявності мережевого живлення
  // Логіка управління реле та виводу на LCD
  if (!isPowerAvailable) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("No Power Supply");

    if (voltage[1] > voltage[3] && voltage[1] > lowb) 
{
      digitalWrite(2, LOW); // Активація реле 1
      digitalWrite(3, HIGH);
      digitalWrite(4, LOW);
      lcd.setCursor(0, 1);
      lcd.print("Relay 1 ON");

    } 
else if (voltage[3] > voltage[1] && voltage[3] > lowb) 
{
      digitalWrite(2, HIGH);
      digitalWrite(3, LOW); // Активація реле 2
      digitalWrite(4, LOW);
      lcd.setCursor(0, 1);
      lcd.print("Relay 2 ON");
  
    } 
else if(voltage[1] < lowb && voltage[3] < lowb && voltage[0] > lowb) 
{
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH); // Активація реле 3 при відсутності мережевого живлення
      lcd.setCursor(0, 1);
      lcd.print("Relay 3 ON");
 
    }
    else if(voltage[1] < lowb && voltage[3] < lowb && voltage[0] < lowb){
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("LOW MODE");
    
    lcd.setCursor(0, 1);
  
}
  }
 else 
 {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH); // Активація реле 4 при наявності мережевого живлення
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Power Supply OK");

    lcd.setCursor(0, 1);
    lcd.print("Relay 3 ON");
  
  }
  // Перемикання відображення інформації на LCD за допомогою кнопки
  int buttonValue = analogRead(A5);
  int buttonState = buttonValue < 60 ? LOW : HIGH;
  if (buttonState != lastButtonState && buttonState == LOW) {
    displayState = (displayState + 1) % 7;
    lastButtonState = buttonState;
  } else if (buttonState == HIGH) {
    lastButtonState = HIGH;
  }

  // Відображення даних на LCD
 /* if (displayState < 5) {
    lcd.clear();
    switch(displayState) {
      case 0: lcd.print("Wind Gen: "); break;
      case 1: lcd.print("Wind Bat: "); break;
      case 2: lcd.print("Solar Panel: "); break;
      case 3: lcd.print("Solar Bat: "); break;
      case 4: lcd.print("Backup Bat: "); break;
    }
    lcd.print(voltage[displayState], 2);
    lcd.print(" V");
  }*/
Serial.print(voltage[0]);
Serial.print("|");
Serial.print(voltage[1]);
Serial.print("|");
Serial.print(voltage[3]);


delay(5000);
 
}