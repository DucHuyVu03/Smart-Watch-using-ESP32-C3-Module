#include <Arduino_GFX_Library.h>
#include "CST816D.h"
//UPDATE 4/5/2024

#define GFX_BL 3
#define I2C_SDA 4
#define I2C_SCL 5
#define TP_INT 0
#define TP_RST 1
CST816D touch(I2C_SDA, I2C_SCL, TP_RST, TP_INT);

#if defined(DISPLAY_DEV_KIT)
Arduino_GFX *gfx = create_default_Arduino_GFX();
#else /* !defined(DISPLAY_DEV_KIT) */

/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
Arduino_DataBus *bus = new Arduino_SWSPI(2 /*DC*/, 10 /*CS*/, 6 /*SCK*/, 7 /*MOSI*/, GFX_NOT_DEFINED /*MISO */);

//Arduino_DataBus *bus = new Arduino_SWSPI(8 /* DC */, 9 /* CS */, 13 /* SCK */, 11 /* MOSI */, GFX_NOT_DEFINED /* MISO */);

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
Arduino_GFX *gfx = new Arduino_GC9A01(  bus, -1 /*RST*/, 0 /*rotation*/, true /*IPS */);
#endif /* !defined(DISPLAY_DEV_KIT) */
int x, y = 0;
int screenWidth = 240;   //tft.width(); // Lấy chiều rộng của màn hình
int screenHeight = 240;
void setup() {
  // put your setup code here, to run once:
  USBSerial.begin(115200);
  Serial.setDebugOutput(true);
  // while(!Serial);
  USBSerial.println("Arduino_GFX Hello World Gfxfont example");
  touch.begin();

#ifdef GFX_EXTRA_PRE_INIT
  GFX_EXTRA_PRE_INIT();
#endif

  // Init Display
  if (!gfx->begin())
  {
    USBSerial.println("gfx->begin() failed!");
  }
  gfx->fillScreen(BLACK);

#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif

   gfx->fillScreen(BLACK);         // Lấp đầy màn hình với màu đen
   delay(10);
   gfx->fillRect(x, y, screenWidth / 2, screenHeight / 2, GREEN);
   gfx->fillRect((screenWidth / 2) + 1, y, screenWidth, (screenHeight / 2), PURPLE);
   gfx->fillRect(x, (screenHeight / 2) + 1, screenWidth / 2, screenHeight, RED);
   gfx->fillRect((screenWidth / 2) + 1, (screenHeight / 2) + 1, screenWidth, screenHeight, YELLOW);
}


void handleTouch(void) 
{
   // Xử lý điều khiển dựa trên tọa độ cảm ứng
   if (x < screenWidth / 2 && y < screenHeight / 2) {
     Serial.println("Bạn đã nhấn vào phân vùng đỏ");
     gfx->fillRect(x, y, screenWidth / 2, screenHeight / 2, WHITE);
   } else if (x >= screenWidth / 2 && y < screenHeight / 2) {
     Serial.println("Bạn đã nhấn vào phân vùng xanh dương");
     gfx->fillRect((screenWidth / 2) + 1, y, screenWidth, (screenHeight / 2), WHITE);
   } else if (x < screenWidth / 2 && y >= screenHeight / 2) {
     Serial.println("Bạn đã nhấn vào phân vùng xanh lá");
     gfx->fillRect(x, (screenHeight / 2) + 1, screenWidth / 2, screenHeight, WHITE);
   } else if (x >= screenWidth / 2 && y >= screenHeight / 2) {
     Serial.println("Bạn đã nhấn vào phân vùng màu vàng");
     gfx->fillRect((screenWidth / 2) + 1, (screenHeight / 2) + 1, screenWidth, screenHeight, WHITE);
   }
   delay(10);
 }


void loop() 
{
  uint16_t x, y;
  uint8_t gesture;
  if (touch.getTouch(&x, &y, &gesture)) 
  {
    handleTouch();
    USBSerial.print("Touched at (");
    USBSerial.print(x);
    USBSerial.print(", ");
    USBSerial.print(y);
    USBSerial.print(") with gesture: ");
    switch (gesture) {
      case None:
        USBSerial.println("None");
        break;
      case SlideDown:
        USBSerial.println("SlideDown");
        break;
      case SlideUp:
        USBSerial.println("SlideUp");
        break;
      case SlideLeft:
        USBSerial.println("SlideLeft");
        break;
      case SlideRight:
        USBSerial.println("SlideRight");
        break;
      case SingleTap:
        USBSerial.println("SingleTap");
        break;
      case DoubleTap:
        USBSerial.println("DoubleTap");
        break;
      case LongPress:
        USBSerial.println("LongPress");
        break;
      default:
        USBSerial.println("Unknown");
        break;
    }
  }
  delay(10); // Đợi một chút trước khi đọc giá trị mới
}


