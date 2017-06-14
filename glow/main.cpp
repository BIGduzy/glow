#include <hwlib.hpp>
#include "glow.hpp"

int main( void ){
    // Wait a bit for the console to start up
    // If we remove this the hwlib gets to slow for the led strip,
    // because the first wait sets the arduino to 12MHz
    hwlib::wait_ms(1000);
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;


    auto dataPin = hwlib::target::pin_out(hwlib::target::pins::d6);

    const int numLeds = 60;
    glow::WS2812B_strip_n<numLeds> ledStrip(dataPin);

    int i = 0;
    int max = numLeds;
    int min = 0;
    bool dir = 1;
    while(true) {
        // Simple code for led pattern
        if ((dir && i == max - 1) || (!dir && i == min)) {
            dir = !dir;

//            for (int j = 0; j < numLeds; ++j) {
//                glow::Color tmp = ledStrip.getPixelColor(j);
//                if ((dir + j) & 1) tmp.invert(); // invert all even
//
//                ledStrip.setPixelColor(j, tmp);
//            }
        }

        if (dir) {
            ++i;
//            ledStrip.brighten(4);
        }
        else {
            --i;
//            ledStrip.darken(4);
        }

        ledStrip.setPixelColor(i, 40, 0, 0);
        ledStrip.setPixelColor(max - i, 0, 40, 40);

        ledStrip.update();
        ledStrip.clear();
        hwlib::wait_ms(100); // Wait a bit
    }

}
