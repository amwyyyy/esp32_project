#include <Arduino.h>
#include <U8g2lib.h>
#include <WiFi.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

const char *ssid = "xiongda";
const char *password = "15999554794";

WiFiServer server(80);

void initWiFi() {
  delay(10);
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

  server.begin();
}

void setup(void) {

  Serial.begin(115200);
  
  initWiFi();
  
  u8g2.begin(/* menu_select_pin= */ 2, /* menu_next_pin= */ 3, /* menu_prev_pin= */ 7, /* menu_home_pin= */ 8);

  u8g2.begin(/*Select=*/ 0, /*Right/Next=*/ 1, /*Left/Prev=*/ 2, /*Up=*/ 4, /*Down=*/ 3, /*Home/Cancel=*/ A6); 

  u8g2.setFont(u8g2_font_6x12_tr);
}

struct menu_entry_type
{
  const uint8_t *font;
  uint16_t icon;
  const char *name;
};

struct menu_state
{
  int16_t menu_start;		/* in pixel */
  int16_t frame_position;		/* in pixel */
  uint8_t position;			/* position, array index */
};

/*
  Icon configuration
  Width and height must match the icon font size
  GAP: Space between the icons
  BGAP: Gap between the display border and the cursor.
*/
#define ICON_WIDTH 32
#define ICON_HEIGHT 32
#define ICON_GAP 4
#define ICON_BGAP 16
#define ICON_Y 32+ ICON_GAP

struct menu_entry_type menu_entry_list[] =
{
  { u8g2_font_open_iconic_embedded_4x_t, 65, "Clock Setup"},
  { u8g2_font_open_iconic_embedded_4x_t, 66, "Gear Game"},
  { u8g2_font_open_iconic_embedded_4x_t, 67, "Flash Light"},
  { u8g2_font_open_iconic_embedded_4x_t, 68, "Home"},
  { u8g2_font_open_iconic_embedded_4x_t, 72, "Configuration"},
  { NULL, 0, NULL } 
};

int8_t button_event = 0;		// set this to 0, once the event has been processed

void check_button_event(void)
{
  if ( button_event == 0 )
    button_event = u8g2.getMenuEvent();
}


void draw(struct menu_state *state)
{
  int16_t x;
  uint8_t i;
  x = state->menu_start;
  i = 0;
  while( menu_entry_list[i].icon > 0 )  
  {
    if ( x >= -ICON_WIDTH && x < u8g2.getDisplayWidth() )
    {
      u8g2.setFont(menu_entry_list[i].font);
      u8g2.drawGlyph(x, ICON_Y, menu_entry_list[i].icon );
    }
    i++;
    x += ICON_WIDTH + ICON_GAP;
    check_button_event();
  }
  u8g2.drawFrame(state->frame_position-1, ICON_Y-ICON_HEIGHT-1, ICON_WIDTH+2, ICON_WIDTH+2);
  u8g2.drawFrame(state->frame_position-2, ICON_Y-ICON_HEIGHT-2, ICON_WIDTH+4, ICON_WIDTH+4);
  u8g2.drawFrame(state->frame_position-3, ICON_Y-ICON_HEIGHT-3, ICON_WIDTH+6, ICON_WIDTH+6);
  check_button_event();
}


void to_right(struct menu_state *state)
{
  if ( menu_entry_list[state->position+1].font != NULL )
  {
    if ( (int16_t)state->frame_position+ 2*(int16_t)ICON_WIDTH + (int16_t)ICON_BGAP < (int16_t)u8g2.getDisplayWidth() )
    {
      state->position++;
      state->frame_position += ICON_WIDTH + (int16_t)ICON_GAP;
    }
    else
    {
      state->position++;      
      state->frame_position = (int16_t)u8g2.getDisplayWidth() - (int16_t)ICON_WIDTH - (int16_t)ICON_BGAP;
      state->menu_start = state->frame_position - state->position*((int16_t)ICON_WIDTH + (int16_t)ICON_GAP);
    }
  }
}

void to_left(struct menu_state *state)
{
  if ( state->position > 0 )
  {
    if ( (int16_t)state->frame_position >= (int16_t)ICON_BGAP+(int16_t)ICON_WIDTH+ (int16_t)ICON_GAP )
    {
      state->position--;
      state->frame_position -= ICON_WIDTH + (int16_t)ICON_GAP;
    }    
    else
    {
      state->position--; 
      state->frame_position = ICON_BGAP;
      state->menu_start = state->frame_position - state->position*((int16_t)ICON_WIDTH + (int16_t)ICON_GAP);      
    }
  }
}


uint8_t towards_int16(int16_t *current, int16_t dest)
{
  if ( *current < dest )
  {
    (*current)++;
    return 1;
  }
  else if ( *current > dest )
  {
    (*current)--;
    return 1;
  }
  return 0;
}

uint8_t towards(struct menu_state *current, struct menu_state *destination)
{
  uint8_t r = 0;
  uint8_t i;
  for( i = 0; i < 6; i++ )
  {
    r |= towards_int16( &(current->frame_position), destination->frame_position);
    r |= towards_int16( &(current->menu_start), destination->menu_start);
  }
  return r;
}

struct menu_state current_state = { ICON_BGAP, ICON_BGAP, 0 };
struct menu_state destination_state = { ICON_BGAP, ICON_BGAP, 0 };

void loop(void) {
  do
  {
    u8g2.firstPage();
    do
    {
      draw(&current_state);  
      u8g2.setFont(u8g2_font_helvB10_tr);  
      u8g2.setCursor((u8g2.getDisplayWidth()-u8g2.getStrWidth(menu_entry_list[destination_state.position].name))/2,u8g2.getDisplayHeight()-5);
      u8g2.print(menu_entry_list[destination_state.position].name);
      check_button_event();
      delay(10);
    } while( u8g2.nextPage() );
    if ( button_event == 1 )
      to_right(&destination_state);
    if ( button_event == 2 )
      to_left(&destination_state);
    if ( button_event == 3 )
    {
      u8g2.setFont(u8g2_font_helvB10_tr);  
      u8g2.userInterfaceMessage("Selection:", menu_entry_list[destination_state.position].name, "", " Ok ");
    }
    if ( button_event > 0 ) {
      button_event = 0;
    }

    WiFiClient client = server.available();
    if (client) {                             
      Serial.println("New Client.");          
      String currentLine = "";                
      while (client.connected()) {            
        if (client.available()) {             
          char c = client.read();             
          Serial.write(c);                    
          if (c == '\n') {                 
            if (currentLine.length() == 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println();

              client.print("<input style=\"width:150px;height:100px\" value=\"LEFT\" type=\"button\" onclick=\"location.href='/H'\" />");
              client.print("<input style=\"width:150px;height:100px\" value=\"RIGHT\" type=\"button\" onclick=\"location.href='/L'\" />");
              client.print("<input style=\"width:150px;height:100px\" value=\"OK\" type=\"button\" onclick=\"location.href='/OK'\" />");

              client.println();
              break;
            } else {
              currentLine = "";
            }
          } else if (c != '\r') {
            currentLine += c;
          }

          if (currentLine.endsWith("GET /H")) {
            Serial.println("\nLEFT");
            button_event = 2;
          }
          if (currentLine.endsWith("GET /L")) {
            Serial.println("\nRIGHT");
            button_event = 1;
          }
          if (currentLine.endsWith("GET /OK")) {
            Serial.println("\nOK");
            button_event = 3;
          }
        }
      }
      
      client.stop();
      Serial.println("Client Disconnected.");
    }
  } while ( towards(&current_state, &destination_state) );
}
