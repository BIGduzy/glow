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

bool TEST(hwlib::string<255> description, bool test) {
    hwlib::cout << description << ": " << hwlib::endl;
    hwlib::cout << ((test) ? "Successed" : "Failed") << hwlib::endl << hwlib::endl;
    return test;
}

int main(void) {
    hwlib::wait_ms(1000);
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;

    unsigned numTests = 40;
    unsigned testResult = 0;


    // **********************************
    //     Color: Default constructor
    // **********************************
    glow::Color c1;
    testResult += TEST("Color: Default constructor (RED)", c1.getRed() == 0);
    testResult += TEST("Color: Default constructor (GREEN)", c1.getGreen() == 0);
    testResult += TEST("Color: Default constructor (BLUE)", c1.getBlue() == 0);


    // **********************************
    // Color: Constructor with parameters
    // **********************************
    glow::Color c(24, 155, 255);
    testResult += TEST("Color: Constructor with parameters (RED)", c.getRed() == 24);
    testResult += TEST("Color: Constructor with parameters (GREEN)", c.getGreen() == 155);
    testResult += TEST("Color: Constructor with parameters (BLUE)", c.getBlue() == 255);


    // **********************************
    //         Color: Brighten
    // **********************************
    c.brighten(25);
    testResult += TEST("Color: Brighten (RED)", c.getRed() == 49);
    testResult += TEST("Color: Brighten (GREEN)", c.getGreen() == 180);
    testResult += TEST("Color: Brighten (BLUE)", c.getBlue() == 255);


    // **********************************
    //         Color: Darken
    // **********************************
    c.darken(50);
    testResult += TEST("Color: Darken (RED)", c.getRed() == 0);
    testResult += TEST("Color: Darken (GREEN)", c.getGreen() == 130);
    testResult += TEST("Color: Darken (BLUE)", c.getBlue() == 205);


    // **********************************
    //         Color: Clear
    // **********************************
    c.clear();
    testResult += TEST("Color: Clear (RED)", c.getRed() == 0);
    testResult += TEST("Color: Clear (GREEN)", c.getGreen() == 0);
    testResult += TEST("Color: Clear (BLUE)", c.getBlue() == 0);


    // **********************************
    //         Color: Setters
    // **********************************
    c.setRed(24);
    c.setGreen(155);
    c.setBlue(255);
    testResult += TEST("Color: Setters (RED)", c.getRed() == 24);
    testResult += TEST("Color: Setters (GREEN)", c.getGreen() == 155);
    testResult += TEST("Color: Setters (BLUE)", c.getBlue() == 255);


    // **********************************
    //         Color: Invert
    // **********************************
    c.invert();
    testResult += TEST("Color: Invert (RED)", c.getRed() == 231); // ~24
    testResult += TEST("Color: Invert (GREEN)", c.getGreen() == 100); // ~155
    testResult += TEST("Color: Invert (BLUE)", c.getBlue() == 0); // ~255




    // ********************************************
    //      WS2812B_strip_n: Default constructor
    // ********************************************
    glow::WS2812B_strip_n<5> strip1;
    testResult += TEST("WS2812B_strip_n: Default constructor (NUMBER OF LEDS)", strip1.getNumLeds() == 5);
    testResult += TEST("WS2812B_strip_n: Default constructor (CORRECT COLOR 0)", strip1.getPixelColor(0) == glow::Color());
    testResult += TEST("WS2812B_strip_n: Default constructor (CORRECT COLOR 1)", strip1.getPixelColor(1) == glow::Color());
    testResult += TEST("WS2812B_strip_n: Default constructor (CORRECT COLOR 1)", strip1.getPixelColor(2) == glow::Color());
    testResult += TEST("WS2812B_strip_n: Default constructor (CORRECT COLOR 1)", strip1.getPixelColor(3) == glow::Color());
    testResult += TEST("WS2812B_strip_n: Default constructor (CORRECT COLOR 1)", strip1.getPixelColor(4) == glow::Color());


    // ********************************************
    // WS2812B_strip_n: Constructor with parameters
    // ********************************************
    hwlib::target::pin_out dataPin = hwlib::target::pin_out(hwlib::target::pins::d7);
    glow::WS2812B_strip_n<2> strip(dataPin);
    testResult += TEST("WS2812B_strip_n: Constructor with parameters (NUMBER OF LEDS)", strip.getNumLeds() == 2);
    testResult += TEST("WS2812B_strip_n: Constructor with parameters (CORRECT COLOR 0)", strip.getPixelColor(0) == glow::Color());
    testResult += TEST("WS2812B_strip_n: Constructor with parameters (CORRECT COLOR 1)", strip.getPixelColor(1) == glow::Color());


    // ********************************************
    //      WS2812B_strip_n: setPixelColor
    // ********************************************
    strip.setPixelColor(0, glow::Color(2, 5, 6));
    strip.setPixelColor(1, glow::Color(200, 125, 206));
    testResult += TEST("WS2812B_strip_n: setPixelColor (CORRECT COLOR 0)", strip.getPixelColor(0) == glow::Color(2, 5, 6));
    testResult += TEST("WS2812B_strip_n: setPixelColor (CORRECT COLOR 1)", strip.getPixelColor(1) == glow::Color(200, 125, 206));


    // ********************************************
    //      WS2812B_strip_n: Clear
    // ********************************************
    strip.clear();
    testResult += TEST("WS2812B_strip_n: clear (CORRECT COLOR 0)", strip.getPixelColor(0) == glow::Color());
    testResult += TEST("WS2812B_strip_n: clear (CORRECT COLOR 1)", strip.getPixelColor(1) == glow::Color());


    // ********************************************
    //      WS2812B_strip_n: Invert
    // ********************************************
    strip.setPixelColor(0, glow::Color(24, 155, 255));
    strip.invert();
    testResult += TEST("WS2812B_strip_n: Invert (CORRECT COLOR 0)", strip.getPixelColor(0) == glow::Color(231, 100, 0));
    testResult += TEST("WS2812B_strip_n: Invert (CORRECT COLOR 1)", strip.getPixelColor(1) == glow::Color(255, 255, 255));


    // ********************************************
    //      WS2812B_strip_n: Darken
    // ********************************************
    strip.darken(200);
    testResult += TEST("WS2812B_strip_n: Darken (CORRECT COLOR 0)", strip.getPixelColor(0) == glow::Color(31, 0, 0));
    testResult += TEST("WS2812B_strip_n: Darken (CORRECT COLOR 1)", strip.getPixelColor(1) == glow::Color(55, 55, 55));


    // ********************************************
    //      WS2812B_strip_n: Brighten
    // ********************************************
    strip.brighten(220);
    testResult += TEST("WS2812B_strip_n: Brighten (CORRECT COLOR 0)", strip.getPixelColor(0) == glow::Color(251, 220, 220));
    testResult += TEST("WS2812B_strip_n: Brighten (CORRECT COLOR 1)", strip.getPixelColor(1) == glow::Color(255, 255, 255));


    hwlib::cout << "Test results: " << testResult << "/" << numTests << " suceeded" << hwlib::endl;
}