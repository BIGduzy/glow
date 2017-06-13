#ifndef GLOW_WS2812B_HPP
#define GLOW_WS2812B_HPP

#include <hwlib.hpp>
#include "Color.hpp"


namespace glow {
typedef unsigned char BYTE;

class WS2812B {
private:
    int numLEDs;
    Color colors[];

    public:
    /**
     * @brief Constructor
     * @param numLEDs the number of leds in the strip
     *
     * @details
     * Sets number of LEDs and fills colors array with 0,0,0 RGB values
     */
    WS2812B(int numLEDs = 1);
    // Getters/setters
    void setPixelColor(int index, BYTE red, BYTE green, BYTE blue);
    glow::Color getPixelColor(int index) const;

    void clear();
    void update();
};

}

#endif // GLOW_WS2812B_HPP
