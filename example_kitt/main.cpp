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
    }
}