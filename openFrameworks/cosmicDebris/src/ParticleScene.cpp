#include "ParticleScene.h"

#define NUM_PARTS 1000
#define NUM_GROUPS 5

ParticleScene::ParticleScene(unsigned char* aVals_, unsigned char* dVals_, int* dVal_){
	// assign the pointers to the serial arrays
	analogVals = aVals_;
	digitalVals = dVals_;
	digitalVal = dVal_;
	// flicker state variables
	flickerPeriod = 100000;
	turnOn = true;
    lastUpdate = 0.0;
	soundTime = 0.0;
	// volume variables
	currLfoFreq = targetLfoFreq = overallVolume = 0.0;
	
	// numbers
	numParts = NUM_PARTS;
	numGroups = NUM_GROUPS;
	
	////////// init vectors
	// populate scene with particles
	for(int i=0; i<numParts; i++){
		myParts.push_back(SimpleParticle());
	}
	// pick random targets
	for(int i=0; i<numGroups; i++){
		myTargets.push_back(ofVec2f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight())));
	}
	// set random targets
	int ai = 0;
	for(vector<SimpleParticle>::iterator it = myParts.begin(); it<myParts.end(); ++it, ++ai){
		// set random group target
		(*it).setTarget(myTargets.at(ai/(numParts/numGroups)));
	}
	/////////
	pColor = ofColor(255,theColor.a);
	bColor = ofColor(0,255);
	
}

ParticleScene::~ParticleScene(){}

void ParticleScene::update(){
	/***** reading the values from the serial
	 for this scene:
	 analog[0] = flicker frequency, lfo frequency 
	 analog[1] = particle size
	 analog[2] = particle color
	 analog[3] = particle alpha
	 analog[4] = particle max velocity
	 analog[5] = overall volume
	 digital[0] = ...
	 digital[1,2] = which kind of flicker
	 *****/
	
	// do updates on every frame.
	flickerPeriod = ofMap(analogVals[0], 40,250, 250, 20, true);
	targetLfoFreq = 1000*PI/flickerPeriod;
	
	// size of particles
	float pSize = ofMap(analogVals[1], 40,250, 1,100, true);
	
	// color/hue control
	float theHue = ofMap(analogVals[2], 40,255, 0,255, true);
	// alpha control a := [1,255]
	float pAlpha = ofMap(analogVals[3], 40,250, 16,255, true);
	theColor = ofColor::fromHsb(theHue, 255, 155, pAlpha);

	// particle max velocity
	float pVel = ofMap(analogVals[4], 40,250, 1.0,5.0, true);

	// overall sound volume
	overallVolume = ofMap(analogVals[5], 40,250, 0.0,1.2, true);

	// what kind of flicker
	whichFlicker = ((*digitalVal)>>3)&0x03;

	// iterate over the particles, set target and size
	for(vector<SimpleParticle>::iterator it = myParts.begin(); it<myParts.end(); ++it){
		// check for dead ones...
		if((*it).isDead() == true){
			// if dead, reset their position, velocity and radius
			(*it).reset(pVel, pSize);
		}
		// update (every frame)
		(*it).update();
	}
}

void ParticleScene::draw(){
	// flicker here
	if((soundTime-lastUpdate)*1000 > flickerPeriod){
		// set colors... sigh...
		// default particle color (white with alpha)
		pColor = ofColor(255,theColor.a);
		// default background (white)
		bColor = ofColor(0,255);		
		switch (whichFlicker) {
			case FLICKER_NEITHER:{
				// flicker particles on/off (white/black)
				if(!turnOn){
					pColor = ofColor(0,255);
				}
			}
				break;				
			case FLICKER_PARTICLE:{
				// flicker particles on/off white/color
				if(!turnOn){
					pColor = theColor;
				}
			}
				break;
			case FLICKER_BGND:{
				// flicker bgnd on/off color/black
				if(turnOn){
					bColor = ofColor(theColor.r,theColor.g,theColor.b);
				}
			}
				break;
			case FLICKER_BOTH:{
				// flicker bgnd+particle on/off color+white/white+color
				if(turnOn){
					bColor = ofColor(theColor.r,theColor.g,theColor.b,255);
				}
				else{
					bColor = ofColor(255,255);
					pColor = theColor;
				}
			}
			default:
				break;
		}
		
		// update flicker variables
		lastUpdate = soundTime;
		turnOn = !turnOn;		
	}
	
	///////// draw every frame !
	ofBackground(bColor);
	// iterate over the particles, set color and draw
	for(vector<SimpleParticle>::iterator it = myParts.begin(); it<myParts.end(); ++it){
		(*it).draw(pColor);
	}	
}

void ParticleScene::audioOut( float * output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount ){
	// TODO: change this???
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

void ParticleScene::onSerialEvent(serialEventArgs &a){}


