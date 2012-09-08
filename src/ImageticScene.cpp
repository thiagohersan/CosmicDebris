#include "ImageticScene.h"

ImageticScene::ImageticScene(unsigned char* aVals_, unsigned char* dVals_, int* dVal_){
	// assign the pointers to the serial arrays
	analogVals = aVals_;
	digitalVals = dVals_;
	digitalVal = dVal_;
	// flicker state variables
    lastUpdate = 0.0;
	flickerPeriod = 100000;
	soundTime = 0;
	//
	turnOn = false;
	currLfoFreq = targetLfoFreq = overallVolume = 0.0;
	// load video
	myVideoPlayer.loadMovie("video0.mov");
	myVideoPlayer.setVolume(0);
	myVideoPlayer.setLoopState(OF_LOOP_NORMAL);
	myVideoPlayer.play();
    whichVideo = 0;
}

ImageticScene::~ImageticScene(){
	myVideoPlayer.close();
}

void ImageticScene::update(){
	/***** reading the values from the serial
	 for this scene:
	 analog[0] = flicker frequency, lfo frequency 
	 analog[1] = ...
	 analog[2] = which video
	 analog[3] = ...
	 analog[4] = ...
	 analog[5] = overall volume
	 digital[0] = ...
	 digital[1] = ...
	 digital[2] = ...
	 *****/
	
	// do updates on every frame.
	flickerPeriod = ofMap(analogVals[0], 40,250, 250, 20, true);
	targetLfoFreq = 1000*PI/flickerPeriod;
	// overall sound volume
	overallVolume = ofMap(analogVals[5], 40,250, 0.0,1.2, true);

    // which video to load
    unsigned int readVideo = ofMap(analogVals[2], 40,250, 0,20, true);
	if(readVideo != whichVideo){
		stringstream ss;
		ss << "video" << readVideo << ".mov";
        myVideoPlayer.stop();
        myVideoPlayer.loadMovie(ss.str());
        myVideoPlayer.setVolume(0);
        myVideoPlayer.setLoopState(OF_LOOP_NORMAL);
        myVideoPlayer.play();
		whichVideo = readVideo;
	}
    
	//
	myVideoPlayer.update();
}

// cheap inverting: glBlendFunc(GL_ONE_MINUS_SRC_COLOR, GL_ZERO);
void ImageticScene::draw(){
	if((soundTime-lastUpdate)*1000 > flickerPeriod){
		turnOn = !turnOn;
		lastUpdate = soundTime;
	}

	// draw video frame to screen
	//ofClear(0,0,0,255);
	ofBackground(turnOn?255:0);
	ofSetHexColor(0xFFFFFFFF);

	glEnable(GL_COLOR_LOGIC_OP);
	glLogicOp(GL_XOR);
	myVideoPlayer.draw(ofGetWidth()/2, ofGetHeight()/2);
	glDisable(GL_COLOR_LOGIC_OP);
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
		
		output[2*i+0] = sin(2*PI*60*soundTime)*lfoVolume*overallVolume;
		output[2*i+1] = sin(2*PI*60*soundTime)*lfoVolume*overallVolume;
	}
}

void ImageticScene::onSerialEvent(serialEventArgs &a){}


