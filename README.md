# 七牛云播放SDK

## 阅读对象
本文档面向所有使用七牛云播放SDK的开发、测试人员等, 要求读者具有一定的Android， iOS， Windows 编程开发经验。

## 1.产品概述

七牛云播放内核涵盖Android、iOS、Windows三个平台，作为一款全平台兼容的软件播放方案，提供了跨终端平台的播放器SDK，以及开放的音视频播放、控制接口和完整的开源调用示例，不仅极大降低开发门槛，同时支持客户快速在多个平台发布产品。  

简要说明：

* **不限制**用户的拉流地址。用户可以只使用七牛云直播SDK而不使用七牛云的云服务。
* 不收取任何授权使用费用(**免费使用**)，不含任何失效时间或者远程下发关闭的后门，没有任何用户标识信息。
* 当前未提供开源代码，如果需要其他定制化开发功能，请通过七牛云联系。

### 1.1 版本信息

| Included Project	 | LICENSE	| VERSION |
|:----------:|:---------------------------:|:--------------:|
| ffmpeg	| LGPL 2.1 or later	| 3.1 |
| openssl	| BSD-Style License	| 1.1.0f |
| libyuv	| BSD-Style License	| 1487 |
| libspeex	| BSD-Style License	| 1.2.0 |

### 1.2 关于费用
七牛云SDK保证，提供的SDK可以用于商业应用，不会收取任何SDK使用费用。

## 2.SDK 功能说明

- [x] 支持首屏秒开。**任意长度**的MP4文件都可以在一秒以内打开播放。直播RTMP，HTTP-FLV和短视频点播都在300毫秒以内。
- [x] 支持预加载。仅下载文件头，节约流量。
- [x] 支持直播时采用变速不变调降低主播和观众的延迟。
- [x] 支持多种域名解析。HTTP-DNS, UDP-DNS, SYS-DNS.
- [x] 支持**RTMP**和**HTTP+FLV**直播方式。
- [x] 点播支持多种格式(MP4、FLV、MP3、M3U8、AAC)。
- [x] 支持视频解码（H264，H265，MPEG4）。
- [x] 支持音频解码（AAC，MP3，SPEEX）。
- [x] 支持软解和硬解 （Android 和 iOS）。
- [x] 支持播放截图。
- [x] 支持快速或慢速播放。（0.1 - 8 倍速）
- [x] 支持文件缓存。
- [x] 支持多码流自适应播放，无缝切换。
- [x] 支持断网时自动重连。
- [x] 支持多实例。
- [x] 支持音量调节，可静音播放。
- [x] 支持后台播放。
- [x] 支持纯音频播放。
- [x] 支持H.265/HEVC播放。
- [x] 支持音量放大，最大可以放大四倍。
- [x] 可设置播放器直播场景下最大缓存时长。

### 2.1 目录说明

#### 2.1.1 安卓系统
* 示例：android/samplePlayer
* 库文件：android\samplePlayer\app\libs。包含 armeabi， armeabi-v7a， arm64-v8a， x86。

#### 2.1.2 苹果系统
* 示例：iOS/Sample
* 库文件：iOS/bin, include, lib

#### 2.1.3 Window
* 示例：win32/SamplePlayer
* 库文件：iwin32/SamplePlayer/bin, include


### 2.2 SDK文档

参见各系统里面的doc目录。(未完成）



