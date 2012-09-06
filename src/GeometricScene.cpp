/*
 */

#include "GeometricScene.h"

#define BUF_LEN 2048
#define NUM_IN_CHANNELS 1

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
	bgndColor = ofColor(255,255);
	
	// noise
	varVar = 0;
	
	// here we go...
	soundBuffer = new float[BUF_LEN*NUM_IN_CHANNELS];
	inCnt = outCnt = 0;
}

GeometricScene::~GeometricScene(){
	delete soundBuffer;
}

void GeometricScene::update(){
	/***** reading the values from the serial
	 for this scene:
	 analog[0] = flicker frequency, lfo frequency
	 analog[1] = shape size
	 analog[2] = shape color
	 analog[3] = shape variation randomness
	 analog[4] = 
	 analog[5] = overall volume
	 digital[0,1,2] = which shape
	 *****/
	
	// do updates on every frame. but the shapes only do any work if the new values are different
	flickerPeriod = ofMap(analogVals[0], 40,250, 250, 20, true);
	// target lfo frequency
	targetLfoFreq = 2000*PI/flickerPeriod;

	float mSize = ofMap(analogVals[1], 40,250, 0,1, true);
	float sHue = ofMap(analogVals[2], 40,255, 0,255, true);
	
	varVar = ofMap(analogVals[3], 40,250, 0,20, true);
	// overall sound volume
	overallVolume = ofMap(analogVals[5], 40,250, 0.0,1.2, true);

	unsigned char mShape = ((*digitalVal)>>3)&0x07;

	shapeColor = ofColor::fromHsb(sHue, 255, 100);
	myMorphable.setSize(mSize);
	myMorphable.setType(mShape);
	
	myMorphable.update();
}

void GeometricScene::draw(){
	// time to update !!
	if((soundTime - lastUpdate)*1000 > flickerPeriod){
		// turn on the shapes, turn background off
		if(turnOn == true){
			ofBackground(bgndColor);
			myMorphable.setColor(shapeColor);
		}
		// turn off shapes, turn background on
		else{
			ofBackground(shapeColor);
			myMorphable.setColor(bgndColor);
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
		//soundBuffer[2*i+0] = input[2*i+0];
		//soundBuffer[2*i+1] = input[2*i+1];
		// interleave on buffer
		for(int j=0; j<nChannels; j++){
			soundBuffer[inCnt%BUF_LEN] = input[nChannels*i+j];
			inCnt++;
		}
	}
}

void GeometricScene::audioOut( float * output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount ){
	for(int i=0; (i<bufferSize)&&(inCnt > outCnt); i++){
		// update soundTime variable (this is what keeps track of total time)
		soundTime += 1.0/48000;

		// very unstable
		//currLfoFreq = ofLerp(currLfoFreq, targetLfoFreq, 1.0/48000);

		// unstable enough for awesomeness
		currLfoFreq = ofLerp(currLfoFreq, targetLfoFreq, 0.001);

		float lfoVolume = 0.5*(sin(currLfoFreq*soundTime)+1.0);

		//output[2*i+0] = soundBuffer[2*i+0]*lfoVolume*overallVolume;
		//output[2*i+1] = soundBuffer[2*i+1]*lfoVolume*overallVolume;

		for(int j=0; j<nChannels; j++){
			output[nChannels*i+j] = soundBuffer[(outCnt*NUM_IN_CHANNELS+(j%NUM_IN_CHANNELS))%BUF_LEN]*lfoVolume*overallVolume;
		}
		//output[2*i+0] = soundBuffer[outCnt%BUF_LEN];
		//output[2*i+1] = soundBuffer[outCnt%BUF_LEN];
		outCnt+=NUM_IN_CHANNELS;
	}
}

void GeometricScene::onSerialEvent(serialEventArgs &a){}
