#ifndef SCENE_H
#define SCENE_H

#include "SerialEventArgs.h"

class Scene {
	
	public:
	virtual ~Scene() {}
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void onSerialEvent(serialEventArgs &a) = 0;
	
	// TODO: add state variables for flickering, on each scene
	
protected:
	// invariant: these are always up to date
	char* analogVals;
	char* digitalVals;
	int* digitalVal;
};

#endif
