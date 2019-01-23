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

#include "TheKnobs.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

const int kNumPrograms = 5;

// LOG FILE GETS PRODUCED INTO THE BIN FOLDER IN \TheKnobs\build-win\vst2\Win32\bin
//ofstream logFile;

enum EParams
{
	kGain = 0,
	kThreshold,
	kDelayMS,
	kFeedback,
	kDryWet,

	kLowPassFilter12Cutoff,
	kLowPassFilter12Resonance,
	kHighPassFilter12Cutoff,
	kHighPassFilter12Resonance,

	kLowPassFilter24Cutoff,
	kLowPassFilter24Resonance,
	kHighPassFilter24Cutoff,
	kHighPassFilter24Resonance,

	kNumKnobs
};

enum ELayout
{
	kWidth = GUI_WIDTH,
	kHeight = GUI_HEIGHT,

	kKnobPositionY = 90,
	kKnobFrames = 64
};

TheKnobs::TheKnobs(IPlugInstanceInfo instanceInfo)
	: IPLUG_CTOR(kNumKnobs, kNumPrograms, instanceInfo), mThreshold(1.), mGain(1.)
{
	TRACE;

	//logFile.open("LogFile.txt");

	//logFile << "IN GUI CONSTRUCTOR" << endl;

	//creates GUI window
	IGraphics* pGraphics = MakeGraphics(this, kWidth, kHeight);

	//pGraphics->AttachPanelBackground(&COLOR_BLUE);
	pGraphics->AttachBackground(BACKGROUND_ID, BACKGROUND_FN);

	//loads knob image
	IBitmap knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, kKnobFrames);

	//CREATES KNOBS FOR GUI
	//arguments are: name, defaultVal, minVal, maxVal, step, label
	GetParam(kGain)->InitDouble("Gain", 50.0, 0.00, 100.0, 0.01, "%");
	pGraphics->AttachControl(new IKnobMultiControl(this, 50, kKnobPositionY, kGain, &knob));

	GetParam(kThreshold)->InitDouble("Distortion Threshold", 0.00, 0.00, 99.99, 0.01, "%");
	pGraphics->AttachControl(new IKnobMultiControl(this, 150, kKnobPositionY, kThreshold, &knob));

	GetParam(kDelayMS)->InitDouble("Delay Time", 100.0, 0.00, 200.0, 0.01, "Milliseconds");
	pGraphics->AttachControl(new IKnobMultiControl(this, 250, kKnobPositionY, kDelayMS, &knob));

	GetParam(kFeedback)->InitDouble("Feedback", 50.0, 0.00, 95.0, 0.01, "%");
	pGraphics->AttachControl(new IKnobMultiControl(this, 325, kKnobPositionY, kFeedback, &knob));

	GetParam(kDryWet)->InitDouble("Dry/Wet", 0.00, 0.00, 100.0, 0.01, "%");
	pGraphics->AttachControl(new IKnobMultiControl(this, 400, kKnobPositionY, kDryWet, &knob));

	GetParam(kLowPassFilter12Cutoff)->InitDouble("-12 LP Cutoff", 0.99, 0.01, 0.99, 0.001, "%");
	pGraphics->AttachControl(new IKnobMultiControl(this, 500, kKnobPositionY, kLowPassFilter12Cutoff, &knob));

	GetParam(kLowPassFilter12Resonance)->InitDouble("-12 LP Resonance", 0.01, 0.01, 0.85, 0.001, "%");
	pGraphics->AttachControl(new IKnobMultiControl(this, 575, kKnobPositionY, kLowPassFilter12Resonance, &knob));

	GetParam(kHighPassFilter12Cutoff)->InitDouble("-12 HP Cutoff", 0.01, 0.01, 0.99, 0.001, "%");
	pGraphics->AttachControl(new IKnobMultiControl(this, 675, kKnobPositionY, kHighPassFilter12Cutoff, &knob));

	GetParam(kHighPassFilter12Resonance)->InitDouble("-12 HP Resonance", 0.01, 0.01, 0.85, 0.001, "%");
	pGraphics->AttachControl(new IKnobMultiControl(this, 750, kKnobPositionY, kHighPassFilter12Resonance, &knob));

	GetParam(kLowPassFilter24Cutoff)->InitDouble("-24 LP Cutoff", 0.99, 0.01, 0.99, 0.001, "%");
	pGraphics->AttachControl(new IKnobMultiControl(this, 850, kKnobPositionY, kLowPassFilter24Cutoff, &knob));

	GetParam(kLowPassFilter24Resonance)->InitDouble("-24 LP Resonance", 0.01, 0.01, 0.85, 0.001, "%");
	pGraphics->AttachControl(new IKnobMultiControl(this, 925, kKnobPositionY, kLowPassFilter24Resonance, &knob));

	GetParam(kHighPassFilter24Cutoff)->InitDouble("-24 HP Cutoff", 0.01, 0.01, 0.99, 0.001, "%");
	pGraphics->AttachControl(new IKnobMultiControl(this, 1025, kKnobPositionY, kHighPassFilter24Cutoff, &knob));

	GetParam(kHighPassFilter24Resonance)->InitDouble("-24 HP Resonance", 0.01, 0.01, 0.85, 0.001, "%");
	pGraphics->AttachControl(new IKnobMultiControl(this, 1100, kKnobPositionY, kHighPassFilter24Resonance, &knob));

	//used to display GUI
	AttachGraphics(pGraphics);

}

