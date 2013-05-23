#include "Arduino.h"
#include <SoftwareSerial.h>
#include "Minitel.cpp"
Minitel* m;
void setup() {
  m = new Minitel(6,7);
  Serial.begin(1200);
}
void demoText() {
  m->clearScreen();
  m->mode(TEXT_MODE);
  m->textColor(WHITE);
  m->bgColor(BLACK);
  m->blink();
  m->noBlink();
  int width = 35;
  int i = 1;
  int j = 0;
  char text[2];
  while(true)
  {
      if(Serial.available())
      {
          text[0] = Serial.read();
          if(text[0] == '\n')
          {
              j++;
              i = 1;
          }
          else
          {
              text[1] = 0;
              m->text(text, i++, 1 + j);
              if(i > width )
              {
                  j++;
                  i = 1;
              }
          }
      }
  }
}
void loop()
{
    demoText();
}
