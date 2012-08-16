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
		// TODO: {TYPE_CIRCLE, TYPE_KAHANE, TYPE_UNO};
		switch (targetType) {
			case TYPE_CIRCLE: {
				// TODO: add target beziers
				for(int i=0; i<targetPoints.size(); i++){
					float angle = ofDegToRad(15+i*30);
					float px = ofClamp(cos(angle), -1, 1);
					float py = ofClamp(sin(angle), -1, 1);
					targetPoints.at(i).set(px,py);
				}
			}
				break;
			case TYPE_SQUARE: {
				float sqrt2 = sqrt(2);
				for(int i=0; i<targetPoints.size(); i++){
					float angle = ofDegToRad(15+i*30);
					float px = ofClamp(sqrt2*cos(angle), -1, 1);
					float py = ofClamp(sqrt2*sin(angle), -1, 1);
					targetPoints.at(i).set(px,py);
				}
			}
				break;
			case TYPE_HOR_RECT: {
				float sqrt32 = sqrt(3)/2.0;
				for(int i=0; i<targetPoints.size(); i++){
					float angle = ofDegToRad(i*30);
					float px = ofClamp(cos(angle), -sqrt32, sqrt32);
					float py = ofClamp(sin(angle), -0.5, 0.5);
					targetPoints.at(i).set(px,py);
				}
			}
				break;
			case TYPE_VER_RECT: {
				float sqrt32 = sqrt(3)/2.0;
				for(int i=0; i<targetPoints.size(); i++){
					float angle = ofDegToRad(i*30);
					float px = ofClamp(cos(angle), -0.5, 0.5);
					float py = ofClamp(sin(angle), -sqrt32, sqrt32);
					targetPoints.at(i).set(px,py);
				}
			}
				break;
			case TYPE_CROSS: {
				// for irregular angle stepping
				//   want to hit 30,45,60,120,135,150,...
				float angle = ofDegToRad(30);
				float sqrt24 = sqrt(2)/4.0;
				float twosqrt3 = 2.0/sqrt(3);
				for(int i=0; i<targetPoints.size(); i++){
					// length of cross arm = 1.0
					//    width of cross arm = 2/sqrt(3)
					float px = twosqrt3*cos(angle);
					float py = twosqrt3*sin(angle);

					// clamp shortest side, or when sides are equal
					if(fabs(px) < 0.99){
						px = ofClamp(px, -sqrt24, sqrt24);
					}
					if(fabs(py) < 0.99){
						py = ofClamp(py, -sqrt24, sqrt24);
					}

					// angle increments are: 15,15,60,15,15,60,...
					angle += ofDegToRad(((i%3)/2)*45+15);

					targetPoints.at(i).set(px,py);
				}
			}
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
		// pinche ofBezierCurve... doesn't accept ofPoints
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
			// move vertex points
			for(int i=0; i<currPoints.size(); i++){
				// set current points to be 95% current values and 5% target values
				ofPoint cp = currPoints.at(i);
				ofPoint tp = targetPoints.at(i);
				currPoints.at(i).set(0.95*cp + 0.05*tp);
				// check if the new point is the same as the target
				samePoints = samePoints && (tp.distance(currPoints.at(i)) < 0.01);
			}
			// move bezier points
			//   assume currLeftBez.size() == currRightBez.size()
			for(int i=0; (i<currLeftBez.size())&&(i<currRightBez.size()); i++){
				// set current points to be 95% current values and 5% target values
				ofPoint clb = currLeftBez.at(i);
				ofPoint tlb = targetLeftBez.at(i);
				currLeftBez.at(i).set(0.95*clb + 0.05*tlb);
				ofPoint crb = currRightBez.at(i);
				ofPoint trb = targetRightBez.at(i);
				currRightBez.at(i).set(0.95*crb + 0.05*trb);
				// check if the new points are the same as the targets
				samePoints = samePoints && (tlb.distance(currLeftBez.at(i)) < 0.01) && (trb.distance(currRightBez.at(i)) < 0.01);
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