#include <hwlib.hpp>
#include <glow.hpp>
#include <math.h>

int main( void ) {
    // Wait a bit for the console to start up
    // If we remove this the hwlib gets to slow for the led strip,
    // because the first wait sets the arduino to 12MHz
    hwlib::wait_ms(1000);
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;

    // Create the datapin
    auto dataPin = hwlib::target::pin_out(hwlib::target::pins::d30);

    // Make ledstrip object with 11 leds
    const int numLeds = 11;
    glow::WS2812B_strip_n<numLeds> ledStrip(dataPin);

    while(true) {
        for (uint8_t j = 0; j < numLeds; j++) {
            ledStrip.setPixelColor(j, glow::Color(0, 255, 255));
            ledStrip.update();
            hwlib::wait_ms(40);
        }

        for (uint8_t i = 0; i < numLeds; i++) {
            uint8_t red = sin(6 * i + 0) * 127 + 128;
            uint8_t green = sin(6 * i + 2) * 127 + 128;
            uint8_t blue = sin(6 * i + 4) * 127 + 128;
            ledStrip.setPixelColor(i, glow::Color(red, green, blue));
            ledStrip.update();
            hwlib::wait_ms(40);
        }

        for (uint8_t j = 0; j < numLeds; j++) {
            ledStrip.setPixelColor(j, glow::Color(0, 0, 255));
            ledStrip.update();
            hwlib::wait_ms(40);
        }
    }
}