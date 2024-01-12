# HomeAssistant 点灯工程

本工程基于安信可中间件实现所有逻辑，主要在Ai-WB2-12F-Kit 上完成所有开发，因此，Ai-WB2-12F-Kit能够非常顺利运行本工程。
>安信可中间件开源仓库地址：
>
>https://github.com/Ai-Thinker-Open/aithinker_dev_open_sdk

本工程主要用于接入HomeAssistant平台，主要实现以下功能：
- 安信可IOT 微信小程序BluFi 配网
- 上电自动连接
- RGB 灯网络指示灯（默认蓝色）
- 接入HomeAssistant 平台（http://47.112.190.192:8123）
- 实现自动设备发现

这些功能已经满足HomeAssistan的基本应用，可以实现点亮一盏LED，默认点亮绿色灯。当在Ai-WB2-12F上运行时，它在HomeAssistant上的信息：

![Alt text](IMG/Ai-WB2-DEV.png)

## 工程克隆

因为本工程实在安信可中间件中运行，因此确保你已经有一个完整的安信可中间件SDK。然后在*aithinker_dev_open_sdk/application/application_version*目录当中创建一个<.txt>文件并命名为：**aihomeassistant_basic_version.txt**
之后在文件当中输入以下内容：
```
name:aihomeassistant_basic
branch:master
commit:
repository:https://e.coding.net/axk/c-hi-obj/aihomeassistant_basic.git


```
>**注意：** 文件的结尾必须要有个空的新行

完成创建之后，回到 aithinker_dev_open_sdk 的根目录，并使用指令：
```
./build.sh update
```
更新SDK，同时把本工程拉取到  <kbd>aithinker_dev_open_sdk</kbd> 的<kbd>application</kbd>,以 <kbd>aithinker_dev_open_sdk</kbd>的一个example 存在。

## 配置工程

安信可中间件支持 <kbd>menuconfig</kbd>配置，在正式编译工程之前，需要做一些配置，例如需要连接的MQTT服务器等，当然，也可以使用默认的配置。
```
./build.sh bl602 aihomeassistant_basic menuconfig
```
之后应该会有以下界面：

![Alt text](IMG/menuconfig.png)

需要使用默认的配置的话，可以选择 *\<Exit\>* 直接退出。
> 默认配置：
> - MQTT server: **wx.ai-thinker.com:1883**
> - HomeAssistant automatic: **homeassistant**
> - HomeAssistant discovery: **Switch**
> - HomeAssistant device name:**Ai-WB2-xxxx** (xxxx为sta mac后四位字符)
> - 网络指示灯：Blue （蓝色）
> - 配网按键：IO8 (Ai-WB2-Kit)

在<kbd>menuconfig</kbd>配置界面中。*HomeAssistant config*选项可以配置**MQTT server 、HomeAssistant automatic 、HomeAssistant discovery和HomeAssistant device name**；
而**网络指示灯和配网按键** 则分别在 *RGB config和buttom config* 选项中选择。完成配置之后，即可保存退出

## 编译工程
在<kbd>aithinker_dev_open_sdk</kbd>跟目录使用：
```
./build.sh bl602 aihomeassistant_basic cn debug
```
即可发起编译，编译成功后，App文件会存放在 *aithinker_dev_open_sdk/out/bl602/aihomeassistant_basic/bin/dev_project.bin*
烧录时，选择该文件即可。