# Firmware
#### 1、介绍

这是一个基于沁恒CH32V_EVB开发套件的机械手云控制系统方案的固件部分。

#### 2、固件源代码架构(Software)

该项目的固件层分为APP、Bsp、Common、Hardware、McuPlatform、OperatingSystem这几个部分，每个部分的功能如下：

##### 2.1、APP

主要为本项目的业务逻辑部分，主要分为串口数据转发、Mqtt订阅和发布业务处理等。

##### 2.2、Bsp

主要是由Mcu平台提供的板级支持包，目前该部分仅支持CH32V_EVB平台。

##### 2.3、Common

主要集成了提供给项目使用的软件组件，其中包括ringbuffer、cJSON、Mqtt、Log系统等其它组件。

##### 2.4、Hardware

主要实现了一些完成项目必备的硬件驱动程序，包括按键驱动、LCD驱动、LED驱动、WIFI驱动等。

##### 2.6、McuPlatform

具体的Mcu平台，目前该部分仅支持CH32_EVB平台。

##### 2.7、OperatingSystem

操作系统层，目前仅包含TencentOS-tiny，它是一个内存占用极小、组件丰富且效率极高的实时操作系统。
