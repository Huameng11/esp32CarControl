/**************************************************************************************************
 * 库文件: CarControl.h
 * 描述: 该库提供了小车的基本控制功能，包括前进、后退、转向等。
 * 创建时间: 2024年8月2日
 * 作者: LiJiahua
 * 版本: 1.0
 **************************************************************************************************/
#ifndef CarControl_h
#define CarControl_h

#include <Arduino.h>

class CarControl {
  public:
    // 构造函数，初始化电机控制引脚
    CarControl(int pwma, int ain1, int ain2, int pwmb, int bin1, int bin2, int stby);
    
    // 初始化函数，设置引脚模式和初始状态
    void begin();
    
    // 控制小车前进
    void moveForward(int speed);
    
    // 控制小车后退
    void moveBackward(int speed);
    
    // 控制小车左转
    void turnLeft(int speed);
    
    // 控制小车右转
    void turnRight(int speed);
    
    // 测量前方障碍物的距离
    double measureDistance();
    
    // 避障逻辑
    void avoidObstacle(int speed);
    
    // 设置小车速度
      void setSpeed(int speedA,int speedB);
  private:
    int _pwma, _ain1, _ain2, _pwmb, _bin1, _bin2, _stby;
};

#endif