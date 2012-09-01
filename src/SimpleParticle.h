
#ifndef SIMPLEPARTICLE_H
#define SIMPLEPARTICLE_H

#include "ofMain.h"

class SimpleParticle {
	
public:
	SimpleParticle();
	~SimpleParticle();

	ofVec2f getPos();
	void setColor(ofColor c);
	void setRadius(float r);
	void setColorAndRadius(ofColor c, float r);
	void setVel(float vv);
	bool isDead();
	void setTarget(ofVec2f tv);
	
	void update();
	void draw();

private:
	ofVec2f pos, vel;	
	ofColor myColor;
	float myRadius;
	float velVal;
};

#endif
