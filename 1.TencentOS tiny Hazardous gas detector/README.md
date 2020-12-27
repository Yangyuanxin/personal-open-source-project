# 腾讯物联网操作系统的危险气体探测仪

#### 目录

##### 00. src_code

项目源代码

##### 01. sd_card_res

SD卡资源

##### 02. framework

项目框架图

#### 介绍

硬件平台：```小熊派STM32L431RCTx开发板```

前面我们分享了几篇文章：

[基于事件型表驱动法菜单框架之小熊派简易气体探测器实战项目开发(上)](http://mp.weixin.qq.com/s?__biz=MzA3MjMzNTc4NA==&mid=2649001834&idx=1&sn=e8f4ee520328398c7791ff487a5ded12&chksm=870fc7adb0784ebbb61817309e503cd1cf170858ce7c291c63bb46808c48a6c144547f626462&scene=21#wechat_redirect)

[基于事件型表驱动法菜单框架之小熊派简易气体探测器实战项目开发(中)](http://mp.weixin.qq.com/s?__biz=MzA3MjMzNTc4NA==&mid=2649001954&idx=1&sn=b88e4057909af0303dfb021bca328266&chksm=870fc725b0784e33a52a61ee379688281cc2773a207622b15cf4dfafad5c96535dd2e4f4b226&scene=21#wechat_redirect)

[表驱动+状态机法AD传感器驱动检测框架](http://mp.weixin.qq.com/s?__biz=MzA3MjMzNTc4NA==&mid=2649002041&idx=1&sn=b33eac1c10a584523c5f7c5ad695c9cf&chksm=870fd8feb07851e8a66d4b9f146b87cb653625942217f5a8ff417c220fc2d212545baa470e6d&scene=21#wechat_redirect)

[有关版本等信息的重要性(以STM32产品开发为例)](http://mp.weixin.qq.com/s?__biz=MzA3MjMzNTc4NA==&mid=2649002172&idx=1&sn=55b5b31a1218c7e75867f66cce2dd557&chksm=870fd87bb078516d2bca305d1d7c4c420497d6f29b9599d3f9fc57671bab915dbc3710642208&scene=21#wechat_redirect)

[TencentOS tiny危险气体探测仪产品级开发](https://mp.weixin.qq.com/s/CVEUo7VqDfvVh2G7FYPdSQ)

[自己动手撸个简单的LCD驱动框架吧!](https://mp.weixin.qq.com/s/q4D1X0vSMqrUcGKHegkcCw)

[TencentOS tiny危险气体探测仪产品级开发重磅高质量更新](https://mp.weixin.qq.com/s/HJ19dNuedLz8bjh-f9HZZw)

但无奈硬件资源有限，无法完成太多功能，但我还是希望有朝一日，我能利用我身边的资源把它做成一个公模开源的手持式危险气体探测仪，并继续完善优化软件逻辑以及产品功能，甚至会加入一些标准化的东西(国标、行标、企标)，让它看起来真正的像一个产品，并且希望有机会能够量产，帮助更多的工程师入门。

## 1、软件组成

### 1.1、软件架构图

![img](https://imgconvert.csdnimg.cn/aHR0cHM6Ly9tbWJpei5xcGljLmNuL21tYml6X3BuZy9TWWljZUpLNzhDSThMRHhEZFZKWDBPWnN6NElvMFg2NmVRU2liRERlRExoNWxxbzZVenJjUmc4dnFKbVc1MkhQZkR3dDhGNFpDQ2VrME4wdkRkdjNMb21nLzY0MA?x-oss-process=image/format,png)

### 1.2、软件框架图

#### 1.2.1 整体框架图

![img](https://imgconvert.csdnimg.cn/aHR0cHM6Ly9tbWJpei5xcGljLmNuL21tYml6X2pwZy9TWWljZUpLNzhDSThicXZmaDA0dzRFNWx3S1pITzgwQVRPY2ZxZEFQN28wRm9KU2pEMnYxQVBvSWljUXliV01WdUZwV2RHb0JJM2J2dkhOME1uZWJOTGpBLzY0MA?x-oss-process=image/format,png)

#### 1.2.2 核心软件框架

#### 1.2.2.1 LCD软件框架

#### (1)LCD驱动框架数据结构

![img](https://imgconvert.csdnimg.cn/aHR0cHM6Ly9tbWJpei5xcGljLmNuL21tYml6X3BuZy9TWWljZUpLNzhDSWliUWxWTDl5V1FtMnN3dVZJbUlxZGhNZTBicE5McGlhUDJRNHNUVWlhRUd1SGlibm1razlwckpBb1Q4eGI0MTVRd2hTV3BQNXppYTQyWHFRZy82NDA?x-oss-process=image/format,png)

![img](https://imgconvert.csdnimg.cn/aHR0cHM6Ly9tbWJpei5xcGljLmNuL21tYml6X3BuZy9TWWljZUpLNzhDSWliUWxWTDl5V1FtMnN3dVZJbUlxZGhNaFpqQTJpY0plSjZPZkZIWkxFM29pYUJOUVk1ZzBxT0NBcEgzWEV4am5RdGliQ0hQMVVpY1ZzVHdxdy82NDA?x-oss-process=image/format,png)

这里提供了将驱动框架与驱动进行对接的能力，`&lcd_driver`拿到的是定义在驱动文件里的一个已经赋值了的结构体`lcd_driver`，这样，当我在别的地方定义一个`LCD_Driver_Model`的变量，就可以将这个变量与驱动结构体进行对接，这样就可以通过这个变量来操作驱动结构体里的接口了。

#### (2)LCD驱动数据结构

![img](https://imgconvert.csdnimg.cn/aHR0cHM6Ly9tbWJpei5xcGljLmNuL21tYml6X3BuZy9TWWljZUpLNzhDSWliUWxWTDl5V1FtMnN3dVZJbUlxZGhNMmljQ1VsSUhyUWljMnE3Sldzbm1BTVhpYVB5dTE2VGt3V3FPdkNCMGxDUkZWYXVGZFp6VXRMSDVnLzY0MA?x-oss-process=image/format,png)

LCD驱动这个数据结构要做的事情就是提供操作LCD驱动能力的接口，这个接口的设计与硬件无关。

![img](https://imgconvert.csdnimg.cn/aHR0cHM6Ly9tbWJpei5xcGljLmNuL21tYml6X3BuZy9TWWljZUpLNzhDSWliUWxWTDl5V1FtMnN3dVZJbUlxZGhNN3dFV2hlWkZDVVNhOUdhN1BUMmVNcVZkMTdWd1FpYXBtS1F5SURnTG9TVjBxN0xrak5rUzd4Zy82NDA?x-oss-process=image/format,png)

驱动框架依赖于驱动接口，这样的话我们需要实现驱动接口里的方法，在对应的方法里，我们要去调用LCD设备相关的接口，进而去操作LCD设备，以下是接口对应的实现：

![img](https://imgconvert.csdnimg.cn/aHR0cHM6Ly9tbWJpei5xcGljLmNuL21tYml6X3BuZy9TWWljZUpLNzhDSWliUWxWTDl5V1FtMnN3dVZJbUlxZGhNZ3hhYXFwZVZFUG5pYTdpY2ljbW9zajZQbzBQZzNvYXEyN1hpYzV6MkJCRTZGR3pDRTdZWGRla1NpYncvNjQw?x-oss-process=image/format,png)

#### (3)LCD设备数据结构

![img](https://imgconvert.csdnimg.cn/aHR0cHM6Ly9tbWJpei5xcGljLmNuL21tYml6X3BuZy9TWWljZUpLNzhDSWliUWxWTDl5V1FtMnN3dVZJbUlxZGhNT3NkZFk1NVZzMWthamxPam5tczdIM3BwWHVaaDVNMHIxZTd1b3hNanJpYVZkaWM2aWJQSlo1aWJtQS82NDA?x-oss-process=image/format,png)

LCD设备所需要做的事情就是将这个数据结构里的功能函数与真实的LCD驱动接口进行对接。比如我们看`LCD_Init`这个接口的实现，这个就是真实调用LCD的真实硬件操作了。





文档说明持续更新中。。。。。。





## 2、其它

另外要注意的地方，SD卡内存放的UI为显示界面一些图标存放的地方，其它的文件由软件自动生成：

- Para.ini(配置文件)

![img](https://imgconvert.csdnimg.cn/aHR0cHM6Ly9tbWJpei5xcGljLmNuL21tYml6X3BuZy9TWWljZUpLNzhDSThMRHhEZFZKWDBPWnN6NElvMFg2NmV4STVPdWIzSkFPcEVxRmFucGZUTWZtVnB1RXB5bDBjaWNZZ3F5RGJEM05OYUNpYUV3RXB3b0R3Zy82NDA?x-oss-process=image/format,png)

- BearPi_Log0.csv(检测记录存储文件)

![img](https://imgconvert.csdnimg.cn/aHR0cHM6Ly9tbWJpei5xcGljLmNuL21tYml6X3BuZy9TWWljZUpLNzhDSThMRHhEZFZKWDBPWnN6NElvMFg2NmV6d0NrQ2p2MVJLaWFEYWJQVXhVOGtLUjM1eHNCRXAxbDlScUc1cFlPVkpwcW50VzhRRmEzeUdBLzY0MA?x-oss-process=image/format,png)

![img](https://imgconvert.csdnimg.cn/aHR0cHM6Ly9tbWJpei5xcGljLmNuL21tYml6X3BuZy9TWWljZUpLNzhDSThMRHhEZFZKWDBPWnN6NElvMFg2NmVqYlFCNkZwcUZuMWRMTnFtblFFRHREb1RzaHlFZ05YbHRZT2czVGlhb2s5TE5DQXNZRUZXeG1BLzY0MA?x-oss-process=image/format,png)

本节代码已同步到码云的代码仓库中,获取方法如下：

#### 1、新建一个文件夹

![img](https://imgconvert.csdnimg.cn/aHR0cHM6Ly9tbWJpei5xcGljLmNuL21tYml6X3BuZy9TWWljZUpLNzhDSTlCSmlibTh1aWNGbFNpYmljRjUxV0dQbGJsVjN1MHdBM2FFTzEzenZuU1JzRUxtUXhRMlVFaENGN0tGWWpBcXEyRncwc21ueFFudDBpYnBoQS82NDA?x-oss-process=image/format,png)

#### 2、使用git clone远程获取代码仓库

项目开源仓库：

```go
https://gitee.com/morixinguan/tencent-os-tiny-hazardous-gas-detector.git
```

![img](https://imgconvert.csdnimg.cn/aHR0cHM6Ly9tbWJpei5xcGljLmNuL21tYml6X3BuZy9TWWljZUpLNzhDSTlCSmlibTh1aWNGbFNpYmljRjUxV0dQbGJsMEZpYmlhaDlTSXFWYmhETkRVcXRxTVcybmtTR3hpYWFuWjlFOFRQV3k5YlFhVU41c3BFU3VpYkZhZy82NDA?x-oss-process=image/format,png)

![img](https://imgconvert.csdnimg.cn/aHR0cHM6Ly9tbWJpei5xcGljLmNuL21tYml6X3BuZy9TWWljZUpLNzhDSTlCSmlibTh1aWNGbFNpYmljRjUxV0dQbGJsNVlDakVCRkpSMWhraWFEbk9HZ205YkdBb1dCS1k2TnFXSGVEMHljc09sQWExMDBUT2RqQ2toZy82NDA?x-oss-process=image/format,png)