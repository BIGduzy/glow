#ifndef GLOW_WS2812B_HPP
#define GLOW_WS2812B_HPP

#include <hwlib.hpp>
#include "Color.hpp"
#include "config.hpp"


namespace glow {
typedef unsigned char BYTE;

// High / low for a 0
#define GLOW_WAIT_T0H() asm volatile("nop\n\t") //these are NOPS - these let us delay clock cycles for more precise timing
#define GLOW_WAIT_T0L() asm volatile("nop\n\t") //these are NOPS - these let us delay clock cycles for more precise timing

// High / low for a 1
#define GLOW_WAIT_T1H() __asm__ volatile("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t") //these are NOPS - these let us delay clock cycles for more precise timing
#define GLOW_WAIT_T1L()  //these are NOPS - these let us delay clock cycles for more precise timing

class WS2812B {
private:
    hwlib::target::pin_out dataPin;
    int numLEDs;
    Color colors[NUMBER_OF_LEDS];

    public:

    /**
     * @brief Default constructor
     *
     * @details
     * Sets data pin to d6 and the number of LEDs to 1, then fills colors array with 0,0,0 RGB values
     */
    WS2812B();
    /**
     * @brief Constructor
     * @param numLEDs the number of leds in the strip
     *
     * @details
     * Sets data pin and number of LEDs and fills colors array with 0,0,0 RGB values
     */
    WS2812B(hwlib::target::pin_out dataPin, int numLEDs);
    // Getters/setters
    void setPixelColor(int index, BYTE red, BYTE green, BYTE blue);
    void setPixelColor(int index, const Color& color);
    Color getPixelColor(int index) const;

    void clear();
    void update();
};

}

#endif // GLOW_WS2812B_HPP
