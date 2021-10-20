#include <stdio.h>
#include "sdkconfig.h"
#include "u8g2_esp32_hal.h"

const uint8_t digitalnumbers_68_tn[347] U8G2_FONT_SECTION("digitalnumbers_68_tn") = 
  "\13\0\5\6\5\6\4\2\7\36+\0\0+\0+\0\0\0\0\0\1B \6\0\300\241\0\60%{"
  "]\303\342\62\62\341\7.Q\371\377b!\14\346`\4j`\14\346 \24\242\362\377\305\37\270\204\314\270"
  "\10\0\61\27E\365\245\42\60\6\341\7\66a\60\2c\20~\300\22\6# \0\62$ze\303\302\62"
  "\60A\23\62\2S\213\377o\134\4&`\211\313 X\342\377Y\214\310\4M\300\14\213\0\0\63\37y"
  "m\303\242\62.!\23\60\342B\213\377O\134\302\325\270\242\305\377'.!\26.\243\42\0\64\35[\335"
  "\303\42P\3c\60\7\241\20\225\377\277\250\13\231\221e\213\377\277;\310\1\1\0\65\42{]\303\342\62"
  "\62\241\26\62\242`\213\377g\63B\23\66A#\62\266\370\377D&\324Bf\134\4\0\66 {]\303"
  "\342\62\62\241\26\62\242`\213\377w\61\4\63\62A\27\225\377\27\x7f\340\22\62\343\42\0\67\36Z\345\303"
  "\302\62\60A\23\62\2S\213\377\317\16p\0\17l\241\26\377\377\354\0\7\4\0\70 {]\303\342\62"
  "\62\341\7.Q\371\x7f\21\24\62\343\62\62\1\24\225\377\27\x7f\340\22\62\343\42\0\71\36{]\303\342\62"
  "\62\341\7.Q\371\x7fQ\27\62#\313\26\377\237\310\204Z\310\214\213\0\0\0\0\0";

void task_test_SSD1306i2c(void *ignore) {
    u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
	u8g2_esp32_hal.sda = 21;
	u8g2_esp32_hal.scl = 22;
	u8g2_esp32_hal_init(u8g2_esp32_hal);

    u8g2_t u8g2;
    u8g2_Setup_ssd1306_i2c_128x64_noname_f(
		&u8g2,
		U8G2_R0,
		u8g2_esp32_i2c_byte_cb,
		u8g2_esp32_gpio_and_delay_cb);  // init u8g2 structure
	u8x8_SetI2CAddress(&u8g2.u8x8, 0x78);

    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);
    u8g2_ClearDisplay(&u8g2);

    u8g2_FirstPage(&u8g2);
    do {
        u8g2_SetFont(&u8g2, digitalnumbers_68_tn);
        u8g2_DrawStr(&u8g2, 10, 40, "2244");
    } while (u8g2_NextPage(&u8g2));

    // for (int i = 0; i < 100; i++) {
    //     u8g2_FirstPage(&u8g2);
    //     do {
    //         u8g2_DrawBox(&u8g2, 10, 26, i, 6);
    //         u8g2_DrawFrame(&u8g2, 10, 26, 100, 6);
    //     } while (u8g2_NextPage(&u8g2));
    //     vTaskDelay(5 / portTICK_PERIOD_MS);
    // }

    vTaskDelete(NULL);
}

void app_main(void)
{
    printf("Hello world!\n");

    xTaskCreatePinnedToCore(task_test_SSD1306i2c, "test_task", 1024 * 4, NULL, 2, NULL, tskNO_AFFINITY);

    printf("task nums: %d\n", uxTaskGetNumberOfTasks());
}
