#include "StaticScene.h"

StaticScene::StaticScene(unsigned char* aVals_, unsigned char* dVals_, int* dVal_){
	// assign the pointers to the serial arrays
	analogVals = aVals_;
	digitalVals = dVals_;
	digitalVal = dVal_;
	// flicker state variables
    lastUpdate = ofGetElapsedTimeMillis();
	turnOn = true;
	lastSize = 0x1;
	
	// volume
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
	 analog[1] = ....
	 analog[2] = ....
	 analog[3] = noise/pixel size
	 analog[4] = ....
	 analog[5] = ....
	 digital[0] = 
	 digital[1,2] = what kind of static/flicker
	 *****/

	// do updates on every frame.
	flickerPeriod = ofMap(analogVals[0], 30,255, 250, 20, true);
	targetLfoFreq = 2000*PI/flickerPeriod;

	// TODO: add sound frequency control ??

	float sSize = ofMap(analogVals[3], 30,255, 0,maxLog2, true);

	// the digital values
	/* mStatic: what kind of static/flicker
	 0. just create new random image every period
	 1. inverse every period
	 */	
	unsigned char mStatic = ((*digitalVal)>>3)&0x03;
	// TODO: finish this !!
	// noise/sine = ((*digitalVal)>>5)&0x01;
	
	// time to update !!
	if((ofGetElapsedTimeMillis() - lastUpdate) > flickerPeriod){
		// static-y image. leet 213 shit
		unsigned char* myPixels = myImage.getPixels();
		int maxI = myImage.getHeight()*myImage.getWidth();
		int iW = myImage.getWidth();
		int iH = myImage.getHeight();
		// pixel per square
		int pps = 0x1<<(int)(sSize);
		
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
		lastUpdate = ofGetElapsedTimeMillis();
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
	// 2*pi*f
	float fr  = 2*PI*200;
	
	for(int i=0; i<bufferSize; i++){
		// update soundTime variable (this is what keeps track of total time)
		soundTime += 1.0/48000;

		// very unstable
		//currLfoFreq = ofLerp(currLfoFreq, targetLfoFreq, 1.0/48000);
		// unstable enough for awesomeness
		currLfoFreq = ofLerp(currLfoFreq, targetLfoFreq, 0.001);
		float currVol = 0.5*sin(currLfoFreq*soundTime)+1.0;
		
		//output[2*i+0] = ofRandom(1)*currVol;
		//output[2*i+1] = ofRandom(1)*currVol;

		output[2*i+0] = sin(fr*soundTime)*currVol;
		output[2*i+1] = sin(fr*soundTime)*currVol;

		// to avoid clicks, only adjust time if volume is low
		if((soundTime > 2*PI) && (currVol < 0.05)){
			soundTime -= 2*PI;
		}
	}

}

