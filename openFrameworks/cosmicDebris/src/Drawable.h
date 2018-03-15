/*
 * Drawable shapes
 */

#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "Geometricable.h"

class Drawable: public Geometricable {
	
public:
	Drawable();
	~Drawable();
	void draw(float x, float y);
	void draw(float x, float y, float v);
	void setType(int type);
	void setSize(float size);
	void setColor(ofColor color);
	int getType();
protected:
	enum TypeEnum {TYPE_CIRCLE, TYPE_SQUARE, TYPE_HOR_RECT, TYPE_VER_RECT, TYPE_CROSS, TYPE_KAHANE, TYPE_UNO, TYPE_SIZE};
	float mySize;
	int myType;
	ofColor myColor;
};

#endif

