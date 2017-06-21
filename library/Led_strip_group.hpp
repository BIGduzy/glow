// ==========================================================================
//
// File      : Led_strip_group.hpp
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

#ifndef LED_STRIP_GROUP_HPP
#define LED_STRIP_GROUP_HPP

#include "Led_strip.hpp"
#include "Led_strip_dummy.hpp"

namespace glow {

/**
 * @class Led_strip_group
 * @author Nick
 * @date 21/06/2017
 * @file Led_strip_group.hpp
 * @brief Decorator for Led_strip used for a Group off led strips.
 * Based on hwlib's port
 */
class Led_strip_group : public Led_strip {
private:
    /* number of strips */
    uint8_t numStrips;
    /* Array with pointers to Led_strip objects */
    Led_strip* strips[4];
public:
    /**
     * @brief Constructor
     * @param s0 Led_strip object
     * @param s1 Led_strip object
     * @param s2 Led_strip object
     * @param s3 Led_strip object
     * @TODO Make this work with dummy strips so it's not necessary  to group exactly four strips
     *
     * @details This class is a group of Led_strips objects, it takes a maximum of 5 led strips.
     * If less than 5 Led_strips are given it fills with dummyObjects.
     * All Children of Led_strip can be used.
     */
//    Led_strip_group(
//        Led_strip& s0 = led_strip_dummy,
//        Led_strip& s1 = led_strip_dummy,
//        Led_strip& s2 = led_strip_dummy,
//        Led_strip& s3 = led_strip_dummy
//    ):
//        strips{&s0, &s1, &s2, &s3}
//    {
//        // Set number of strips by checking where the first dummy starts
//        for (numStrips = 0; numStrips < 5; ++numStrips) {
//            if (strips[numStrips] == &led_strip_dummy) {
//                break;
//            }
//        }
//    };

    /**
     * @brief Constructor
     * @param s0 Led_strip object
     * @param s1 Led_strip object
     * @param s2 Led_strip object
     * @param s3 Led_strip object
     * @TODO Make this work with dummy strips so it's not necessary  to group exactly four strips
     *
     * @details This class is a group of Led_strips objects.
     * All Children of Led_strip can be used.
     */
    Led_strip_group(
        Led_strip& s0,
        Led_strip& s1,
        Led_strip& s2,
        Led_strip& s3
    ):
        strips{&s0, &s1, &s2, &s3}
    {
        // Set number of strips by checking where the first dummy starts
        numStrips = 4;
    };

    // *******************
    //   Getters/setters
    // *******************
    uint8_t getNumStrips() {
        return numStrips;
    };
    // *******************
    // END Getters/setters
    // *******************

    // *******************
    //      Overrides
    // *******************
    /**
     * @brief Sets the color for a single LED pixel index for every strip
     * @param index The index number of the LED pixel (we count from 0)
     * @param color glow::Color Object with RGB values
     */
    inline void setPixelColor(unsigned index, const Color& color) override {
        for (uint8_t i = 0; i < numStrips; ++i) {
            strips[i]->setPixelColor(index, color);
        }
    };

    /**
     * @brief Set the color of every pixel in every strip
     * @param color glow::Color Object with RGB values
     */
    inline void setPixelColorAll(const Color& color) {
        for (uint8_t i = 0; i < numStrips; ++i) {
            Led_strip& cur = *(strips[i]);
            for (unsigned j = 0; j < cur.getNumLeds(); ++j) {
                cur.setPixelColor(j, color);
            }
        }
    }

    /**
     * @brief Returns the color for a single LED pixel
     * @param index The index number of the LED pixel (we count from 0)
     * @return Object with the RGB values
     */
    inline Color getPixelColor(unsigned index) const override {
        return strips[0]->getPixelColor(index);
    };

    /**
     * @brief Returns the number of leds of the leds strip with the least LEDs (if you want the number of leds in 1 strip just give the index)
     * @return Number of LEDs
     */
    inline unsigned getNumLeds() const override {
        unsigned max = 0;
        for (uint8_t i = 0; i < numStrips; ++i) {
            if (strips[i]->getNumLeds() > max) max = strips[i]->getNumLeds();
        }
        return max;
    };

    /**
     * @brief Returns the number of 1 LED strip
     * @param The index of the strip (couting from 0)
     * @return Number of LEDs
     */
    inline unsigned getNumLeds(uint8_t index) const {
        return strips[index]->getNumLeds();
    };


    /**
     * @brief Uses the clear function for every LED strip
     */
    void clear() override {
        for (uint8_t i = 0; i < numStrips; ++i) {
            strips[i]->clear();
        }
    };

    /**
     * @brief Uses the brighten function for every LED strip
     */
    void brighten(const uint8_t& strength = 1) override {
        for (uint8_t i = 0; i < numStrips; ++i) {
            strips[i]->brighten(strength);
        }
    };

    /**
     * @brief Uses the darken function for every LED strip
     */
    void darken(const uint8_t& strength = 1) override {
        for (uint8_t i = 0; i < numStrips; ++i) {
            strips[i]->darken(strength);
        }
    };

    /**
     * @brief Uses the invert function for every LED strip
     */
    void invert() override {
        for (uint8_t i = 0; i < numStrips; ++i) {
            strips[i]->invert();
        }
    };

    /**
     * @brief "Renders" the LED strips
     *
     * @details
     * Uses the update function for every LED strip
     */
    void update() override {
        for (uint8_t i = 0; i < numStrips; ++i) {
            strips[i]->update();
        }
    };
};

}

#endif // LED_STRIP_GROUP_HPP
