/**************************************************************************************************
 * 库文件: PinDefinitions.h
 * 描述: 该头文件定义了智能小车所使用的所有引脚。
 * 创建时间: 2024年8月2日
 * 作者: LiJiahua
 * 版本: 1.0
 **************************************************************************************************/
#ifndef PinDefinitions_h
#define PinDefinitions_h

// 定义电机控制引脚
const int PWMA = 32;
const int AIN1 = 27;
const int AIN2 = 26;
const int PWMB = 33;
const int BIN1 = 19;
const int BIN2 = 18;
const int STBY = 23;



// 定义声光报警器引脚
const int led = 4;
const int beer = 2;

// 定义功能按键引脚
int buttonPins[4] = {35, 13, 14, 25};

#endif