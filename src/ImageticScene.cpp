#include "ImageticScene.h"

ImageticScene::ImageticScene(unsigned char* aVals_, unsigned char* dVals_, int* dVal_){
	// assign the pointers to the serial arrays
	analogVals = aVals_;
	digitalVals = dVals_;
	digitalVal = dVal_;
	// flicker state variables
    lastUpdate = 0.0;
	flickerPeriod = 100000;
	//
	currLfoFreq = targetLfoFreq = overallVolume = 0.0;
	// TODO: open video
	
}

ImageticScene::~ImageticScene(){}

void ImageticScene::update(){
	/***** reading the values from the serial
	 for this scene:
	 analog[0] = flicker frequency, lfo frequency 
	 analog[1] = ...
	 analog[2] = ...
	 analog[3] = ...
	 analog[4] = ...
	 analog[5] = overall volume
	 digital[0] = ...
	 digital[1] = ...
	 digital[2] = ...
	 *****/
	
	// do updates on every frame.
	flickerPeriod = ofMap(analogVals[0], 40,250, 250, 20, true);
	targetLfoFreq = 2000*PI/flickerPeriod;
	// overall sound volume
	overallVolume = ofMap(analogVals[5], 40,250, 0.0,1.2, true);
}

void ImageticScene::draw(){
	// TODO: draw video frame to screen
}

// TODO: fill this out
void ImageticScene::audioOut( float * output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount ){
	for(int i=0; i<bufferSize; i++){
		// update soundTime variable (this is what keeps track of total time)
		soundTime += 1.0/48000;
		
		// very unstable
		//currLfoFreq = ofLerp(currLfoFreq, targetLfoFreq, 1.0/48000);
		
		// unstable enough for awesomeness
		currLfoFreq = ofLerp(currLfoFreq, targetLfoFreq, 0.001);
		
		float lfoVolume = 0.5*(sin(currLfoFreq*soundTime)+1.0);
		
		//output[2*i+0] = soundBuffer[2*i+0]*lfoVolume*overallVolume;
		//output[2*i+1] = soundBuffer[2*i+1]*lfoVolume*overallVolume;
	}
}

void ImageticScene::onSerialEvent(serialEventArgs &a){}


