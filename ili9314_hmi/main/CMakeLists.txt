idf_component_register(SRCS "task_mag.c" "page/main_page.c"
                            "desktop/logo.c"
                            "desktop/sky.c"
                            "desktop/flower.c"
                            "desktop/leaf.c"
                            "music.c"
                            "gpio.c"
                            "storage.c"
                            "wifi.c"
                            "task_mag.c"
                            "pwm.c"
                            "http_time.c"
                            "wifi_scan.c"
                            "page/wifi_scan_page.c"
                            "page/weather_page.c"
                            "page/music_page.c"
                            "page/setting_page.c"
                    INCLUDE_DIRS "" "inc")

# Create a SPIFFS image from the contents of the 'spiffs' directory
# that fits the partition named 'storage'. FLASH_IN_PROJECT indicates that
# the generated image should be flashed when the entire project is flashed to
# the target with 'idf.py -p PORT flash'.
# spiffs_create_partition_image(storage ../spiffs FLASH_IN_PROJECT)


