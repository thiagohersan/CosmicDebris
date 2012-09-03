
#include "SimpleParticle.h"

SimpleParticle::SimpleParticle(){
	//ofPoint loc, vel, acc;	
	myColor = ofColor(0xFFFFFFFF);
	myRadius = 30;
	velVal = ofRandom(-2.0,2.0);

	//////
	pos.set(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
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
	velVal = ofRandom(-2.0,2.0);
	pos.set(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
	vel.set(0,0);
}
void SimpleParticle::reset(float vv, float r) {
	myRadius = r;
	velVal = ofRandom(-vv,vv);
	pos.set(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
	vel.set(0,0);
}


bool SimpleParticle::isDead() {
    // if out of bounds... or speed = 0
    if ((pos.x-myRadius>ofGetWidth()) || (pos.x+myRadius<0) || (pos.y-myRadius>ofGetHeight()) || (pos.y+myRadius<0) || (vel.squareLength() < 0.25)) {
		return true;
    }
    return false;
}

void SimpleParticle::setTarget(ofVec2f tv) {
	// unit vector towards target??
    vel = pos - tv;
    if (vel.squareLength() > 1){
		vel.normalize();
    }
    vel *= velVal;
}


void SimpleParticle::update() {
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


