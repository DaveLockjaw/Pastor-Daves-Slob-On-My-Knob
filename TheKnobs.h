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

//CODE ONLY COMPILES IN VST2 FORM AS OF RIGHT NOW! MAKE SURE TO RIGHT CLICK AND SET AS STARTUP PROJECT

#ifndef __THEKNOBS__
#define __THEKNOBS__

#pragma warning( suppress : 4101 4129 )
#include "IPlug_include_in_plug_hdr.h"
#include "Filter.h"

#include <fstream>

using namespace std;

//Main Knob Class
class TheKnobs : public IPlug
{
public:
	TheKnobs(IPlugInstanceInfo instanceInfo);
	~TheKnobs();

	void Reset();
	void OnParamChange(int paramIdx);
	void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);

	//gain and distortion prototypes
	void gainVars();
	void distortionVars();

	//delay prototypes
	void delayVars();
	void resetDelay();

	//-12 db filter prototypes
	void lowPassCutoff12Vars();
	void lowPassResonance12Vars();
	void highPassCutoff12Vars();
	void highPassResonance12Vars();

	//-24 db filter prototypes
	void lowPassCutoff24Vars();
	void lowPassResonance24Vars();
	void highPassCutoff24Vars();
	void highPassResonance24Vars();

private:
	//gain and distortion member variables
	double mGain = 1.0;
	double mThreshold;

	//delay member variables
	double mDelayTime;
	double mFeedback;
	double mWet;

	//delay buffer member variables
	double* mpBuffer = NULL;
	int mReadIndex = 0;
	int mWriteIndex = 0;
	int mBufferSize = 0;

	//Polymorphic Filter Objects used for the frequency filters
	AudioEffect* mLowPassFilter12 = new FrequencyFilter12;
	AudioEffect* mHighPassFilter12 = new FrequencyFilter12;
	AudioEffect* mLowPassFilter24 = new FrequencyFilter24;
	AudioEffect* mHighPassFilter24 = new FrequencyFilter24;
};

#endif
