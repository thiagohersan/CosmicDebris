#ifndef SCENE_H
#define SCENE_H

//#include "SerialEventArgs.h"
#include "ofEvents.h"

#define NUM_ANALOG 6
#define NUM_DIGITAL 6

class serialEventArgs : public ofEventArgs{
public:
	unsigned char type_;
	unsigned char id_;
	unsigned char value_;
};


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
	char analogVals[NUM_ANALOG];
	char digitalVals[NUM_DIGITAL];
	int digitalVal;
};

#endif
