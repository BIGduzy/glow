// ==========================================================================
//
// File      : TC230.cpp
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

// Credits to: Nino van Galen V1B


#ifndef TCS230_HPP
#define TCS230_HPP

#include "hwlib.hpp"

class colour_sensor {
private:
	hwlib::pin_in & d_pin;
	hwlib::pin_out & s0;
	hwlib::pin_out & s1;
	hwlib::pin_out & s2;
	hwlib::pin_out & s3;
public:
	//Create a colour_sensor object, installed pins get set to 0 by default.
	colour_sensor(hwlib::pin_in &d_pin,
		hwlib::pin_out & s0,
		hwlib::pin_out & s1,
		hwlib::pin_out & s2,
		hwlib::pin_out & s3):
	d_pin( d_pin ),
	s0( s0 ),
	s1( s1 ),
	s2( s2 ),
	s3( s3 )
		{
		s0.set(0);
		s1.set(0);
		s2.set(0);
		s3.set(0);
	};
	//brief
	//Set the scale output.
	//detail
	//The scale variable gets set as the scaled output of the pin.
	//With the following settings:
	// 0 = Power down
	// 1 = 2%
	// 2 = 20%
	// 3 = 100%
	//Everything under 0 is Power down and everything above 3 is 100%.

	void set_scale(int scale){
		if( scale <= 0 )
			{
			s0.set(0);
			s1.set(0);
		}

		if( scale == 1 )
			{
			s0.set(1);
			s1.set(0);
		}

		if( scale == 2 )
			{
			s0.set(0);
			s1.set(1);
		}

		if( scale >= 3 )
			{
			s0.set(1);
			s1.set(1);
		}
	}
	//returns the frequenty
	int get_red(unsigned int sample_time = 50000)
		{
		s2.set(0);
		s3.set(0);
		return get_frequency(d_pin, sample_time);
	}
	int get_green(unsigned int sample_time = 50000)
		{
		s2.set(1);
		s3.set(1);
		return get_frequency(d_pin, sample_time);
	}
	int get_blue(unsigned int sample_time = 50000)
		{
		s2.set(0);
		s3.set(1);
		return get_frequency(d_pin, sample_time);
	}
	int get_light(unsigned int sample_time = 50000)
		{
		s2.set(1);
		s3.set(0);
		return get_frequency(d_pin, sample_time);
	}

	int get_frequency( hwlib::pin_in & pin, unsigned int sample_time = 50000)
		{
		int frequency = 0;								//set frequency to 0 for start
		auto start = hwlib::now_us();					//set start time
		auto cycle_time = hwlib::now_us();				//initialize cycle timer
		bool state = pin.get();							//get start state of pin
		while( ( cycle_time - start ) < sample_time)	//keep reading for as long as sample time is not registered
		{
			if(state != pin.get())
			{
				state = !state;
				frequency++;
			}
//			hwlib::wait_us(40);
			cycle_time = hwlib::now_us();
		}
		return frequency/2;								//Two flanks in one duty cycle
	};
};

///brief
///Read frequency from pin_in
///details
///Read frequency from a pin.
///Sample time is 0.5 seconds by default.

#endif //TCS230_HPP
