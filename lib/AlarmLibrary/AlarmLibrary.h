/**************************************************************************************************
 * 库文件: AlarmLibrary.h
 * 描述: 该库提供了声光报警功能，用于在特定事件发生时发出警报。
 * 创建时间: 2024年8月2日
 * 作者: LiJiahua
 * 版本: 1.0
 **************************************************************************************************/
#ifndef AlarmLibrary_h
#define AlarmLibrary_h

#include <Arduino.h>

class AlarmLibrary {
  public:
    AlarmLibrary(int beerPin, int ledPin);
    void setAlarm(int count,int atime);

  private:
    int _beerPin;
    int _ledPin;
};

#endif