// Destructor to free up memory from the buffer and from the Polymorphic Objects
TheKnobs::~TheKnobs()
{
	if (mpBuffer)
	{
		delete[] mpBuffer;

		//logFile << "Deleted mBuffer" << endl;
	}

	if (mLowPassFilter12)
	{
		delete mLowPassFilter12;
		//logFile << "Deleted mLowPassFilter12 Object" << endl;
	}

	if (mHighPassFilter12)
	{
		delete mHighPassFilter12;

		//logFile << "Deleted mHighPassFilter12 Object" << endl;
	}

	if (mLowPassFilter24)
	{
		delete mLowPassFilter24;

		//logFile << "Deleted mLowPassFilter24 Object" << endl;
	}

	if (mHighPassFilter24)
	{
		delete mHighPassFilter24;

		//logFile << "Deleted mLowHighFilter24 Object" << endl;
	}
}

//Function that does the most! It processes the incoming signal from your DAW (Digital Audio Workstation)
void TheKnobs::ProcessDoubleReplacing(
	double** inputs,
	double** outputs,
	int nFrames)
{
	//Sterio Input and output variables
	double* in1 = inputs[0];
	double* in2 = inputs[1];
	double* out1 = outputs[0];
	double* out2 = outputs[1];

	for (int s = 0; s < nFrames; ++s, ++in1, ++in2, ++out1, ++out2) // for loop to cycle through frame / AUDIO SIGNAL
	{

		//first we read our delayed output
		double yn = mpBuffer[mReadIndex];

		//if the delay is 0 samples we just feed it the input
		if (mDelayTime == 0)
		{
			yn = *in1;
		}

		//now write to our delay buffer
		mpBuffer[mWriteIndex] = *in1 + mFeedback * yn;

		//.. and then perform the calculation for the output. Notice how the *in is multiplied by 1 - mWet (which gives the dry level, since wet + dry = 1)
		*out1 = (mWet * yn + (1 - mWet) * *in1);

		//increment the write index, wrapping if it goes out of bounds.
		++mWriteIndex;
		if (mWriteIndex >= mBufferSize)
		{
			mWriteIndex = 0;
		}

		//same with the read index
		++mReadIndex;
		if (mReadIndex >= mBufferSize)
		{
			mReadIndex = 0;
		}

		//Distortion Section
		if (*in1 >= 0) {
			// Make sure positive values can't go above the threshold:
			*out1 += fmin(*in1, mThreshold);
		}
		else {
			// Make sure negative values can't go below the threshold:
			*out1 += fmax(*in1, -mThreshold);
		}

		*out1 /= mThreshold;

		//Filter Section
		*out1 = mLowPassFilter12->process(*out1);
		*out1 = mHighPassFilter12->process(*out1);
		*out1 = mLowPassFilter24->process(*out1);
		*out1 = mHighPassFilter24->process(*out1);

		//Gain Section
		*out1 *= mGain;

		//copies the left output to the right output / eventually want to make this plugin stereo
		*out2 = *out1;
	}
}

//Function used to reset and add to the delay buffer
void TheKnobs::Reset()
{
	TRACE;
	IMutexLock lock(this);

	mBufferSize = 2 * GetSampleRate(); //creates a buffer size of two seconds (samplerate * 2)
	//samplerate is how many samples of audio signal are carried per second
	
	//if buffer isn't NULL / Delete
	if (mpBuffer)
	{
		delete[] mpBuffer;
		
		//logFile << "Had To Delete Delay Buffer" << endl;
	}

	//creates new buffer of mBufferSize
	mpBuffer = new double[mBufferSize];

	resetDelay();
	delayVars();

	//logFile << "IN THEKNOB'S RESET FUNCTION" << endl;
}

//SDK function that constantly updates member variables and change the numerical values when the knobs are turned
void TheKnobs::OnParamChange(int paramIdx)
{
	IMutexLock lock(this);

	//Functions to handle the updating, and improve neatness.
	//Functions are directly below OnParamChange
	gainVars();
	distortionVars();
	delayVars();

	lowPassCutoff12Vars();
	lowPassResonance12Vars();
	lowPassCutoff24Vars();
	lowPassResonance24Vars();

	highPassCutoff12Vars();
	highPassResonance12Vars();
	highPassCutoff24Vars();
	highPassResonance24Vars();

}

