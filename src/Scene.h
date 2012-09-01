#ifndef SCENE_H
#define SCENE_H

#include "SerialEventArgs.h"
#include "ofMain.h"

class Scene: public ofBaseSoundInput, public ofBaseSoundOutput {
	
public:
	virtual ~Scene() {}
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void onSerialEvent(serialEventArgs &a) = 0;
protected:
	// invariant: these are always up to date
	unsigned char* analogVals;
	unsigned char* digitalVals;
	int* digitalVal;
	
	// state variables for flickering, on each scene
	float flickerPeriod;
	float soundTime, lastUpdate;
	// sound functions
	virtual void audioOut( float * output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount ) = 0;
};

#endif
