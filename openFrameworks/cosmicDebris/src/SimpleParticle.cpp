
#include "SimpleParticle.h"

SimpleParticle::SimpleParticle(){
	//ofPoint loc, vel, acc;	
	myColor = ofColor(0xFFFFFFFF);
	myRadius = 30;
	velVal = ofRandom(1.0,2.0)*(int(ofRandom(2.0))*2-1);

	//////
	pos.set(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
	target.set(-1,-1);
	vel.set(0,0);
}
SimpleParticle::~SimpleParticle(){}


ofVec2f SimpleParticle::getPos() {
    return pos;
}

// setters
void SimpleParticle::setColor(ofColor c) {
    myColor = c;
}
void SimpleParticle::setRadius(float r) {
	myRadius = r;
}
void SimpleParticle::resetPos() {
	velVal = velVal = ofRandom(1.0,2.0)*(int(ofRandom(2.0))*2-1);
	pos.set(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
	vel.set(0,0);
}
void SimpleParticle::reset(float vv, float r) {
	myRadius = r;
	velVal = ofRandom(1.0,vv)*(int(ofRandom(2.0))*2-1);
	pos.set(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
	vel.set(0,0);
}


bool SimpleParticle::isDead() {
	// if out of bounds.
    if ((pos.x-myRadius>ofGetWidth()) || (pos.x+myRadius<0) || (pos.y-myRadius>ofGetHeight()) || (pos.y+myRadius<0)){
		return true;
    }
	// or near target
	if(pos.squareDistance(target) < vel.squareLength()){
		return true;
	}
    return false;
}

void SimpleParticle::setTarget(ofVec2f tv) {
	target.set(tv);
}

void SimpleParticle::update() {
	// calculate velocity as a unit vector towards target
    vel = pos - target;
    if (vel.squareLength() > 1){
		vel.normalize();
		vel *= velVal;
    }
	
	// update position
	pos += vel;
}

void SimpleParticle::draw() {
	ofSetColor(myColor);
	ofFill();
	ofCircle(pos, myRadius);
	ofNoFill();
	ofCircle(pos, myRadius);
}
void SimpleParticle::draw(ofColor c) {
    myColor = c;
	ofSetColor(myColor);
	ofFill();
	ofCircle(pos, myRadius);
	ofNoFill();
	ofCircle(pos, myRadius);
}
