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
char keyPress;
char input[pwLength];
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
int m = 0; // delete
int o = 0; // delete
int h = 0; // delete
int k = 0;

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
  pinMode(13, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(9600); // delete
}


void loop() {
  keyPress = customKeypad.getKey();
  if (digitalRead(resetButton) == LOW) {
    softReset();
  }
 
  while(h == 0){ // delete
    Serial.println("Enter Password : "); // delete
    h++; // delete
  } // delete
  if (mode == 1) {
    if (pwIndex < pwLength - 1){
      lcd.print("Enter Password:");
      if (keyPress) {
        if (keyPress != 'D' && keyPress != 'A' && keyPress != 'B' && keyPress != 'C') {
          input[pwIndex] = keyPress;
          lcd.print("____");
          lcd.print(input);
          Serial.println(input); // delete
          pwIndex++;
        } else if (keyPress == 'D') {
          mode *= -1;
          i = 0;
          m = 0;
          o = 0;
          clearInput();
          Serial.println("Confirm Password:"); // delete
        } else if (keyPress == 'A' && pwIndex > 0){
            pwIndex--;
            input[pwIndex] = 0;
            Serial.println(input); // delete
        }
      }
    }
    if (pwIndex == pwLength - 1) {
      if(keyPress){
        if (keyPress == 'B'){
          lcd.clear();
          if (!strcmp(input, Master)) {
            lcd.print("Welcome Back");
            lcd.setCursor(7, 1);
            lcd.print("Master");
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
          clearInput();
          h = 0;
        }
        else if (keyPress == 'A' && pwIndex > 0){
            pwIndex--;
            input[pwIndex] = 0;
            Serial.println(input); // delete
        }
        else if (keyPress == 'D') {
          mode *= -1;
          i = 0;
          m = 0;
          o = 0;
          clearInput();
          Serial.println("Confirm Password:"); // delete
        }
      }
    }
  }

  else if (mode == -1) {
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("Enter Password:");
    if (k == 0){
    if (pwIndex < pwLength - 1) {
      if (keyPress) {
        if (keyPress == 'D') {
          mode *= -1;
          h = 0;
          clearInput();
        } else if( keyPress == 'A'){
          if (pwIndex > 0){
            pwIndex--;
            input[pwIndex] = 0;
            Serial.println(input); // delete
          }
        } else if (keyPress != 'A' && keyPress != 'B' && keyPress != 'C' && keyPress != 'D') {
          input[pwIndex] = keyPress;
          lcd.setCursor(pwIndex, 1);
          lcd.print("*");
          Serial.println(keyPress);
          Serial.println(input);
          pwIndex++;
        }
      }
    }

      if (pwIndex == pwLength - 1) {
        if (keyPress == 'A'){
          pwIndex--;
          input[pwIndex] = 0;
          Serial.println(input); // delete
        }
        if (keyPress == 'D') {
          mode = 1;
          h = 0;
          clearInput();
        }
        if (keyPress == 'B'){
          if (strcmp(input, Master)) {
            lcd.clear();
            lcd.print("Incorrect");
            Serial.println("Mali"); // delete
            digitalWrite(buzzerPin, HIGH);
            delay(100);
            digitalWrite(buzzerPin, LOW);
            delay(100);
            digitalWrite(buzzerPin, HIGH);
            delay(100);
            digitalWrite(buzzerPin, LOW);
            delay(100);
            attempt++;
            clearInput();
          }
          if (!strcmp(input, Master)){
            k = 1;
            attempt = 0;
            Serial.println("K is now 1");
          }
        }
      }
    } else if (k == 1){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Set new Password");
      lcd.setCursor(0, 1);
      lcd.print("____");
      m++;
      if (m == 1) {
        npwIndex = 0;
        Serial.print("Set new Password"); // delete
        Serial.println(npwIndex);
        keyPress = 0;
      }

      if (npwIndex < pwLength - 1) {
        if (keyPress){
          if (keyPress != 'A' && keyPress != 'B' && keyPress != 'C' && keyPress != 'D') {
            newMaster[npwIndex] = keyPress;
            lcd.print(newMaster);
            Serial.println(keyPress); // delete
            Serial.print("NewMaster"); // delete
            Serial.println(newMaster); // delete
            npwIndex++;
          }
          if (keyPress == 'A') {
            if (npwIndex > 0){
              npwIndex--;
              newMaster[npwIndex] = 0;
              Serial.print("NewMaster"); // delete
              Serial.println(newMaster); // delete
            }
          }
          if (keyPress == 'D') {
            mode = 1;
            h = 0;
            clearInput();
            while (npwIndex != 0) {
              newMaster[npwIndex--] = 0;
            }
            k = 0;
          }
        }
      }         
      if (npwIndex == pwLength - 1) {
        if (keyPress){
          if (keyPress == 'A'){
            npwIndex--;
            newMaster[npwIndex] = 0;
            Serial.print("NewMaster"); // delete
            Serial.println(newMaster); // delete
          }
          if (keyPress == 'D') {
            mode *= -1;
            k = 0;
            h = 0;
            clearInput();
            while (npwIndex != 0) {
              newMaster[npwIndex--] = 0;
            }
            
          }
          if (keyPress == 'B'){
            for (int o = 0; o < 4 ; o++) {
              Master[o] = newMaster[o];
            }
            lcd.setCursor(0, 0);
            lcd.print("New Password Set");
            Serial.print("New Password Set : "); // delete
            Serial.println(Master); // delete
            mode = 1;
            k = 0;
            h = 0;
            npwIndex = 0;
            clearInput();
            
          }
        }
      }
      
    }
  }
  breachShield(attempt);
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
    Serial.println("LOCKED"); // delete
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
  clearInput();
  mode = 1;
  m = 0;
  h = 0;
  k = 0;
  o = 0;

  lcd.clear();
  delay(2000);
  Serial.println("RESET COMPLETED"); // delete
  lcd.print("RESET COMPLETE");
}

void clearInput() {
  while (pwIndex != 0) {
    input[pwIndex--] = 0;
  }
  return;
}
