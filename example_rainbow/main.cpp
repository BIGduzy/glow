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

    // Create the datapins
    auto dataPin = hwlib::target::pin_out(hwlib::target::pins::d30);
    auto dataPin1 = hwlib::target::pin_out(hwlib::target::pins::d31);
    auto dataPin2 = hwlib::target::pin_out(hwlib::target::pins::d32);
    auto dataPin3 = hwlib::target::pin_out(hwlib::target::pins::d33);

    // Create led strips
    glow::WS2812B_strip_n<9> pinkyFinger(dataPin);
    glow::WS2812B_strip_n<11> ringFinger(dataPin2);
    glow::WS2812B_strip_n<11> middleFinger(dataPin3);
    glow::WS2812B_strip_n<11> indexFinger(dataPin1);

    // Create group of led strips
    glow::Led_strip_group stripClub(pinkyFinger, ringFinger, indexFinger, middleFinger);

    while(true) {
        for (uint8_t j = 0; j < stripClub.getNumLeds(); j++) {
            stripClub.setPixelColor(j, glow::Color(0, 255, 255));
            stripClub.update();
            hwlib::wait_ms(40);
        }

        for (uint8_t i = 0; i < stripClub.getNumLeds(); i++) {
            uint8_t red = sin(6 * i + 0) * 127 + 128;
            uint8_t green = sin(6 * i + 2) * 127 + 128;
            uint8_t blue = sin(6 * i + 4) * 127 + 128;
            stripClub.setPixelColor(i, glow::Color(red, green, blue));
            stripClub.update();
            hwlib::wait_ms(40);
        }

        for (uint8_t j = 0; j < stripClub.getNumLeds(); j++) {
            stripClub.setPixelColor(j, glow::Color(0, 0, 255));
            stripClub.update();
            hwlib::wait_ms(40);
        }
    }
}