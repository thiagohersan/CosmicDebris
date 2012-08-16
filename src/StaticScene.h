#ifndef STATICSCENE_H
#define STATICSCENE_H

#include "Scene.h"

class StaticScene: public Scene {
	
public:
	StaticScene(char* aVals_, char* dVals_, int* dVal_);
	~StaticScene();
	void setup();
	void update();
	void draw();
	void onSerialEvent(serialEventArgs &a);
};

#endif
