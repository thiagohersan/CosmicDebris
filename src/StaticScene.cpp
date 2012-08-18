#include "StaticScene.h"

StaticScene::StaticScene(unsigned char* aVals_, unsigned char* dVals_, int* dVal_){
	// assign the pointers to the serial arrays
	analogVals = aVals_;
	digitalVals = dVals_;
	digitalVal = dVal_;
	// flicker state variables
    lastUpdate = ofGetElapsedTimeMillis();
	bool turnOn = true;
	
	// debug
	myFont.loadFont("Arial Bold.ttf",32);
	myColor = ofColor(ofRandom(255),ofRandom(255), ofRandom(255));
}

StaticScene::~StaticScene(){
}

void StaticScene::update(){
	// debug
	int i = abs(ofRandom(ofRandom(12), ofRandom(14, 55)) * ofNoise(ofRandom(0.5)));
	i += 1;
}

void StaticScene::draw(){
	// debug
	ofBackground(255);
	ofSetColor(myColor);
	myFont.drawString("Test from StaticScene", 100,100);
}

/**/
void StaticScene::onSerialEvent(serialEventArgs &a){
}
/**/
