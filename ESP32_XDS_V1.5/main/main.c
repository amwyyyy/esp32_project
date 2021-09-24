#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_err.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"
#include "/Users/denis/IdeaProjects/esp-idf/components/wpa_supplicant/include/esp_supplicant/esp_wpa2.h"
#include "esp_wifi.h"
#include "/Users/denis/IdeaProjects/esp-idf/components/nvs_flash/include/nvs_flash.h"
#include "/Users/denis/IdeaProjects/esp-idf/components/spi_flash/include/esp_partition.h"
#include "esp_netif.h"
#include "esp_smartconfig.h"
#include "esp_sntp.h"
#include "/Users/denis/IdeaProjects/esp-idf/examples/common_components/protocol_examples_common/include/protocol_examples_common.h"
#include "/Users/denis/IdeaProjects/esp-idf/examples/common_components/protocol_examples_common/include/addr_from_stdin.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
// Littlevgl 头文件
#include "lvgl/lvgl.h"	  // LVGL头文件
#include "lvgl_helpers.h" // 助手 硬件驱动相关
#include "lv_examples/src/lv_demo_widgets/lv_demo_widgets.h"

/* 宏定义WiFi更新标识码、WiFi名称和密码 */
#define MY_WIFI_UPDATE  520               
#define MY_WIFI_SSID    "WiFi"
#define MY_WIFI_PASSWD  "passwd"

#define TAG "LittlevGL Demo"
#define LV_TICK_PERIOD_MS 10

/* 宏定义WiFi连接事件标志位、连接失败标志位及智能配网标志位 */
#define  CONNECTED_BIT        BIT0   
#define  ESPTOUCH_DONE_BIT    BIT1   
#define  WIFI_FAIL_BIT        BIT2   

/* 定义一个WiFi连接事件标志组句柄 */
static EventGroupHandle_t s_wifi_event_group;

//http组包宏，获取日历信息及城市拼音的http接口参数
#define WEB_SERVER "i.tianqi.com"
#define WEB_URL "http://i.tianqi.com/index.php?c=code&a=getcode&id=25&py="  

//http组包宏，获取城市代码的http接口参数
#define WEB_SERVER1 "toy1.weather.com.cn"
//http组包宏，获取天气的http接口参数
#define WEB_SERVER2 "www.weather.com.cn"


//http请求包  GET
static const char *REQUEST = "GET " WEB_URL " HTTP/1.1\n"
    "Host: "WEB_SERVER"\n"
    "User-Agent: Mozilla/5.0 (iPhone; CPU iPhone OS 11_0 like Mac OS X) AppleWebKit/604.1.38 (KHTML, like Gecko) Version/11.0 Mobile/15A372 Safari/604.1\n"
    "\n";

char *T_cspy = NULL; // 缓存数组指针--保存城市拼音
char cspy2[200] ="GET http://toy1.weather.com.cn/search?cityname=";
char *REQUEST1 = " HTTP/1.1\r\n"
    "Host: "WEB_SERVER1"\r\n"
    "User-Agent: Mozilla/5.0 (iPhone; CPU iPhone OS 11_0 like Mac OS X) AppleWebKit/604.1.38 (KHTML, like Gecko) Version/11.0 Mobile/15A372 Safari/604.1\r\n"
    "\r\n";
char *T_cspy2 =&cspy2[0];

char *T_csdm = NULL; // 缓存数组指针--保存城市代码
char p[200] ="GET http://www.weather.com.cn/weather/";
char *REQUEST2 = ".shtml HTTP/1.1\r\n"
	"Host: "WEB_SERVER2"\r\n"
	"User-Agent: Mozilla/5.0 (iPhone; CPU iPhone OS 11_0 like Mac OS X) AppleWebKit/604.1.38 (KHTML, like Gecko) Version/11.0 Mobile/15A372 Safari/604.1\r\n"
	"\r\n";
char *pp =&p[0];

struct 
{
   char  *rizi;      // 日期
   char  *xwtianqi;  // 天气图标
   char  *tianqi;    // 天气
   char  *zgwendu;   // 最高温度
   char  *zdwendu;   // 最低温度
   char  *fengx;     // 风向
   char  *fengji;    // 风级
   char  biaozhiwei; // 标志位
} weather1, weather2, weather3;

//图片申明
LV_IMG_DECLARE(ewm);     
LV_IMG_DECLARE(dl_0);    
LV_IMG_DECLARE(dl_1);    
LV_IMG_DECLARE(dl_2);    
LV_IMG_DECLARE(dl_3);    
LV_IMG_DECLARE(windows_kj0);   
LV_IMG_DECLARE(windows_gd0);    
LV_IMG_DECLARE(windows_gd1);    
LV_IMG_DECLARE(windows_gd2);    
LV_IMG_DECLARE(windows_gd3);    
LV_IMG_DECLARE(windows_gd4);    
LV_IMG_DECLARE(windows_gd5);    
LV_IMG_DECLARE(windows_gd6);   
LV_IMG_DECLARE(windows_gd7);    
LV_IMG_DECLARE(windows_gd8);    
LV_IMG_DECLARE(windows_gd9);    
LV_IMG_DECLARE(windows_gd10);   
LV_IMG_DECLARE(windows_gd11);   
LV_IMG_DECLARE(bpbj0);   
LV_IMG_DECLARE(bpbj1);  
LV_IMG_DECLARE(bpbj2);   
LV_IMG_DECLARE(bpbj3);  
LV_IMG_DECLARE(bpbj4);   
LV_IMG_DECLARE(bpbj5);   
LV_IMG_DECLARE(bpbj6);   
LV_IMG_DECLARE(bpbj7);  
LV_IMG_DECLARE(bpbj8);  
LV_IMG_DECLARE(bpbj9);   
LV_IMG_DECLARE(tq_0);   
LV_IMG_DECLARE(tq_1);   
LV_IMG_DECLARE(tq_2);    
LV_IMG_DECLARE(tq_3);    
LV_IMG_DECLARE(tq_4);    
LV_IMG_DECLARE(tq_5);   
LV_IMG_DECLARE(tq_6);   
LV_IMG_DECLARE(tq_7);   
LV_IMG_DECLARE(tq_8);   
LV_IMG_DECLARE(tq_9);    
LV_IMG_DECLARE(tq_10);   
LV_IMG_DECLARE(tq_11);  
LV_IMG_DECLARE(tq_12);   
LV_IMG_DECLARE(tq_13);  
LV_IMG_DECLARE(tq_14);   
LV_IMG_DECLARE(tq_15);   
LV_IMG_DECLARE(tq_16);  
LV_IMG_DECLARE(tq_17);   
LV_IMG_DECLARE(tq_18);   
LV_IMG_DECLARE(tq_19);   
LV_IMG_DECLARE(tq_20);   
LV_IMG_DECLARE(tq_21);   
LV_IMG_DECLARE(tq_22);   
LV_IMG_DECLARE(tq_23);   
LV_IMG_DECLARE(tq_24);   
LV_IMG_DECLARE(tq_29);   
LV_IMG_DECLARE(tq_30);   
LV_IMG_DECLARE(tq_31);   
LV_IMG_DECLARE(tq_32);   
LV_IMG_DECLARE(tq_49);  
LV_IMG_DECLARE(tq_53);   
LV_IMG_DECLARE(tq_54);  
LV_IMG_DECLARE(tq_55);   
LV_IMG_DECLARE(tq_56);  
LV_IMG_DECLARE(tq_57);   
LV_IMG_DECLARE(tq_58);  
LV_IMG_DECLARE(tq_99);   
LV_IMG_DECLARE(tq_301);   
LV_IMG_DECLARE(tq_302);  
LV_IMG_DECLARE(wztb_32); 

//汉字声明
// LV_IMG_DECLARE(myfont_cshz_18); 
// LV_IMG_DECLARE(myfont_cshz_24); 
// LV_IMG_DECLARE(myfont_3500hz_18); 
LV_FONT_DECLARE(myfont_cshz_18);
LV_FONT_DECLARE(myfont_cshz_24);
LV_FONT_DECLARE(myfont_3500hz_18);


//img 图像控件声明，声明在外部回调中需要调用
lv_obj_t *img1 = NULL;  
lv_obj_t *img2 = NULL;  
lv_obj_t *img3 = NULL;  
lv_obj_t *img4 = NULL;  

//标签控件声明，声明在外部回调中需要调用
lv_obj_t *label2 = NULL;  
lv_obj_t *label3 = NULL;  
lv_obj_t *label4 = NULL;  
lv_obj_t *label5 = NULL;  
lv_obj_t *label5_1 = NULL;  
lv_obj_t *label5_2 = NULL;  
lv_obj_t *label5_3 = NULL; 
lv_obj_t *label5_4 = NULL;  
lv_obj_t *label5_5 = NULL;  
lv_obj_t *label6 = NULL;  
lv_obj_t *label6_1 = NULL;  
lv_obj_t *label6_2 = NULL;  
lv_obj_t *label6_3 = NULL;  
lv_obj_t *label6_4 = NULL;  
lv_obj_t *label6_5 = NULL;  
lv_obj_t *label7 = NULL;  
lv_obj_t *label7_1 = NULL;  
lv_obj_t *label7_2 = NULL;  
lv_obj_t *label7_3 = NULL;  
lv_obj_t *label7_4 = NULL; 
lv_obj_t *label7_5 = NULL; 
//LVGL任务
lv_task_t *task1 = NULL;

//创建滚轮对象-显示农历等信息
lv_obj_t *roller1 = NULL;

//创建页签对象-显示天气信息
lv_obj_t *tabview = NULL;
//创建页签对象-显示背景图片
lv_obj_t *bjtuyq = NULL;

//创建一个信号量来处理对lvgl的并发调用
SemaphoreHandle_t xGuiSemaphore; // 创建一个GUI信号量

//创建句柄
TaskHandle_t xHandle_task1 = NULL;
TaskHandle_t xHandle_task2 = NULL;

static void lv_tick_task(void *arg);    //LVGL 时钟任务
static void gui_task(void *pvParameter);	 //GUI任务
static void dashboard_task(void *pvParameter);	 //表盘任务
static void smart_config_task(void *pvParameter);  //SmartConfig任务
void draw_img(void);

static void get_lunar_task(void *pvParameter);  //网络天气任务
static void get_city_code_task(void *pvParameter);  //网络天气任务
static void get_weather_task(void *pvParameter);  //天气网任务
static void sntp_task(void *pvParameter); //Sntp任务
void initialize_sntp(void);
void get_now_time(void);

// LVGL 时钟任务
static void lv_tick_task(void *arg)
{
	(void)arg;
	lv_tick_inc(LV_TICK_PERIOD_MS);
}

