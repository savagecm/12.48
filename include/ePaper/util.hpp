#pragma once
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <iconv.h>
#define USE_BCM2835_LIB true

#ifdef USE_BCM2835_LIB
#include <bcm2835.h>
#elif USE_WIRINGPI_LIB
#include <wiringPi.h>
#elif USE_DEV_LIB
#include "RPI_sysfs_gpio.h"
#endif
/**
 * data
**/
#define UBYTE uint8_t
#define UWORD uint16_t
#define UDOUBLE uint32_t

// Display resolution
#define EPD_12in48B_MAX_WIDTH 1304
#define EPD_12in48B_MAX_HEIGHT 984

#define EPD_12in48B_M1_WIDTH 648
#define EPD_12in48B_M1_HEIGHT (EPD_MAX_HEIGHT / 2)
#define EPD_12in48B_S1_WIDTH (EPD_12in48B_MAX_WIDTH - 648) //656
#define EPD_12in48B_S1_HEIGHT (EPD_MAX_HEIGHT / 2)
#define EPD_12in48B_M2_WIDTH (EPD_12in48B_MAX_WIDTH - 648) //656
#define EPD_12in48B_M2_HEIGHT (EPD_MAX_HEIGHT / 2)
#define EPD_12in48B_S2_WIDTH 648
#define EPD_12in48B_S2_HEIGHT (EPD_MAX_HEIGHT / 2)

/**
 * GPIO config
**/
#define EPD_SCK_PIN 11
#define EPD_MOSI_PIN 10

#define EPD_M1_CS_PIN 8
#define EPD_S1_CS_PIN 7
#define EPD_M2_CS_PIN 17
#define EPD_S2_CS_PIN 18

#define EPD_M1S1_DC_PIN 13
#define EPD_M2S2_DC_PIN 22

#define EPD_M1S1_RST_PIN 6
#define EPD_M2S2_RST_PIN 23

#define EPD_M1_BUSY_PIN 5
#define EPD_S1_BUSY_PIN 19
#define EPD_M2_BUSY_PIN 27
#define EPD_S2_BUSY_PIN 24
#define SYSFS_GPIO_IN 0
#define SYSFS_GPIO_OUT 1

#define SYSFS_GPIO_LOW 0
#define SYSFS_GPIO_HIGH 1

#define NUM_MAXBUF 4
#define DIR_MAXSIZ 60

#define SYSFS_GPIO_DEBUG 0
#if SYSFS_GPIO_DEBUG
#define SYSFS_GPIO_Debug(__info, ...) printf("Debug: " __info, ##__VA_ARGS__)
#else
#define SYSFS_GPIO_Debug(__info, ...)
#endif

// BCM GPIO for Jetson nano
#define GPIO4 4      // 7, 4
#define GPIO17 7     // 11, 17
#define GPIO18 18    // 12, 18
#define GPIO27 27    // 13, 27
#define GPIO22 22    // 15, 22
#define GPIO23 23    // 16, 23
#define GPIO24 24    // 18, 24
#define SPI0_MOSI 10 // 19, 10
#define SPI0_MISO 9  // 21, 9
#define GPIO25 28    // 22, 25
#define SPI0_SCK 11  // 23, 11
#define SPI0_CS0 8   // 24, 8
#define SPI0_CS1 7   // 26, 7
#define GPIO5 5      // 29, 5
#define GPIO6 6      // 31, 6
#define GPIO12 12    // 32, 12
#define GPIO13 13    // 33, 13
#define GPIO19 19    // 35, 19
#define GPIO16 16    // 36, 16
#define GPIO26 26    // 37, 26
#define GPIO20 20    // 38, 20
#define GPIO21 21    // 40, 21

#if DEBUG
#define Debug(__info, ...) printf("Debug: " __info, ##__VA_ARGS__)
#else
#define Debug(__info, ...)
#endif

/**
 * SPI communication mode
**/
enum SPIMode
{
    Mode0, /* Clock Polarity is 0 and Clock Phase is 0 */
    Mode1, /* Clock Polarity is 0 and Clock Phase is 1 */
    Mode2, /* Clock Polarity is 1 and Clock Phase is 0 */
    Mode3, /* Clock Polarity is 1 and Clock Phase is 1 */
};

/**
 * Define SPI type
**/
enum SPIType
{
    Master,
    Slave,
};

/**
 * Define SPI attribute
**/
struct SOFTWARE_SPI
{
    UWORD SCLK_PIN;
    UWORD MOSI_PIN;
    UWORD MISO_PIN;
    UWORD CS_PIN;
    SPIMode Mode;
    SPIType Type;
    UWORD Clock;
};

/**
 * Display rotate
**/
#define ROTATE_0 0
#define ROTATE_90 90
#define ROTATE_180 180
#define ROTATE_270 270

/**
 * Display Flip
**/
enum MIRROR_IMAGE
{
    MIRROR_NONE = 0x00,
    MIRROR_HORIZONTAL = 0x01,
    MIRROR_VERTICAL = 0x02,
    MIRROR_ORIGIN = 0x03,
} MIRROR_IMAGE;
#define MIRROR_IMAGE_DFT MIRROR_NONE

/**
 * image color
**/
#define WHITE 0xFF
#define BLACK 0x00
#define RED 0x0F

#define IMAGE_BACKGROUND WHITE
#define FONT_FOREGROUND BLACK
#define FONT_BACKGROUND WHITE

/**
 * The size of the point
**/
enum DOT_PIXEL
{
    DOT_PIXEL_1X1 = 1, // 1 x 1
    DOT_PIXEL_2X2,     // 2 X 2
    DOT_PIXEL_3X3,     // 3 X 3
    DOT_PIXEL_4X4,     // 4 X 4
    DOT_PIXEL_5X5,     // 5 X 5
    DOT_PIXEL_6X6,     // 6 X 6
    DOT_PIXEL_7X7,     // 7 X 7
    DOT_PIXEL_8X8,     // 8 X 8
} ;
#define DOT_PIXEL_DFT DOT_PIXEL_1X1 //Default dot pilex

/**
 * Point size fill style
**/
enum DOT_STYLE
{
    DOT_FILL_AROUND = 1, // dot pixel 1 x 1
    DOT_FILL_RIGHTUP,    // dot pixel 2 X 2
};
#define DOT_STYLE_DFT DOT_FILL_AROUND //Default dot pilex

/**
 * Line style, solid or dashed
**/
enum LINE_STYLE
{
    LINE_STYLE_SOLID = 0,
    LINE_STYLE_DOTTED,
};

/**
 * Whether the graphic is filled
**/
enum DRAW_FILL
{
    DRAW_FILL_EMPTY = 0,
    DRAW_FILL_FULL,
} ;

struct PAINT
{
    UBYTE *Image;
    UBYTE *RImage;
    UWORD Width;
    UWORD Height;
    UWORD WidthMemory;
    UWORD HeightMemory;
    UWORD Color;
    UWORD Rotate;
    UWORD Mirror;
    UWORD WidthByte;
    UWORD HeightByte;
};