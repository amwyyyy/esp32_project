#ifndef _wifi_scan_h_
#define _wifi_scan_h_

/**
 * @brief wifi执行事件
 * 
 */
#define SCAN_DONE_BIT BIT0
#define SCAN_LIST_DONE_BIT BIT1
#define CONNECT_WIFI_BIT BIT2
#define TRY_CONNECT_WIFI_BIT BIT3
#define CONNECT_DONE_BIT BIT4
#define CONNECT_FAIL_BIT BIT5

/**
 * @brief wifi当前状态
 * 
 */
#define WIFI_OP_TYPED_NULL 0        //空闲
#define WIFI_OP_TYPED_CONNECT 1     //连接中
#define WIFI_OP_TYPED_TRY_CONNECT 2 //尝试连接中
#define WIFI_OP_TYPED_DISCONNECT 3  //断开连接
#define WIFI_OP_TYPED_SCAN_START 4  //扫描
#define WIFI_OP_TYPED_SCAN_DONE 5   //扫描完毕

/**
 * @brief wifi当前是否连接
 * 
 */
#define WIFI_CONNECT_OK 0   //已连接
#define WIFI_CONNECT_NULL 1 //未连接

/**
 * @brief Get the ip str object 输出ipv4地址字符串
 * 
 * @return char* 字符串指针地址
 */
char *get_ip_str(void);
/**
 * @brief 扫描wifi
 * 
 */
void wifi_scan_init(void);
/**
 * @brief 连接wifi
 * 
 * @param ssid ssid
 * @param password 密码
 */
void wifi_connect_init(uint8_t ssid[32], uint8_t password[64]);

#endif