//分割函数
char *splitx(char *str,char *s,char *e){
    char *t1=NULL;
    char *t2=NULL;
    char *t3=NULL;
    uint8_t lensx=0;
    //在字符串str中查找第一次出现字符串s的位置，不包含终止符“\0”
    t1=strstr(str,s);
    if(t1==NULL){
        ESP_LOGI("FUNC_splitx","t1 valave is NULL");
        return ESP_OK;
    }
    //在字符串t1中查找第一次出现字符串e的位置，不包含终止符“\0”
    t2=strstr(t1,e);
    if(t2==NULL){
        ESP_LOGI("FUNC_splitx","t2 valave is NULL");
        return ESP_OK;
    }
    //printf("t1=%d\n t2=%d\n s=%d\n",strlen(t1),strlen(t2),strlen(s));
    lensx=strlen(t1)-strlen(t2)-strlen(s);//strlen返回计数器值（不包含“\0”）
    //printf("lensx=%d\n",lensx);
    char t='\0';
    t3=(char *)malloc(sizeof(char)*lensx+1);//malloc动态内存分配
    memset(t3,t,sizeof(char)*lensx+1); //为新申请的内存做初始化
    //把t1所指向的字符串中以t1地址开始的前lensx个字节复制到t3所指的数组中，并返回被复制后的t3
    strncpy(t3,t1+sizeof(char)*strlen(s),lensx);//strncpy将指定长度的字符串复制到字符数组中
    return t3;
 }

//初始化显示以及开机界面
//任务函数
static void gui_task(void *pvParameter)
{
	(void)pvParameter;
	xGuiSemaphore = xSemaphoreCreateMutex(); // 创建GUI信号量
	lv_init();								 // 初始化LittlevGL
	lvgl_driver_init();						 // 初始化液晶SPI驱动 触摸芯片SPI/IIC驱动

	// 初始化缓存
	static lv_color_t buf1[DISP_BUF_SIZE];
	static lv_color_t buf2[DISP_BUF_SIZE];
	static lv_disp_buf_t disp_buf;
	uint32_t size_in_px = DISP_BUF_SIZE;
	lv_disp_buf_init(&disp_buf, buf1, buf2, size_in_px);

	// 添加并注册触摸驱动
	lv_disp_drv_t disp_drv;
	lv_disp_drv_init(&disp_drv);
	disp_drv.flush_cb = disp_driver_flush;
	disp_drv.buffer = &disp_buf;
	lv_disp_drv_register(&disp_drv);

	// 定期处理GUI回调
	const esp_timer_create_args_t periodic_timer_args = {
		.callback = &lv_tick_task,
		.name = "periodic_gui"};
	esp_timer_handle_t periodic_timer;
	ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
	ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));

    //创建LVGL图片刷新任务
	draw_img();

	while (1)
	{
		vTaskDelay(1);
		// 尝试锁定信号量，如果成功，调用处理LVGL任务
		if (xSemaphoreTake(xGuiSemaphore, (TickType_t)10) == pdTRUE)
		{
			lv_task_handler();			   // 处理LVGL任务
			xSemaphoreGive(xGuiSemaphore); // 释放信号量
		}
	}
	vTaskDelete(NULL); 
}

static void wifi_event_handler(void* arg, esp_event_base_t event_base,int32_t event_id, void* event_data)
{
	static int retry_num = 0;           /* 记录wifi重连次数 */
	// STA开始工作
	if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
		esp_wifi_connect();   
		xTaskCreate(smart_config_task, "SmartConfig_Task", 1024*4, NULL, 3, NULL);//创建动态任务SmartConfig_Task
	}
	// STA 断开事件
	else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
		esp_wifi_connect();   
		retry_num++;
		ESP_LOGI(TAG,"retry to connect to the AP %d times. \n",retry_num);
		/* WiFi重连次数大于6 */
		if (retry_num > 6){xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);}
		xEventGroupClearBits(s_wifi_event_group, CONNECTED_BIT); 
	} 
	// STA 获取到IP
	else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
		xEventGroupSetBits(s_wifi_event_group, CONNECTED_BIT);  
		xEventGroupClearBits(s_wifi_event_group, WIFI_FAIL_BIT); 
	} 
	// SmartConfig 扫描完成
	else if (event_base == SC_EVENT && event_id == SC_EVENT_SCAN_DONE) {
		ESP_LOGI(TAG, "Scan done");  
	}
	// SmartConfig 找到信道
	else if (event_base == SC_EVENT && event_id == SC_EVENT_FOUND_CHANNEL) {
		ESP_LOGI(TAG, "Found channel");   
	}
	// SmartConfig 获取到WIFI名和密码
	else if (event_base == SC_EVENT && event_id == SC_EVENT_GOT_SSID_PSWD) {
		ESP_LOGI(TAG, "Got SSID and password");
		smartconfig_event_got_ssid_pswd_t *evt = (smartconfig_event_got_ssid_pswd_t *)event_data;//赋值账号和密码
		wifi_config_t wifi_config; 
		char ssid[33] = { 0 }; 
		char password[65] = { 0 };
		bzero(&wifi_config, sizeof(wifi_config_t));  
		memcpy(wifi_config.sta.ssid, evt->ssid, sizeof(wifi_config.sta.ssid));
		memcpy(wifi_config.sta.password, evt->password, sizeof(wifi_config.sta.password));
		wifi_config.sta.bssid_set = evt->bssid_set;
		if (wifi_config.sta.bssid_set == true) {
			memcpy(wifi_config.sta.bssid, evt->bssid, sizeof(wifi_config.sta.bssid));
		}

		memcpy(ssid, evt->ssid, sizeof(evt->ssid));   
		memcpy(password, evt->password, sizeof(evt->password));  
		ESP_LOGI(TAG, "SSID:%s", ssid);   
		ESP_LOGI(TAG, "PASSWORD:%s", password);  

		/* 将得到的WiFi名称和密码存入NVS*/
		nvs_handle wificfg_nvs_handler;
		ESP_ERROR_CHECK( nvs_open("WiFi_cfg", NVS_READWRITE, &wificfg_nvs_handler) );
		ESP_ERROR_CHECK( nvs_set_str(wificfg_nvs_handler,"wifi_ssid",ssid) );
		ESP_ERROR_CHECK( nvs_set_str(wificfg_nvs_handler,"wifi_passwd",password) );
		ESP_ERROR_CHECK( nvs_commit(wificfg_nvs_handler) ); /* 提交 */
		nvs_close(wificfg_nvs_handler);                     /* 关闭 */ 
		ESP_LOGI(TAG,"smartconfig save wifi_cfg to NVS .\n"); 

		// 断开默认的
		ESP_ERROR_CHECK( esp_wifi_disconnect() );
		// 设置获取的ap和密码到寄存器
		ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
		// 连接获取的ssid和密码
		ESP_ERROR_CHECK( esp_wifi_connect() );
	}
	// SmartConfig 发送应答完成
	else if (event_base == SC_EVENT && event_id == SC_EVENT_SEND_ACK_DONE) {
		xEventGroupSetBits(s_wifi_event_group, ESPTOUCH_DONE_BIT);
	}	
}

//LVGL任务二  
void task2_cb(void)
{
    static uint8_t i=0;
	if(i==0){
		i=1;
		lv_img_set_src(img4, &dl_0);
	} else if(i==1){
		i=2;lv_img_set_src(img4, &dl_1);
	} else if(i==2){
		i=3;
		lv_img_set_src(img4, &dl_2);
	} else if(i==3){
		i=0;
		lv_img_set_src(img4, &dl_3);
	}
}

//LVGL任务三 
void task3_cb(void)
{
	static uint8_t page_id = 0,page_id1 = 0;
	page_id++;

	if(page_id==4) {
		page_id = 0;
		page_id1++;
	}
	if(page_id1==4) {
		page_id1 = 0;
	}
	lv_tabview_set_tab_act(tabview,page_id,LV_ANIM_ON);//带有切换动画效果
	lv_tabview_set_tab_act(bjtuyq,page_id1,LV_ANIM_ON);//带有切换动画效果
}

