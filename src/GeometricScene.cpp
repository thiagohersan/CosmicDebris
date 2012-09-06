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

	// volume control
	overallVolume = 0.0;
	currLfoFreq = targetLfoFreq = 0.0;

	// colors
	shapeColor = ofColor(255,255,255);
	bgndColor = ofColor(0,0,0);
	
	// noise
	varVar = 0;
	
	// here we go...
	// TODO: variable sized buffer please
	soundBuffer = new float[1024*2];
}

GeometricScene::~GeometricScene(){
	delete soundBuffer;
}

void GeometricScene::update(){
	/***** reading the values from the serial
	 for this scene:
	 analog[0] = flicker frequency, lfo frequency
	 analog[1] = shape size
	 analog[2] = bgnd color
	 analog[3] = shape color
	 analog[4] = shape variation randomness
	 analog[5] = overall volume
	 digital[0,1,2] = which shape
	 *****/
	
	// do updates on every frame. but the shapes only do any work if the new values are different
	flickerPeriod = ofMap(analogVals[0], 40,250, 250, 20, true);
	float mSize = ofMap(analogVals[1], 40,250, 0,1, true);
	float bHue = ofMap(analogVals[2], 40,250, 0,255, true);
	float sHue = ofMap(analogVals[3], 40,250, 0,255, true);
	
	varVar = ofMap(analogVals[4], 40,250, 0,20, true);
	unsigned char mShape = ((*digitalVal)>>3)&0x07;

	// overall sound volume
	overallVolume = ofMap(analogVals[5], 40,250, 0.0,1.2, true);
	// target lfo frequency
	targetLfoFreq = 2000*PI/flickerPeriod;

	shapeColor = ofColor::fromHsb(sHue, 255, 255);
	bgndColor  = ofColor::fromHsb(bHue, 255, 255);
	myMorphable.setSize(mSize);
	myMorphable.setType(mShape);
	
	myMorphable.update();
}

void GeometricScene::draw(){
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
			myMorphable.setColor(ofColor(255,255,255,255));
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
void GeometricScene::audioIn( float * input, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount ){
	for(int i=0; (i<bufferSize); i++){
		//soundBuffer[i] *= 0.5;
		//soundBuffer[i] += 0.5*input[i];
		// TODO: memcopy ?
		//soundBuffer[2*i+0] = input[2*i+2];
		//soundBuffer[2*i+1] = input[2*i+3];
		soundBuffer[2*i+0] = input[2*i+0];
		soundBuffer[2*i+1] = input[2*i+1];
	}
}

void GeometricScene::audioOut( float * output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount ){
	for(int i=0; (i<bufferSize); i++){
		// update soundTime variable (this is what keeps track of total time)
		soundTime += 1.0/48000;

		// very unstable
		//currLfoFreq = ofLerp(currLfoFreq, targetLfoFreq, 1.0/48000);

		// unstable enough for awesomeness
		currLfoFreq = ofLerp(currLfoFreq, targetLfoFreq, 0.001);

		float lfoVolume = 0.5*(sin(currLfoFreq*soundTime)+1.0);

		//output[2*i+0] = soundBuffer[2*i+0]*lfoVolume*overallVolume;
		//output[2*i+1] = soundBuffer[2*i+1]*lfoVolume*overallVolume;

		output[2*i+0] = soundBuffer[2*i+0]*overallVolume;
		output[2*i+1] = soundBuffer[2*i+1]*overallVolume;		
	}
}

void GeometricScene::onSerialEvent(serialEventArgs &a){}
