#include <LiquidCrystal_I2C.h>
#include <Keypad.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);

// led pins
const int LED1 = A0;
const int LED2 = A1;
const int LED3 = A2;
//

//soft-reset button
const int resetButton = 10;
//

//buzzer control
const int buzzerPin = A3;
//

//----------------------definitions----------------------

#define pwLength 5
char customkey;
char Data[pwLength];
char Master[pwLength] = "1234";
char newMaster[pwLength];

//placement of input
byte pwIndex = 0;


//number of rows and columns
const byte ROWS = 4;
const byte COLS = 4;

//keypad array
char hexakeys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

//Keypad pins
const byte rowPins[ROWS] = { 9, 8, 7, 6 };
const byte colPins[COLS] = { 5, 4, 3, 2 };


// keymap
Keypad customKeypad = Keypad(makeKeymap(hexakeys), rowPins, colPins, ROWS, COLS);

//mode switch
int mode = 1;

//number of attempts;
int attempt = 0;
//----------------------definitions----------------------

int i;
int npwIndex = 0;
int ppwIndex = 0;
int m = 0;
int o = 0;
int h = 0;

//Buzzer intervals
unsigned long currentMillis = millis();
unsigned long previousMillis = 0;
const long interval = 100;
//

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(resetButton, INPUT_PULLUP);
  Serial.begin(9600);
}


void loop() {
  customkey = customKeypad.getKey();
  if (digitalRead(resetButton) == LOW) {
    softReset();
  }

  lcd.clear();
  lcd.setCursor(0, 0);

  while(h == 0){
    Serial.println("Enter Password : ");
    h++;
  }
  if (mode == 1) {
    lcd.print("Enter Password:");
    if (customkey) {
      if (customkey != 'D' && customkey != 'A' && customkey != 'B' && customkey != 'C') {
        Data[pwIndex] = customkey;
        lcd.setCursor(pwIndex, 1);
        lcd.print("*");
        Serial.print(customkey);
        Serial.print("   ");
        Serial.println(Data);
        pwIndex++;
      } else if (customkey == 'D') {
        mode *= -1;
        i = 0;
        m = 0;
        o = 0;
        clearData();
        Serial.println("Confirm Password:");
      }
    }

    if (pwIndex == pwLength - 1) {
      lcd.clear();
      if (!strcmp(Data, Master)) {
        lcd.print("Access Granted");
        Serial.println("Access Granted");
        attempt = 0;
        delay(2000);
        lcd.clear();
      } else {
        lcd.print("Incorrect");
        Serial.println("Incorrect");
        digitalWrite(buzzerPin, HIGH);
        delay(100);
        digitalWrite(buzzerPin, LOW);
        delay(100);
        digitalWrite(buzzerPin, HIGH);
        delay(100);
        digitalWrite(buzzerPin, LOW);
        attempt++;
        delay(700);
      }
      lcd.clear();
      clearData();
      h = 0;
    }

    breachShield(attempt);
  }

  else if (mode == -1) {
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("Confirm Password");
    if (pwIndex < pwLength - 1) {
      if (customkey) {
        if (customkey == 'D') {
          mode *= -1;
          h = 0;
          pwIndex = 0;
        } else if (customkey != 'A' && customkey != 'B' && customkey != 'C' && customkey != 'D') {
          Data[pwIndex] = customkey;
          lcd.setCursor(pwIndex, 1);
          lcd.print("*");
          Serial.println(customkey);
          Serial.println(Data);
          pwIndex++;
          Serial.println(pwIndex);
        }
      }
    }

    if (pwIndex == 4) {
      m++;
      if (m == 1) {
        delay(1000);
        Serial.println("Set new Password");
        npwIndex = 0;
        customkey = 0;
      }
      if (strcmp(Data, Master)) {
        lcd.clear();
        lcd.print("Incorrect");
        Serial.println("Mali");
        digitalWrite(buzzerPin, HIGH);
        delay(100);
        digitalWrite(buzzerPin, LOW);
        delay(100);
        digitalWrite(buzzerPin, HIGH);
        delay(100);
        digitalWrite(buzzerPin, LOW);
        delay(100);
        clearData();
      }
      if (!strcmp(Data, Master)){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Set new Password");
        lcd.setCursor(0, 1);
        if (npwIndex != pwLength - 1) {
          if (customkey) {
            if (customkey != 'A' && customkey != 'B' && customkey != 'C' && customkey != 'D') {
              newMaster[npwIndex] = customkey;
              lcd.setCursor(npwIndex, 1);
              lcd.print(customkey);
              Serial.println(customkey);
              Serial.print("NewMaser");
              Serial.println(newMaster);
              npwIndex++;
            }
            if (customkey == 'D') {
              mode *= -1;
              clearData();
              while (npwIndex != 0) {
                newMaster[npwIndex--] = 0;
              }
            }
          }
        }
        if (npwIndex == pwLength - 1) {
          for (int o = 0; o < 4 ; o++) {
            Master[o] = newMaster[o];
            Serial.print("New Master Set : "); 
            Serial.println(Master);
          }
          lcd.setCursor(0, 0);
          lcd.print("New Password Set");
          Serial.print("New Password Set : ");
          Serial.println(Master);
          mode = 1;
          npwIndex = 0;
          softReset();
        }
      }
      
    }
  }
}

void breachShield(int attempt) {
  int state = LOW;
  if (attempt == 0) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(buzzerPin, LOW);
  } else if (attempt == 1) {
    digitalWrite(LED1, HIGH);
  } else if (attempt == 2) {
    digitalWrite(LED2, HIGH);
  } else if (attempt == 3) {
    digitalWrite(LED3, HIGH);
    Serial.println("LOCKED");
  }

  while (attempt == 3) {
    if (digitalRead(resetButton) == HIGH) {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        if (state == LOW) {
          state = HIGH;
        } else {
          state = LOW;
        }
      }
      digitalWrite(buzzerPin, state);

    } else if (digitalRead(resetButton) == LOW) {
      digitalWrite(buzzerPin, LOW);
      softReset();
      break;
    }
  }
}

void softReset() {
  attempt = 0;
  breachShield(attempt);
  clearData();
  mode = 1;
  lcd.clear();
  delay(2000);
  Serial.println("RESET COMPLETED");
  lcd.print("RESET COMPLETE");
}

void clearData() {
  while (pwIndex != 0) {
    Data[pwIndex--] = 0;
  }
  return;
}