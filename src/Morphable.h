/*
 *
 */

#ifndef MORPHABLE_H
#define MORPHABLE_H

#include "Geometricable.h"

// number of points in a shape
#define NUM_OF_POINTS 12
// period for morphing between shapes
#define MORPH_PERIOD 10
// math constants
//#define SQRT2 1.41421356237

class Morphable: public Geometricable {
	
public:
	Morphable();
	~Morphable();
	void draw(float x, float y);
	void draw(float x, float y, float v);
	void draw(float x, float y, ofColor c);
	void draw(float x, float y, float v, ofColor c);
	void setType(int type);
	void setSize(float size);
	// testing
	int getType();
	float getSize();
protected:
	enum TypeEnum {TYPE_CIRCLE, TYPE_SQUARE, TYPE_HOR_RECT, TYPE_VER_RECT, TYPE_CROSS, TYPE_KAHANE, TYPE_UNO, TYPE_SIZE};
	enum StateEnum {STATE_STEADY, STATE_MORPHING};
	float currSize, targetSize;
	int currType, targetType;
	int currState;
	int lastMorph;
	
	vector<ofPoint> currPoints, targetPoints;
	vector<ofPoint> currLeftBez, targetLeftBez, currRightBez, targetRightBez;
	
	void morphStep();
};

#endif

