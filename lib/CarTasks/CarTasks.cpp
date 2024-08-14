#include "CarTasks.h"

// 构造函数，初始化类成员
CarTasks::CarTasks(CarControl &car, AlarmLibrary &alarm, CarTracking &tracking, MyMPU6050 &mpu6050)
    : car(car), alarm(alarm), tracking(tracking), mpu6050(mpu6050) {}

/*-----------------------------------
题目1  直线A-B
需调试参数：编码器AB值，角度值记录
函数参数： speed  stopnum
-------------------------------------*/
void CarTasks::FirstAtoB() {
    int speed=30;
    delay(500);
    alarm.setAlarm(2, 200);
    car.moveForward(speed);
    car.setSpeed(speed, speed);
    delay(1900);
    car.setSpeed(0, 0);
    alarm.setAlarm(2, 200);
}

/*-----------------------------------
题目2  绕圈A-D
-------------------------------------*/
void CarTasks::SecondAtoD() {
    delay(500);
    int speed = 37; // 速度0-80 ，一般不调
    Serial.println("SecondAtoD");
    int trackStatus = 0;
    float targetYaw = 184; // 转完半圈，目标角度，179-181
    float speedKP = 0.16;  // 0.16-5
    unsigned int tempdata[2] = {0}; // 数据缓存区
    alarm.setAlarm(2, 200);

    // A-B直线
    car.setSpeed(speed, speed);
    trackStatus = waitForTrackStatus(tempdata);
    car.setSpeed(0, 0);
    alarm.setAlarm(2, 200);
    delay(50);

    // B-C 一段圆弧
    trackStatus = trackUntilYaw(speed, speedKP, targetYaw);
    delay(100);
    car.setSpeed(0, 0);
    alarm.setAlarm(2, 200);

    // 角度控制
    float realYaw = MyGetYaw();
    MyJudgeYaw(realYaw, targetYaw,speed, true);
    car.setSpeed(0, 0);

    // C-D直线
    delay(600);
    car.setSpeed(speed, speed);
    delay(600);
    trackStatus = waitForTrackStatus(tempdata);
    car.setSpeed(0, 0);
    alarm.setAlarm(2, 200);
    delay(50);

    // D-A 二段圆弧
    trackStatus = trackUntilYaw(speed, speedKP, 350);
    delay(200);
    car.setSpeed(0, 0);
    alarm.setAlarm(2, 200);
}

/*-----------------------------------
题目3  8字绕圈A-A
-------------------------------------*/
void CarTasks::ThirdAtoA() {
    Serial.println("ThirdAtoA");
    delay(500);
    int speed = 37; // 速度0-80 ，一般不调
    float iniYaw = 36;
    int trackStatus = 0;
    float targetYaw = 145;
    float speedKP = 0.16;  // 0.16-5
    unsigned int tempdata[2] = {0}; // 数据缓存区
    alarm.setAlarm(2, 200);

    // 第0步，角度调整
    MyJudgeYaw(10, iniYaw,speed, true);

    // 第一段A-C 直线
    car.setSpeed(speed, speed);
    trackStatus = waitForTrackStatus(tempdata);
    car.setSpeed(0, 0);
    alarm.setAlarm(2, 200);
    delay(50);
    float realYaw = MyGetYaw();
    MyJudgeYaw(realYaw, 22,speed, true);

    // 第二段C-B 圆弧巡线
    trackStatus = trackUntilYaw(speed, speedKP, 185);
    delay(10);
    car.setSpeed(0, 0);
    alarm.setAlarm(2, 200);

    // 角度控制
    realYaw = MyGetYaw();
    MyJudgeYaw(realYaw, targetYaw,speed, true);
    car.setSpeed(0, 0);
    delay(300);

    // 第三段 B-D 直线
    car.setSpeed(speed, speed);
    delay(300);
    trackStatus = waitForTrackStatus(tempdata);
    car.setSpeed(0, 0);
    alarm.setAlarm(2, 200);
    realYaw = MyGetYaw();
    MyJudgeYaw(realYaw, 170,speed, true);

    delay(50);

    // 第四段D-A
    trackStatus = trackUntilYaw(speed, speedKP, 350);
    delay(10);
    car.setSpeed(0, 0);
    alarm.setAlarm(2, 200);
}

/*-----------------------------------
题目4  绕4圈A-A
-------------------------------------*/
void CarTasks::ForthAtoA() {
    int speed = 37;
    for (int i = 0; i < 4; i++) {
        Serial.println("ForthAtoA");
        ThirdAtoA();   // 程序3

        car.setSpeed(0, 0); // 小车停止 0 0
        car.setSpeed(speed, speed - 5);  // 小车启动 速度值speed
        delay(350);    // 延时350ms
        car.setSpeed(0, 0);  // 小车停止
        delay(3000);  // 等待3秒

        // 前进一点
        MyJudgeYaw(5, 10,speed, true);   // 转向
        car.setSpeed(0, 0);      // 停止
        delay(500);   // 延时500ms
    }
}

/*-----------------------------------
第0步，角度调整
函数参数： realYaw 当前偏航角
           targetYaw 目标偏航角
           isTurn 是否转向
-------------------------------------*/
void CarTasks::MyJudgeYaw(float realYaw, float targetYaw, int speed,bool isTurn) {
    while (realYaw > targetYaw + 1 || realYaw < targetYaw - 1) {
        Serial.println(MyGetYaw()); // 获取并打印Yaw数据
        float error = targetYaw - realYaw;
        if (isTurn) {
            if (error > 0) {
                car.setSpeed(speed / 1.5, -speed / 1.5);
            } else {
                car.setSpeed(-speed / 1.5, speed / 1.5);
            }
        }
        delay(10);
        realYaw = MyGetYaw();
    }
}

/*-----------------------------------
获取当前偏航角
-------------------------------------*/
float CarTasks::MyGetYaw() {
    float yaw = mpu6050.getYaw();
    if (yaw < 0) {
        yaw += 360;
    } else if (yaw > 351 && yaw < 359.99) {
        yaw = 0.1;
    }
    return yaw;
}

/*-----------------------------------
等待循迹状态
函数参数： tempdata 数据缓存区
-------------------------------------*/
int CarTasks::waitForTrackStatus(unsigned int tempdata[2]) {
    int trackStatus = 0;
    do {
        tracking.readData(tempdata);
        if ((tempdata[0] >> 1) % 2 == 1) {
            if ((tempdata[0] >> 3) % 2 == 0) {
                trackStatus = 1;
            }
        }
        delay(10);
    } while (!trackStatus);
    return trackStatus;
}

/*-----------------------------------
循迹直到特定偏航角
函数参数： speed 速度
           speedKP 速度KP值
           targetYaw 目标偏航角
-------------------------------------*/
int CarTasks::trackUntilYaw(int speed, float speedKP, float targetYaw) {
    int trackStatus = 1;
    bool isContinue = true;
    while (trackStatus) {
        tracking.trackPID(speed, speedKP, isContinue);
        trackStatus = tracking.getStatus();
        delay(10);
        
        // 检查当前偏航角是否达到或超过目标偏航角
        if (MyGetYaw() > targetYaw) {
            isContinue = false; // 将 isContinue 设置为 false
        }
        
        // 如果 isContinue 为 false 且 trackStatus 不为 0，则跳出循环
        if (!isContinue && trackStatus != 0) {
            break;
        }
    }
    return trackStatus;
}