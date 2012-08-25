#ifndef IMAGETICSCENE_H
#define IMAGETICSCENE_H

#include "Scene.h"

class ImageticScene: public Scene {
	
public:
	ImageticScene(unsigned char* aVals_, unsigned char* dVals_, int* dVal_);
	~ImageticScene();
	void update();
	void draw();
	void onSerialEvent(serialEventArgs &a);
protected:
	ofTrueTypeFont myFont;
	ofColor myColor;

	// DEBUG
	void audioRequested(float * output, int bufferSize, int nChannels);
};

#endif
