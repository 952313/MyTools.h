# MyTools.h – Arduino 工具库

**版本：V5**  
**许可证：GNU Affero General Public License v3.0**  
**作者：YangJincheng**  
**最后更新：2026年2月**

---

## 📦 简介

`MyTools.h` 是一个为 Arduino 平台设计的轻量级工具库，旨在通过封装常用硬件组件（如电机、灰度传感器、按键、LED 等）来简化主程序代码，提升代码可读性和复用性，遵循 **DRY（Don‘t Repeat Yourself）** 原则。

---

## 📁 文件结构

- `MyToolsV5.h`：主头文件，包含所有类定义
- 使用时只需将此文件放入项目目录，并在代码中 `#include "MyToolsV5.h"`

---

## 🧰 功能模块

### 1. `SingleMotor` – 单路电机控制

#### 功能
控制一个直流电机的正转、反转、刹车、滑行。

#### 构造函数
```cpp
SingleMotor motor(forwardPin, backwardPin);
```
- `forwardPin`：正转引脚（PWM）
- `backwardPin`：反转引脚（PWM）

#### 常用方法
| 方法 | 说明 |
|------|------|
| `setSpeed(speed)` | 设置速度 -255~255（负值反转） |
| `operator()(speed)` | 同 `setSpeed` |
| `brake()` | 快速刹车（双高） |
| `coast()` | 滑行（双低） |
| `dynamicBrake()` | 动态刹车（PWM=0） |

---

### 2. `Motors` – 双电机控制器

#### 功能
控制左右两个电机，适用于小车底盘。

#### 构造函数
```cpp
Motors motors(leftForward, leftBackward, rightForward, rightBackward);
```
默认引脚：左(6,9)，右(10,11)

#### 常用方法
| 方法 | 说明 |
|------|------|
| `operator()(leftSpeed, rightSpeed)` | 设置左右电机速度 |
| `forward(speed)` | 前进 |
| `backward(speed)` | 后退 |
| `turnLeft(speed)` | 左转 |
| `turnRight(speed)` | 右转 |
| `rotate(speed)` | 原地旋转 |
| `coast()` / `brake()` | 滑行 / 刹车 |
| `stop(useBrake)` | 停止，默认滑行 |
| `safeDrive(l, r, max)` | 限制速度范围 |
| `getCurrentSpeed()` | 获取当前速度 |
| `printStatus()` | 打印速度状态 |

---

### 3. `SingleGrayscaleSensor` – 单路灰度传感器

#### 功能
检测黑白状态，支持阈值判断。

#### 构造函数
```cpp
SingleGrayscaleSensor sensor(pin, threshold);
```

#### 常用方法
| 方法 | 说明 |
|------|------|
| `getValue()` | 获取原始模拟值 |
| `isBlack()` / `isWhite()` | 判断黑白 |
| `operator bool()` | 是否检测到黑线 |
| `getStr()` | 返回 "BLACK"/"WHITE" |

---

### 4. `GrayscaleSensors` – 双路灰度传感器

#### 功能
封装左右两路灰度传感器，适用于巡线小车。

#### 构造函数
```cpp
GrayscaleSensors sensors(leftPin, leftThresh, rightPin, rightThresh);
```

#### 常用方法
| 方法 | 说明 |
|------|------|
| `getValueLeft()` / `getValueRight()` | 原始值 |
| `isLeftBlack()` / `isLeftWhite()` | 左状态 |
| `isRightBlack()` / `isRightWhite()` | 右状态 |
| `isBothBlack()` / `isBothWhite()` | 双黑 / 双白 |
| `isOnLine()` / `isOffLine()` | 是否在线 / 离线 |
| `isOnTrack()` | 双黑 |
| `getDirection()` | -1右转，1左转，0直行 |
| `getStr()` | 状态字符串 |

---

### 5. `BUTTON` – 按键输入

#### 功能
支持高低电平触发，去抖需自行处理。

#### 构造函数
```cpp
BUTTON btn(pin, mode = 0);
```
- `mode=0`：按下为高电平
- `mode=1`：按下为低电平

#### 常用方法
| 方法 | 说明 |
|------|------|
| `isPressed()` | 是否按下 |
| `operator bool()` | 同 `isPressed` |
| `GetStr()` | 返回 "pressed"/"released" |
| `printStatus()` | 打印状态 |
| `waitUntilRelease()` | 阻塞直到松开 |

---

### 6. `LED` – 数字输出控制

#### 功能
控制 LED 的亮灭，支持高低电平触发模式。

#### 构造函数
```cpp
LED led(pin, mode = 0);
```
- `mode=0`：低电平点亮
- `mode=1`：高电平点亮

#### 常用方法
| 方法 | 说明 |
|------|------|
| `on()` / `off()` | 开/关 |
| `toggle()` | 切换状态 |
| `operator()(state)` | 直接设置状态 |
| `isOn()` | 返回是否亮起 |
| `test()` | 闪烁测试（需 Serial） |

---

### 7. `MyPair` – 通用键值对容器

#### 功能
存储两个任意类型的值，支持构造、输出。

#### 示例
```cpp
MyPair<int, float> p(10, 3.14);
Serial.println(p.toString()); // "(10, 3.14)"
```

---

## 🚀 快速示例

```cpp
#include "MyToolsV5.h"

Motors motors;
GrayscaleSensors sensors(A0, 500, A1, 500);
BUTTON btn(2);
LED led(13);

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (btn.isPressed()) {
    led.toggle();
    delay(200);
  }

  if (sensors.isOnLine()) {
    int dir = sensors.getDirection();
    motors(dir * 100, -dir * 100);  // 转向
  } else if (sensors.isBothBlack()) {
    motors.forward(150);
  } else {
    motors.coast();
  }

  Serial.println(sensors.getStr());
}
```

---

## 🧪 注意事项

- 本库不包含软件去抖，请在需要时自行添加 `delay()` 或使用去抖算法。
- 模拟引脚请使用 `A0, A1…` 等宏定义。
- 所有类均设计为**轻量、无动态内存分配**，适合资源受限的 Arduino 环境。
- 如需扩展功能，建议通过继承或组合方式实现，而非修改库文件。

---

## 🐞 问题反馈

如果你在使用过程中发现 Bug，或有功能建议，欢迎提交至 Issues 页面。  
即使不确定是否为 Bug，也欢迎提出讨论。
