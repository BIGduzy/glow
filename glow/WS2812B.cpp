#include "WS2812B.hpp"

glow::WS2812B::WS2812B(int numLEDs):
    numLEDs(numLEDs)
{
    // The number of leds can't be zero or less
    if (numLEDs < 1) numLEDs = 1;

    colors[numLEDs];
    // Fill the color array with 0,0,0 rgb values
    clear();
}

void glow::WS2812B::setPixelColor(int index, BYTE red, BYTE green, BYTE blue) {
    glow::Color& c = colors[index];
    c.setRed(red);
    c.setGreen(green);
    c.setBlue(blue);
}

glow::Color glow::WS2812B::getPixelColor(int index) const {
    return colors[index];
}

void glow::WS2812B::clear() {
    for (int i = 0; i < numLEDs; ++i) {
        colors[i] = Color(0,0,0);
    }
}

void glow::WS2812B::update() {
    // Every led has 3 bytes, green, red and blue (the WS2812B is in this order instead of RGB)
    int numBytes = numLEDs * 3;
    int numBits = numBytes * 8;

    // We first add the values of every bit to an array, this can't be done in the for loop that sets the pins
    // because we don't have the time for that
    // We loop through every led and then add every bit in the tree color bytes to the bits array,
    // So for one led we have 24 bits (1 * 3 * 8) and for two we have 48 bits (2 * 3 * 8 etc...
    bool bits[numBits];
    for (int i = 0; i < numLEDs; ++i) {
        // Get the current color
        const Color& cur = colors[i];
        int index = i * 8;

        // For all three bytes
        for (short j = 0; j < 3; ++j) {
            BYTE curColor;
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
        }

        hwlib::cout << "--BEGIN STREAM--" << hwlib::endl << hwlib::endl;

        for (int i = 0; i < numBits; ++i) {
            hwlib::cout << bits[i] << " ";

            if ((i + 1) % 8 == 0) hwlib::cout << hwlib::endl;
            if ((i + 1) % (8 * 3) == 0) hwlib::cout << hwlib::endl;
        }

        hwlib::cout << "--END STREAM--" << hwlib::endl;
    }
}