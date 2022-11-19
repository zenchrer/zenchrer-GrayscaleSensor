# zenchrer-GrayscaleSensor #
## 项目介绍 ##
&nbsp;&nbsp;&nbsp;&nbsp;本项目是一种适用于机器人赛道巡线的数字量灰度传感器，可安装IRT9909集成红外传感器以及其他3mm光敏二极管，内置STM32G030F6P6微控制器，可以实现赛道手动按键校准以及串口回传传感器数据等功能。<br>&nbsp;&nbsp;&nbsp;&nbsp;硬件使用立创EDA专业版绘制，请使用立创eda专业版打开工程。软件使用Keil MDK+STM32CubeMX编写。
***
# 注意 #
+ I2C版本建议使用100kbps频率，不容易遇到bug
+ 存在于`I2C版本`，使用MDK ac5选项编译会硬件上访，使用 MDK ac6则不会
+ 提一嘴，使用Embedded IDE里面的ac5配置没有bug，暂时没搞明白哪里的问题
# **使用说明** #

## 按键操作
### 单击按键 ###
+ 在`I2C版本`中加入单击按键查看器件id（在查看id模式下，闪烁的LED数量即器件id）
  + 进入查看id模式后，单击更改id（单击时间间隔需大于100ms）
  + 查看id模式三秒无操作自动退出 
+ `Uart版本`此操作无效
### 双击按键 ###
+ 双击按键进入校准模式，进入后依次让每个传感器采集到赛道线与赛道背景信息后，双击按键退出校准模式，退出后赛道数据会保存到MCU的内部flash中，掉电不丢失。
### 更新固件 ###
+ 关于固件升级
  + 请按照`S1->CLK`、`S2->SWD`的方式接入Debugger 
  + `UART版本`烧录前，请使用main.h中的DEBUG宏定义来开启关闭SWD调试功能，
    + 关闭SWD调试烧录后，可执行`全片擦除`后再烧录新的固件
  + `I2C版本`使用长按开启或关闭SWD接口
  + 开启SWD后S1，S2引脚以及对应的LED失效
## 通信协议
### I2C版本 ###
+ 一条I2C总线上可挂载多个id不同（可单击更改）的传感器，Demo展示了同时挂载两个传感器的例子
### UART版本 ###
+ 一发一收，主机发送`0x57`，传感器返回一个`HEX`数据，解析为`BIN`即为灰度数据