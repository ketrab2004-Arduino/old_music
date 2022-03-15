#ifndef __DRAW_BMP__

#define USE_IMAGERY 0
#define __DRAW_BMP__

#if USE_IMAGERY
#include <SD.h>
#include <Adafruit_ILI9341.h>

void bmpDraw(Adafruit_ILI9341 &tft, String &filename, uint8_t x, uint16_t y);


// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File &f);

uint32_t read32(File &f);

#endif
#endif