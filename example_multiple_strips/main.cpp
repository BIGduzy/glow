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
    auto dataPin1 = hwlib::target::pin_out(hwlib::target::pins::d31);
    auto dataPin2 = hwlib::target::pin_out(hwlib::target::pins::d32);
    auto dataPin3 = hwlib::target::pin_out(hwlib::target::pins::d33);

    // Make ledstrip object with 11 leds
    const int numLeds = 11;
    glow::WS2812B_strip_n<numLeds> pinkyFinger(dataPin);
    glow::WS2812B_strip_n<numLeds> ringFinger(dataPin1);
    glow::WS2812B_strip_n<numLeds> indexFinger(dataPin2);
    glow::WS2812B_strip_n<numLeds> middleFinger(dataPin3);

    glow::Led_strip_group stripClub(pinkyFinger, ringFinger, indexFinger, middleFinger);

    unsigned i = 0;
    unsigned min = 0;
    bool dir = 1;
    while(true) {
        // When i is max or minimum invert the direction
        if ((dir && i == stripClub.getNumLeds() - 1) || (!dir && i == min)) {
            dir = !dir;
        }

        // Increase of decrease i based on direction
        if (dir) ++i;
        else  --i;

        // Set the color of the led at position i (we count from 0)
        stripClub.setPixelColor(i, glow::Color(255, 0, 255));
        // Set the color of the led at position oppesite of i
        stripClub.setPixelColor(stripClub.getNumLeds() - i, glow::Color(0, 255, 255));

        // Render the LED strip
        stripClub.update();
        // Turn every pixel off
        stripClub.clear();

        // Wait 0.1 sec
        hwlib::wait_ms(100);
    }
}