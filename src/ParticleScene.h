#ifndef PARTICLESCENE_H
#define PARTICLESCENE_H

#include "Scene.h"
#include "SimpleParticle.h"

class ParticleScene: public Scene {

public:
	ParticleScene(unsigned char* aVals_, unsigned char* dVals_, int* dVal_);
	~ParticleScene();
	void update();
	void draw();
	void onSerialEvent(serialEventArgs &a);
protected:
	bool turnOn;
	float currLfoFreq, targetLfoFreq, overallVolume;
	enum FlickerEnum {FLICKER_NEITHER, FLICKER_PARTICLE, FLICKER_BGND, FLICKER_BOTH};
	unsigned char whichFlicker;
	ofColor theColor;
	void audioOut( float * output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount );
private:
	vector<SimpleParticle> myParts;
	vector<ofVec2f> myTargets;
	int numParts, numGroups;
};

#endif
