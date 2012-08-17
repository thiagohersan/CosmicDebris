
// need a state machine to tell whether we're fading in/out or performing.
//   also need to grab the values from serial port to choose scene
//   but the scene keeps track of the pulse frequency

// this will keep the analog/digital arrays
//   and pass pointers to the scenes.

// NOTE: careful to do memory management when changing scenes.

#ifndef CANVAS_H
#define CANVAS_H

#include "SerialEventArgs.h"
#include "StaticScene.h"
#include "ofMain.h"

#define NUM_ANALOG 6
#define NUM_DIGITAL 6

#define FADE_STEP 5

class Canvas {
	
public:
	Canvas();
	~Canvas();
	void update();
	void draw();
	void onSerialEvent(serialEventArgs &a);
	
protected:
	// invariant: these are always up to date
	char analogVals[NUM_ANALOG];
	char digitalVals[NUM_DIGITAL];
	int digitalVal;
	
	enum StateEnum {STATE_STEADY, STATE_FADING};
	enum SceneEnum {SCENE_STATIC, SCENE_GEOMETRY, SCENE_IMAGE};

	// keep track of curresnt state (fade or steady)
	int currState;

	// fade amount. goes from 0->255->0
	float fadeAlpha;
	
	// keep track of current and next scenes. these are based on SceneEnum
	int currScene, nextScene;
	// fucking pointers... hooray!
	Scene* theScene;
};


#endif