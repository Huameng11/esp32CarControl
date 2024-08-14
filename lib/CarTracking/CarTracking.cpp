#include "CarTracking.h"

#define WL_SERIAL Serial2 // 串口2

CarTracking::CarTracking(CarControl &car) : car(car), status(0) {}

void CarTracking::begin() {
  uartInit();
}

void CarTracking::uartInit() {
  WL_SERIAL.begin(9600);
}

void CarTracking::readData(unsigned int *data) {
  unsigned char buffer[3] = {0}; // 数据缓存区
  unsigned char count = 0;       // 数组计数
  unsigned int receivedData = 0; // 数据缓存区

  WL_SERIAL.write(0x57);
  unsigned char timeout = 0;
  while (1) {
    delay(1); // 延时很重要，是标准1ms

    if (WL_SERIAL.available() > 0) {
      buffer[count++] = WL_SERIAL.read(); // 依次读取接收到的数据
      if (count == 3) {
        receivedData = buffer[1];
        receivedData <<= 8;
        receivedData |= buffer[2];

        *data = buffer[0];
        *(data + 1) = receivedData;
        break;
      }
    }
    timeout++;
    if (timeout > 10) break; // 延时100ms后跳出循环
  }
}

void CarTracking::trackPID(int pwm, float P,bool isTurn) {
  static float integralError = 0, lastError = 0;
  static int leftPwm, rightPwm; // 左右轮速度
  unsigned int tempData[2] = {0}; // 数据缓存区
  int error = 0; // 偏差值
  float I = 0, D = 0; // 积分系数，微分系数，取值范围0.01-0.9

  readData(tempData);

  if ((tempData[0] >> 1) % 2 == 1) { // 在线上
    
    if ((tempData[0] >> 3) % 2 == 0) { // 部分在线上
      status = 1;
      if (tempData[0] % 2 == 0) {
        error = -tempData[1];
      } else if (tempData[0] % 2 == 1) {
        error = tempData[1];
      }
      integralError += error;

      rightPwm = pwm - (error * P + integralError * I + (error - lastError) * D);
      leftPwm = pwm + (error * P + integralError * I + (error - lastError) * D);

      lastError = error;

      // 最高速和最低速限制
      if (leftPwm > (pwm + 20)) leftPwm = (pwm + 20);
      if (rightPwm > (pwm + 20)) rightPwm = (pwm + 20);
      if (leftPwm <= 10) leftPwm = 10;
      if (rightPwm <= 10) rightPwm = 10;

      car.setSpeed(leftPwm, rightPwm);
    } else if ((tempData[0] >> 3) % 2 == 1) { // 全部在线上
      status = 0;
      //car.setSpeed(0, 0);
      //delay(1000);
    }
  } else if ((tempData[0] >> 1) % 2 == 0) { // 出线
    status = 0;
    if ((tempData[0] >> 2) % 2 == 0) { // 左出线
      if (isTurn==true)
      {
        status=2;
        car.setSpeed(10, pwm+10);
      }
     } else if ((tempData[0] >> 2) % 2 == 1) { // 右出线
       if (isTurn==true)
      {
        status=3;
        car.setSpeed(pwm+10, 10);
      }
       
     }
  }
}

int CarTracking::getStatus() {
  return status;
}