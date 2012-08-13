#ifndef STATICSCENE_H
#define STATICSCENE_H


#include "Scene.h"

class StaticScene: public Scene {
	
public:
	StaticScene();
	~StaticScene();
	void setup();
	void update();
	void draw();
	void onSerialEvent(serialEventArgs &a);
};

#endif
