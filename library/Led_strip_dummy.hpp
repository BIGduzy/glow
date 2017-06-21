// ==========================================================================
//
// File      : Led_strip_dummy.hpp
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

#ifndef LED_STRIP_DUMMY_HPP
#define LED_STRIP_DUMMY_HPP

#include "Led_strip.hpp"

namespace glow {

/**
 * @class Led_strip_dummy
 * @author Nick
 * @date 21/06/2017
 * @file Led_strip_dummy.hpp
 * @brief Dummy class, based on hwlib-pin-dummies.hpp
 */
class Led_strip_dummy : public Led_strip {
public:
    Led_strip_dummy() {};

    inline void setPixelColor(unsigned index, const Color& color) {};
    inline Color getPixelColor(unsigned index) const { return Color(); };
    inline unsigned getNumLeds() const { return 0; };
    void clear() {};
    void brighten(const uint8_t& strength = 1) {};
    void darken(const uint8_t& strength = 1) {};
    void invert() {};
    void update() {};
};

extern Led_strip_dummy led_strip_dummy;

}

#endif // LED_STRIP_DUMMY_HPP
