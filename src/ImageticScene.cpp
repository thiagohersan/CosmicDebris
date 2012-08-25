#include "ImageticScene.h"

ImageticScene::ImageticScene(unsigned char* aVals_, unsigned char* dVals_, int* dVal_){
	// assign the pointers to the serial arrays
	analogVals = aVals_;
	digitalVals = dVals_;
	digitalVal = dVal_;
	// flicker state variables
    lastUpdate = ofGetElapsedTimeMillis();
	bool turnOn = true;
	
	// DEBUG
	myFont.loadFont("Arial Bold.ttf",32);
	myColor = ofColor(ofRandom(255),ofRandom(255), ofRandom(255));
}

ImageticScene::~ImageticScene(){
}

void ImageticScene::update(){}

void ImageticScene::draw(){
	// DEBUG
	ofBackground(255);
	ofSetColor(myColor);
	myFont.drawString("Test from ImageticScene", 100,100);
}

// TODO: fill this out
void ImageticScene::audioRequested(float * output, int bufferSize, int nChannels){}

void ImageticScene::onSerialEvent(serialEventArgs &a){}


