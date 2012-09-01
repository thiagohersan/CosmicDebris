
#include "SimpleParticle.h"

SimpleParticle::SimpleParticle(){
	//ofPoint loc, vel, acc;	
	myColor = ofColor(0xFFFFFFFF);
	myRadius = 30;
	velVal = ofRandom(-2,2);
	
	//////
	pos.set(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
	vel.set(0,0);
	acc.set(0,0);
}

ofVec2f SimpleParticle::getPos() {
    return pos;
}

void SimpleParticle::setColor(ofColor c) {
    myColor = c;
}

bool SimpleParticle::isDead() {
    // if out of bounds... or speed = 0
    if ((pos.x-myRadius>ofGetWidth()) || (pos.x+myRadius<0) || (pos.y-myRadius>ofGetHeight()) || (pos.y+myRadius<0) || (vel.squareLength() < 0.5)) {
		return true;
    }
    return false;
}

