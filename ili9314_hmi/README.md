# lil9341_HMI
```
该项目基于 esp32-s2-hmi-devkit 开发板设计，采用esp32s2 wrover模组，屏幕采用lil9314，lgvl界面。
```
[![Fork me on Gitee](https://gitee.com/chenqinglang/ili9314_hmi/widgets/widget_3.svg)](https://gitee.com/chenqinglang/ili9314_hmi)
## Photo
```
因设计失误，实物机械按键位置布局不当，只能安装一个机械按键，PCB文件已改进，年后重新打样3D。
```

<img src=./doc/image/image(2).png width=50% />
<img src=./doc/image/image(1).png width=50% />
<img src=./doc/image/image(3).png width=50% />
<img src=./doc/image/image(4).jpg width=50% />
<img src=./doc/image/image(5).jpg width=50% />
<img src=./doc/image/image(6).jpg width=50% />
<img src=./doc/image/image(7).jpg width=50% />

## 内存使用
```
idf.py menuconfig 中开启 Try to allocate memories of WiFi and LWIP in SPIRAM firstly. If failed, allocate inter ，优先把 WIFI LWIP 内存防止到片外RAM。
关闭 Enable LWIP IRAM optimization，关闭 WIFI RX IRAM speed optimization.
```
<img src=./doc/image/image(8).png width=80% />
<img src=./doc/image/image(9).png width=80% />
<img src=./doc/image/image(10).png width=80% />

## LVGL使用注意
```
使用 keyboard 依赖 textarea 时候，需要删除 keyboard 与 textarea，需要先删除 keyboard 再删除 textarea
```
```
点击键盘获取焦点后，导航会被设置为编辑模式，关闭键盘后需要执行 lv_group_set_editing(group, false); 设置会导航模式，否则会影响工作
```
```
lv_group_add_obj(group, keyboard_pwd); 前先 lv_group_set_editing(group, true); 设置为编辑模式，可以马上让键盘获取焦点免去要先点击一次获取焦点
```
```
在右下角显示CPU使用率和FPS计数，"lv_conf.h"
#define LV_USE_PERF_MONITOR     0
```