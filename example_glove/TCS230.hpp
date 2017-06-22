// ==========================================================================
//
// File      : TC230.cpp
// Part of   : C++ Glow, Lightweight WS2812B LED strip library
// Author    : Nick Bout
// Author    : Nono van Galen
// Github    : https://github.com/BIGduzy/glow
// Copyright : nick.bout@student.hu.nl 2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================


#ifndef TCS230_HPP
#define TCS230_HPP

enum class SCALING_MODES { OFF, LOW, MEDIUM, HIGH };
enum class COLOR_MODES { RED, GREEN, BLUE, CLEAR };

#include <hwlib.hpp>
#include <Color.hpp>

class TCS230 {
private:
    /* Pin used by color sensor to return data */
	hwlib::target::pin_in& dataPin;
    /* Pins to set scaling */
	hwlib::target::pin_out& s0;
	hwlib::target::pin_out& s1;
    /* Pins to set filter */
	hwlib::target::pin_out& s2;
	hwlib::target::pin_out& s3;

    /* Scaling mode */
    SCALING_MODES mode = SCALING_MODES::MEDIUM;

    /* Variables used for color mapping */
    unsigned minRed = 1000;
    unsigned minGreen = 1000;
    unsigned minBlue = 1000;

    unsigned maxRed = 0;
    unsigned maxGreen = 0;
    unsigned maxBlue = 0;
public:
    //Create a colour_sensor object, installed pins get set to 0 by default.
    TCS230(
        hwlib::target::pin_in& dataPin,
        hwlib::target::pin_out& s0,
        hwlib::target::pin_out& s1,
        hwlib::target::pin_out& s2,
        hwlib::target::pin_out& s3
    ):
        dataPin(dataPin),
        s0(s0),
        s1(s1),
        s2(s2),
        s3(s3)
    {};

    // *******************
    // Getters/setters
    // *******************
    /**
     * @brief Setter for the scaling mode
     * @param mode
     */
    void setMode(SCALING_MODES mode) {
        mode = mode;
    }
    // *******************
    // End Getters/setters
    // *******************

    /**
     * @brief Returns rgb values from the color sensor
     * @param sampleTime The measure duration in microseconds
     * @return glow::Color object with the RGB values
     */
    glow::Color getColor(unsigned sampleTime = 50000) {
        // Set scaling
        setScale(mode);
        hwlib::wait_ms(1); // Wait a bit for the sensor to turn on

        // Set filter to red and then measure frequency
        setFilter(COLOR_MODES::RED);
        unsigned redPulse = getFrequency(sampleTime);
        // Set filter to green and then measure frequency
        setFilter(COLOR_MODES::GREEN);
        unsigned greenPulse = getFrequency(sampleTime);
        // Set filter to blue and then measure frequency
        setFilter(COLOR_MODES::BLUE);
        unsigned bluePulse = getFrequency(sampleTime);

        // Turn color sensor off
        setScale(SCALING_MODES::OFF);


        // Adjust the minimum values
        if (minRed > redPulse) minRed = redPulse;
        if (minGreen > greenPulse) minGreen = greenPulse;
        if (minBlue > bluePulse) minBlue = bluePulse;

        // Adjust the maximum values
        if (maxRed < redPulse) maxRed = redPulse;
        if (maxGreen < greenPulse) maxGreen = greenPulse;
        if (maxBlue < bluePulse) maxBlue = bluePulse;

        // Map the color to an actual rgb value
        int red = map(redPulse, minRed, maxRed, 0, 255);
        int green = map(greenPulse, minGreen, maxGreen, 0, 255);
        int blue = map(bluePulse, minBlue, maxBlue, 0, 255);

        // return created color
        return glow::Color(red, green, blue);
    }

    /**
     * @brief get the clear value
     * @param sampleTime The measure duration in microseconds
     * @return Raw clear value from the color sensor
     */
    unsigned getClear(unsigned sampleTime = 50000) {
        s2.set(1);
        s3.set(0);
        return getFrequency(sampleTime);
    }

    // Credits to: Nino van Galen V1B
    unsigned getFrequency(unsigned sampleTime = 50000) {
        unsigned frequency = 0;
        auto start = hwlib::now_us();
        auto currentTime = hwlib::now_us();
        bool state = dataPin.get();

        // Loop for a sertan time (default 50.000us)
        while( ( currentTime - start ) < sampleTime) {
            if(state != dataPin.get()) {
                state = !state;
                frequency++;
            }
            currentTime = hwlib::now_us();
        }

        // 50% duty cycle so we devide by 2
        return frequency / 2;
    };

private:
    /**
     * @brief Remap value between given min and max
     * @param value The value
     * @param inMin Minimum of the input(raw) value
     * @param inMax Maximum of the input(raw) value
     * @param outMin Minimum of the output value
     * @param outMax Maximum of the output value
     * @return The remapped value
     */
    int map(int value, int inMin, int inMax, int outMin, int outMax) {
        return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
    };

        /**
     * @brief Sets the output scaling
     * @param mode One of the 4 modes from the SCALING_MODES enum
     *
     * @detail
     * Set the mode pins according to the scaling
     * OFF = Power down     (s0: 0, s1: 0)
     * LOW = 2% scaling     (s0: 1, s1: 0)
     * Medium = 20% scaling (s0: 0, s1: 1)
     * High = 100% scaling  (s0: 1, s1: 1)
     */
    void setScale(SCALING_MODES mode) {
        bool s0_on = false;
        bool s1_on = false;

        // Turn on pins when they need to be
        if (mode == SCALING_MODES::LOW || mode == SCALING_MODES::HIGH) s0_on = true;
        if (mode == SCALING_MODES::MEDIUM || mode == SCALING_MODES::HIGH) s1_on = true;

        hwlib::cout << "s0: " << s0_on << " s1: " << s1_on << hwlib::endl;

        s0.set(s0_on);
        s1.set(s1_on);
    }

    /**
     * @brief Sets filter
     * @param mode One of the 4 COLOR_MODES
     *
     * @detail
     * Set the filter pins according to the color
     * RED = Power down     (s0: 0, s1: 0)
     * GREEN = 2% scaling   (s0: 1, s1: 1)
     * BLUE = 20% scaling   (s0: 0, s1: 1)
     * CLEAR = 100% scaling (s0: 1, s1: 0)
     */
    void setFilter(COLOR_MODES mode) {
        bool s2_on = false;
        bool s3_on = false;

        // Turn on pins when they need to be
        if (mode == COLOR_MODES::CLEAR || mode == COLOR_MODES::GREEN) s2_on = true;
        if (mode == COLOR_MODES::BLUE || mode == COLOR_MODES::GREEN)  s3_on = true;

        hwlib::cout << "s2: " << s2_on << " s3: " << s3_on << hwlib::endl;

        s2.set(s2_on);
        s3.set(s3_on);
    }
};

#endif //TCS230_HPP
