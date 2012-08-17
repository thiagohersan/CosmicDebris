#include "StaticScene.h"

StaticScene::StaticScene(unsigned char* aVals_, unsigned char* dVals_, int* dVal_){
	// assign the pointers to the serial arrays
	analogVals = aVals_;
	digitalVals = dVals_;
	digitalVal = dVal_;
	// flicker state variables
    lastUpdate = ofGetElapsedTimeMillis();
	bool turnOn = true;	
}

StaticScene::~StaticScene(){
}

void StaticScene::update(){
}

void StaticScene::draw(){
}

/**/
void StaticScene::onSerialEvent(serialEventArgs &a){
}
/**/
