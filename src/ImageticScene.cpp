#include "ImageticScene.h"

ImageticScene::ImageticScene(unsigned char* aVals_, unsigned char* dVals_, int* dVal_){
	// assign the pointers to the serial arrays
	analogVals = aVals_;
	digitalVals = dVals_;
	digitalVal = dVal_;
	// flicker state variables
    lastUpdate = ofGetElapsedTimeMillis();
	turnOn = true;
	flickerPeriod = 100000;
	
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
void ImageticScene::audioOut( float * output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount ){}

void ImageticScene::onSerialEvent(serialEventArgs &a){}


