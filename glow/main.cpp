#include <hwlib.hpp>
#include <glow/WS2812B.hpp>

int main( void ){
    // Wait a bit for the console to start up
    // If we remove this the hwlib gets to slow for the led strip,
    // because the first wait sets the arduino to 12MHz
    hwlib::wait_ms(1000);
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;


    auto led = hwlib::target::pin_out(hwlib::target::pins::d6);

    glow::WS2812B ledStrip(1);

    ledStrip.update();

    while(true) {}

}
