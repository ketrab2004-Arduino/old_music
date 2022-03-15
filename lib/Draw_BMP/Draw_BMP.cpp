#include <stdint.h>
#include "Draw_BMP.h"

#if USE_IMAGERY
// This function opens a Windows Bitmap (BMP) file and
// displays it at the given coordinates.  It's sped up
// by reading many pixels worth of data at a time
// (rather than pixel by pixel).  Increasing the buffer
// size takes more of the Arduino's precious RAM but
// makes loading a little faster.  20 pixels seems a
// good balance.

#define BUFFPIXEL 20

void bmpDraw(Adafruit_ILI9341 &tft, String &filename, uint8_t x, uint16_t y) {

    Serial.println("Called bmpDraw()");

    File     bmpFile;
    int      bmpWidth, bmpHeight;   // W+H in pixels
    uint8_t  bmpDepth;              // Bit depth (currently must be 24)
    uint32_t bmpImageoffset;        // Start of image data in file
    uint32_t rowSize;               // Not always = bmpWidth; may have padding
    uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
    uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
    //boolean  goodBmp = false;       // Set to true on valid header parse
    boolean  flip    = true;        // BMP is stored bottom-to-top
    int      w, h, row, col;
    uint8_t  r, g, b;
    uint32_t pos = 0, startTime = millis();

    if((x >= tft.width()) || (y >= tft.height())) return;

    bmpFile = SD.open(filename);

    Serial.println(bmpFile ? "bmp!" : "no bmp :(");

    // return of file doesn't exist
    if (!bmpFile) {
        return;
    }

    Serial.println("didn't return");

    // Parse BMP header
    if(read16(bmpFile) == 0x4D42) { // BMP signature
        Serial.println("found bmp file signature");
        (void)read32(bmpFile); // Read & ignore filesize
        (void)read32(bmpFile); // Read & ignore creator bytes
        bmpImageoffset = read32(bmpFile); // Start of image data
        Serial.print("bmpImageOffset = ");
        Serial.println(bmpImageoffset);
        // Read DIB header
        (void)read32(bmpFile); //ignore header
        bmpWidth  = read32(bmpFile);
        bmpHeight = read32(bmpFile);
        if(read16(bmpFile) == 1) { // # planes -- must be '1'
            Serial.println("planes is 1");
            bmpDepth = read16(bmpFile); // bits per pixel
            if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed
                Serial.println("bmp depth is 24 and uncompressed, supported!");
                //goodBmp = true; // Supported BMP format -- proceed!        
                // BMP rows are padded (if needed) to 4-byte boundary
                rowSize = (bmpWidth * 3 + 3) & ~3;
                // If bmpHeight is negative, image is in top-down order.
                // This is not canon but has been observed in the wild.
                if(bmpHeight < 0) {
                    bmpHeight = -bmpHeight;
                    flip      = false;
                }
                // Crop area to be loaded
                w = bmpWidth;
                h = bmpHeight;
                Serial.print("Width: ");
                Serial.print(w);
                Serial.print(", Height: ");
                Serial.println(h);
                if((x+w-1) >= tft.width())  w = tft.width()  - x;
                if((y+h-1) >= tft.height()) h = tft.height() - y;
                Serial.print("Clipped Width: ");
                Serial.print(w);
                Serial.print(", Height: ");
                Serial.println(h);

                tft.startWrite();
                // Set TFT address window to clipped image bounds
                tft.setAddrWindow(x, y, w, h);
                for (row=0; row<h; row++) { // For each scanline...
                    // Seek to start of scan line.  It might seem labor-
                    // intensive to be doing this on every line, but this
                    // method covers a lot of gritty details like cropping
                    // and scanline padding.  Also, the seek only takes
                    // place if the file position actually needs to change
                    // (avoids a lot of cluster math in SD library).
                    if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
                        pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
                    else     // Bitmap is stored top-to-bottom
                        pos = bmpImageoffset + row * rowSize;

                    if(bmpFile.position() != pos) { // Need seek?
                        bmpFile.seek(pos);
                        buffidx = sizeof(sdbuffer); // Force buffer reload
                    }
                    for (col=0; col<w; col++) { // For each pixel...
                        // Time to read more pixel data?
                        if (buffidx >= sizeof(sdbuffer)) { // Indeed
                            bmpFile.read(sdbuffer, sizeof(sdbuffer));
                            buffidx = 0; // Set index to beginning
                        }
                        // Convert pixel from BMP to TFT format, push to display
                        b = sdbuffer[buffidx++];
                        g = sdbuffer[buffidx++];
                        r = sdbuffer[buffidx++];
                        tft.pushColor(tft.color565(r,g,b));
                    } // end pixel
                } // end scanline
                tft.endWrite();
            } // end goodBmp
        }
    }
    bmpFile.close();
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File &f) {
    uint16_t result;
    ((uint8_t *)&result)[0] = f.read(); // LSB
    ((uint8_t *)&result)[1] = f.read(); // MSB
    return result;
}

uint32_t read32(File &f) {
    uint32_t result;
    ((uint8_t *)&result)[0] = f.read(); // LSB
    ((uint8_t *)&result)[1] = f.read();
    ((uint8_t *)&result)[2] = f.read();
    ((uint8_t *)&result)[3] = f.read(); // MSB
    return result;
}

#endif