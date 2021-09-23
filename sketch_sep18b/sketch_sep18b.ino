#include <Arduino.h>
#include <U8g2lib.h>
#include <WiFi.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

const char *ssid = "xiongda";
const char *password = "15999554794";

WiFiServer server(80);

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup() {
    Serial.begin(115200);

    delay(10);

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    u8g2.begin(); 
    server.begin();
}

const char* str = "Init.";
void loop() {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.drawStr(0, 24, str);
  } while (u8g2.nextPage());

  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.print("<input style=\"width:150px;height:100px\" value=\"RED\" type=\"button\" onclick=\"location.href='/H'\" />");
            client.print("<input style=\"width:150px;height:100px\" value=\"GREEN\" type=\"button\" onclick=\"location.href='/L'\" />");

            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        if (currentLine.endsWith("GET /H")) {
          Serial.println("\nRED");
          str = "RED";
        }
        if (currentLine.endsWith("GET /L")) {
          Serial.println("\nGREEN");
          str = "GREEN";
        }
      }
    }
    
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
