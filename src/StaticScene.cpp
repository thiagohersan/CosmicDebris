#include "StaticScene.h"

StaticScene::StaticScene(unsigned char* aVals_, unsigned char* dVals_, int* dVal_){
	// assign the pointers to the serial arrays
	analogVals = aVals_;
	digitalVals = dVals_;
	digitalVal = dVal_;
	// flicker state variables
    lastUpdate = ofGetElapsedTimeMillis();
	bool turnOn = true;
	lastSize = 0x1;
	
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
}

void StaticScene::update(){
	/***** reading the values from the serial
	 for this scene:
	 analog[0] = frequency
	 analog[1] = ....
	 analog[2] = ....
	 analog[3] = noise/pixel size
	 analog[4] = ....
	 analog[5] = ....
	 digital[0,1,2] = what kind of static/flicker
	 *****/
	
	// do updates on every frame.
	flickerPeriod = ofMap(analogVals[0], 30,255, 250, 20, true);
	float sSize = ofMap(analogVals[3], 30,255, 0,maxLog2, true);
	// the digital value


	/* mStatic: what kind of static/flicker
	 0. just create new random image every period
	 1. random/inverse every period/2
	 */	
	unsigned char mStatic = ((*digitalVal)>>3)&0x07;
	

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
