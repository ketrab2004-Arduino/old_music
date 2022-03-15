// requires "Adafruit ILI9341 TFT" library  (FastLED_SPITFT_GFX)

#define DEBUG 1
#include <Extra_Debug.h>

#define buzzer 41

#define USE_IMAGERY 0
#if USE_IMAGERY // attempt to add images
#include <SPI.h>
#include <Wire.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>

// #include <SdFat.h>
// #include <Adafruit_SPIFlash.h>
// #include <Adafruit_ImageReader.h>

#include <Draw_BMP.h>

#define TFT_CS 10
#define TFT_DC 9
#define TFT_BL 5
#define SD_CS  4

#define BRIGHTNESS  127

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
#endif

#include "songs.h"
#include "player.h"

void setup()
{
    pinMode(buzzer, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);

    debugBegin(115200);

    #if USE_IMAGERY
    SD.begin(SD_CS);

    pinMode(TFT_BL, OUTPUT);
    analogWrite(TFT_BL, BRIGHTNESS);

    tft.begin();
    tft.setRotation(1);

    String toLoad = "/TRAINS/LOC-E.BMP";
    // String toLoad = "/MUSIC/NEVERG~1/0.BMP";

    File f = SD.open(toLoad);

    debug("name: ");
    debugln(f.name());
    debug("size: ");
    debugln(f.size());
    debugln(f.isDirectory() ? "is directory" : "isn't a directory");
    debugln(SD.exists(toLoad) ? "exists" : "doesnt exist");
    f.close();

    debugln("==========");
    for (uint16_t j = 0; j <= 240-48; j+=48) {
        for (uint16_t i = 0; i <= 320-48; i+=48) {
            bmpDraw(tft, toLoad, i,j);
        }
    }
    #endif
    
    playMusic(
        songNotes,
        buzzer
        #if USE_IMAGERY
        ,tft
        #endif
    );
    noTone(buzzer); // stop music just incase
}

// blink builtin led when finished
bool on = false;
unsigned long lastMilli = 0;
void loop()
{
    unsigned long currentMilli = millis();

    if (currentMilli - lastMilli > 35) {
        digitalWrite(LED_BUILTIN, on ? HIGH : LOW);
        on = !on;

        lastMilli = currentMilli;
    }
}
