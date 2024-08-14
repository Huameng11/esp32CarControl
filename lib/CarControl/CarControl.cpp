#include "CarControl.h"

// 构造函数，初始化电机控制引脚
CarControl::CarControl(int pwma, int ain1, int ain2, int pwmb, int bin1, int bin2, int stby)
  : _pwma(pwma), _ain1(ain1), _ain2(ain2), _pwmb(pwmb), _bin1(bin1), _bin2(bin2), _stby(stby) {}

// 初始化函数，设置引脚模式和初始状态
void CarControl::begin() {
  pinMode(_ain1, OUTPUT);
  pinMode(_ain2, OUTPUT);
  pinMode(_bin1, OUTPUT);
  pinMode(_bin2, OUTPUT);
  pinMode(_pwma, OUTPUT);
  pinMode(_pwmb, OUTPUT);
  pinMode(_stby, OUTPUT);

  digitalWrite(_ain1, 1);
  digitalWrite(_ain2, 0);
  digitalWrite(_bin1, 1);
  digitalWrite(_bin2, 0);
  digitalWrite(_stby, 1);
  analogWrite(_pwma, 0);
  analogWrite(_pwmb, 0);
}

// 控制小车前进
void CarControl::moveForward(int speed) {
  digitalWrite(_ain1, 1);
  digitalWrite(_ain2, 0);
  digitalWrite(_bin1, 1);
  digitalWrite(_bin2, 0);
  //setSpeed(speed, speed);
}

// 控制小车后退
void CarControl::moveBackward(int speed) {
  digitalWrite(_ain1, 0);
  digitalWrite(_ain2, 1);
  digitalWrite(_bin1, 0);
  digitalWrite(_bin2, 1);
  //setSpeed(speed, speed);
}

// 控制小车左转
void CarControl::turnLeft(int speed) {
  setSpeed(speed / 3, speed);
}

// 控制小车右转
void CarControl::turnRight(int speed) {
  setSpeed(speed, speed / 3);
}

// 设置小车速度 值范围0-100
void CarControl::setSpeed(int speedA, int speedB) {
  ////////////////////////左轮方向控制////////////////////////////
   if(speedA > 0)
   {
     digitalWrite(_ain1, HIGH);
     digitalWrite(_ain2, LOW);
   }
   else if(speedA < 0)
   {
     digitalWrite(_ain1, LOW);
     digitalWrite(_ain2, HIGH);
     speedA = -speedA;
   }
   
////////////////////////右轮方向控制////////////////////////////
   if(speedB > 0)
   {
     digitalWrite(_bin1, HIGH);
     digitalWrite(_bin2, LOW);
   }
   else if(speedB < 0)
   {
     digitalWrite(_bin1, LOW);
     digitalWrite(_bin2, HIGH);
     speedB = -speedB;
   }

 ////////////////////////速度控制////////////////////////////
  analogWrite(_pwma, speedA*2.55);
  analogWrite(_pwmb, speedB*2.25);
}