//GAIN FUNCTIONS
void TheKnobs::gainVars()
{
	//updates gain
	mGain = GetParam(kGain)->Value() / 100.;

	//logFile << "IN GAIN FUNCTION" << endl;
}

//DISTORTION VARS
void TheKnobs::distortionVars()
{
	//When this knob is turned, the distortion threshold is decreased. The signal is then limited (never exceeds threshold) 
	//and processed later so that the volume doesn't get too quiet
	mThreshold = 1 - (GetParam(kThreshold)->Value() / 100.);

	//logFile << "IN DISTORTION FUNCTION" << endl;
}

//DELAY FUNCTIONS
void TheKnobs::delayVars()
{
	//updates delay time, delay feekback, and delay dry/wet
	mDelayTime = GetParam(kDelayMS)->Value() * GetSampleRate() / 1000.0;
	mFeedback = GetParam(kFeedback)->Value() / 100.0;
	mWet = GetParam(kDryWet)->Value() / 100.0;

	//Makes sure the read index is the right distance behind the write index of the buffer
	mReadIndex = mWriteIndex - (int)mDelayTime;
	if (mReadIndex < 0)
	{
		mReadIndex += mBufferSize;
	}

	//logFile << "IN DELAY FUNCTION" << endl;
}

//function used to reset the delay buffer
void TheKnobs::resetDelay()
{
	if (mpBuffer)
	{
		//Memset fills the "buffer with 0s"
		memset(mpBuffer, 0, mBufferSize * sizeof(double));
	}

	mWriteIndex = 0;
	mReadIndex = 0;

	//logFile << "IN RESET DELAY FUNCTION" << endl;
}

// FREQUENCY FILTER 12 FUNCTIONS
void TheKnobs::lowPassCutoff12Vars()
{
	//sets filter and updates the cutoff of the -12 db lowpass filter when the knob is turned
	mLowPassFilter12->setFilterMode(FrequencyFilter12::FILTER_MODE_LOWPASS);
	mLowPassFilter12->setCutoff(GetParam(kLowPassFilter12Cutoff)->Value());

	//logFile << "IN -12 DB LOWPASS CUTOFF FUNCTION" << endl;
}

void TheKnobs::lowPassResonance12Vars()
{
	//updates the resonance of the -12 db lowpass filter when the knob is turned
	mLowPassFilter12->setResonance(GetParam(kLowPassFilter12Resonance)->Value());

	//logFile << "IN -12 DB LOWPASS RESONANCE FUNCTION" << endl;
}

void TheKnobs::highPassCutoff12Vars()
{
	//sets filter and updates the cutoff of the -12 db highpass filter when the knob is turned
	mHighPassFilter12->setFilterMode(FrequencyFilter12::FILTER_MODE_HIGHPASS);
	mHighPassFilter12->setCutoff(GetParam(kHighPassFilter12Cutoff)->Value());

	//logFile << "IN -12 DB HIGHPASS CUTOFF FUNCTION" << endl;
}

void TheKnobs::highPassResonance12Vars()
{
	//updates the resonance of the -12 db highpass filter when the knob is turned
	mHighPassFilter12->setResonance(GetParam(kHighPassFilter12Resonance)->Value());

	//logFile << "IN -12 DB HIGHPASS RESONANCE FUNCTION" << endl;
}

// FREQUENCY FILTER 24 FUNCTIONS
void TheKnobs::lowPassCutoff24Vars()
{
	//sets filter and updates the cutoff of the -24 db lowpass filter when the knob is turned
	mLowPassFilter24->setFilterMode(FrequencyFilter24::FILTER_MODE_LOWPASS);
	mLowPassFilter24->setCutoff(GetParam(kLowPassFilter24Cutoff)->Value());

	//logFile << "IN -24 DB LOWPASS CUTOFF FUNCTION" << endl;
}

void TheKnobs::lowPassResonance24Vars()
{
	//updates the resonance of the -24 db lowpass filter when the knob is turned
	mLowPassFilter24->setResonance(GetParam(kLowPassFilter24Resonance)->Value());

	//logFile << "IN -24 DB LOWPASS RESONANCE FUNCTION" << endl;
}

void TheKnobs::highPassCutoff24Vars()
{
	//sets filter and updates the cutoff of the -24 db highpass filter when the knob is turned
	mHighPassFilter24->setFilterMode(FrequencyFilter24::FILTER_MODE_HIGHPASS);
	mHighPassFilter24->setCutoff(GetParam(kHighPassFilter24Cutoff)->Value());

	//logFile << "IN -24 DB HIGHPASS CUTOFF FUNCTION" << endl;
}

void TheKnobs::highPassResonance24Vars()
{
	//updates the resonance of the -24 db highpass filter when the knob is turned
	mHighPassFilter24->setResonance(GetParam(kHighPassFilter24Resonance)->Value());

	//logFile << "IN -24 DB HIGHPASS RESONANCE FUNCTION" << endl;
}

