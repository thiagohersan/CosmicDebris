#ifndef STATICSCENE_H
#define STATICSCENE_H

#include "Scene.h"

class StaticScene: public Scene {
	
public:
	StaticScene(unsigned char* aVals_, unsigned char* dVals_, int* dVal_);
	~StaticScene();
	void update();
	void draw();
	void onSerialEvent(serialEventArgs &a);
protected:
	ofImage myImage;
private:
	float maxLog2;
};

#endif
