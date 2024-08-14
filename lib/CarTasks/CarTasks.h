/**************************************************************************************************
 * 库文件: CarTasks.h
 * 描述: 该库封装了智能小车的各种任务，包括直线行驶、绕圈行驶等。
 * 创建时间: 2024年8月2日
 * 作者: LiJiahua
 * 版本: 1.0
 **************************************************************************************************/
#ifndef CAR_TASKS_H
#define CAR_TASKS_H

#include <Arduino.h>
#include "CarControl.h"
#include "AlarmLibrary.h"
#include "CarTracking.h"
#include "myMPU6050.h"

class CarTasks {
public:
    CarTasks(CarControl &car, AlarmLibrary &alarm, CarTracking &tracking, MyMPU6050 &mpu6050);

    void FirstAtoB();
    void SecondAtoD();
    void ThirdAtoA();
    void ForthAtoA();

private:
    CarControl &car;
    AlarmLibrary &alarm;
    CarTracking &tracking;
    MyMPU6050 &mpu6050;

    void MyJudgeYaw(float realYaw, float targetYaw, int speed, bool isTurn);
    float MyGetYaw();
    int waitForTrackStatus(unsigned int tempdata[2]);
    int trackUntilYaw(int speed, float speedKP, float targetYaw);
};

#endif