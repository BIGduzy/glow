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

    while(true) {

        glow::Color color = cs.getColor();
        hwlib::cout << "(" << (unsigned)color.getRed() << ", ";
        hwlib::cout << (unsigned)color.getGreen() << ", ";
        hwlib::cout << (unsigned)color.getBlue() << ")";
        hwlib::cout << hwlib::endl;

        if (color.getRed() < 40 && color.getGreen() < 40 && color.getBlue() < 40) {
            // TODO: set random color
            color.setRed(0);
            color.setGreen(0);
            color.setBlue(0);
        } else if (color.getRed() > color.getGreen() && color.getRed() > color.getBlue()) {
            color.setRed(255);
            color.setGreen(0);
            color.setBlue(0);
        } else if (color.getBlue() > color.getRed() && color.getBlue() > color.getGreen()) {
            color.setBlue(255);
            color.setRed(0);
            color.setGreen(0);
        } else {
            color.setGreen(255);
            color.setRed(0);
            color.setBlue(0);
        }
//        hwlib::cout << "(" << (unsigned)c.getRed() << ", ";
//        hwlib::cout << (unsigned)c.getGreen() << ", ";
//        hwlib::cout << (unsigned)c.getBlue() << ")";
//        hwlib::cout << hwlib::endl;

//        stripClub.setPixelColor(1, c);
//        stripClub.setPixelColor(3, c);
//        stripClub.setPixelColor(5, c);
//        stripClub.setPixelColor(7, c);
//        stripClub.setPixelColor(9, c);

        for (unsigned i = 0; i < stripClub.getNumLeds()-3; i++) {
            stripClub.setPixelColor(i, glow::Color(0, 0, 0));
            stripClub.setPixelColor(i + 3, color);
            stripClub.setPixelColor(i + 2, color);
            stripClub.setPixelColor(i + 1, color);
            stripClub.update();
            hwlib::wait_ms(100);
        }
        for (unsigned i = stripClub.getNumLeds(); i >= 3; i--) {
            if (i > 0) {
                stripClub.setPixelColor(i, glow::Color(0, 0, 0));
            }
            stripClub.setPixelColor(i - 3, color);
            stripClub.setPixelColor(i - 2, color);
            stripClub.setPixelColor(i - 1, color);
            stripClub.update();
            hwlib::wait_ms(100);
        }

//        pinkyFinger.update();
////        stripClub.update();
//        hwlib::wait_ms(100);
//        ringFinger.update();
//        hwlib::wait_ms(100);
//        middleFinger.update();
//        hwlib::wait_ms(100);
//        indexFinger.update();
//        hwlib::wait_ms(100);
    }

}