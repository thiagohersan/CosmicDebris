#include "Canvas.h"

Canvas::Canvas(ofBaseApp * that){
	digitalVal = 0;
	currScene = nextScene = SCENE_STATIC;
	theScene = new StaticScene(analogVals, digitalVals, &digitalVal);
	currState = STATE_FADING;
	fadeAlpha = 255;
	
	if(that != NULL){
		// DEBUG
		mySoundStream.listDevices();
		mySoundStream.setup(that, 2, 0, 48000, 512, 2);
		mySoundStream.setOutput(theScene);
	}
}

Canvas::~Canvas(){
	delete theScene;
}

void Canvas::update(){
	// deal with states
	if(currState == STATE_FADING) {
		// update fade alpha
		fadeAlpha += FADE_STEP;
		
		// if we get to the max, stay on black for a while
		if(fabs(fadeAlpha) >= 255) {
			// wait on black for a while
			currState = STATE_BLACK;
		}
		// if we get to the min
		else if(fabs(fadeAlpha) <= 1) {
			// done fading
			fadeAlpha = 1;
			currState = STATE_STEADY;
		}
	}
	else if(currState == STATE_STEADY){
		// do nothing for now
	}
	else if(currState == STATE_BLACK){
		// update fade alpha
		fadeAlpha += FADE_STEP;
		
		// if we're done waiting here, change scenes
		if(fabs(fadeAlpha) > 500){
			// do some memory management
			delete theScene;
			// clear background to erase previous scene
			ofBackground(0);
			// stop audio
			mySoundStream.stop();

			// pick new scene
			switch (nextScene) {
				case SCENE_STATIC:{
					theScene = new StaticScene(analogVals, digitalVals, &digitalVal);
				}
					break;
				case SCENE_GEOMETRY:{
					theScene = new GeometricScene(analogVals, digitalVals, &digitalVal);
				}
					break;
				case SCENE_IMAGE:{
					theScene = new ImageticScene(analogVals, digitalVals, &digitalVal);
					//theScene = new StaticScene(analogVals, digitalVals, &digitalVal);
				}
					break;
				default:{
					theScene = new StaticScene(analogVals, digitalVals, &digitalVal);
				}
					break;
			}

			// restart audio
			mySoundStream.setOutput(theScene);
			mySoundStream.start();

			// update current scene
			currScene = nextScene;

			// switch directions and go back to fade in
			fadeAlpha = -254.0;
			currState = STATE_FADING;			
		}
	}
	
	
	// dealt with states, now deal with serial numbers
	// non-blocking. can change scenes while fading
	// TODO: fix this!! we lose scene when we change during a fade-in
	
	/***** grab scene number from lowest 3 bits of digitalVal ****/
	unsigned char sceneFromVal = (digitalVal&0x07);
	
	// if val from serial not equal to next state, we have to trigger a change
	if(sceneFromVal != (nextScene&0x07)){
		nextScene = sceneFromVal;
		currState = STATE_FADING;
	}
	
	theScene->update();
}

void Canvas::draw(){
	// pretty much always draw the scene
	theScene->draw();
	// draw a fade rectangle
	ofSetColor(ofColor(0,0,0,ofClamp(fabs(fadeAlpha),0,255)));
	ofFill();
	// kind of lazy. using globals. meh.
	ofRect(ofGetWidth()/2, ofGetHeight()/2, ofGetWidth(), ofGetHeight());
}


/**/
void Canvas::onSerialEvent(serialEventArgs &a){
	unsigned char type_ = a.type_;
	unsigned char id_ = a.id_;
	unsigned char value_ = a.value_;
	
	if(type_ == 'A'){
		analogVals[id_%NUM_ANALOG] = value_;
		printf("Canvas: Setting analog[%d] to %d\n", (int)(id_%NUM_ANALOG), value_);
	}
	else if(type_ == 'D'){
		digitalVals[id_%NUM_DIGITAL] = (value_==0xff);
		printf("Canvas: Seeting digital[%d] to %d\n", (int)(id_%NUM_DIGITAL), (value_==0xff));
		// set id_-th bit of digitalVal based on value_
		//   on box, the buttons are numbered: [0,1,...,5]
		//   but here I want them to read as bits on a number, so I have to flip their order with NUM_DIGITAL-1-id_
		//   leet shit
		unsigned char mask0 =   ((value_==0xff)&0x1)<<(NUM_DIGITAL-1-id_);    // all zeros and v
		unsigned char mask1 = ~(((value_!=0xff)&0x1)<<(NUM_DIGITAL-1-id_));   // all ones and v
		
		digitalVal = (mask0|digitalVal)&mask1;
		printf("Canvas: digital val = %d\n", digitalVal);
	}
}
/**/
