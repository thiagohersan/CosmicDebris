
#ifndef SIMPLEPARTICLE_H
#define SIMPLEPARTICLE_H

#include "ofMain.h"

class SimpleParticle {
	
public:
	SimpleParticle();
	~SimpleParticle();

	ofVec2f getPos();
	void setColor(ofColor c);
	void setAlpha(float a);
	void setRadius(float r);
	void setVel(float vv);
	bool isDead();
	void setTarget(ofVec2f tv);
	
	void update();
	void draw();

private:
	ofVec2f pos, vel, acc;	
	ofColor myColor;
	float myRadius;
	float velVal;
};

#endif
