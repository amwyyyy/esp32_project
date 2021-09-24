set(SOURCES main.c )
idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS .
                    INCLUDE_DIRS $ENV{IDF_PATH}/components/nvs_flash/include
                    INCLUDE_DIRS $ENV{IDF_PATH}/components/spi_flash/include
                    INCLUDE_DIRS $ENV{IDF_PATH}/components/wpa_supplicant/include/esp_supplicant
                    INCLUDE_DIRS $ENV{IDF_PATH}/examples/common_components/protocol_examples_common/include
                    REQUIRES lvgl_esp32_drivers lvgl_touch lvgl_tft lvgl lv_examples)

target_compile_definitions(${COMPONENT_LIB} PRIVATE LV_CONF_INCLUDE_SIMPLE=1)
