#ifndef SCENE_H
#define SCENE_H

#include "SerialEventArgs.h"

class Scene {
	
	public:
	virtual ~Scene() {}
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void onSerialEvent(serialEventArgs &a) = 0;
	
	// TODO:
	//   - add state variables
	//   - think about how these will change...
	//   - do I need another object in between main and the scenes??
	
protected:
	// invariant: these are always up to date
	char* analogVals;
	char* digitalVals;
	int* digitalVal;
};

#endif
