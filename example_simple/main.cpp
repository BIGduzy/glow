#include <hwlib.hpp>
#include "../library/WS2812B_strip_n.hpp"
#include <math.h>

int main( void ) {
    // Wait a bit for the console to start up
    // If we remove this the hwlib gets to slow for the led strip,
    // because the first wait sets the arduino to 12MHz
    hwlib::wait_ms(1000);
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;


    auto dataPin = hwlib::target::pin_out(hwlib::target::pins::d30);

    const int numLeds = 11;
    glow::WS2812B_strip_n<numLeds> ledStrip(dataPin);

    int i = 0;
    int max = numLeds;
    int min = 0;
    bool dir = 1;
    ledStrip.clear();
    while(true) {
        // Simple code for led pattern
        if ((dir && i == max - 1) || (!dir && i == min)) {
            dir = !dir;
        }

        if (dir) ++i;
        else  --i;

//        ledStrip.setPixelColor(i, glow::Color(255, 0, 255));
//        ledStrip.setPixelColor(max - i, glow::Color(255, 255, 0));
        for (int i = 0; i < numLeds-3; i++) {
            ledStrip.setPixelColor(i, glow::Color(0, 0, 0));
            ledStrip.setPixelColor(i + 3, glow::Color(255, 0, 0));
            ledStrip.setPixelColor(i + 2, glow::Color(255, 0, 0));
            ledStrip.setPixelColor(i + 1, glow::Color(255, 0, 0));
            ledStrip.update();
            hwlib::wait_ms(100);
        }
        for (int i = numLeds; i >= 3; i--) {
            if (i > 0) {
                ledStrip.setPixelColor(i, glow::Color(0, 0, 0));
            }
            ledStrip.setPixelColor(i - 3, glow::Color(255, 0, 0));
            ledStrip.setPixelColor(i - 2, glow::Color(255, 0, 0));
            ledStrip.setPixelColor(i - 1, glow::Color(255, 0, 0));
            ledStrip.update();
            hwlib::wait_ms(100);
        }
//        for (uint8_t j = 0; j < numLeds; j++) {
//            ledStrip.setPixelColor(j, glow::Color(255, 0, 0));
//            ledStrip.update();
//            hwlib::wait_ms(40);
//        }
//
//        for (int i = 0; i < (int) numLeds; i++) {
//            uint8_t red = sin(1 * i + 0) * 127 + 128;
//            uint8_t green = sin(3 * i + 2) * 127 + 128;
//            uint8_t blue = sin(4 * i + 4) * 127 + 128;
//            ledStrip.setPixelColor(i, glow::Color(red, green, blue));
//            ledStrip.update();
//            hwlib::wait_ms(40);
//        }
//
//        for (uint8_t j = 0; j < numLeds; j++) {
//            ledStrip.setPixelColor(j, glow::Color(255, 0, 0));
//            ledStrip.update();
//            hwlib::wait_ms(40);
//        }

    }

}