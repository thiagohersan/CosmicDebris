
#ifndef SIMPLEPARTICLE_H
#define SIMPLEPARTICLE_H

#include "ofMain.h"

class SimpleParticle {
	
public:
	SimpleParticle();
	~SimpleParticle();

	ofVec2f getPos();
	void setColor(ofColor c);
	bool isDead();
private:
	ofVec2f pos, vel, acc;	
	ofColor myColor;
	float myRadius;
	float velVal;
};

#endif
