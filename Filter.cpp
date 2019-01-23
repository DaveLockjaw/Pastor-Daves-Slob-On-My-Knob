/*
Author: David Mann Jr.
Class: CSI-240-01
Assignment: FINAL PROJECT
Assigned: 11/14/17
Due Date: 12/4/17

Description:
This program allows the user to put the DLL that this framework builds into a DAW and use the plugin while
producing digital music!

Certification of Authenticity:
I certify that this is entirely my own work, except where I have given
fully-documented references to the work of others. I understand the
definition and consequences of plagiarism and acknowledge that the assessor
of this assignment may, for the purpose of assessing this assignment:
- Reproduce this assignment and provide a copy to another member of
academic staff; and/or
- Communicate a copy of this assignment to a plagiarism checking service
(which may then retain a copy of this assignment on its database for the purpose of future plagiarism checking)

FULLY DOCUMENTED REFERENCES TO THE WORK OF OTHERS:
 -USED THE WDL-OL FRAMEWORK/ LIBRARY FOR C++ (HELPED A LOT FOR GETTING A JUMPSTART ON PLUGIN DEVELOPMENT)
 -USED ALGORITHMS AND CODE FROM MARTIN FINKE'S BLOG "MAKING AUDIO PLUGINS" (VERY HELPFUL FOR GIVING ME AN UNDERSTANDING TO WHAT I WAS PROGRAMMING
 -USED ALGORITHMS AND CODE FROM LUKE ZEITLIN'S BLOG ABOUT HOW DELAYS WORK TO LEARN HOW DELAYS ARE MADE
 -USED THE ASIO SDK
 -USED THE STEINBERG VST SDK
 -USED RTAUDIO FOR REALTIME AUDIO
 -USED THE IPLUGEFFECT BASE FOR A STARTING POINT (SO MY PROJECT WOULD COMPILE RIGHT USING THE SDK AND LIBRARIES)
*/

#include "Filter.h"

//Polymorphic functions for processing the Frequency Filters

//-12 DB filter process function
double FrequencyFilter12::process(double inputValue) {
	buf0 += cutoff * (inputValue - buf0 + feedbackAmount * (buf0 - buf1));
	buf1 += cutoff * (buf0 - buf1);
	
	//switch so the function knows what filter to produce
	switch (mode) {
	case FILTER_MODE_LOWPASS:
		return buf1;
	case FILTER_MODE_HIGHPASS:
		return inputValue - buf0;
	default:
		return 0.0;
	}
}

//-24 DB filter process function
double FrequencyFilter24::process(double inputValue) {
	buf0 += cutoff * (inputValue - buf0 + feedbackAmount * (buf0 - buf1));
	buf1 += cutoff * (buf0 - buf1);
	buf2 += cutoff * (buf1 - buf2);
	buf3 += cutoff * (buf2 - buf3);

	//switch so the function knows what filter to produce
	switch (mode) {
	case FILTER_MODE_LOWPASS:
		return buf3;
	case FILTER_MODE_HIGHPASS:
		return inputValue - buf3;
	default:
		return 0.0;
	}
}