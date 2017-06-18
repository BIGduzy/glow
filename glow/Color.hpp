#ifndef GLOW_COLOR_HPP
#define GLOW_COLOR_HPP
#include <algorithm>

namespace glow {


/**
 * @class Color
 * @author Nick
 * @date 16/06/2017
 * @file Color.hpp
 * @brief RGB Color class
 */
class Color {
private:
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
public:

    /**
     * @brief Default constructor
     *
     * @details
     * Sets rgb values to 0,0,0
     */
    Color():
        red(0), green(0), blue(0)
    {};
    /**
     * @brief Constructor
     * @param red The red value, 0 - 255
     * @param green The green value, 0 - 255
     * @param blue The red value, 0 - 255
     *
     * @details
     * Sets rgb values
     */
    Color(uint8_t red, uint8_t green, uint8_t blue):
        red(red), green(green), blue(blue)
    {};

    // *****************
    // Getters & setters
    // *****************
    /**
     * @brief Getter for red value
     */
    inline uint8_t getRed() const { return red; };
    /**
     * @brief Setter for red value
     */
    inline void setRed(const uint8_t& newValue) { red = newValue; };

    /**
     * @brief Getter for green value
     */
    inline uint8_t getGreen() const { return green; };
    /**
     * @brief Setter for red value
     */
    inline void setGreen(const uint8_t& newValue) { green = newValue; };

    /**
     * @brief Getter for green value
     */
    inline uint8_t getBlue() const { return blue; };
    /**
     * @brief Setter for red value
     */
    inline void setBlue(const uint8_t& newValue) { blue = newValue; };
    // *********************
    // END Getters & setters
    // *********************


    /**
     * @brief Clears the color
     *
     * @details
     * Sets all RGB values to 0
     */
    void clear() {
        red = 0;
        blue = 0;
        green = 0;
    }

    /**
     * @brief Makes color brighter
     * @param strength How much to increase the brightness, 0 - 255
     *
     * @details
     * Raises every rgb value by given strenght, the function clamps values at 255 so
     * there is no overflow
     */
    void brighten(const uint8_t& strength = 1) {
        red = std::min(red + strength, 255); // Prevent overflow of variable
        green = std::min(green + strength, 255); // Prevent overflow of variable
        blue = std::min(blue + strength, 255); // Prevent overflow of variable

    }

    /**
     * @brief Makes color darker
     * @param strength How much to decrease the brightness, 0 - 255
     *
     * @details
     * Lowers every rgb value by given strength, the function clamps values at 0 so
     * there is no underflow
     */
    void darken(const uint8_t& strength = 1) {
        red = std::max(red - strength, 0); // Prevent underflow of variable
        green = std::max(green - strength, 0); // Prevent underflow of variable
        blue = std::max(blue - strength, 0); // Prevent underflow of variable
    }

    /**
     * @brief Inverts color
     *
     * @details
     * Inverts every rgb value, red becomes cyan, green becomes magenta etc
     */
    void invert() {
        // Invert every color byte with the bitwise NOT
        red = ~red;
        green = ~green;
        blue = ~blue;
    }
};

}

#endif // GLOW_COLOR_HPP