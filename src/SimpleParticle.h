
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
	void setTarget(ofVec2f tv);
	void resetPos();
	void reset(float vv, float r);
	bool isDead();

	void update();
	void draw();
	void draw(ofColor c);
private:
	ofVec2f pos, target, vel;	
	ofColor myColor;
	float myRadius;
	float velVal;
};

#endif
