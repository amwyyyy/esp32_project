file(GLOB_RECURSE SOURCES lv_examples/*.c lv_examples/assets/*.c)
idf_component_register(SRCS ${SOURCES}
                       INCLUDE_DIRS .
                       REQUIRES lvgl)
