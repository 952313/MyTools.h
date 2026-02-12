# MyTools.h
一个用于在Arduino中简化主程序的程序

## 用法
### 加入

你需要在程序开头加上
```cpp
#include "MyToolsVXX.h"
```
请把VXX改成你下载的版本号文件名
请把文件放在项目的同目录内

### BUTTON
这是按钮，可以通过`BUTTON button(pin);`注册一个名叫`button`的，引脚号是`pin`的按钮
> 高级用法
> 
> 世界上有两种按钮
> 
> 1.点击后高电平，平时低电平
> 
> 请用`BUTTON button(pin,m = 1);`注册
> 
> 2.点击后低电平，平时高电平
> 
> 请用`BUTTON button(pin,m = 0);`注册，m=0是可选参数，不写默认为0
> 
如何获取按钮状态？
你可以直接把按钮类别转换为bool类型，得出的结果表示是否按下
样例
```cpp
#include "MyToolsV5.h"

BUTTON button1(3);
void setup(){
  Serial.begin(9600);
}
void loop(){
  Serial.println((bool)button1);
```
在代码中，你的串口会收到一堆0，点击后是1
你也可以把`Serial.println((bool)button1);`改成`Serial.println(button1.GetStr);`会收到pressed（按压）或released（放开）

## 提示
欢迎提出Bug（提交至Issues）
如果你不确定是不是Bug，你也可以放进Issues。
