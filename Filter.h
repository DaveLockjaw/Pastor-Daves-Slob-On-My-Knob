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

#pragma once

class AudioEffect
{
public:
	enum FilterMode {
		FILTER_MODE_LOWPASS = 0,
		FILTER_MODE_HIGHPASS
	};

	AudioEffect()
	{
		cutoff = 0.99;
		resonance = 0.0,
		mode = FILTER_MODE_LOWPASS;
		buf0 = 0.0;
		buf1 = 0.0;
		calculateFeedbackAmount();
	};

	//pure virtual function for polymorphism
	virtual double process(double inputValue) = 0;

	//filter functions
	inline void setCutoff(double newCutoff) { cutoff = newCutoff; calculateFeedbackAmount(); };
	inline void setResonance(double newResonance) { resonance = newResonance; calculateFeedbackAmount(); };
	inline void setFilterMode(FilterMode newMode) { mode = newMode; }

	//protected filter member variables that inherited classes can use
protected:
	double cutoff;
	double resonance;
	FilterMode mode;
	double feedbackAmount;
	inline void calculateFeedbackAmount() { feedbackAmount = resonance + resonance / (1.0 - cutoff); }
	double buf0;
	double buf1;
};

//-12 DB inherited class of AudioEffect
class FrequencyFilter12 : public AudioEffect
{
public:
	double process(double inputvalue);
};

//-12 DB child inherited of AudioEffect /  has a few parameters that only -24 has
class FrequencyFilter24 : public AudioEffect
{
public:

	//Frequency Filter constructor to add the new buffers
	FrequencyFilter24()
	{
		buf2 = 0.0;
		buf3 = 0.0;
	}

	double process(double inputvalue);

protected:
	double buf2;
	double buf3;
};