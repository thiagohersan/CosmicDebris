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
	enum SoundEnum {SOUND_NOISE, SOUND_SINE, SOUND_RECORDED};
	float lastSize;
	float soundTime, lastUpdate;
private:
	float maxLog2;
	
	void audioRequested(float * output, int bufferSize, int nChannels);
	float currFreq, targetFreq, currLfoFreq, targetLfoFreq, overallVolume;
	unsigned char whichSound;
	unsigned char whichStatic;
};

#endif
