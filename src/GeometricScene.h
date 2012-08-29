#ifndef GEOMETRICSCENE_H
#define GEOMETRICSCENE_H

#include "Scene.h"
#include "Morphable.h"

class GeometricScene: public Scene {
	
public:
	GeometricScene(unsigned char* aVals_, unsigned char* dVals_, int* dVal_);
	~GeometricScene();
	void update();
	void draw();
	void onSerialEvent(serialEventArgs &a);
protected:
	Morphable myMorphable;
	ofColor shapeColor, bgndColor;
	// for random variations
	float varVar;
	// for flickering
	//unsigned int lastUpdate;
	float soundTime, lastUpdate;
private:
	void audioOut( float * output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount );
};

#endif
