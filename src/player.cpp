#include <Arduino.h>
#include <Noting.h>
#include <stdint.h>

#define USE_IMAGERY 1

#if USE_IMAGERY
#include <SPI.h>
#include <Wire.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Draw_BMP.h>

#define WIDTH 320
#define HEIGHT 240
#endif

#define DEBUG 1
#include <Extra_Debug.h>

void addToString(String &string, uint16_t pos, char c) {
    if (pos == string.length()) { // if string is 1 char too short for c
        string += c;

    } else if(pos > string.length()) { // if string is multiple chars too short
        for (int i = 0; i < pos - string.length() -1; i++) {
            string += " ";
        }
        string += c;

    } else { // if string isn't too short
        string.setCharAt(pos, c);
    }
}
void fillString(String &string, char c = ' ') {
    for (int i = 0; i < string.length(); i++) {
        string.setCharAt(i, c);
    }
}

void playMusic(
    String notes,
    uint8_t pinId
    #if USE_IMAGERY
    , Adafruit_ILI9341 tft
    #endif
    ) {
    uint8_t bpm = 120;
    uint8_t octave = 4;

    // keep track if currently inside of an [] block
    bool settingsMode = false;
    bool passedEqual = false;
    uint8_t depth = 0;
    String currentSetting = "";
    String currentParam = "";
    
    for (int i = 0; i < notes.length(); i+=2) {
        char note = notes.charAt(i);

        // ignore these characters
        if (note==' ' || note=='\n' || note=='\0') {
            i--; // avoid double step
            continue; // continue to next i
        }

        // check for settings block
        if (note=='[') {
            settingsMode = true;
            i--; // avoid double step
            continue;
        }

        if (settingsMode) {

            if (note==']' || note==';' || note==',') { // if end of setting

                // switch cases can only be used on integral or enum types
                if (currentSetting.startsWith("octave") || currentSetting.startsWith("oct")) {
                    currentParam.trim();
                    octave = currentParam.toInt();
                } else if (currentSetting.startsWith("bpm")) {
                    currentParam.trim();
                    bpm = currentParam.toInt();
                } 
                #if USE_IMAGERY
                else if (currentSetting.startsWith("img")) {
                    debug("Loading image: ");
                    debugln(currentParam);

                    bmpDraw(tft, currentParam, 0,0);

                    // File file = SD.open(currentParam);
                    // uint8_t *buffer = (uint8_t*)malloc(sizeof(uint8_t) * 20000);
                    // file.readBytes(buffer, 20000);
                    // tft.drawBitmap(0,0, buffer, WIDTH, HEIGHT, ILI9341_BLUE);
                }
                #endif

                if (note==']') { // if end of setting block
                    settingsMode = false; // end of setting
                } else { // else if end of setting wasn't end of setting block
                    
                }
                // reset after applying setting
                depth = 0;
                fillString(currentParam);
                fillString(currentSetting);
                passedEqual = false;
            } else {
                if (note=='=') {
                    i--;
                    passedEqual = true;
                    depth = 0;
                    continue;
                }
                if (passedEqual) {
                    addToString(currentParam, depth, note);
                    depth++;
                } else {
                    addToString(currentSetting, depth, note);
                    depth++;
                }
            }
            i--; // avoid double step
        } else {
            debug(note);
            debug(" ");
            debugln(i);

            int length = getNoteLength(1, 4, bpm); // default note length

            if (isDigit(notes.charAt(i+1))) {
                int j = 0;
                while (true) {
                    char c = notes.charAt(i+1 +j);

                    if (c == '/') {
                        if (j < currentSetting.length()-1) {
                            currentSetting.trim(); // trim trailing whitespace
                        }
                        j++;
                        break;
                    }

                    addToString(currentSetting, j, c);
                    j++;
                }

                int k = 0;
                while (true) {
                    char c = notes.charAt(i+1+j +k);

                    if (!isDigit(c)) {
                        if (k < currentParam.length()-1) {
                            currentParam.trim(); // trim trailing whitespace
                        }

                        break;
                    }

                    addToString(currentParam, k, c);
                    k++;
                }

                debug(currentSetting);
                debug(" / ");
                debug(currentParam);
                debug(" = ");
                debugln(getNoteLength(currentSetting.toInt(), currentParam.toInt(), bpm));
                
                length = getNoteLength(currentSetting.toInt(), currentParam.toInt(), bpm);

                i+= currentSetting.length() + currentParam.length();

                // clean used variables
                fillString(currentSetting);
                fillString(currentParam);
            } else { // no number after note
                i--;
            }

            noTone(pinId);
            delay(50);
            if (note != '_') {
                tone(pinId, getNoteFrequency(note, octave), length);
            } else {
                noTone(pinId); // play no tone if silence (_) character is used
            }

            // turn builtin led on when silence is played
            digitalWrite(LED_BUILTIN, note == '_' ? HIGH : LOW);

            delay(length);
        }
    }
}
