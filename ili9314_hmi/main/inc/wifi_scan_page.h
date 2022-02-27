#ifndef _wifi_scan_page_h_
#define _wifi_scan_page_h_

/**
 * @brief ssid列表最多展示个数
 * 
 */
#define LIST_MAX_SSID 10
/**
 * @brief Get the wifi scan page ssid info object 获取 wifi_scan_page 页面的当前 ssid 与 密码信息
 * 
 * @param ssid 输出 ssid
 * @param pwd 输出 密码
 * @return uint8_t true 有效信息 false 无效信息
 */
uint8_t get_wifi_scan_page_ssid_info(char *ssid, char *pwd);
/**
 * @brief 页面任务
 * 
 */
void wifi_scan_page_task(void);

#endif
