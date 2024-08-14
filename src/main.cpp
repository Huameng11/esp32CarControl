/**************************************************************************************************
 * 程序功能: 智能小车控制程序
 * 描述: 该程序控制一个智能小车，通过按键触发不同的任务，包括直线行驶、绕圈行驶等。
 *       小车使用MPU6050进行姿态检测，通过灰度传感器进行循迹。
 * 创建时间: 2024年8月2日
 * 作者: LiJiahua
 * 版本: 1.0
 **************************************************************************************************/


#include <Arduino.h>
#include "PinDefinitions.h" //引脚定义库
#include "AlarmLibrary.h"   //声光报警
#include "CarControl.h"   //小车控制库
#include "CarTracking.h"  //灰度寻迹库
//#include "myMPU6050.h" // 确保你有一个适用于ESP32的MPU6050库
#include <Wire.h> // 使用ESP32的I2C库
#include "CarTasks.h" // 包含新创建的库

// 注册所有实例
CarControl car(PWMA, AIN1, AIN2, PWMB, BIN1, BIN2, STBY);  //小车实例
AlarmLibrary Myalarm(beer, led);                     // 创建AlarmLibrary实例
CarTracking tracking(car);                        // 循迹实例
globalMPU6050Params MPU6050Params = {10, 0, 0, 0};// 全局MPU6050参数结构体
MyMPU6050 My6050; // 创建MPU6050对象

// 创建CarTasks实例
CarTasks tasks(car, Myalarm, tracking, My6050);

// 异步任务函数,每10ms更新获取一次陀螺仪角度值
void asyncTask(void *parameter) {
    while (1) {
        My6050.IMUupdate();
        vTaskDelay(10 / portTICK_PERIOD_MS); // 延迟10ms
    }
}

void CarBegin();
void handleButtonPress(int buttonIndex);

void setup() {
    CarBegin();
    for (int i = 0; i < 4; i++) {
        pinMode(buttonPins[i], INPUT_PULLDOWN);
    }
    // 创建异步任务
    xTaskCreate(
        asyncTask,       // 任务函数
        "AsyncTask",     // 任务名称
        10000,           // 任务堆栈大小
        NULL,            // 任务参数
        1,               // 任务优先级
        NULL             // 任务句柄
    );
}

void loop() {
    handleButtonPress(0);
    handleButtonPress(1);
    handleButtonPress(2);
    handleButtonPress(3);
    delay(50);
}

/*--------------------------------------
初始化相关代码
-------------------------------------*/
void CarBegin() {
    Serial.begin(115200);
    car.begin();
    Wire.begin(); // 使用默认的I2C引脚（SDA: GPIO21, SCL: GPIO22）
    tracking.begin();
    // MPU6050初始化
    while (!My6050.mpuInit(&Wire)) {
        Serial.println("MPU6050 NOT_FOUND!");
        delay(3000);
    }
    Serial.println("MPU6050 found!");

    // 设置MPU6050数据更新频率
    My6050.begin(1000.0f / (float)MPU6050Params.MPU6050dt);

    // MPU6050数据滤波优化
    My6050.getDataErrorSum();
    My6050.getDataError();
    Serial.println("MPU6050 datafilter succeed!");
    delay(2000);
    Myalarm.setAlarm(1, 500);
}

// 按键处理函数定义
void handleButtonPress(int buttonIndex) {
    bool status = false;
    if (digitalRead(buttonPins[buttonIndex])) {
        delay(10);
        if (digitalRead(buttonPins[buttonIndex]) && !status) {
            Serial.print("key");
            Serial.println(buttonIndex + 1);
            switch (buttonIndex) {
                case 0: tasks.FirstAtoB(); break;
                case 1: tasks.SecondAtoD(); break;
                case 2: tasks.ThirdAtoA(); break;
                case 3: tasks.ForthAtoA(); break;
            }
            status = !status;
        } else if (!digitalRead(buttonPins[buttonIndex])) {
            status = false;
        }
    }
}