#ifndef GLOW_WS2812B_STRIP_N_HPP
#define GLOW_WS2812B_STRIP_N_HPP

#include <hwlib.hpp>
#include "Color.hpp"
#include "config.hpp"

// Only print in debug modus
#if DEBUG
    void printStream(int numBits, bool bits[]) {
        hwlib::cout << "--BEGIN STREAM--" << hwlib::endl << hwlib::endl;
        for (int i = 0; i < numBits; ++i) {
            hwlib::cout << bits[i] << " ";
            if ((i + 1) % 8 == 0) hwlib::cout << hwlib::endl;
            if ((i + 1) % (8 * 3) == 0) hwlib::cout << hwlib::endl;
        }
        hwlib::cout << "--END STREAM--" << hwlib::endl;
    }
    #define debugPrintStream(numBits, bits) printStream(numBits, bits)
#else
    #define debugPrintStream(numBits, bits)
#endif

namespace glow {

// High / low for a 0
#define GLOW_WAIT_T0H() asm volatile("nop\n\t") // These are NOPS - these let us delay clock cycles for more precise timing
#define GLOW_WAIT_T0L() asm volatile("nop\n\t")
// High / low for a 1
#define GLOW_WAIT_T1H() __asm__ volatile("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t")
#define GLOW_WAIT_T1L()  // No need to wait, since going back to the beginning of the loop takes long enough

/**
 * @class WS2812B_strip_n
 * @author Nick
 * @date 14/06/2017
 * @file WS2812B_strip_n.hpp
 * @brief Template class for the WS2812B LED strip
 */
template<int NUMBER_OF_LEDS>
class WS2812B_strip_n {
private:
    /* The data pin */
    // I know this is supposed to be and reference to the pin, but I forgot this when making the sendBits function.
    // So if you make it a reference now the timing is off and the led strip won't receive 0's, thus making every LED pixel white
    hwlib::target::pin_out dataPin;
    /* The number of LEDs */
    int numLEDs = NUMBER_OF_LEDS;
    /* Array with color objects containing the color data for every LED */
    Color colors[NUMBER_OF_LEDS];

public:
    /**
     * @brief Default constructor
     *
     * @details
     * Sets data pin to d6, the number of LEDs and then fills colors array with 0,0,0 RGB values
     */
    WS2812B_strip_n():
        dataPin(hwlib::target::pins::d6),
        numLEDs(NUMBER_OF_LEDS)
    {
        // Fill the color array with 0,0,0 rgb values
        clear();
    };

    /**
     * @brief Constructor
     * @param numLEDs the number of leds in the strip
     *
     * @details
     * Sets data pin, the number of LEDs and then fills colors array with 0,0,0 RGB values
     */
    WS2812B_strip_n(hwlib::target::pin_out dataPin):
        dataPin(dataPin),
        numLEDs(NUMBER_OF_LEDS)
    {
        // The number of leds can't be zero or less
        if (numLEDs < 1) numLEDs = 1;

        // Fill the color array with 0,0,0 rgb values
        clear();
    };

    // ***************
    // Getters/setters
    // ***************
    /**
     * @brief Sets The color for a single LED pixel
     * @param index The index number of the LED pixel (we count from 0)
     * @param red The red value, 0 - 255
     * @param green The green value, 0 - 255
     * @param blue The red value, 0 - 255
     */
    void setPixelColor(int index, uint8_t red, uint8_t green, uint8_t blue) {
        glow::Color& c = colors[index];
        c.setRed(red);
        c.setGreen(green);
        c.setBlue(blue);
    };

    /**
     * @brief Sets the color for a single LED pixel
     * @param index The index number of the LED pixel (we count from 0)
     * @param color glow::Color Object with RGB values
     */
    void setPixelColor(int index, const Color& color) {
        colors[index] = color;
    };

    /**
     * @brief Returns the color for a single LED pixel
     * @param index The index number of the LED pixel (we count from 0)
     * @return Object with the RGB values
     */
    Color getPixelColor(int index) const {
        return colors[index];
    };

    // *******************
    // END Getters/setters
    // *******************

    /**
     * @brief Turns every LED pixel in the LED strip off
     *
     * @details
     * Loops through every color in the color array and sets the RGB values to 0,0,0
     */
    void clear() {
        for (int i = 0; i < numLEDs; ++i) {
            colors[i].clear();
        }
    };

    /**
     * @brief Makes the color of every LED pixel brighter
     * @param strength How much to increase the brightness, 0 - 255
     *
     * @details
     * Raises every rgb value for every LED pixel by given strength, the function clamps values at 255 so
     * there is no overflow
     */
    void brighten(const uint8_t& strength = 1) {
        for (int i = 0; i < numLEDs; ++i) {
            colors[i].brighten(strength);
        }
    };

    /**
     * @brief Makes the color of every LED pixel darker (less bright, since LEDs can't be black)
     * @param strength How much to decrease the brightness, 0 - 255
     *
     * @details
     * Lowers every rgb value for every LED pixel by given strength, the function clamps values at 0 so
     * there is no underflow
     */
    void darken(const uint8_t& strength = 1) {
        for (int i = 0; i < numLEDs; ++i) {
            colors[i].darken(strength);
        }
    };

