/*
 * interface for drawable geometric shape objects
 */

#ifndef GEOMETRICABLE_H
#define GEOMETRICABLE_H

#include "ofMain.h"

class Geometricable {
	
public:
	virtual ~Geometricable() {}
	virtual void setType(int type) = 0;
	virtual void setSize(float size) = 0;
	virtual void draw(float x, float y, float v, ofColor c) = 0;
};

#endif