//表盘显示任务
static void dashboard_task(void *pvParameter)
{
	int i=0;
    //tabview 分页标签控件
	bjtuyq = lv_tabview_create(lv_scr_act(), NULL);
	lv_obj_set_size(bjtuyq,240,240);  
	lv_obj_align(bjtuyq,NULL,LV_ALIGN_IN_TOP_LEFT,0,0);
	lv_tabview_set_btns_pos(bjtuyq,LV_TABVIEW_TAB_POS_NONE);
	lv_obj_t *bj1 = lv_tabview_add_tab(bjtuyq, "b1");	
	lv_obj_t *bj2 = lv_tabview_add_tab(bjtuyq, "b2");
	lv_obj_t *bj3 = lv_tabview_add_tab(bjtuyq, "b3");
	lv_obj_t *bj4 = lv_tabview_add_tab(bjtuyq, "b4");

    //显示表盘背景
	lv_obj_t *img5 = lv_img_create(bj1, NULL);
	lv_img_set_src(img5, &bpbj2);
	lv_obj_align(img5, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	lv_page_set_scrollbar_mode(bj1, LV_SCRLBAR_MODE_OFF);  

    //显示表盘背景
	lv_obj_t *img6 = lv_img_create(bj2, NULL);
	lv_img_set_src(img6, &bpbj4);
	lv_obj_align(img6, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	lv_page_set_scrollbar_mode(bj2, LV_SCRLBAR_MODE_OFF); 
	//显示表盘背景
	lv_obj_t *img7 = lv_img_create(bj3, NULL);
	lv_img_set_src(img7, &bpbj5);
	lv_obj_align(img7, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	lv_page_set_scrollbar_mode(bj3, LV_SCRLBAR_MODE_OFF);  
	//显示表盘背景
	lv_obj_t *img8 = lv_img_create(bj4, NULL);
	lv_img_set_src(img8, &bpbj3);
	lv_obj_align(img8, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	lv_page_set_scrollbar_mode(bj4, LV_SCRLBAR_MODE_OFF);  

    //文本-电量换颜色
	// static lv_style_t text_style1;	
	// lv_style_init(&text_style1);	
	// lv_style_set_text_font(&text_style1, LV_STATE_DEFAULT, &lv_font_montserrat_24); 
    // lv_style_set_text_color(&text_style1, LV_STATE_DEFAULT, LV_COLOR_WHITE);

	// lv_obj_t *label1 = lv_label_create(lv_scr_act(), NULL); 
	// lv_label_set_text(label1, LV_SYMBOL_BATTERY_3);		
	// lv_obj_align(label1, NULL, LV_ALIGN_IN_TOP_LEFT, 200, 5); 
    // lv_obj_add_style(label1,LV_LABEL_PART_MAIN, &text_style1);  

    //显示哆啦A梦图片
	img4 = lv_img_create(lv_scr_act(), NULL);				 
	lv_img_set_src(img4, &dl_0);							
	lv_obj_align(img4, NULL, LV_ALIGN_IN_TOP_LEFT, 145, 39); 

    //创建LVGL任务-哆啦A梦动图
	//lv_task_t *task2 = lv_task_create((lv_task_cb_t)task2_cb, 200, LV_TASK_PRIO_HIGH, NULL); 
	lv_task_create((lv_task_cb_t)task2_cb, 200, LV_TASK_PRIO_HIGH, NULL); 

    //显示位置图标
	lv_obj_t *wztb = lv_img_create(lv_scr_act(), NULL);
	lv_img_set_src(wztb, &wztb_32);
	lv_obj_align(wztb, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 102);  

    //创建背景样式
    static lv_style_t bg_style;
    lv_style_init(&bg_style);	
    lv_style_set_text_font(&bg_style, LV_STATE_DEFAULT, &myfont_cshz_24); 
	lv_style_set_text_color(&bg_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    //创建背景样式
    static lv_style_t bghz_style;
    lv_style_init(&bghz_style);	
    lv_style_set_text_font(&bghz_style, LV_STATE_DEFAULT, &myfont_3500hz_18); 
	lv_style_set_text_color(&bghz_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    //Label2 长文本动态滚动显示
	label2 = lv_label_create(lv_scr_act(), NULL);	
	lv_label_set_long_mode(label2, LV_LABEL_LONG_SROLL_CIRC);	
	lv_obj_set_width(label2, 100);		
	lv_obj_set_height(label2, 24);  
	lv_label_set_text(label2, NULL);
	lv_obj_align(label2, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 40);		
    lv_obj_add_style(label2,LV_LABEL_PART_MAIN, &bg_style); 

    //创建页签背景样式
    static lv_style_t yq_style;
    lv_style_init(&yq_style);	
	lv_style_set_text_font(&yq_style, LV_STATE_DEFAULT, &myfont_cshz_18); 
    lv_style_set_bg_opa(&yq_style, LV_STATE_DEFAULT, LV_OPA_30);  
    lv_style_set_bg_color(&yq_style, LV_STATE_DEFAULT, LV_COLOR_GRAY);  
    lv_style_set_text_color(&yq_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    //tabview 分页标签控件
	tabview = lv_tabview_create(lv_scr_act(), NULL);
	lv_obj_set_size(tabview,240,100);  
	lv_obj_align(tabview,NULL,LV_ALIGN_IN_TOP_LEFT,0,139);
	lv_tabview_set_btns_pos(tabview,LV_TABVIEW_TAB_POS_NONE);
	lv_obj_t *tab1 = lv_tabview_add_tab(tabview, "Tab 1");	
	lv_obj_t *tab2 = lv_tabview_add_tab(tabview, "Tab 2");
	lv_obj_t *tab3 = lv_tabview_add_tab(tabview, "Tab 3");
	lv_obj_t *tab4 = lv_tabview_add_tab(tabview, "Tab 4");
    lv_obj_add_style(tabview,LV_LABEL_PART_MAIN, &yq_style); 

    //显示天气图标
	lv_obj_t *TQ_TB1 = lv_img_create(tab1, NULL);
	lv_img_set_src(TQ_TB1, NULL); 
	//lv_img_set_src(TQ_TB1, &tq_99);  //显示天气图标
	lv_obj_align(TQ_TB1, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 18);  
    lv_page_set_scrollbar_mode(tab1, LV_SCRLBAR_MODE_OFF); 

	lv_obj_t *TQ_TB2 = lv_img_create(tab2, NULL);
	lv_img_set_src(TQ_TB2, NULL);  
	//lv_img_set_src(TQ_TB2, &tq_99);  //显示天气图标
	lv_obj_align(TQ_TB2, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 18);  
    lv_page_set_scrollbar_mode(tab2, LV_SCRLBAR_MODE_OFF); 

	lv_obj_t *TQ_TB3 = lv_img_create(tab3, NULL);
	lv_img_set_src(TQ_TB3, NULL); 
	//lv_img_set_src(TQ_TB3, &tq_99);  //显示天气图标
	lv_obj_align(TQ_TB3, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 18);  
	lv_page_set_scrollbar_mode(tab3, LV_SCRLBAR_MODE_OFF);  

	lv_obj_t * label = lv_label_create(tab4, NULL);			
	lv_label_set_text(label, "生于忧患,死于安乐!!");		// 设置标签内容
	lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_LEFT,20,35); 
	lv_obj_add_style(label,LV_LABEL_PART_MAIN, &bghz_style); 
    lv_page_set_scrollbar_mode(tab4, LV_SCRLBAR_MODE_OFF); 

    //创建LVGL任务-页签动图
	//lv_task_t *task3 = lv_task_create((lv_task_cb_t)task3_cb, 3000, LV_TASK_PRIO_MID, NULL);
    lv_task_create((lv_task_cb_t)task3_cb, 3000, LV_TASK_PRIO_MID, NULL);

    //Label4 位置文本显示
	label4 = lv_label_create(lv_scr_act(), NULL);   
	lv_label_set_text(label4, NULL);	
	lv_obj_align(label4, wztb, LV_ALIGN_IN_TOP_LEFT,32,7); 
	lv_obj_add_style(label4,LV_LABEL_PART_MAIN, &yq_style);  

    //Label5 今天日子文本显示
	label5 = lv_label_create(tab1, NULL);  
	lv_label_set_text(label5, NULL);	
	lv_obj_align(label5, NULL, LV_ALIGN_IN_TOP_LEFT,90,8); 
	lv_obj_add_style(label5,LV_LABEL_PART_MAIN, &bghz_style); 

	//Label5_1 今天天气文本显示
	label5_1 = lv_label_create(tab1, NULL);   
	lv_label_set_text(label5_1, NULL);	
	lv_obj_align(label5_1, NULL, LV_ALIGN_IN_TOP_LEFT,90,31); 
	lv_obj_add_style(label5_1,LV_LABEL_PART_MAIN, &bghz_style);  

	//Label5_2 今天最低气温文本显示
	label5_2 = lv_label_create(tab1, NULL);  
	lv_label_set_text(label5_2, NULL);
	lv_obj_align(label5_2, NULL, LV_ALIGN_IN_TOP_LEFT,144,54); 
	lv_obj_add_style(label5_2,LV_LABEL_PART_MAIN, &bghz_style);  

	//Label5_3 今天风向文本显示
	label5_3 = lv_label_create(tab1, NULL);  
	lv_label_set_text(label5_3, NULL);	
	lv_obj_align(label5_3, NULL, LV_ALIGN_IN_TOP_LEFT,182,54); 
	lv_obj_add_style(label5_3,LV_LABEL_PART_MAIN, &bghz_style);  

	//Label5_4 今天风力文本显示
	label5_4 = lv_label_create(tab1, NULL);  
	lv_label_set_text(label5_4, NULL);	
	lv_obj_align(label5_4, NULL, LV_ALIGN_IN_TOP_LEFT,90,77); 
	lv_obj_add_style(label5_4,LV_LABEL_PART_MAIN, &bghz_style); 

	//Label5_5 今天最高温度显示
	label5_5 = lv_label_create(tab1, NULL);  
	lv_label_set_text(label5_5, NULL);
	lv_obj_align(label5_5, NULL, LV_ALIGN_IN_TOP_LEFT,90,54); 
	lv_obj_add_style(label5_5,LV_LABEL_PART_MAIN, &bghz_style); 

	//Label5_5 今天最高温度显示
	lv_obj_t *label5_6 = lv_label_create(tab1, NULL);  
	lv_label_set_text(label5_6, "/");	
	lv_obj_align(label5_6, NULL, LV_ALIGN_IN_TOP_LEFT,126,54); 
	lv_obj_add_style(label5_6,LV_LABEL_PART_MAIN, &bghz_style);  
/*****************************************************/
    //Label6 明天日子文本显示
	label6 = lv_label_create(tab2, NULL); 
	lv_label_set_text(label6, NULL);	
	lv_obj_align(label6, NULL, LV_ALIGN_IN_TOP_LEFT,90,8); 
	lv_obj_add_style(label6,LV_LABEL_PART_MAIN, &bghz_style); 

	//Label6_1 明天天气文本显示
	label6_1 = lv_label_create(tab2, NULL);  
	lv_label_set_text(label6_1, NULL);	
	lv_obj_align(label6_1, NULL, LV_ALIGN_IN_TOP_LEFT,90,31); 
	lv_obj_add_style(label6_1,LV_LABEL_PART_MAIN, &bghz_style); 

	//Label5_2 明天最低气温文本显示
	label6_2 = lv_label_create(tab2, NULL);  
	lv_label_set_text(label6_2, NULL);
	lv_obj_align(label6_2, NULL, LV_ALIGN_IN_TOP_LEFT,144,54); 
	lv_obj_add_style(label6_2,LV_LABEL_PART_MAIN, &bghz_style); 

	//Label5_3 明天风向文本显示
	label6_3 = lv_label_create(tab2, NULL);  
	lv_label_set_text(label6_3, NULL);
	lv_obj_align(label6_3, NULL, LV_ALIGN_IN_TOP_LEFT,182,54); 
	lv_obj_add_style(label6_3,LV_LABEL_PART_MAIN, &bghz_style);  

	//Label6_4 明天风力文本显示
	label6_4 = lv_label_create(tab2, NULL);  
	lv_label_set_text(label6_4, NULL);
	lv_obj_align(label6_4, NULL, LV_ALIGN_IN_TOP_LEFT,90,77); 
	lv_obj_add_style(label6_4,LV_LABEL_PART_MAIN, &bghz_style); 

	//Label5_5 明天最高温度显示
	label6_5 = lv_label_create(tab2, NULL);  
	lv_label_set_text(label6_5, NULL);	
	lv_obj_align(label6_5, NULL, LV_ALIGN_IN_TOP_LEFT,90,54); 
	lv_obj_add_style(label6_5,LV_LABEL_PART_MAIN, &bghz_style); 

	//Label5_5 明天最高温度显示
	lv_obj_t *label6_6 = lv_label_create(tab2, NULL); 
	lv_label_set_text(label6_6, "/");	
	lv_obj_align(label6_6, NULL, LV_ALIGN_IN_TOP_LEFT,126,54);
	lv_obj_add_style(label6_6,LV_LABEL_PART_MAIN, &bghz_style); 
/*****************************************************/
    //Label7  后天日子文本显示
	label7 = lv_label_create(tab3, NULL);  
	lv_label_set_text(label7, NULL);	
	lv_obj_align(label7, NULL, LV_ALIGN_IN_TOP_LEFT,90,8); 
	lv_obj_add_style(label7,LV_LABEL_PART_MAIN, &bghz_style); 

	//Label7_1 后天天气文本显示
	label7_1 = lv_label_create(tab3, NULL);  
	lv_label_set_text(label7_1, NULL);
	lv_obj_align(label7_1, NULL, LV_ALIGN_IN_TOP_LEFT,90,31); 
	lv_obj_add_style(label7_1,LV_LABEL_PART_MAIN, &bghz_style);  

	//Label7_2 后天最低气温文本显示
	label7_2 = lv_label_create(tab3, NULL);  
	lv_label_set_text(label7_2, NULL);
	lv_obj_align(label7_2, NULL, LV_ALIGN_IN_TOP_LEFT,144,54); 
	lv_obj_add_style(label7_2,LV_LABEL_PART_MAIN, &bghz_style); 

	//Label7_3 后天风向文本显示
	label7_3 = lv_label_create(tab3, NULL);  
	lv_label_set_text(label7_3, NULL);
	lv_obj_align(label7_3, NULL, LV_ALIGN_IN_TOP_LEFT,182,54);
	lv_obj_add_style(label7_3,LV_LABEL_PART_MAIN, &bghz_style); 

	//Label7_4 后天风力文本显示
	label7_4 = lv_label_create(tab3, NULL); 
	lv_label_set_text(label7_4, NULL);
	lv_obj_align(label7_4, NULL, LV_ALIGN_IN_TOP_LEFT,90,77);
	lv_obj_add_style(label7_4,LV_LABEL_PART_MAIN, &bghz_style); 

	//Label7_5 后天最高温度显示
	label7_5 = lv_label_create(tab3, NULL); 
	lv_label_set_text(label7_5, NULL);
	lv_obj_align(label7_5, NULL, LV_ALIGN_IN_TOP_LEFT,90,54); 
	lv_obj_add_style(label7_5,LV_LABEL_PART_MAIN, &bghz_style); 

	//Label7_6 后天温度分割显示
	lv_obj_t *label7_6 = lv_label_create(tab3, NULL); 
	lv_label_set_text(label7_6, "/");	
	lv_obj_align(label7_6, NULL, LV_ALIGN_IN_TOP_LEFT,126,54); 
	lv_obj_add_style(label7_6,LV_LABEL_PART_MAIN, &bghz_style); 

	while (1)
	{   
		if(weather1.biaozhiwei==1)
		{
			lv_label_set_text(label5,weather1.rizi);	   
            lv_label_set_text(label5_1, weather1.tianqi);	
			lv_label_set_text(label5_2, weather1.zdwendu);
			lv_label_set_text(label5_3, weather1.fengx);	
			lv_label_set_text(label5_4, weather1.fengji);	
			lv_label_set_text(label5_5, weather1.zgwendu);	

			lv_label_set_text(label6,weather2.rizi);	   
            lv_label_set_text(label6_1, weather2.tianqi);	
			lv_label_set_text(label6_2, weather2.zdwendu);
			lv_label_set_text(label6_3, weather2.fengx);	
			lv_label_set_text(label6_4, weather2.fengji);	
			lv_label_set_text(label6_5, weather2.zgwendu);	
			
			lv_label_set_text(label7,weather3.rizi);	   
            lv_label_set_text(label7_1, weather3.tianqi);	
			lv_label_set_text(label7_2, weather3.zdwendu);	
			lv_label_set_text(label7_3, weather3.fengx);	
			lv_label_set_text(label7_4, weather3.fengji);	
			lv_label_set_text(label7_5, weather3.zgwendu);	

			// lv_img_set_src(TQ_TB1, &tq_1);  //显示天气图标
			// lv_img_set_src(TQ_TB2, &tq_1);  //显示天气图标
			// lv_img_set_src(TQ_TB3, &tq_9);  //显示天气图标
            
			i=atoi(weather1.xwtianqi);
			switch(i)
			{
                case 0: lv_img_set_src(TQ_TB1, &tq_0); break; //显示天气图标
                case 1: lv_img_set_src(TQ_TB1, &tq_1); break; 
                case 2: lv_img_set_src(TQ_TB1, &tq_2); break; 
				case 3: lv_img_set_src(TQ_TB1, &tq_3); break; 
				case 4: lv_img_set_src(TQ_TB1, &tq_4); break; 
				case 5: lv_img_set_src(TQ_TB1, &tq_5); break; 
				case 6: lv_img_set_src(TQ_TB1, &tq_6); break; 
				case 7: lv_img_set_src(TQ_TB1, &tq_7); break; 
				case 8: lv_img_set_src(TQ_TB1, &tq_8); break; 
				case 9: lv_img_set_src(TQ_TB1, &tq_9); break; 
				case 10: lv_img_set_src(TQ_TB1, &tq_10); break; 
				case 11: lv_img_set_src(TQ_TB1, &tq_11); break; 
				case 12: lv_img_set_src(TQ_TB1, &tq_12); break; 
				case 13: lv_img_set_src(TQ_TB1, &tq_13); break; 
				case 14: lv_img_set_src(TQ_TB1, &tq_14); break; 
				case 15: lv_img_set_src(TQ_TB1, &tq_15); break; 
				case 16: lv_img_set_src(TQ_TB1, &tq_16); break; 
				case 17: lv_img_set_src(TQ_TB1, &tq_17); break; 
				case 18: lv_img_set_src(TQ_TB1, &tq_18); break; 
				case 19: lv_img_set_src(TQ_TB1, &tq_19); break; 
				case 20: lv_img_set_src(TQ_TB1, &tq_20); break; 
				case 21: lv_img_set_src(TQ_TB1, &tq_21); break; 
				case 22: lv_img_set_src(TQ_TB1, &tq_22); break; 
				case 23: lv_img_set_src(TQ_TB1, &tq_23); break; 
				case 24: lv_img_set_src(TQ_TB1, &tq_24); break; 
				case 29: lv_img_set_src(TQ_TB1, &tq_29); break; 
				case 30: lv_img_set_src(TQ_TB1, &tq_30); break; 
				case 31: lv_img_set_src(TQ_TB1, &tq_31); break; 
				case 32: lv_img_set_src(TQ_TB1, &tq_32); break; 
				case 49: lv_img_set_src(TQ_TB1, &tq_49); break; 
				case 53: lv_img_set_src(TQ_TB1, &tq_53); break; 
				case 54: lv_img_set_src(TQ_TB1, &tq_54); break; 
				case 55: lv_img_set_src(TQ_TB1, &tq_55); break;
				case 56: lv_img_set_src(TQ_TB1, &tq_56); break; 
				case 57: lv_img_set_src(TQ_TB1, &tq_57); break; 
				case 58: lv_img_set_src(TQ_TB1, &tq_58); break; 
				case 301: lv_img_set_src(TQ_TB1, &tq_301); break; 
				case 302: lv_img_set_src(TQ_TB1, &tq_302); break; 
				default: lv_img_set_src(TQ_TB1, &tq_99); break; 
			}

			i=atoi(weather2.xwtianqi);
			switch(i)
			{
                case 0: lv_img_set_src(TQ_TB2, &tq_0); break; 
                case 1: lv_img_set_src(TQ_TB2, &tq_1); break; 
                case 2: lv_img_set_src(TQ_TB2, &tq_2); break; 
				case 3: lv_img_set_src(TQ_TB2, &tq_3); break; 
				case 4: lv_img_set_src(TQ_TB2, &tq_4); break; 
				case 5: lv_img_set_src(TQ_TB2, &tq_5); break; 
				case 6: lv_img_set_src(TQ_TB2, &tq_6); break; 
				case 7: lv_img_set_src(TQ_TB2, &tq_7); break; 
				case 8: lv_img_set_src(TQ_TB2, &tq_8); break; 
				case 9: lv_img_set_src(TQ_TB2, &tq_9); break; 
				case 10: lv_img_set_src(TQ_TB2, &tq_10); break; 
				case 11: lv_img_set_src(TQ_TB2, &tq_11); break; 
				case 12: lv_img_set_src(TQ_TB2, &tq_12); break; 
				case 13: lv_img_set_src(TQ_TB2, &tq_13); break; 
				case 14: lv_img_set_src(TQ_TB2, &tq_14); break; 
				case 15: lv_img_set_src(TQ_TB2, &tq_15); break; 
				case 16: lv_img_set_src(TQ_TB2, &tq_16); break; 
				case 17: lv_img_set_src(TQ_TB2, &tq_17); break; 
				case 18: lv_img_set_src(TQ_TB2, &tq_18); break; 
				case 19: lv_img_set_src(TQ_TB2, &tq_19); break; 
				case 20: lv_img_set_src(TQ_TB2, &tq_20); break; 
				case 21: lv_img_set_src(TQ_TB2, &tq_21); break; 
				case 22: lv_img_set_src(TQ_TB2, &tq_22); break; 
				case 23: lv_img_set_src(TQ_TB2, &tq_23); break; 
				case 24: lv_img_set_src(TQ_TB2, &tq_24); break; 
				case 29: lv_img_set_src(TQ_TB2, &tq_29); break; 
				case 30: lv_img_set_src(TQ_TB2, &tq_30); break; 
				case 31: lv_img_set_src(TQ_TB2, &tq_31); break; 
				case 32: lv_img_set_src(TQ_TB2, &tq_32); break; 
				case 49: lv_img_set_src(TQ_TB2, &tq_49); break; 
				case 53: lv_img_set_src(TQ_TB2, &tq_53); break; 
				case 54: lv_img_set_src(TQ_TB2, &tq_54); break; 
				case 55: lv_img_set_src(TQ_TB2, &tq_55); break; 
				case 56: lv_img_set_src(TQ_TB2, &tq_56); break; 
				case 57: lv_img_set_src(TQ_TB2, &tq_57); break; 
				case 58: lv_img_set_src(TQ_TB2, &tq_58); break; 
				case 301: lv_img_set_src(TQ_TB2, &tq_301); break; 
				case 302: lv_img_set_src(TQ_TB2, &tq_302); break; 				
				default: lv_img_set_src(TQ_TB2, &tq_99); break; 
			}

			i=atoi(weather3.xwtianqi);
			switch(i)
			{
                case 0: lv_img_set_src(TQ_TB3, &tq_0); break; //显示天气图标
                case 1: lv_img_set_src(TQ_TB3, &tq_1); break; 
                case 2: lv_img_set_src(TQ_TB3, &tq_2); break; 
				case 3: lv_img_set_src(TQ_TB3, &tq_3); break; 
				case 4: lv_img_set_src(TQ_TB3, &tq_4); break; 
				case 5: lv_img_set_src(TQ_TB3, &tq_5); break; 
				case 6: lv_img_set_src(TQ_TB3, &tq_6); break; 
				case 7: lv_img_set_src(TQ_TB3, &tq_7); break; 
				case 8: lv_img_set_src(TQ_TB3, &tq_8); break; 
				case 9: lv_img_set_src(TQ_TB3, &tq_9); break;
				case 10: lv_img_set_src(TQ_TB3, &tq_10); break; 
				case 11: lv_img_set_src(TQ_TB3, &tq_11); break; 
				case 12: lv_img_set_src(TQ_TB3, &tq_12); break; 
				case 13: lv_img_set_src(TQ_TB3, &tq_13); break; 
				case 14: lv_img_set_src(TQ_TB3, &tq_14); break; 
				case 15: lv_img_set_src(TQ_TB3, &tq_15); break; 
				case 16: lv_img_set_src(TQ_TB3, &tq_16); break; 
				case 17: lv_img_set_src(TQ_TB3, &tq_17); break; 
				case 18: lv_img_set_src(TQ_TB3, &tq_18); break; 
				case 19: lv_img_set_src(TQ_TB3, &tq_19); break; 
				case 20: lv_img_set_src(TQ_TB3, &tq_20); break; 
				case 21: lv_img_set_src(TQ_TB3, &tq_21); break;
				case 22: lv_img_set_src(TQ_TB3, &tq_22); break; 
				case 23: lv_img_set_src(TQ_TB3, &tq_23); break; 
				case 24: lv_img_set_src(TQ_TB3, &tq_24); break; 
				case 29: lv_img_set_src(TQ_TB3, &tq_29); break; 
				case 30: lv_img_set_src(TQ_TB3, &tq_30); break; 
				case 31: lv_img_set_src(TQ_TB3, &tq_31); break; 
				case 32: lv_img_set_src(TQ_TB3, &tq_32); break; 
				case 49: lv_img_set_src(TQ_TB3, &tq_49); break; 
				case 53: lv_img_set_src(TQ_TB3, &tq_53); break; 
				case 54: lv_img_set_src(TQ_TB3, &tq_54); break; 
				case 55: lv_img_set_src(TQ_TB3, &tq_55); break; 
				case 56: lv_img_set_src(TQ_TB3, &tq_56); break; 
				case 57: lv_img_set_src(TQ_TB3, &tq_57); break; 
				case 58: lv_img_set_src(TQ_TB3, &tq_58); break; 
				case 301: lv_img_set_src(TQ_TB3, &tq_301); break; 
				case 302: lv_img_set_src(TQ_TB3, &tq_302); break; 			
				default: lv_img_set_src(TQ_TB3, &tq_99); break;
			}

			for(int countdown = 360; countdown >= 0; countdown--) {
			vTaskDelay(1000 / portTICK_PERIOD_MS);
            }
		}
	}
}

// wifi 连接成功
void successful_wifi(void)
{
	xTaskCreate(sntp_task, "sntp_task", 1024*2, NULL, 3, NULL);//创建动态任务-网络时间任务
	xTaskCreatePinnedToCore(get_lunar_task, "get_lunar_task", 1024*5, NULL, 1, &xHandle_task1, tskNO_AFFINITY);  //初始化显示以及开机界面
	// 如果要使用任务创建图形，则需要创建固定任务,否则可能会出现诸如内存损坏等问题
	//创建动态任务--表盘任务
	xTaskCreatePinnedToCore(dashboard_task, "dashboard_task", 1024*8, NULL, 5, NULL, 1); 
}

// SmartConfig 任务
static void smart_config_task(void * pvParameters)
{
	EventBits_t Bits,uxBits;

    vTaskDelay(200);
	while (1){
		//死等事件组：CONNECTED_BIT | ESPTOUCH_DONE_BIT   事件组等待位
		Bits = xEventGroupWaitBits(s_wifi_event_group, CONNECTED_BIT | WIFI_FAIL_BIT, true, false, portMAX_DELAY);
        //如果WIFI连接成功
		if(Bits & CONNECTED_BIT) {
            ESP_LOGI(TAG, "WiFi Connected to ap A");  
			lv_obj_del(img1); 
	        lv_obj_del(img2); 
	        lv_task_del(task1);  
			task1 = NULL;
            successful_wifi();
			vTaskDelete(NULL);
		}
        //如果WIFI连接失败
		else if (Bits & WIFI_FAIL_BIT){
            ESP_LOGI(TAG, "WiFi connection failed A");  
			lv_obj_del(img1); 
			lv_obj_del(img2); 
			lv_task_del(task1);  
			task1 = NULL;
			//显示二维码图片
			img3 = lv_img_create(lv_scr_act(), NULL); 
			lv_img_set_src(img3, &ewm);	
			lv_obj_align(img3, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

			//微信配网
			ESP_ERROR_CHECK(esp_smartconfig_set_type(SC_TYPE_AIRKISS));
			//开始SmartConfig
			smartconfig_start_config_t cfg = SMARTCONFIG_START_CONFIG_DEFAULT(); 
			ESP_ERROR_CHECK( esp_smartconfig_start(&cfg) ); 
			while (1){
				//死等事件组：CONNECTED_BIT | ESPTOUCH_DONE_BIT   事件组等待位  连接成功  和  smartconfing配网事件
				uxBits = xEventGroupWaitBits(s_wifi_event_group, CONNECTED_BIT | ESPTOUCH_DONE_BIT, true, false, portMAX_DELAY);
				//WIFI已连接AP
				if(uxBits & CONNECTED_BIT) {
					ESP_LOGI(TAG, "WiFi Connected to ap B"); 
				}
				//SmartConfig结束
				else if (uxBits & ESPTOUCH_DONE_BIT){
					ESP_LOGI(TAG, "smartconfig over");  
                    esp_smartconfig_stop();  
					lv_obj_del(img3);	
                    successful_wifi();
					vTaskDelete(NULL);
				}
			}
		}
	}
}

//农历信息任务函数
static void get_lunar_task(void * pvParameters)
{
    //socket的属性按IP版本设置问题
    const struct addrinfo hints = {
        .ai_family = AF_INET,  
        .ai_socktype = SOCK_STREAM,  
    };
    struct addrinfo *res;
    struct in_addr *addr;
    int s, r;
    char recv_buf[512]; 

	char csxx[70];  //保存城市-星期-农历-阳历等信息字符串  13+18+10+35
	char *T_csxx = csxx; // 缓存数组指针
	static char rwbz =1; 

    while(1) {
        int err = getaddrinfo(WEB_SERVER, "80", &hints, &res); 

        if(err != 0 || res == NULL) {  //如果返回参数不等于0  或者  指针地址为空
            ESP_LOGE(TAG, "DNS lookup failed err=%d res=%p", err, res); 
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;  
        }

        addr = &((struct sockaddr_in *)res->ai_addr)->sin_addr;
        ESP_LOGI(TAG, "DNS lookup succeeded. IP=%s", inet_ntoa(*addr));  //DNS查找成功

        //创建套接字
        s = socket(res->ai_family, res->ai_socktype, 0);
        if(s < 0) {
            ESP_LOGE(TAG, "... Failed to allocate socket.");  
            freeaddrinfo(res);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;   
        }
        ESP_LOGI(TAG, "... allocated socket\r\n"); 

        if(connect(s, res->ai_addr, res->ai_addrlen) != 0) {
            ESP_LOGE(TAG, "... socket connect failed errno=%d", errno);
            close(s);
            freeaddrinfo(res); //释放由getaddrinfo返回的存储空间
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue; 
        }

        ESP_LOGI(TAG, "... connected");
        freeaddrinfo(res);

        if (write(s, REQUEST, strlen(REQUEST)) < 0) {
            ESP_LOGE(TAG, "... socket send failed");//套接字发送失败
            close(s);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }

        ESP_LOGI(TAG, "... socket send success");  

		//城市--汉字
		char *substr="/></div>"; //查找第一处字符串  遇到"时前面加\; 
		char *substr1="/></div>";//字符串的左部分
		char *substr2="天气";   //字符串的右部分

		//城市拼音信息
		char *substr12="href=\"http://"; //查找第一处字符串  遇到"时前面加\; 
		char *substr13="href=\"http://";//字符串的左部分
		char *substr14=".tianqi.com/?tq\"";   //字符串的右部分

		//公历-年月日
		char *substr3="wtwind\">"; //查找第一处字符串  遇到"时前面加\; 
		char *substr4="wtwind\">";//字符串的左部分
		char *substr5="星期";   //字符串的右部分

		//星期信息
		char *substr6="日  "; //查找第一处字符串  遇到"时前面加\; 
		char *substr7="日  ";//字符串的左部分
		char *substr8=" <br>";   //字符串的右部分

		//农历信息
		char *substr9=" <br>"; //查找第一处字符串  遇到"时前面加\; 
		char *substr10=" <br>";//字符串的左部分
		char *substr11="</div>";   //字符串的右部分

		char *st=NULL;  //暂存第一次出现字符串的首位置
		char *st1=NULL;  //要提取的字符串首位置
            
        /*读取HTTP响应*/
        do {
            bzero(recv_buf, sizeof(recv_buf));//recv_buf要置0的数据的起始地址；sizeof(recv_buf)要置0的数据字节个数
            //函数从打开的设备或文件中读取数据。
            r = read(s, recv_buf, sizeof(recv_buf)-1);  //局部变量r表示系统实际所读取的字符数量

            //获取城市信息
            st=strstr(recv_buf,substr); //在字符串recv_buf中查找第一次出现字符串的位置，不包含终止符'\0'
			if(st!=NULL){  //不为空说明找到了字符串
				st1=splitx(st,substr1,substr2);
				st=NULL;
			}
			if(st1!=NULL&&st1!=ESP_OK){  
				lv_label_set_text(label4, st1); //文本显示位置信息
				//printf("城市=%s\n",st1);	 
				st1=NULL;  //将ST1置空
			}

			//获取城市拼音信息
            st=strstr(recv_buf,substr12); 
			if(st!=NULL){  
				st1=splitx(st,substr13,substr14);
				st=NULL;
			}
			if(st1!=NULL&&st1!=ESP_OK){  
				T_cspy = st1;
				//printf("城市拼音=%s\n",T_cspy);	 
				if(rwbz) {  
					rwbz=0;  
					xTaskCreate(get_city_code_task, "get_city_code_task", 1024*4, NULL, 1, NULL);  //创建动态任务-网络获得城市代码任务   只创建一次
					//xTaskCreatePinnedToCore(http_get_task1, "http_get_task1", 1024*4, NULL, 3, NULL, 1);  //初始化显示以及开机界面
				}
				st1=NULL;  
			}

			//获取公历-年月日信息
            st=strstr(recv_buf,substr3); 
			if(st!=NULL){  
				st1=splitx(st,substr4,substr5);
				st=NULL;
			}
			if(st1!=NULL&&st1!=ESP_OK){  
				memcpy(T_csxx, st1, 17); 
				T_csxx[17] = ' ';
				T_csxx += 18;
				//printf("公历=%s\n",st1);	
				st1=NULL; 
			}

			//获取星期信息
            st=strstr(recv_buf,substr6); 
			if(st!=NULL) {  
				st1=splitx(st,substr7,substr8);
				st=NULL;
			}
			if(st1!=NULL&&st1!=ESP_OK){  
				memcpy(T_csxx, st1, 9); 
				T_csxx[9] = ' ';
				T_csxx += 10;
				//printf("星期=%s\n",st1);	 
				st1=NULL;  
			}

			//获取农历信息
            st=strstr(recv_buf,substr9); 
			if(st!=NULL){  
				st1=splitx(st,substr10,substr11);
				st=NULL;
			}
			if(st1!=NULL&&st1!=ESP_OK){  
				T_csxx[31] = ' ';
				T_csxx[32] = ' ';
				T_csxx[33] = ' ';
				T_csxx[34] = ' ';
				memcpy(T_csxx, st1, 35); 
				T_csxx[35] = '\0';
				T_csxx += 36;   
				lv_label_set_text(label2, csxx);//设置显示文本
				//printf("农历=%s\n",st1);	 
				st1=NULL;  
				//printf("csxx=%s\n",csxx);	 		
				break;
			}
        } while(r > 0);  
        //打印剩余多少内容没有读取
        ESP_LOGI(TAG, "... done reading from socket. Last read return=%d errno=%d\r\n", r, errno);
        close(s);  //关闭文件

        for(int countdown = 300; countdown >= 0; countdown--) {
            ESP_LOGI(TAG, "%d... ", countdown);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
		ESP_LOGI(TAG, "Starting again1!"); 
		//恢复任务2
		vTaskResume(xHandle_task2);
		//挂起自身任务（任务1）
		vTaskSuspend(NULL); 
    }
}

//获取城市代码-任务函数  
static void get_city_code_task(void * pvParameters)
{
    //socket的属性按IP版本设置问题
    const struct addrinfo hints = {
        .ai_family = AF_INET,  
        .ai_socktype = SOCK_STREAM, 
    };
    struct addrinfo *res;
    struct in_addr *addr;
    int s, r;
    char recv_buf[512];  //512字节的网页数据缓冲区

	strcat(cspy2, T_cspy);
	strcat(cspy2, REQUEST1);

    while(1) {
        int err = getaddrinfo(WEB_SERVER1, "80", &hints, &res);  //自动生成这些参数

        if(err != 0 || res == NULL) {  //如果返回参数不等于0  或者  指针地址为空
            ESP_LOGE(TAG, "DNS lookup failed err=%d res=%p", err, res);  //DNS查找失败错误
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;   //终止当前的循环  跳过后面的程序
        }

        addr = &((struct sockaddr_in *)res->ai_addr)->sin_addr;
        ESP_LOGI(TAG, "DNS lookup succeeded. IP=%s", inet_ntoa(*addr));  //DNS查找成功
        //创建套接字
        s = socket(res->ai_family, res->ai_socktype, 0);
        if(s < 0) {
            ESP_LOGE(TAG, "... Failed to allocate socket.");  
            freeaddrinfo(res);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;   
        }
        ESP_LOGI(TAG, "... allocated socket\r\n"); 
        //connect()用于建立与指定socket的连接。
        if(connect(s, res->ai_addr, res->ai_addrlen) != 0) {
            ESP_LOGE(TAG, "... socket connect failed errno=%d", errno);
            close(s);
            freeaddrinfo(res); //释放由getaddrinfo返回的存储空间
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue; 
        }
        ESP_LOGI(TAG, "... connected");
        freeaddrinfo(res);

        if (write(s, T_cspy2, strlen(T_cspy2)) < 0) {
            ESP_LOGE(TAG, "... socket send failed");
            close(s);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "... socket send success"); 

		//城市代码数组
		char *substr="([{\"ref\":\""; //查找第一处字符串  遇到"时前面加\; 
		char *substr1="([{\"ref\":\"";//字符串的左部分
		char *substr2="~";   //字符串的右部分

		char *st=NULL;  //暂存第一次出现字符串的首位置
		char *st1=NULL;  //要提取的字符串首位置
            
        /*读取HTTP响应*/
        do {
            bzero(recv_buf, sizeof(recv_buf));//recv_buf要置0的数据的起始地址；sizeof(recv_buf)要置0的数据字节个数
            //函数从打开的设备或文件中读取数据。
            r = read(s, recv_buf, sizeof(recv_buf)-1);  //局部变量r表示系统实际所读取的字符数量

            //获取城市信息
            st=strstr(recv_buf,substr); //在字符串recv_buf中查找第一次出现字符串的位置，不包含终止符'\0'
			if(st!=NULL) {  //不为空说明找到了字符串
				st1 = splitx(st,substr1,substr2);//将字符串ST分割，分割符号"temp\":\""，要分割的第几个符号
				st = NULL;
			}
			if(st1!=NULL&&st1!=ESP_OK){  //如果ST1不为空并且ST1不为OK  --正常时
				//printf("城市代码=%s\n",st1);	
				T_csdm = st1;  //城市代码  指针地址赋值 到另一个指针变量的地址
				//printf("T_csdm=%s\n",T_csdm);	
				xTaskCreatePinnedToCore(get_weather_task, "get_weather_task", 1024*8, NULL, 1, &xHandle_task2, tskNO_AFFINITY);  //初始化显示以及开机界面
				vTaskSuspend(xHandle_task1);  //挂起任务1
				vTaskDelete(NULL); 
				st1 = NULL;  //将ST1置空
				break;
			}
        } while(r > 0); 

        //打印剩余多少内容没有读取
        ESP_LOGI(TAG, "... done reading from socket. Last read return=%d errno=%d\r\n", r, errno);
        close(s);  
        for(int countdown = 40; countdown >= 0; countdown--) {
            ESP_LOGI(TAG, "%d... ", countdown);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        ESP_LOGI(TAG, "Starting again 1!");  
    }
}

//天气网任务函数  
static void get_weather_task(void * pvParameters)
{
    //socket的属性按IP版本设置问题
    const struct addrinfo hints = {
        .ai_family = AF_INET,  
        .ai_socktype = SOCK_STREAM,  
    };
    struct addrinfo *res;
    struct in_addr *addr;
    int s, r, i=1 , j=1;

    char recv_buf[1263];  //512字节的网页数据缓冲区  356
    char *T_buf=NULL;
	strcat(p, T_csdm);  
	strcat(p, REQUEST2); 

	//内容开头
	//日子汉字
	char *substr="（今天）</h1>";
	char *substr1="<h1>";
	char *substr2="</h1>";
	//下午天气图标
	char *substr3="<big class=\"png40 n";
	char *substr4="\"></big>";
	//今日天气汉字
	char *substr5="class=\"wea\">";
	char *substr6="</p>";
	//最高温度
	char *substr7="<span>";
	char *substr8="</span>";
	//最低温度
	char *substr9="<i>";
	char *substr10="</i>";
	//风向
	char *substr11="<span title=\"";
	char *substr12="\" class=";
	//风级
	char *substr13="<i>";
	char *substr14="</i>";

	char *st=NULL;  
	char *st1=NULL; 

    while(1) {
        int err = getaddrinfo(WEB_SERVER2, "80", &hints, &res);  
        if(err != 0 || res == NULL) {  //如果返回参数不等于0  或者  指针地址为空
            ESP_LOGE(TAG, "DNS lookup failed err=%d res=%p", err, res);  //DNS查找失败错误
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;   
        }
        addr = &((struct sockaddr_in *)res->ai_addr)->sin_addr;
        ESP_LOGI(TAG, "DNS lookup succeeded. IP=%s", inet_ntoa(*addr));  //DNS查找成功
        //创建套接字
        s = socket(res->ai_family, res->ai_socktype, 0);
        if(s < 0) {
            ESP_LOGE(TAG, "... Failed to allocate socket.");  
            freeaddrinfo(res);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;   
        }
        ESP_LOGI(TAG, "... allocated socket\r\n");  
        if(connect(s, res->ai_addr, res->ai_addrlen) != 0) {
            ESP_LOGE(TAG, "... socket connect failed errno=%d", errno);
            close(s);
            freeaddrinfo(res); //释放由getaddrinfo返回的存储空间
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue; 
        }
        ESP_LOGI(TAG, "... connected");
        freeaddrinfo(res);
        if (write(s, pp, strlen(pp)) < 0) {
            ESP_LOGE(TAG, "... socket send failed");
            close(s);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "... socket send success");  

        /*读取HTTP响应*/
        do {
            bzero(recv_buf, sizeof(recv_buf));//recv_buf要置0的数据的起始地址；sizeof(recv_buf)要置0的数据字节个数
            //函数从打开的设备或文件中读取数据。
            r = read(s, recv_buf, sizeof(recv_buf)-1);  //局部变量r表示系统实际所读取的字符数量
            //获取城市信息
            st=strstr(recv_buf,substr); //在字符串recv_buf中查找第一次出现字符串的位置，不包含终止符'\0'
			if(st!=NULL){  //不为空说明找到了字符串
			 //printf("recv_buf=%s\n",recv_buf);	 //打印获取到的

			 st=strstr(recv_buf,substr1); //在字符串recv_buf中查找第一次出现字符串的位置，不包含终止符'\0'
			 st1=splitx(st,substr1,substr2);//将字符串ST分割，分割符号"temp\":\""，要分割的第几个符号
			 if(st1!=NULL&&st1!=ESP_OK){  //如果ST1不为空并且ST1不为OK  --正常时
			  weather1.rizi=st1;
			  //printf("今天=%s\n",weather1.rizi);	
			 }

			 st=strstr(recv_buf,substr3); //在字符串recv_buf中查找第一次出现字符串的位置，不包含终止符'\0'
			 st1=splitx(st,substr3,substr4);//将字符串ST分割，分割符号"temp\":\""，要分割的第几个符号
			 if(st1!=NULL&&st1!=ESP_OK){  //如果ST1不为空并且ST1不为OK  --正常时
			  weather1.xwtianqi=st1;
			  //printf("今天图标=%s\n",weather1.xwtianqi);	
			 }

			 st=strstr(recv_buf,substr5); //在字符串recv_buf中查找第一次出现字符串的位置，不包含终止符'\0'
			 st1=splitx(st,substr5,substr6);//将字符串ST分割，分割符号"temp\":\""，要分割的第几个符号
			 if(st1!=NULL&&st1!=ESP_OK){  //如果ST1不为空并且ST1不为OK  --正常时
			  weather1.tianqi=st1;
			  //printf("今天天气=%s\n",weather1.tianqi);	 
			 }

			 st=strstr(recv_buf,substr7); //在字符串recv_buf中查找第一次出现字符串的位置，不包含终止符'\0'
			 st1=splitx(st,substr7,substr8);//将字符串ST分割，分割符号"temp\":\""，要分割的第几个符号
			 if(st1!=NULL&&st1!=ESP_OK){  //如果ST1不为空并且ST1不为OK  --正常时
			  weather1.zgwendu=st1;
			  //printf("今天最高温度=%s\n",weather1.zgwendu);	
			 }

			 st=strstr(recv_buf,substr9); //在字符串recv_buf中查找第一次出现字符串的位置，不包含终止符'\0'
			 st1=splitx(st,substr9,substr10);//将字符串ST分割，分割符号"temp\":\""，要分割的第几个符号
			 if(st1!=NULL&&st1!=ESP_OK){  //如果ST1不为空并且ST1不为OK  --正常时
			  weather1.zdwendu=st1;
			  //printf("今天最低温度=%s\n",weather1.zdwendu);	
			 }

			 st=strstr(recv_buf,substr11); //在字符串recv_buf中查找第一次出现字符串的位置，不包含终止符'\0'
			 st1=splitx(st,substr11,substr12);//将字符串ST分割，分割符号"temp\":\""，要分割的第几个符号
			 if(st1!=NULL&&st1!=ESP_OK){  //如果ST1不为空并且ST1不为OK  --正常时
			  weather1.fengx=st1;
			  //printf("今天风向=%s\n",weather1.fengx);	
			 }			 

			 st=strstr(recv_buf,substr11); //在字符串recv_buf中查找第一次出现字符串的位置，不包含终止符'\0'
			 st1=splitx(st,substr13,substr14);//将字符串ST分割，分割符号"temp\":\""，要分割的第几个符号
			 if(st1!=NULL&&st1!=ESP_OK){  //如果ST1不为空并且ST1不为OK  --正常时
			  weather1.fengji=st1;
			  //printf("今天风力=%s\n",weather1.fengji);	 
			 }
             T_buf=&recv_buf[0];  //512字节的网页数据缓冲区  356
			 T_buf=T_buf+480;
			 //printf("T_buf=%s\n",T_buf);	 //打印获取到的
			 st=strstr(T_buf,substr1); //在字符串recv_buf中查找第一次出现字符串的位置，不包含终止符'\0'
			 st1=splitx(st,substr1,substr2);//将字符串ST分割，分割符号"temp\":\""，要分割的第几个符号
			 if(st1!=NULL&&st1!=ESP_OK){  //如果ST1不为空并且ST1不为OK  --正常时
			  weather2.rizi=st1;
			  //printf("明天=%s\n",weather2.rizi);	 
			 }	

			 st=strstr(T_buf,substr3); //在字符串recv_buf中查找第一次出现字符串的位置，不包含终止符'\0'
			 st1=splitx(st,substr3,substr4);//将字符串ST分割，分割符号"temp\":\""，要分割的第几个符号
			 if(st1!=NULL&&st1!=ESP_OK){  //如果ST1不为空并且ST1不为OK  --正常时
			  weather2.xwtianqi=st1;
			  //printf("明天图标=%s\n",weather2.xwtianqi);	 
			 }

			 st=strstr(T_buf,substr5); //在字符串recv_buf中查找第一次出现字符串的位置，不包含终止符'\0'
			 st1=splitx(st,substr5,substr6);//将字符串ST分割，分割符号"temp\":\""，要分割的第几个符号
			 if(st1!=NULL&&st1!=ESP_OK){  //如果ST1不为空并且ST1不为OK  --正常时
			  weather2.tianqi=st1;
			  //printf("明天天气=%s\n",weather2.tianqi);	 
			 }

			 st=strstr(T_buf,substr7); //在字符串recv_buf中查找第一次出现字符串的位置，不包含终止符'\0'
			 st1=splitx(st,substr7,substr8);//将字符串ST分割，分割符号"temp\":\""，要分割的第几个符号
			 if(st1!=NULL&&st1!=ESP_OK){  //如果ST1不为空并且ST1不为OK  --正常时
			  weather2.zgwendu=st1;
			  //printf("明天最高温度=%s\n",weather2.zgwendu);	 
			 }

			 st=strstr(T_buf,substr5); //在字符串recv_buf中查找第一次出现字符串的位置，不包含终止符'\0'
			 st1=splitx(st,substr9,substr10);//将字符串ST分割，分割符号"temp\":\""，要分割的第几个符号
			 if(st1!=NULL&&st1!=ESP_OK){  //如果ST1不为空并且ST1不为OK  --正常时
			  weather2.zdwendu=st1;
			  //printf("明天最低温度=%s\n",weather2.zdwendu);	 
			 }

			 st=strstr(T_buf,substr11); //在字符串recv_buf中查找第一次出现字符串的位置，不包含终止符'\0'
			 st1=splitx(st,substr11,substr12);//将字符串ST分割，分割符号"temp\":\""，要分割的第几个符号
			 if(st1!=NULL&&st1!=ESP_OK){  //如果ST1不为空并且ST1不为OK  --正常时
			  weather2.fengx=st1;
			  //printf("明天风向=%s\n",weather2.fengx);	 
			 }			 

			 st=strstr(T_buf,substr11); //在字符串recv_buf中查找第一次出现字符串的位置，不包含终止符'\0'
			 st1=splitx(st,substr13,substr14);//将字符串ST分割，分割符号"temp\":\""，要分割的第几个符号
			 if(st1!=NULL&&st1!=ESP_OK){  //如果ST1不为空并且ST1不为OK  --正常时
			  weather2.fengji=st1;
			  //printf("明天风力=%s\n",weather2.fengji);	 
			 }
			 T_buf=T_buf+340;	  		 		 	 
             //printf("T_buf=%s\n",T_buf);	 
			 st=strstr(T_buf,substr1); //在字符串recv_buf中查找第一次出现字符串的位置，不包含终止符'\0'
			 st1=splitx(st,substr1,substr2);//将字符串ST分割，分割符号"temp\":\""，要分割的第几个符号
			 if(st1!=NULL&&st1!=ESP_OK){  //如果ST1不为空并且ST1不为OK  --正常时
			  weather3.rizi=st1;
			  //printf("后天=%s\n",weather3.rizi);	 
			 }	

			 st=strstr(T_buf,substr3); //在字符串recv_buf中查找第一次出现字符串的位置，不包含终止符'\0'
			 st1=splitx(st,substr3,substr4);//将字符串ST分割，分割符号"temp\":\""，要分割的第几个符号
			 if(st1!=NULL&&st1!=ESP_OK){  //如果ST1不为空并且ST1不为OK  --正常时
			  weather3.xwtianqi=st1;
			  //printf("后天图标=%s\n",weather3.xwtianqi);	
			 }

			 st=strstr(T_buf,substr5); //在字符串recv_buf中查找第一次出现字符串的位置，不包含终止符'\0'
			 st1=splitx(st,substr5,substr6);//将字符串ST分割，分割符号"temp\":\""，要分割的第几个符号
			 if(st1!=NULL&&st1!=ESP_OK){  //如果ST1不为空并且ST1不为OK  --正常时
			  weather3.tianqi=st1;
			  //printf("后天天气=%s\n",weather3.tianqi);	 
			 }

			 st=strstr(T_buf,substr7); //在字符串recv_buf中查找第一次出现字符串的位置，不包含终止符'\0'
			 st1=splitx(st,substr7,substr8);//将字符串ST分割，分割符号"temp\":\""，要分割的第几个符号
			 if(st1!=NULL&&st1!=ESP_OK){  //如果ST1不为空并且ST1不为OK  --正常时
			  weather3.zgwendu=st1;
			  //printf("后天最高温度=%s\n",weather3.zgwendu);	
			 }

			 st=strstr(T_buf,substr5); //在字符串recv_buf中查找第一次出现字符串的位置，不包含终止符'\0'
			 st1=splitx(st,substr9,substr10);//将字符串ST分割，分割符号"temp\":\""，要分割的第几个符号
			 if(st1!=NULL&&st1!=ESP_OK){  //如果ST1不为空并且ST1不为OK  --正常时
			  weather3.zdwendu=st1;
			  //printf("后天最低温度=%s\n",weather3.zdwendu);	
			 }

			 st=strstr(T_buf,substr11); //在字符串recv_buf中查找第一次出现字符串的位置，不包含终止符'\0'
			 st1=splitx(st,substr11,substr12);//将字符串ST分割，分割符号"temp\":\""，要分割的第几个符号
			 if(st1!=NULL&&st1!=ESP_OK){  //如果ST1不为空并且ST1不为OK  --正常时
			  weather3.fengx=st1;
			  //printf("后天风向=%s\n",weather3.fengx);	 
			 }			 

			 st=strstr(T_buf,substr11); //在字符串recv_buf中查找第一次出现字符串的位置，不包含终止符'\0'
			 st1=splitx(st,substr13,substr14);//将字符串ST分割，分割符号"temp\":\""，要分割的第几个符号
			 if(st1!=NULL&&st1!=ESP_OK){  //如果ST1不为空并且ST1不为OK  --正常时
			  weather3.fengji=st1;
			  //printf("后天风力=%s\n",weather3.fengji);	
			 }

			 st1=NULL;  //将ST1置空
			 st=NULL;
			 break;
			}
        } while(r > 0);  

        //打印剩余多少内容没有读取
        ESP_LOGI(TAG, "... done reading from socket. Last read return=%d errno=%d\r\n", r, errno);
        close(s);  

        i=strcmp(weather1.rizi,weather2.rizi);
        j=strcmp(weather3.rizi,weather2.rizi);
        for(int countdown = 300; countdown >= 0; countdown--) {
			if((i==0)||(j==0))  //说明采的数据有问题
		    {
			   break;
            }else weather1.biaozhiwei=1;
            ESP_LOGI(TAG, "%d... ", countdown);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
		ESP_LOGI(TAG, "Starting again2!"); 
		vTaskResume(xHandle_task1);  //恢复任务1
		vTaskSuspend(NULL);//挂起自身任务（任务2）
    }
}

//SNTP获取并显示网络时间
static void sntp_task(void * pvParameters)
{
    static lv_style_t font_style;  //创建一个样式
    lv_style_init(&font_style);  //初始化样式
    lv_style_set_text_font(&font_style, LV_STATE_DEFAULT, &lv_font_montserrat_24);  

	label3 = lv_label_create(lv_scr_act(), NULL);   //创建时间文本标签
	lv_style_set_text_color(&font_style, LV_STATE_DEFAULT, LV_COLOR_WHITE); 
	lv_label_set_text(label3, NULL);	
	lv_obj_align(label3, NULL, LV_ALIGN_IN_TOP_LEFT,10,10); 
    lv_obj_add_style(label3,LV_LABEL_PART_MAIN, &font_style); 

	initialize_sntp(); //初始化Sntp

	while (1)
	{
		get_now_time();	 
		//sntp_stop();  //停止SNTP
		vTaskDelay(100); 
	}
}

//初始化Sntp
void initialize_sntp(void)
{
	ESP_LOGI(TAG, "------------Initializing SNTP-----------");
	sntp_setoperatingmode(SNTP_OPMODE_POLL);  
	sntp_setservername(0, "ntp1.aliyun.com"); 
	sntp_init();
}

//打印当前网络时间
void get_now_time(void)
{
	time_t now = 0;
	struct tm timeinfo = {0};
	char strftime_buf[30]; // 实际时间的字符串

	uint8_t C_Str = 0;				 // 字符串字节计数
	char A_Str_Data[20] = {0};		 // 【"日期"】字符串数组
	char *T_A_Str_Data = A_Str_Data; // 缓存数组指针
	char A_Str_Clock[10] = {0};		 // 【"时间"】字符串数组
	char *Str_Head_Week;			 // 【"星期"】字符串首地址
	char *Str_Head_Month;			 // 【"月份"】字符串首地址
	char *Str_Head_Day;				 // 【"日数"】字符串首地址
	char *Str_Head_Clock;			 // 【"时钟"】字符串首地址
	char *Str_Head_Year;			 // 【"年份"】字符串首地址

	setenv("TZ", "CST-8", 1); //改变或增加环境变量的内容  环境变量名称，变量内容，是否要改变已存在的环境变量
	tzset();				 

	time(&now);					  //获取网络时间， 64bit的秒计数
	localtime_r(&now, &timeinfo); //转换成具体的时间参数  C语言自带函数-可重入函数，线程安全

	ESP_LOGI(TAG, "-------current time: %d:%d:%d:%d:%d:%d:%d:%d:%d", //显示获取到的时间戳
			 timeinfo.tm_isdst, timeinfo.tm_yday,
			 timeinfo.tm_wday, timeinfo.tm_year,
			 timeinfo.tm_mon, timeinfo.tm_mday,
			 timeinfo.tm_hour, timeinfo.tm_min,
			 timeinfo.tm_sec);
	strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo); //将时间格式化   保存地址，最大字符数，定义格式，指向TM结构的指针

	ESP_LOGI(TAG, "The current date / time in China is: %s", strftime_buf); 

	// 【"年份" + ' '】填入日期数组
	Str_Head_Year = strftime_buf; // 设置起始地址
	while (*Str_Head_Year)		  // 找到【"实际时间"】字符串的结束字符'\0'
		Str_Head_Year++;		  //地址自增1
	// 【注：API返回的实际时间字符串，最后还有一个换行符，所以这里 -5】
	Str_Head_Year -= 4; // 获取【"年份"】字符串的首地址
	T_A_Str_Data[4] = ' ';
	memcpy(T_A_Str_Data, Str_Head_Year, 4); // 【"年份" + ' '】填入日期数组
	T_A_Str_Data += 5;						// 指向【"年份" + ' '】字符串的后面的地址

	// 获取【日期】字符串的首地址
	Str_Head_Week = strftime_buf;
	// "星期" 字符串的首地址
	Str_Head_Month = strstr(Str_Head_Week, " ") + 1; // "月份" 字符串的首地址
	Str_Head_Day = strstr(Str_Head_Month, " ") + 2;	 // "日数" 字符串的首地址
	Str_Head_Clock = strstr(Str_Head_Day, " ") + 1;	 // "时钟" 字符串的首地址

	// 【"月份" + ' '】填入日期数组
	C_Str = Str_Head_Day - Str_Head_Month;		 // 【"月份" + ' '】的字节数
	memcpy(T_A_Str_Data, Str_Head_Month, C_Str); // 【"月份" + ' '】填入日期数组
	T_A_Str_Data += C_Str;						 // 指向【"月份" + ' '】字符串的后面的地址

	// 【"日数" + ' '】填入日期数组
	C_Str = Str_Head_Clock - Str_Head_Day;	   // 【"日数" + ' '】的字节数
	memcpy(T_A_Str_Data, Str_Head_Day, C_Str); // 【"日数" + ' '】填入日期数组
	T_A_Str_Data += C_Str;					   // 指向【"日数" + ' '】字符串的后面的地址

	// 【"星期" + ' '】填入日期数组
	C_Str = Str_Head_Month - Str_Head_Week - 1; // 【"星期"】的字节数
	memcpy(T_A_Str_Data, Str_Head_Week, C_Str); // 【"星期"】填入日期数组
	T_A_Str_Data += C_Str;						// 指向【"星期"】字符串的后面的地址

    //时间确定。连接到WiFi并通过NTP获得时间
    if (timeinfo.tm_year > (2016 - 1900)) {
		// LCD显示【"日期"】、【"时钟"】字符串
		memcpy(A_Str_Clock, Str_Head_Clock, 8); // 【"时钟"】字符串填入时钟数组
		A_Str_Clock[8] = '\0';
		lv_label_set_text(label3, A_Str_Clock); //文本显示--动态文本（字符串形式）LCD显示时间
    }
}

//LVGL开机图任务
void startup_task(void)
{
	static uint8_t page_id = 0;

	switch (page_id)
	{
	case 0:
		page_id = 1;
		lv_img_set_src(img2, &windows_gd0);
		break;
	case 1:
		page_id = 2;
		lv_img_set_src(img2, &windows_gd1);
		break;
	case 2:
		page_id = 3;
		lv_img_set_src(img2, &windows_gd2);
		break;
	case 3:
		page_id = 4;
		lv_img_set_src(img2, &windows_gd3);
		break;
	case 4:
		page_id = 5;
		lv_img_set_src(img2, &windows_gd4);
		break;
	case 5:
		page_id = 6;
		lv_img_set_src(img2, &windows_gd5);
		break;
	case 6:
		page_id = 7;
		lv_img_set_src(img2, &windows_gd6);
		break;
	case 7:
		page_id = 8;
		lv_img_set_src(img2, &windows_gd7);
		break;
	case 8:
		page_id = 9;
		lv_img_set_src(img2, &windows_gd8);
		break;
	case 9:
		page_id = 10;
		lv_img_set_src(img2, &windows_gd9);
		break;
	case 10:
		page_id = 11;
		lv_img_set_src(img2, &windows_gd10);
		break;
	case 11:
		page_id = 0;
		lv_img_set_src(img2, &windows_gd11);
		break;
	default:
		break;
	}
	//lv_task_del(task1);task1 = NULL;
}

//显示开机图片
//创建LVGL图片刷新任务
void draw_img(void)
{
	//显示开机背景--windows
	img1 = lv_img_create(lv_scr_act(), NULL);
	lv_img_set_src(img1, &windows_kj0);
	lv_obj_align(img1, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    //显示滚动图片第一张
	img2 = lv_img_create(lv_scr_act(), NULL);
	lv_img_set_src(img2, &windows_gd0);
	lv_obj_align(img2, NULL, LV_ALIGN_IN_TOP_LEFT, 78, 164);
    //创建一个LVGL任务--每100个节拍运行一次
	task1 = lv_task_create((lv_task_cb_t)startup_task, 100, LV_TASK_PRIO_MID, NULL);
}

// 主函数
void app_main()
{
	ESP_LOGI(TAG, "\nAPP is start!~\n");
	ESP_ERROR_CHECK( nvs_flash_init() );  //初始化存储
    ESP_ERROR_CHECK(esp_netif_init());  //初始化网络接口

    /* 初始化非易失性存储库 (NVS) */
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        /* NVS分区被截断，需要删除,然后重新初始化NVS */
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

	// 如果要使用任务创建图形，则需要创建固定任务,否则可能会出现诸如内存损坏等问题
	xTaskCreatePinnedToCore(gui_task, "gui", 1024*3, NULL, 3, NULL, 1);  //初始化显示以及开机界面

    /* 定义一个NVS操作句柄 */
    nvs_handle wificfg_nvs_handler1;
    /* 打开一个NVS命名空间 */
    ESP_ERROR_CHECK( nvs_open("WiFi_cfg", NVS_READWRITE, &wificfg_nvs_handler1) );

    uint32_t wifi_update = 0;
    err = nvs_get_u32(wificfg_nvs_handler1,"wifi_update",&wifi_update);
    if(MY_WIFI_UPDATE == wifi_update )
        ESP_LOGI(TAG,"wifi_cfg needn't to update. \n");
    else
    {
        ESP_LOGI(TAG,"wifi_cfg update now... \n");
        ESP_ERROR_CHECK( nvs_set_str(wificfg_nvs_handler1,"wifi_ssid",MY_WIFI_SSID) );
        ESP_ERROR_CHECK( nvs_set_str(wificfg_nvs_handler1,"wifi_passwd",MY_WIFI_PASSWD) ); 
        ESP_ERROR_CHECK( nvs_set_u32(wificfg_nvs_handler1,"wifi_update",MY_WIFI_UPDATE) );
        ESP_LOGI(TAG,"wifi_cfg update ok. \n");
    }
    ESP_ERROR_CHECK( nvs_commit(wificfg_nvs_handler1) ); /* 提交 */
    nvs_close(wificfg_nvs_handler1);                     /* 关闭 */

	s_wifi_event_group = xEventGroupCreate();  //创建WIFI连接事件组
	ESP_ERROR_CHECK(esp_event_loop_create_default());//默认事件循环
	esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta(); //创建默认wifi站点--STA
    assert(sta_netif);  //断言  为假终止程序
	// wifi设置:默认设置，等待SmartConfig配置
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
	// 注册wifi事件  分别是WIFI事件，IP地址事件，smartconfig事件
	ESP_ERROR_CHECK( esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL) );
	ESP_ERROR_CHECK( esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL) );
	ESP_ERROR_CHECK( esp_event_handler_register(SC_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL) );

    //取出NVS中的WIFI账号和密码--取出NVS中的WIFI账号和密码--取出NVS中的WIFI账号和密码
    nvs_handle wificfg_nvs_handler; /* 定义一个NVS操作句柄 */
    char wifi_ssid[32] = { 0 };     /* 定义一个数组用来存储ssid*/
    char wifi_passwd[65] = { 0 };   /* 定义一个数组用来存储passwd */
    size_t len;
    /* 打开一个NVS命名空间 */
    ESP_ERROR_CHECK( nvs_open("WiFi_cfg", NVS_READWRITE, &wificfg_nvs_handler) );
    len = sizeof(wifi_ssid);    /* 从NVS中获取ssid */
    ESP_ERROR_CHECK( nvs_get_str(wificfg_nvs_handler,"wifi_ssid",wifi_ssid,&len) );
    len = sizeof(wifi_passwd);      /* 从NVS中获取passwd */
    ESP_ERROR_CHECK( nvs_get_str(wificfg_nvs_handler,"wifi_passwd",wifi_passwd,&len) );
    ESP_ERROR_CHECK( nvs_commit(wificfg_nvs_handler) ); /* 提交 */
    nvs_close(wificfg_nvs_handler);                     /* 关闭 */
    /* 设置WiFi连接的ssid和password参数 */
    wifi_config_t wifi_config;
    bzero(&wifi_config, sizeof(wifi_config_t)); /* 将结构体数据清零 */
    memcpy(wifi_config.sta.ssid, wifi_ssid, sizeof(wifi_config.sta.ssid));
    memcpy(wifi_config.sta.password, wifi_passwd, sizeof(wifi_config.sta.password));

	// 设置sta模式
	ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
	/* 设置WiFi连接的参数，主要是ssid和password */
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
	// 启动wifi
	ESP_ERROR_CHECK( esp_wifi_start() );
}