    /**
     * @brief Inverts the color of every LED pixel
     *
     * @details
     * Inverts every rgb value for every LED pixel, red becomes cyan, green becomes magenta etc
     */
    void invert() {
        for (int i = 0; i < numLEDs; ++i) {
            colors[i].invert();
        }
    };


    /**
     * @brief "Renders" the LED strip
     *
     * @details
     * Calculates the bits that need to be send (based on color) and then sends the bits to te WS2812B strip
     */
    void update() {
        // Every led has 3 bytes, green, red and blue (the WS2812B is in GRB order instead of RGB)
        int numBytes = numLEDs * 3;
        int numBits = numBytes * 8;

        // We first add the values of every bit to an array, this can't be done in the for loop that sets the pins
        // because we don't have the time for that
        bool bits[numBits];
        setBits(bits);

        debugPrintStream(numBits, bits); // This is only done in debug modus, see the macro at the top of the file

        // Send the data to the chip
        sendBits(numBits, bits);
    };

// *****************
// Private functions
// *****************
private:

    /**
     * @brief Calculates the bits 24 bits for every LED pixel based on the RGB value and puts them in the bit array
     * @param bits array with 24 * number of LEDs length
     */
    void setBits(bool bits[]) {
        // We loop through every led and then add every bit in the three color bytes to the bits array,
        // So for one led we have 24 bits (1 * 3 * 8) and for two we have 48 bits (2 * 3 * 8 etc...
        int index = 0;
        for (int i = 0; i < numLEDs; ++i) {
            // Get the current color
            const Color& cur = colors[i];

            // For all three bytes
            for (short j = 0; j < 3; ++j) {
                uint8_t curColor;
                if (j % 3 == 0) { curColor = cur.getGreen(); } // First iteration is the green byte
                else if (j % 3 == 1) { curColor = cur.getRed(); } // Second iteration is the red byte
                else { curColor = cur.getBlue(); } // Thirth iteration is the blue byte

                // We check if a bit needs to be high or low,
                // first we mask it with the bit, then we check if it is high or low
                // e.g. if the curColor value is 255 (0B11111111), then (curColor & 0B10000000)
                // results in 0B10000000 and 0B10000000 && 0B10000000 in true.
                // But if curColor value is 5 (0B00000101), then (curColor & 0B10000000)
                // results in 0B00000000 and 0B00000000 && 0B10000000 in false
                // We do this for all 8 bits.
                // The + 0 prevents OCD triggers
                bits[index + 0] = ((curColor & 0B10000000) && 0B10000000); // bit 7 (msb)
                bits[index + 1] = ((curColor & 0B01000000) && 0B01000000); // bit 6
                bits[index + 2] = ((curColor & 0B00100000) && 0B00100000); // bit 5
                bits[index + 3] = ((curColor & 0B00010000) && 0B00010000); // bit 4
                bits[index + 4] = ((curColor & 0B00001000) && 0B00001000); // bit 3
                bits[index + 5] = ((curColor & 0B00000100) && 0B00000100); // bit 2
                bits[index + 6] = ((curColor & 0B00000010) && 0B00000010); // bit 1
                bits[index + 7] = ((curColor & 0B00000001) && 0B00000001); // bit 0 (lsb)

                // Increment index by 8 for the next 8 bits
                index += 8;
            } // End color bytes loop
        } // End leds loop
    };

    /**
     * @brief Sends the data to the WS2812B strip
     * @param numBits Length of bits array
     * @param bits Array with bits that need to be send
     */
    void sendBits(const int& numBits, const bool bits[]) {
        // The chip is controlled by sending 24 bits for every LED (3 bytes for every color)
        // This is done by setting the pins high and then low for a certain time
        // Because this needs to be done insanely fast, we create small delays with inline asembly
        // (which are defined macros at the top of the file)
        // The bits are in green red blue order
        // |G7|G6|G5|G4|G3|G2|G1|G0|R7|R6|R5|R4|R3|R2|R1|R0|B7|B6|B5|B4|B3|B2|B1|B0|
        //
        // | T0H | 0 code ,high voltage time | 0.4us      | ±150ns |
        // | T1H | 1 code ,high voltage time | 0.8us      | ±150ns |
        // | T0L | 0 code , low voltage time | 0.85us     | ±150ns |
        // | T1L | 1 code ,low voltage time  | 0.45us     | ±150ns |
        // | RES |     low voltage time      | Above 50µs |        |
        //
        // 0 Code:                      1 Code:
        //  _______                    _____________
        // |       |     T0L     |    |             |  T1L  |
        // | <---> | <---------> |    | <---------> | <---> |
        // |  T0H  |_____________|    |     T0L     |_______|
        // For more information see the datasheet (https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf)
        const bool* p = bits;
        int n = numBits;
        do {
            if (*p++) {
                dataPin.set(1);
                GLOW_WAIT_T1H();
                dataPin.set(0);
                GLOW_WAIT_T1L();
            } else {
                dataPin.set(1);
                GLOW_WAIT_T0H();
                dataPin.set(0);
                GLOW_WAIT_T0L();
            }
        } while(--n);
    };
}; // End class

}; // End name space
#endif // GLOW_WS2812B_STRIP_N_HPP
