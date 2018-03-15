#ifndef IMAGETICSCENE_H
#define IMAGETICSCENE_H

#include "Scene.h"
#include "ofVideoPlayer.h"

class ImageticScene: public Scene {
	
public:
	ImageticScene(unsigned char* aVals_, unsigned char* dVals_, int* dVal_);
	~ImageticScene();
	void update();
	void draw();
	void onSerialEvent(serialEventArgs &a);
protected:
	bool turnOn;
	void audioOut( float * output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount );
	float currLfoFreq, targetLfoFreq, overallVolume;
private:
	ofVideoPlayer myVideoPlayer;
    unsigned int whichVideo;
};

#endif
