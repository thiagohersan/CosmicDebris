/*
 */

#include "GeometricScene.h"

GeometricScene::GeometricScene(unsigned char* aVals_, unsigned char* dVals_, int* dVal_){
	// assign the pointers to the serial arrays
	analogVals = aVals_;
	digitalVals = dVals_;
	digitalVal = dVal_;
	
	// flicker state variables
	flickerPeriod = 100000;
    lastUpdate = 0.0;
	soundTime = 0.0;
	turnOn = true;
	
	// colors
	shapeColor = ofColor(255,255,255);
	bgndColor = ofColor(0,0,0);
	
	// noise
	varVar = 0;
}

GeometricScene::~GeometricScene(){
}

void GeometricScene::update(){
	/***** reading the values from the serial
	 for this scene:
	 analog[0] = flicker frequency, lfo frequency
	 analog[1] = shape size
	 analog[2] = bgnd color
	 analog[3] = shape color
	 analog[4] = shape variation randomness
	 analog[5] = ....
	 digital[0,1,2] = which shape
	 *****/
	
	// do updates on every frame. but the shapes only do any work if the new values are different
	flickerPeriod = ofMap(analogVals[0], 30,255, 250, 20, true);
	float mSize = ofMap(analogVals[1], 30,255, 0,1, true);
	float bHue = ofMap(analogVals[2], 30,255, 0,255, true);
	float sHue = ofMap(analogVals[3], 30,255, 0,255, true);
	
	varVar = ofMap(analogVals[4], 30,255, 0,20, true);
	unsigned char mShape = ((*digitalVal)>>3)&0x07;
	
	shapeColor = ofColor::fromHsb(sHue, 255, 255);
	bgndColor  = ofColor::fromHsb(bHue, 255, 255);
	myMorphable.setSize(mSize);
	myMorphable.setType(mShape);
	
	myMorphable.update();
}

void GeometricScene::draw(){
	// TODO: draw this on a buffer?
	// time to update !!
	if((soundTime - lastUpdate)*1000 > flickerPeriod){
		// turn on the shapes, turn background off
		if(turnOn == true){
			ofBackground(0,0,0,255);
			myMorphable.setColor(shapeColor);
		}
		// turn off shapes, turn background on
		else{
			ofBackground(bgndColor);
			myMorphable.setColor(ofColor(255,0,0,255));
		}
		
		// finally, draw the shapes
		// lazy !! accessing globals here!!
		for(int i=100; i<ofGetHeight(); i+=200){
			for(int j=100; j<ofGetWidth(); j+=200){
				myMorphable.draw(j,i,varVar);
			}
		}
		// update flicker variables
		lastUpdate = soundTime;
		turnOn = !turnOn;
	}
}

// TODO: fill this out

void GeometricScene::audioOut( float * output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount ){
	for(int i=0; i<bufferSize; i++){
		// update soundTime variable (this is what keeps track of total time)
		soundTime += 1.0/48000;
	}
}

void GeometricScene::onSerialEvent(serialEventArgs &a){}
