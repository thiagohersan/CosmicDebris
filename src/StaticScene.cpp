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
	 analog[0] = flicker frequency
	 analog[1] = sound frequency
	 analog[2] = ....
	 analog[3] = noise/pixel size
	 analog[4] = ....
	 analog[5] = ....
	 digital[0] = what kind of static/flicker
	 digital[1,2] = what kind of sound
	 *****/
	
	// do updates on every frame.
	flickerPeriod = ofMap(analogVals[0], 30,255, 250, 20, true);
	targetLfoFreq = 2000*PI/flickerPeriod;
	
	// sound frequency control f := [60,200]
	targetFreq = 2*PI*ofMap(analogVals[1], 30,255, 60,200, true);
	
	// size of pixel groups in image
	float sSize = ofMap(analogVals[3], 30,255, 0,maxLog2, true);
	
	// the digital values
	/* mStatic: what kind of static/flicker
	 0. just create new random image every period
	 1. inverse every period
	 */
	unsigned char mStatic = ((*digitalVal)>>5)&0x01;
	
	// more digital values
	/* picks the kind of noise
	 0. random noise
	 1. pure sine wave
	 2. pre-recorded jazz
	 */
	whichSound = ((*digitalVal)>>3)&0x03;

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
		
		printf("switch!! currTime = %f, vol = %f\n", soundTime, 0.5*(sin(currLfoFreq*soundTime)+1.0));
		
		// puta... que confusão
		switch (mStatic) {
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

void StaticScene::audioRequested(float * output, int bufferSize, int nChannels){	
	for(int i=0; i<bufferSize; i++){
		// update soundTime variable (this is what keeps track of total time)
		soundTime += 1.0/48000;
		
		// very unstable
		//currLfoFreq = ofLerp(currLfoFreq, targetLfoFreq, 1.0/48000);
		
		// unstable enough for awesomeness
		currLfoFreq = ofLerp(currLfoFreq, targetLfoFreq, 0.001);

		float currVol = 0.5*(sin(currLfoFreq*soundTime)+1.0);

		switch (whichSound) {
			case SOUND_NOISE:{
				output[2*i+0] = ofRandom(-1,1)*currVol;
				output[2*i+1] = ofRandom(-1,1)*currVol;
			}
				break;
			case SOUND_SINE:{
				output[2*i+0] = sin(currFreq*soundTime)*currVol;
				output[2*i+1] = sin(currFreq*soundTime)*currVol;
			}
			default:
				break;
		}

		// to avoid clicks, only adjust stuff when volume is low
		if(currVol < 0.01){
			currFreq = currFreq*0.9 + targetFreq*0.1;
			if (soundTime > 2*PI){
				//soundTime -= 2*PI;
			}
		}
	}

}

