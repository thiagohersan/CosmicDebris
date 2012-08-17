#include "Canvas.h"

Canvas::Canvas(){
	digitalVal = 0;
	currScene = nextScene = SCENE_STATIC;
	theScene = new StaticScene(analogVals, digitalVals, &digitalVal);
	currState = STATE_FADING;
	fadeAlpha = 255;
}

Canvas::~Canvas(){
	delete theScene;
}

// test this !!
void Canvas::update(){
	// deal with states
	if(currState == STATE_FADING) {
		// update fade alpha
		fadeAlpha += (fabs(fadeAlpha)/fadeAlpha)*FADE_STEP;
		
		// if we get to the max, change scenes
		if(fabs(fadeAlpha) >= 255) {
			// do some memory management
			delete theScene;
			
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
					//theScene = new ImageticScene(analogVals, digitalVals, &digitalVal);
					theScene = new StaticScene(analogVals, digitalVals, &digitalVal);
				}
					break;
				default:{
					theScene = new StaticScene(analogVals, digitalVals, &digitalVal);
				}
					break;
			}
			
			// update current scene
			currScene = nextScene;
			
			// switch directions
			fadeAlpha = -255.0;
		}
		// if we get to the min
		else if(fabs(fadeAlpha) <= 0) {
			// done fading
			fadeAlpha = 0;
			currState = STATE_STEADY;
		}
	}
	else if(currScene == STATE_STEADY){
		// do nothing for now
	}
	
	// dealt with states, now deal with serial numbers
	// non-blocking. can change scenes while fading
	// TODO: test this !!!
	
	/***** grab scene number from lowest 3 bits of digitalVal ****/
	unsigned char sceneFromVal = (digitalVal&0x07);
	
	// if val from serial not equal to next state, we have to trigger a change
	if(sceneFromVal != (nextScene&0x07)){
		nextScene = sceneFromVal;
		currScene = STATE_FADING;
	}
	
	theScene->update();
}

// TODO: Test this!!
void Canvas::draw(){
	// pretty much always draw the scene
	theScene->draw();
	// draw a fade rectangle
	ofEnableAlphaBlending();
	ofSetColor(ofColor(0,0,0,fabs(fadeAlpha)));
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
