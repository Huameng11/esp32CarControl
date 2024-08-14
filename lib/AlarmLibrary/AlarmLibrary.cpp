#include "AlarmLibrary.h"

AlarmLibrary::AlarmLibrary(int beerPin, int ledPin) {
  _beerPin = beerPin;
  _ledPin = ledPin;
  pinMode(_beerPin, OUTPUT);
  pinMode(_ledPin, OUTPUT);
}

void AlarmLibrary::setAlarm(int count,int atime) {
  for (int i = 0; i < count; i++) {
    digitalWrite(_beerPin, HIGH);
    digitalWrite(_ledPin, HIGH);
    delay(atime); // 持续
    digitalWrite(_beerPin, LOW);
    digitalWrite(_ledPin, LOW);
    delay(atime); // 持续
  }
}