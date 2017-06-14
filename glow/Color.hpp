#ifndef GLOW_COLOR_HPP
#define GLOW_COLOR_HPP


namespace glow {
typedef unsigned char BYTE;

class Color {
private:
    BYTE red = 0;
    BYTE green = 0;
    BYTE blue = 0;
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
    Color(BYTE red, BYTE green, BYTE blue):
        red(red), green(green), blue(blue)
    {};

    // *****************
    // Getters & setters
    // *****************
    /**
     * @brief Getter for red value
     */
    inline BYTE getRed() const { return red; };
    /**
     * @brief Setter for red value
     */
    inline void setRed(const BYTE& newValue) { red = newValue; };

    /**
     * @brief Getter for green value
     */
    inline BYTE getGreen() const { return green; };
    /**
     * @brief Setter for red value
     */
    inline void setGreen(const BYTE& newValue) { green = newValue; };

    /**
     * @brief Getter for green value
     */
    inline BYTE getBlue() const { return blue; };
    /**
     * @brief Setter for red value
     */
    inline void setBlue(const BYTE& newValue) { blue = newValue; };
};

}

#endif // GLOW_COLOR_HPP