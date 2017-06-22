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
#include "TCS230.hpp"

int map(int value, int inMin, int inMax, int outMin, int outMax) {
    return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

int main( void ) {
    // Wait a bit for the console to start up
    // If we remove this the hwlib gets to slow for the led strip,
    // because the first wait sets the arduino to 12MHz
    hwlib::wait_ms(1000);
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;

    // Create the LED strips
    auto dataPin = hwlib::target::pin_out(hwlib::target::pins::d30);
    auto dataPin1 = hwlib::target::pin_out(hwlib::target::pins::d31);
    auto dataPin2 = hwlib::target::pin_out(hwlib::target::pins::d32);
    auto dataPin3 = hwlib::target::pin_out(hwlib::target::pins::d33);

    glow::WS2812B_strip_n<9> pinkyFinger(dataPin);
    glow::WS2812B_strip_n<11> ringFinger(dataPin2);
    glow::WS2812B_strip_n<11> middleFinger(dataPin3);
    glow::WS2812B_strip_n<11> indexFinger(dataPin1);

    glow::Led_strip_group stripClub(pinkyFinger, ringFinger, indexFinger, middleFinger);

    // Create color sensor
    hwlib::target::pin_out s0(hwlib::target::pins::d4);
    hwlib::target::pin_out s1(hwlib::target::pins::d5);
    hwlib::target::pin_in colorDataPin(hwlib::target::pins::d6);
    hwlib::target::pin_out s2(hwlib::target::pins::d7);
    hwlib::target::pin_out s3(hwlib::target::pins::d8);
    TCS230 cs(colorDataPin, s0, s1, s2, s3);
    cs.setMode(SCALING_MODES::MEDIUM);

    unsigned i = 0;
    unsigned min = 0;
    bool dir = 1;
    glow::Color oldColor;
    while(true) {
        // GET the color
        glow::Color color = cs.getColor();
        hwlib::cout << "(" << (unsigned)color.getRed() << ", ";
        hwlib::cout << (unsigned)color.getGreen() << ", ";
        hwlib::cout << (unsigned)color.getBlue() << ")";
        hwlib::cout << hwlib::endl;

        // Modify color so we can see it better in the glove
        if (color.getRed() < 40 && color.getGreen() < 40 && color.getBlue() < 40) {
            // TODO: set random color
            color = oldColor;
        } else if (color.getRed() > color.getGreen() && color.getRed() > color.getBlue()) {
            color.darken(50); // Set other colors darker to make red stand out more
            color.setRed(255);
        } else if (color.getBlue() > color.getRed() && color.getBlue() > color.getGreen()) {
            color.darken(50); // Set other colors darker to make blue stand out more
            color.setBlue(255);
        } else {
            color.darken(50); // Set other colors darker to make green stand out more
            color.setGreen(255);
        }
        oldColor = color;


        // When i is max or minimum invert the direction
        if ((dir && i == stripClub.getNumLeds() - 4) || (!dir && i == min)) {
            dir = !dir;
        }

        // Increase of decrease i based on direction
        if (dir) ++i;
        else  --i;

        // Create kitt patern
        stripClub.setPixelColor(i, color);
        stripClub.setPixelColor(i + 1, color);
        stripClub.setPixelColor(i + 2, color);

        // Render the LED strips
        stripClub.update();
        // Turn every pixel off
        stripClub.clear();

        // Wait 0.1 sec
        hwlib::wait_ms(100);
    }
}