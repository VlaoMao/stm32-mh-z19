//
// Created by sl on 18.02.17.
//

#ifndef STM32_MH_Z19_PCD8544_H
#define STM32_MH_Z19_PCD8544_H

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include <wchar.h>


#define PCD8544_RST GPIO0
#define PCD8544_RST_PORT GPIOB

#define PCD8544_DC GPIO1
#define PCD8544_DC_PORT GPIOA
#define PCD8544_SCE GPIO12
#define PCD8544_SCE_PORT GPIOB

#define PCD8544_SPI SPI2
#define PCD8544_SPI_PORT GPIOB
#define PCD8544_SPI_MOSI GPIO15
#define PCD8544_SPI_SCK  GPIO13
#define PCD8544_SPI_SS  GPIO12


//#define LCD_VCC_Pin GPIO_PIN_9
//#define LCD_VCC_GPIO_Port GPIOA
//#define LCD_CE_Pin GPIO_PIN_11
//#define LCD_CE_GPIO_Port GPIOA
//#define LCD_RST_Pin GPIO_PIN_12
//#define LCD_RST_GPIO_Port GPIOA
//#define LCD_DC_Pin GPIO_PIN_15
//#define LCD_DC_GPIO_Port GPIOA
//#define LCD_CLK_Pin GPIO_PIN_3
//#define LCD_CLK_GPIO_Port GPIOB
//#define LCD_DIN_Pin GPIO_PIN_5
//#define LCD_DIN_GPIO_Port GPIOB

//#define LCD_VCC_Pin GPIO9
//#define LCD_VCC_GPIO_Port GPIOA

//#define PCD8544_RST GPIO12
//#define PCD8544_RST_PORT GPIOA

//#define PCD8544_DC GPIO15
//#define PCD8544_DC_PORT GPIOA

//#define PCD8544_SCE GPIO11
//#define PCD8544_SCE_PORT GPIOA

//#define PCD8544_SPI SPI2

//#define PCD8544_SPI_PORT GPIOB
//#define PCD8544_SPI_MOSI GPIO5 // DIN
//#define PCD8544_SPI_SCK  GPIO3 // CLK
//#define PCD8544_SPI_SS  GPIO12

#ifndef _BV
#define _BV(bit) (1<<(bit))
#endif

//--- cut here --- cut here --- cut here --- :=)

#define BLACK 1
#define WHITE 0

#define LCDWIDTH 84
#define LCDHEIGHT 48


// H = 0
#define PCD8544_COMMAND_MODE 0x20
#define PCD8544_EXTENDED_MODE 0x21
#define PCD8544_DISPLAY_MODE 0x0c
#define PCD8544_SET_PAGE 0x40
#define PCD8544_SET_ADDRESS 0x80

// H = 1
#define PCD8544_SETTEMP 0x04
#define PCD8544_SETBIAS 0x10
#define PCD8544_SETVOP 0x80

//--- cut here --- cut here --- cut here --- :=)

#define PCD8544_DISPLAY_INVERTED 0x0D
#define PCD8544_DISPLAY_NORMAL 0x0C

#define SCE_LOW gpio_clear(PCD8544_SCE_PORT, PCD8544_SCE);
#define SCE_HIGH gpio_set(PCD8544_SCE_PORT, PCD8544_SCE);
#define DC_HIGH  gpio_set(PCD8544_DC_PORT, PCD8544_DC);
#define DC_LOW  gpio_clear(PCD8544_DC_PORT, PCD8544_DC);

#ifndef PCD8544_BIAS
#define PCD8544_BIAS 4
#endif //PCD8544_BIAS

void pcd8544_init(void);
void pcd8544_display(void);
void pcd8544_setContrast(uint8_t contrast); // maximum value is 0x7f
void pcd8544_drawPixel(int16_t x, int16_t y, uint16_t color);
void pcd8544_drawBitmap(int x, int y, const uint8_t *bitmap, int sx, int sy);
uint8_t pcd8544_getPixel(int8_t x, int8_t y);
void pcd8544_drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void pcd8544_drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void pcd8544_drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void pcd8544_drawVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void pcd8544_drawHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void pcd8544_clearDisplay();
void pcd8544_drawText(uint8_t x, uint8_t y, uint8_t color, wchar_t *text);
void pcd8544_setInverted(int inverted);
void clrScr();

#endif //STM32_MH_Z19_PCD8544_H
