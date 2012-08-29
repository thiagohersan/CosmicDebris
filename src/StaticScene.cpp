#include "StaticScene.h"

StaticScene::StaticScene(unsigned char* aVals_, unsigned char* dVals_, int* dVal_){
	// assign the pointers to the serial arrays
	analogVals = aVals_;
	digitalVals = dVals_;
	digitalVal = dVal_;
	// flicker state variables
	turnOn = true;
	lastUpdate = 0.0;
	// size
	lastSize = 0x1;
	
	// sound
	soundTime = 0.0;
	currLfoFreq = targetLfoFreq = 0.0;
	currFreq = targetFreq = 0.0;
	
	//
	overallVolume = sineVolume = 0.0;

	// image to draw static noise pixels
	myImage.allocate(ofGetWidth(),ofGetHeight(), OF_IMAGE_GRAYSCALE);
	maxLog2 = log2(max(myImage.getHeight(),myImage.getWidth()));
	
	// initial image
	// static-y image. leet 213 shit
	unsigned char* myPixels = myImage.getPixels();
	int maxI = myImage.getWidth() * myImage.getHeight();
	int iW = myImage.getWidth();
	int iH = myImage.getHeight();
	// pixel per square
	int pps = 0x1;
	for(int i=0; i<iH; i+=pps){
		int r = i*iW;
		for(int j=0; j<iW; j+=pps){
			int v = ((int)ofRandom(2))*255;
			int rj = r+j;
			for(int k=0; k<pps; k++){
				int rr = k*iW+rj;
				for(int l=0;l<pps;l++){
					if((rr+l) < maxI){
						myPixels[rr+l] = v;
					}
				}
			}
		}
	}
	myImage.update();
}

StaticScene::~StaticScene(){
	// close audio ?
}

void StaticScene::update(){
	/***** reading the values from the serial
	 for this scene:
	 analog[0] = flicker frequency, lfo frequency 
	 analog[1] = pixel size
	 analog[2] = which sample to play
	 analog[3] = sound frequency
	 analog[4] = sine/noise  volume
	 analog[5] = overall volume
	 digital[0] = what kind of static/flicker
	 digital[1] = what kind of sound noise/sine
	 digital[2] = trigger pre-recorded sound
	 *****/

	// do updates on every frame.
	flickerPeriod = ofMap(analogVals[0], 30,255, 250, 20, true);
	targetLfoFreq = 2000*PI/flickerPeriod;
	
	// size of pixel groups in image
	float sSize = ofMap(analogVals[1], 30,255, 0,maxLog2, true);

	// sound frequency control f := [60,200]
	unsigned int readSample = ofMap(analogVals[2], 30,255, 0,20, true);
	if(readSample != whichSample){
		stringstream ss;
		ss << "sound" << readSample << ".mp3"; 
		mySoundPlayer.loadSound(ss.str());
		mySoundPlayer.setMultiPlay(true);
		whichSample = readSample;
	}

	// sound frequency control f := [60,200]
	targetFreq = 2*PI*ofMap(analogVals[3], 30,255, 60,200, true);

	// volume control for sine/noise
	sineVolume = ofMap(analogVals[4], 40,255, 0.0,1.0, true);
	
	// overall sound volume
	overallVolume = ofMap(analogVals[5], 40,255, 0.0,1.0, true);
	
	/* whichStatic: what kind of static/flicker
	 0. just create new random image every period
	 1. inverse every period
	 */
	whichStatic = ((*digitalVal)>>5)&0x01;
	
	/* picks the kind of noise
	 0. random noise
	 1. pure sine wave
	 */
	whichSound = ((*digitalVal)>>4)&0x01;
	
	/*
	 trigger samples
	 */
	unsigned char triggerVal = ((*digitalVal)>>3)&0x01;
	if(triggerVal != lastTriggerVal){
		lastTriggerVal = triggerVal;
		if(triggerVal == 0x1){
			mySoundPlayer.play();
		}
	}


	// time to update !!
	// use the sound time (in seconds) to control flicker update
	// this might be a problem if sound stream goes out...
	//if((ofGetElapsedTimeMillis() - lastUpdate) > flickerPeriod){
	if((soundTime - lastUpdate)*1000 > flickerPeriod){
		// static-y image. leet 213 shit
		unsigned char* myPixels = myImage.getPixels();
		int maxI = myImage.getHeight()*myImage.getWidth();
		int iW = myImage.getWidth();
		int iH = myImage.getHeight();
		// pixel per square
		int pps = 0x1<<(int)(sSize);
		
		// puta... que confusão
		switch (whichStatic) {
				// new random image every period
			case STATIC_RANDOM:{
				for(int i=0; i<iH; i+=pps){
					int r = i*iW;
					for(int j=0; j<iW; j+=pps){
						int v = ((int)ofRandom(2))*255;
						int rj = r+j;
						for(int k=0; k<pps; k++){
							int rr = k*iW+rj;
							for(int l=0;l<pps;l++){
								if((rr+l) < maxI){
									myPixels[rr+l] = v;
								}
							}
						}
					}
				}
			}
				break;
			case STATIC_INVERSE:{
				for(int i=0; i<iH; i+=pps){
					int r = i*iW;
					for(int j=0; j<iW; j+=pps){
						int v = ((int)ofRandom(2))*255;
						int rj = r+j;
						for(int k=0; k<pps; k++){
							int rr = k*iW+rj;
							for(int l=0;l<pps;l++){
								if((rr+l) < maxI){
									myPixels[rr+l] = (sSize != lastSize)?v:~myPixels[rr+l];
								}
							}
						}
					}
				}
			}
				break;
			default:
				break;
		}
		
		myImage.update();
		//lastUpdate = ofGetElapsedTimeMillis();
		// soundTime is in seconds
		lastUpdate = soundTime;
		lastSize = sSize;
		turnOn = !turnOn;
	}
	
	
	
}

