// ==========================================================================
//
// File      : main.cpp
// Part of   : C++ Glow, Lightweight WS2812B LED strip library
// Author    : Nick Bout
// Github    : https://github.com/BIGduzy/glow
// Copyright : nick.bout@student.hu.nl 2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

#include <hwlib.hpp>
#include <glow.hpp>

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

    unsigned i = 0;
    unsigned min = 0;
    bool dir = 1;
    while(true) {
        // When i is max or minimum invert the direction
        if ((dir && i == ledStrip.getNumLeds() - 1) || (!dir && i == min)) {
            dir = !dir;
        }

        // Increase of decrease i based on direction
        if (dir) ++i;
        else  --i;

        // Set the color of the led at position i (we count from 0)
        ledStrip.setPixelColor(i, glow::Color(255, 0, 255));
        // Set the color of the led at position oppesite of i
        ledStrip.setPixelColor(ledStrip.getNumLeds() - i, glow::Color(255, 255, 0));

        // Render the LED strip
        ledStrip.update();
        // Turn every pixel off
        ledStrip.clear();

        // Wait 0.1 sec
        hwlib::wait_ms(100);
    }

}