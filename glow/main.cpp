#include <hwlib.hpp>
#include "WS2812B_strip_n.hpp"

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
        if ((dir && i == max - 1) || (!dir && i == min)) dir = !dir;
        if (dir) ++i;
        else --i;

        ledStrip.setPixelColor(i, 40, 0, i);
        ledStrip.setPixelColor(max - i, 40, i, 0);

        ledStrip.update();
        ledStrip.clear();
        hwlib::wait_ms(100); // Wait a sec
    }

}
