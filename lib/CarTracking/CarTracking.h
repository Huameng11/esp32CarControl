/**************************************************************************************************
 * 库文件: CarTracking.h
 * 描述: 该库提供了灰度寻迹功能，用于检测小车在地面上的位置并进行循迹。
 * 创建时间: 2024年8月2日
 * 作者: LiJiahua
 * 版本: 1.0
 **************************************************************************************************/
#ifndef CarTracking_h
#define CarTracking_h

#include <Arduino.h>
#include "CarControl.h"

enum TrackingStatus {
    TRACKING_ON_LINE,   // 在线上
    TRACKING_OFF_LINE,  // 出线
    TRACKING_FULL_LINE  // 全部在线上
};

class CarTracking {
public:
    CarTracking(CarControl &car);
    void begin();
    void trackPID(int pwm, float P,bool isTurn);
    int getStatus();
    void readData(unsigned int *data);
private:
    CarControl &car;
    int status;
    void uartInit();
    //void readData(unsigned int *data);
};

#endif