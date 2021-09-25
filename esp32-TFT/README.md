# esp32 TFT彩屏投屏

![投屏测试](https://images.gitee.com/uploads/images/2021/0811/201012_5c0e385c_8230134.jpeg "46CE76441F9755A60DA0D1C276951F2F.jpg")

#### 介绍
彩屏投屏助手 利用tcp协议 将电脑屏幕实时发送给esp32处理显示，理论10帧以上

新版投屏助手1.3（只能使用esp32—sever版arduino程序）：路由器网络240x320分辨率可达10帧，已经测试。

建议用电脑给esp32开热点，传输速度会比连接路由器快（原因在问题发现版块）

作者：大超

B站:super大大怪i

创了一个交流群：519926351，群类有esp32离线安装包，库文件和引脚图，有问题可以讨论

彩屏投屏助手链接：

链接：https://pan.baidu.com/s/1N6OwRVh6Bafnsij4Yy-JKw 
提取码：kxco

![输入图片说明](https://images.gitee.com/uploads/images/2021/0715/165024_1a872ad8_8230134.png "%]@9]RCQARY~TCD_`)X}D1G.png")

#### 软件架构
软件架构说明

上位机：采用Python编程，ui界面用的python自带的tkinter，图像处理采用opencv，数据传输基于tcp协议

下位机：采用arduino编程，显示库采用TFT_eSPI,jpg图片解码采用 [Tjpg_Decoder](https://github.com/Bodmer/TJpg_Decoder)，两个库需要自行安装

#### 安装教程

如果你是其他尺寸屏幕的话，可以修改TFT_eSPI 里的 User_Setup.h ，记得把spi速度改为80M（有的屏幕80M可能会白屏，适当调低），屏幕接线和TFT_espi配置教程->[配置教程](https://blog.csdn.net/finedayforu/article/details/108975245)

#### 使用说明


1.  电脑和esp32必须要在同一局域网内,可以电脑给esp32开热点连接，将arduino 里的 wifi 密码 上位机ip 端口（上位机开启投屏时会显示）修改好后，烧录至esp32，pc端打开彩屏投屏助手，选择好屏幕参数，点击开始投屏，如果没有你的屏幕尺寸可以手动修改‘宽x高’ 以小写x分割，然后复位esp32连接。
2.  镜像和质量选项，可以在数据传输中设置，彩屏动态显示，质量设置越低，画质越模糊，同时图片内存会变小，默认95，不建议往上加（内存大，esp32处理慢），可以根据自己需要适当降低提高帧率
3.  截屏区域，屏幕尺寸，同异步，在开启投屏前设置
4.  工作流程：esp32显示完一帧后，会向上位机发送请求下一帧指令‘ok’，没有处理完当前帧就会发送‘no’，上位机收到ok后才发送下一帧，这样避免了沾包问题

#### 注意事项

1.由于tcp高速传输数据耗费CPU资源较大，esp32发热严重，加上80Mspi，芯片几乎是全频工作，平均电流为0.21A。芯片过热，数据传输可能会中断。
2.若出现主机发送帧率较低情况，网络原因，线路拥塞，建议用无线网卡开热点。


#### 同步异步对比

投屏助手1.0版本

![同步](https://images.gitee.com/uploads/images/2021/0717/113024_137b901e_8230134.png "OF~{Z[$(~PUA{PTJ_MAYVQ8.png")

![异步](https://images.gitee.com/uploads/images/2021/0717/113043_bd2d4b83_8230134.png "(AUJT7TZZIA9OO[EWS@BILI.png")

############################################

投屏助手1.1版本

![高帧率](https://images.gitee.com/uploads/images/2021/0723/183928_04dcf880_8230134.png "45D1B7F8CC075C210CF08C98FEEC88F4.png")

投屏助手1.2版本

![传输方式变化](https://images.gitee.com/uploads/images/2021/0801/134613_44d444b4_8230134.png ")00KTN[UCT(FFGK$582Q_FA.png")

#### 问题发现
一、为什么esp32连接电脑热点的传输速度比路由器的高？

路由器作为中间层：二次转发数据有时延（主要原因），多设备连接，网络波动大，tcp经常丢包重传，传输耗时严重
电脑热点：连接设备单一，网络干扰小，传输稳定，延时小，直接和esp32组成回环网络，没有路由器等中间层转发数据，速度快

二、
1.1版本 点击开始投屏后，出现报错 Error: [WinError 10013] 以一种访问权限不允许的方式做了一个访问套接字的尝试

原因上位机默认端口8080被占用

解决方法：win+R 输入cmd打开命令行依次输入：

 **netstat -ano|findstr 8080** 

 **tasklist |findstr pid号** 

 **taskkill /pid pid号 /F**  

![端口占用解决](https://images.gitee.com/uploads/images/2021/0724/111205_e1a55780_8230134.png "端口占用解决.png")

三、
不同的esp32固件包，代码执行效率不一样，下面对比 vscode+platformio 上面安装的esp32 dev包，和arduino 老版本esp32 dev包 

![platformio](https://images.gitee.com/uploads/images/2021/0723/182843_38ce2e89_8230134.png "`BE8P[6DKL)9XP}J({Y{L13.png")

![arduino](https://images.gitee.com/uploads/images/2021/0723/182942_bf079584_8230134.png "MB5D](QWP$[KAD4_HE03V)3.png")

#### 版本区别 

软件帮助菜单里有更新详情

1.1：自定义截屏区域，极大提升截屏帧率

1.1.0：在1.1基础上增加端口选择设置，修复帧过大停止发送的问题。

1.2：增加端口选择设置，关闭tcp nagle算法，数据改为整包发送，socket非阻塞式收发数据

1.3：esp32作为服务端，Pc作为客户端，解决连接路由器，esp32传输速度慢，导致帧率低的情况

选择：下位机建议选择DMA版本，提升解码速度，上位机路由器网络建议使用1.3版本（需使用esp32-server版本下位机程序），电脑使用热点1.1.0版本自我感觉比较快，因电脑和网络环境不同，自行对比速度

![版本区别](https://images.gitee.com/uploads/images/2021/0811/195245_cf2ad4d3_8230134.png "1.3.png")
