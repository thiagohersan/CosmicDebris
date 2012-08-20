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
    lastUpdate = ofGetElapsedTimeMillis();
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
	 analog[0] = frequency
	 analog[1] = shape color
	 analog[2] = bgnd color
	 analog[3] = shape size
	 analog[4] = shape variation randomness
	 analog[5] = ....
	 digital[0,1,2] = which shape
	 *****/
	
	// do updates on every frame. but the shapes only do any work if the new values are different
	flickerPeriod = ofMap(analogVals[0], 30,255, 250, 20, true);
	float sHue = ofMap(analogVals[1], 30,255, 0,255, true);
	float bHue = ofMap(analogVals[2], 30,255, 0,255, true);
	float mSize = ofMap(analogVals[3], 30,255, 0,1, true);
	varVar = ofMap(analogVals[4], 30,255, 0,20, true);
	unsigned char mShape = ((*digitalVal)>>3)&0x07;
	
	shapeColor = ofColor::fromHsb(sHue, 255, 255);
	bgndColor  = ofColor::fromHsb(bHue, 255, 255);
	myMorphable.setSize(mSize);
	myMorphable.setType(mShape);
	
	myMorphable.update();
}

void GeometricScene::draw(){
	// time to update !!
	if((ofGetElapsedTimeMillis() - lastUpdate) > flickerPeriod){
		// turn on the shapes, turn background off
		if(turnOn == true){
			ofBackground(0,0,0,255);
			myMorphable.setColor(shapeColor);
			// TODO: sound on
		}
		// turn off shapes, turn background on
		else{
			ofBackground(bgndColor);
			myMorphable.setColor(ofColor(255,0,0,255));
			// TODO: sound off
		}
		
		// finally, draw the shapes
		// lazy !! accessing globals here!!
		for(int i=100; i<ofGetHeight(); i+=200){
			for(int j=100; j<ofGetWidth(); j+=200){
				myMorphable.draw(j,i,varVar);
			}
		}
		// update flicker variables
		lastUpdate = ofGetElapsedTimeMillis();
		turnOn = !turnOn;
	}
}

void GeometricScene::onSerialEvent(serialEventArgs &a){}
