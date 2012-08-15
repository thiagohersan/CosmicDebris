/*
 */

#include "Morphable.h"

Morphable::Morphable(){
	// initial conditions
	currSize = targetSize = 0.0;
	currType = targetType = TYPE_SQUARE;
	currState = STATE_STEADY;
	lastMorph = ofGetElapsedTimeMillis();
	// init vectors
	for(int i=0; i<NUM_OF_POINTS; i++){
		currPoints.push_back(ofPoint(0,0));
		targetPoints.push_back(ofPoint(0,0));
		currLeftBez.push_back(ofPoint(0,0));
		targetLeftBez.push_back(ofPoint(0,0));
		currRightBez.push_back(ofPoint(0,0));
		targetRightBez.push_back(ofPoint(0,0));
	}
	// force initial morph
	this->setType(TYPE_CIRCLE);
}
Morphable::~Morphable(){}

// for setting the type of the object
void Morphable::setType(int type){
	// check if new type is different from current type
	if((type%TYPE_SIZE) != currType){
		targetType = type%TYPE_SIZE;
		
		// most shapes don't use beziers, so clear them here and set them up later if needed
		for(int i=0; i<targetLeftBez.size(); i++){
			targetLeftBez.at(i).set(0,0);
			targetRightBez.at(i).set(0,0);
		}	
		
		// set up all the target vectors
		// TODO: fill this out !!!!
		switch (targetType) {
			case TYPE_SQUARE:
				// TODO: use for loop and radians
				targetPoints.at(0).set(-1,-1);
				targetPoints.at(1).set(-1/3,-1);
				targetPoints.at(2).set(1/3,-1);

				targetPoints.at(3).set(1,-1);
				targetPoints.at(4).set(1,-1/3);
				targetPoints.at(5).set(1,1/3);

				targetPoints.at(6).set(1,1);
				targetPoints.at(7).set(1/3,1);
				targetPoints.at(8).set(-1/3,1);

				targetPoints.at(9).set(-1,1);
				targetPoints.at(10).set(-1,1/3);
				targetPoints.at(11).set(-1,-1/3);
				break;
			default:
				// do nothing
				break;
		}
		currState = STATE_MORPHING;
	}
	
}

// set the target size in pixels
//    size:=[0,1],
//    currSize:=[50,400]
//    targetSize:=[50,400]
void Morphable::setSize(float size){
	// if the new size is different than the current size, go morph
	if(size != targetSize) {
		targetSize = ofMap(size, 0, 1, 30, 400);
		currState = STATE_MORPHING;
	}
}

// for debugging
int Morphable::getType(){
	return currType;
}
float Morphable::getSize(){
	return currSize;
}


// x,y are the center coordinates
// x,y,v are in pixels
void Morphable::draw(float x, float y){
	this->draw(x,y,0,ofColor(255,255,255));
}
void Morphable::draw(float x, float y, float v){
	this->draw(x,y,v,ofColor(255,255,255));
}
void Morphable::draw(float x, float y, ofColor c){
	this->draw(x,y,0,c);
}

// TODO: TEST THIS!!!
void Morphable::draw(float x, float y, float v, ofColor c){
	// take a step before drawing
	if(currState == STATE_MORPHING) {
		this->morphStep();
	}

	// reminder: currSize:=[50,400]
	//           v:=pixels
	ofPoint rp = ofPoint(ofRandom(-v, v), ofRandom(-v, v));
	float alpha = (currSize<100)?255.0:(ofMap(currSize,100,400,255,0));
	ofEnableAlphaBlending();
	ofSetColor(ofColor(c,alpha));
	ofFill();

	ofBeginShape();
	for(int i=0; i<currPoints.size(); i++){
		// bezier polygon. two vertex points and two control points
		ofPoint v0 = ofPoint(x,y) + currSize*currPoints.at((i+0)%currPoints.size());
		ofPoint v1 = ofPoint(x,y) + currSize*currPoints.at((i+1)%currPoints.size());
		// c0 is to the right of vertex i
		// c1 is to the left of vertex i+1
		ofPoint c0 = v0 + currSize*currRightBez.at((i+0)%currPoints.size());
		ofPoint c1 = v1 + currSize*currLeftBez.at((i+1)%currPoints.size());

		// add variation
		v0 += rp;
		v1 += rp;
		c0 += rp;
		c1 += rp;

		// finally
		ofVertex(v0);
		// pinche ofBezierCurve...
		ofBezierVertex(c0.x,c0.y, c1.x,c1.y, v1.x,v1.y);
	}
	ofEndShape();
	ofDisableAlphaBlending();
	
}

void Morphable::morphStep(){
	// figure out if it's time to step
	if((ofGetElapsedTimeMillis()-lastMorph) > MORPH_PERIOD){
		// size morph
		//   if target and curr differ by more than 1 pixel, keep morphing
		bool sameSize = true;
		if(fabs(targetSize-currSize) > 1.0){
			currSize = 0.9*currSize + 0.1*targetSize;
			sameSize = false;
		}
		
		// type morph
		//   if the target points and curr points aren't the same, keep morphing
		if(targetType != currType){
			bool samePoints = true;
			for(int i=0; i<currPoints.size(); i++){
				// set current points to be 95% current values and 5% target values
				ofPoint cp = currPoints.at(i);
				ofPoint tp = targetPoints.at(i);
				currPoints.at(i).set(0.95*cp + 0.05*tp);
				// check if these 2 points are the same
				samePoints = samePoints && (tp.distance(currPoints.at(i)) < 0.01);
			}
			
			// if all points are the same, then we're done morphing type
			if(samePoints == true){
				currType = targetType;
			}
		}

		// if done morphing type and size, we're done morphing
		if((currType == targetType) && (sameSize == true)){
			currState = STATE_STEADY;
		}

		// update lastMorph step time
		lastMorph = ofGetElapsedTimeMillis();
	}
}