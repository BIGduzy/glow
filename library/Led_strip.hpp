// ==========================================================================
//
// File      : Led_strip.hpp
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

#ifndef LED_STRIP_HPP
#define LED_STRIP_HPP

namespace glow {

/**
 * @class Led_strip
 * @author Nick
 * @date 21/06/2017
 * @file Led_strip.hpp
 * @brief Abstract super class for all Led strip objects
 */
class Led_strip {
public:
    Led_strip(){};

    virtual void setPixelColor(unsigned index, const Color& color) = 0;
    virtual Color getPixelColor(unsigned index) const = 0;
    virtual unsigned getNumLeds() const = 0;
    virtual void clear() = 0;
    virtual void brighten(const uint8_t& strength = 1) = 0;
    virtual void darken(const uint8_t& strength = 1) = 0;
    virtual void invert() = 0;
    virtual void update() = 0;
};

}

#endif // LED_STRIP_HPP
