bool ComparaArreglos(char *ClaveDesarme, char *ClaveUsuario) {
  int ContArreglo = 0;
  for (int i = 0; i < 7; i++) {
    if (ClaveDesarme[i] == ClaveUsuario[i])
      ContArreglo = ContArreglo + 1;
  }
  if (ContArreglo == 7) {
    return true;
  }
  else {
    return false;
  }
}
#define LEDCONTA 14
#define LEDSAL 25
#define UP 13
#define DOWN 33
#define ARM 32
#include <Wire.h>
#include "SSD1306.h"
//0.66: 64*48 otros: 128*32
SSD1306Wire display(0x3c, SDA, SCL, GEOMETRY_128_32);
static uint8_t counter = 20;
static int x = 0, y = 5, Estado = 1, Suma = 0, c1 = 1, c2 = 1, c3 = 1;
static char ClaveDesarme[7] = {'u', 'u', 'd', 'd', 'u', 'd', 'a'};
static char ClaveUsuario[7];


void setup() {
  Serial.begin(115200);

  digitalWrite(LEDCONTA, HIGH);
  digitalWrite(LEDSAL, LOW);
  pinMode(LEDCONTA, OUTPUT);
  pinMode(LEDSAL, OUTPUT);
  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(ARM, INPUT_PULLUP);
  display.init();
  display.setContrast(255);
  display.clear();
}
void Tarea() {
  switch (Estado) {
    case 1: {
        digitalWrite(LEDCONTA, HIGH);
        digitalWrite(LEDSAL, LOW);

        if (digitalRead(UP) == 0) {
          Suma = 1;
          c1 = 0;
        }
        if (digitalRead(DOWN) == 0) {
          Suma = -1;
          c2 = 0;
        }

        if (digitalRead(UP) == 1 && digitalRead(DOWN) == 1) {
          c3 = 0;
        }
        if (c3 == 0) {
          if (c2 == 0 || c1 == 0) {
            if (counter <= 60 && counter >= 10) {
              display.setTextAlignment(TEXT_ALIGN_LEFT);
              display.setFont(ArialMT_Plain_16);
              display.clear();
              display.drawString(x, y, String(counter));
              display.display();
              counter = (counter + Suma);
            }
            display.clear();
            display.drawString(x, y, String(counter));
            display.display();
            counter = (counter);
          }
          c1 = 1;
          c2 = 1;
          c3 = 1;
        }

        if (digitalRead(ARM) == 0) {
          Estado = 2;
        }

        break;
      }
    case 2: {
        static uint32_t previousMillis = 0;
        static uint8_t LEDsal = LOW;
        const uint32_t interval = 1000;
        uint32_t currentMillis = millis();

        digitalWrite(LEDCONTA, LOW);
        if (currentMillis - previousMillis >= 1000) {
          previousMillis = currentMillis;
          if (LEDsal == LOW) {
            LEDsal = HIGH;
          } else {
            LEDsal = LOW;
          }
          digitalWrite(LEDSAL, LEDsal);
          if  (counter > 0 ) {
            counter--;
          }
          display.clear();
          display.drawString(x, y, String(counter));
          display.display();
        }
        tarea2();

        if (counter == 0) {
          display.clear();
          display.drawString(x, y, "¡¡Boom!!");
          display.display();
          Estado = 1;
          counter = 20;
        }

        break;
      }
    default:
      display.clear();
      display.drawString(x, y, "ERROR");
      display.display();
      break;
  }

}


void tarea2() {
  static char LectorB;
  static int SupB = 0;

  int b1 = 1, b2 = 1, b3 = 1, bt = 1;
  if (digitalRead(UP) == 0) {
    LectorB = 'u';
    b1 = 0;
  }
  if (digitalRead(DOWN) == 0) {
    LectorB = 'd';
    b2 = 0;
  }
  if (digitalRead(ARM) == 0) {
    LectorB = 'a';
    b3 = 0;
  }
  if (digitalRead(UP) == 1 && digitalRead(DOWN) == 1 && digitalRead(ARM) == 1) {
    bt = 0;
  }
  if (bt == 0) {
    if (b1 == 0 || b2 == 0 || b3 == 0) {
      ClaveUsuario[SupB] = LectorB;
      SupB++;
      display.clear();
      display.drawString(x, y, String(ClaveUsuario));
      display.display();
      LectorB = 'p';

    }
  }
  if (SupB == 7) {
    if (ComparaArreglos(ClaveDesarme, ClaveUsuario)) {
      Estado = 1;
      counter = 20;
    }
    SupB = 0;
  }
}

void loop() {
  Tarea();

}
