#ifndef STATICSCENE_H
#define STATICSCENE_H

#include "Scene.h"

class StaticScene: public Scene {
	
public:
	StaticScene(unsigned char* aVals_, unsigned char* dVals_, int* dVal_);
	~StaticScene();
	void update();
	void draw();
	void onSerialEvent(serialEventArgs &a);
protected:
	ofImage myImage;
	enum StaticEnum {STATIC_RANDOM, STATIC_INVERSE};
	enum SoundEnum {SOUND_NOISE, SOUND_SINE};
	float lastSize;
	float currFreq, targetFreq, currLfoFreq, targetLfoFreq, overallVolume, sineVolume;
	void audioOut( float * output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount );
	ofSoundPlayer mySoundPlayer;
private:
	float maxLog2;
	unsigned char whichSound;
	unsigned char whichStatic;
	unsigned char whichSample, lastTriggerVal;
};

#endif
