/*
此软件基于 GNU Affero General Public License v3.0 许可。
完整许可证文本请访问：https://www.gnu.org/licenses/agpl-3.0.txt

版权所有 (c) 2026 YangJincheng
*/

#pragma once
#include <Arduino.h>

class LED{
protected:
  const  uint8_t pin;
  const bool mode;// mode: 0 = 低电平点亮 (阴极接地)
            //       1 = 高电平点亮 (阳极接VCC)
  bool state;

public:
  LED(uint8_t p,bool m = 0): pin(p), mode(m), state(false) {
    pinMode(pin, OUTPUT);
    off();
  }

  void operator()(bool status){
    state = status;
    digitalWrite(pin, mode == status);
  }


  void on(){
    state = true;
    digitalWrite(pin, mode);
  }

  void off(){
    state = false;
    digitalWrite(pin, !mode);
  }

  bool isOn(){
    return state;
  }

  void toggle() {
      state = !state;
      operator()(state);
  }

  void test(){
    for(short i = 0; i < 3; i++){
      on();
      Serial.println("Turned on.");
      delay(100);
      off();
      Serial.println("Turned off.");
      delay(100);
    }
  }
};


template <typename T1, typename T2>
struct MyPair {
  T1 first;
  T2 second;
  
  // 使用成员初始化列表
  MyPair() : first(), second() {}
  
  MyPair(const T1& f, const T2& s) : first(f), second(s) {}
  
  // 添加移动语义支持
  MyPair(T1&& f, T2&& s) : first(std::move(f)), second(std::move(s)) {}
  
  // swap可以更高效
  void swap(MyPair& other) noexcept {
    using std::swap;
    swap(first, other.first);
    swap(second, other.second);
  }
  
  // 添加字符串化方法，便于调试
  String toString() const {
    return "(" + String(first) + ", " + String(second) + ")";
  }
};

// 单个电机控制器
class SingleMotor {
  private:
    short pinF;  // 正转/前进引脚 (如 IN1)
    short pinB;  // 反转/后退引脚 (如 IN2)
  
  public:
    // 构造函数：初始化引脚
    SingleMotor(short forwardPin, short backwardPin) {
      pinF = forwardPin;
      pinB = backwardPin;
      pinMode(pinF, OUTPUT);
      pinMode(pinB, OUTPUT);
      coast();  // 初始化时设为滑行状态
    }
    
    // 设置速度：-255~255（负值为反转）
    void setSpeed(short speed) {
      if (speed > 0) {
        analogWrite(pinF, speed);
        digitalWrite(pinB, LOW);  // 明确使用digitalWrite
      } else if (speed < 0) {
        digitalWrite(pinF, LOW);  // 明确使用digitalWrite
        analogWrite(pinB, -speed); // 取绝对值
      } else {
        // 速度为0：使用滑行（与老师代码一致）
        coast();
      }
    }
    
    void operator()(short speed){
      setSpeed(speed);  // 修正语法错误，去掉多余的(*this)
    }

    // 快速刹车（两个引脚都设为HIGH）
    void brake() {
      digitalWrite(pinF, HIGH);
      digitalWrite(pinB, HIGH);
    }

    // 滑行（两个引脚都设为LOW - 与老师motor(0,0)效果相同）
    void coast() {
      digitalWrite(pinF, LOW);
      digitalWrite(pinB, LOW);
    }
    
    // 可选：添加动态刹车（PWM=0，某些驱动器可能有不同效果）
    void dynamicBrake() {
      analogWrite(pinF, 0);
      analogWrite(pinB, 0);
    }
};

// 双电机控制器
class Motors {
  public:  // 改为public以便直接访问
    SingleMotor left;
    SingleMotor right;
  
  private:
    // 原来的motor函数改名为drive
    short lastLeftSpeed = 0;
    short lastRightSpeed = 0;

    void drive(short leftSpeed, short rightSpeed) {
      lastLeftSpeed = leftSpeed;
      lastRightSpeed = rightSpeed;
      left.setSpeed(leftSpeed);
      right.setSpeed(rightSpeed);
    }

  public:
    // 构造函数：使用你的引脚定义
    Motors(short lpF = 6, short lpB = 9, short rpF = 10, short rpB = 11)
      : left(lpF, lpB), right(rpF, rpB) {
      // 初始化完成
    }
    
    // 兼容你原来的 motor() 函数
    void motor(short leftSpeed, short rightSpeed) {
      drive(leftSpeed, rightSpeed);
    }
    
    // 运算符重载：支持Motors(leftSpeed, rightSpeed)调用
    void operator()(short leftSpeed, short rightSpeed) {
      drive(leftSpeed, rightSpeed);
    }
    
    // 快捷方法
    void coast() {
      left.coast();
      right.coast();
    }
    
    void brake() {
      left.brake();
      right.brake();
    }
    
    // 灵活的stop方法，类似Python的def stop(type=False):
    // 使用布尔参数控制停止方式
    void stop(bool useBrake = false) {
      if (useBrake == false) {
        coast();  // 默认：滑行停止
      } else {
        brake();  // 如果传入true：刹车停止
      }
    }
    
    // 可选：添加带参数的动态刹车
    void dynamicBrake() {
      left.dynamicBrake();
      right.dynamicBrake();
    }
    void forward(short speed) {
      drive(speed, speed);
    }
    
    void backward(short speed) {
      drive(-speed, -speed);
    }
    
    void turnLeft(short speed) {
      drive(-speed, speed);
    }
    
    void turnRight(short speed) {
      drive(speed, -speed);
    }
    
    void rotate(short speed) {
      drive(speed, -speed);
    }
    
    // 安全速度设置（防止过速）
    void safeDrive(short leftSpeed, short rightSpeed, short maxSpeed = 255) {
      leftSpeed = constrain(leftSpeed, -maxSpeed, maxSpeed);
      rightSpeed = constrain(rightSpeed, -maxSpeed, maxSpeed);
      drive(leftSpeed, rightSpeed);
    }

    MyPair<short, short> getCurrentSpeed() const {
      return MyPair<short, short>(lastLeftSpeed, lastRightSpeed);
    }
    
    void printStatus() const {
      Serial.print("Left: ");
      Serial.print(lastLeftSpeed);
      Serial.print(", Right: ");
      Serial.println(lastRightSpeed);
    }
};