void StaticScene::draw(){
	// always draw
	ofSetColor(255,255,255,255);
	// stupid setrectmode bullshit
	myImage.draw(ofGetWidth()/2,ofGetHeight()/2);
}

/**/
void StaticScene::onSerialEvent(serialEventArgs &a){
}
/**/

// audi-oh
void StaticScene::audioOut( float * output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount ){
	for(int i=0; i<bufferSize; i++){
		// update soundTime variable (this is what keeps track of total time)
		soundTime += 1.0/48000;
		
		// very unstable
		//currLfoFreq = ofLerp(currLfoFreq, targetLfoFreq, 1.0/48000);
		
		// unstable enough for awesomeness
		currLfoFreq = ofLerp(currLfoFreq, targetLfoFreq, 0.001);

		float lfoVolume = 0.5*(sin(currLfoFreq*soundTime)+1.0);

		// set volume on sampler
		mySoundPlayer.setVolume(lfoVolume*overallVolume);
		
		switch (whichSound) {
			case SOUND_NOISE:{
				float samp = ofRandom(-1,1);
				if(whichStatic == STATIC_INVERSE){
					samp *= lfoVolume;
				}
				
				output[2*i+0] = samp*0.8*sineVolume*overallVolume;
				output[2*i+1] = samp*0.8*sineVolume*overallVolume;
			}
				break;
			case SOUND_SINE:{
				output[2*i+0] = sin(currFreq*soundTime)*lfoVolume*sineVolume*overallVolume;
				output[2*i+1] = sin(currFreq*soundTime)*lfoVolume*sineVolume*overallVolume;
			}
			default:
				break;
		}
		
		// to avoid clicks, only adjust stuff when volume is low
		if((lfoVolume*overallVolume) < 0.01){
			currFreq = currFreq*0.9 + targetFreq*0.1;
			if (soundTime > 2*PI){
				//soundTime -= 2*PI;
			}
		}
	}
	
}

