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
    Color(BYTE red, BYTE green, BYTE blue);
    // Getters & setters
    inline BYTE getRed() const { return red; };
    inline void setRed(const BYTE& newValue) { red = newValue; };

    inline BYTE getGreen() const { return green; };
    inline void setGreen(const BYTE& newValue) { green = newValue; };

    inline BYTE getBlue() const { return blue; };
    inline void setBlue(const BYTE& newValue) { blue = newValue; };
};

}

#endif // GLOW_COLOR_HPP