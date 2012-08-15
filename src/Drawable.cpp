/*
 *  Drawable.cpp
 *  cosmicDebrisOF
 *
 *  Created by admin on 8/15/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "Drawable.h"

Drawable::Drawable(){
	mySize = 0.0;
	myType = TYPE_CIRCLE;
}
Drawable::~Drawable(){
}

// for setting the type of the object
void Drawable::setType(int type){
	myType = type%TYPE_SIZE;
}

// set the size (relative to [0,1])
void Drawable::setSize(float size){
	mySize = size;
}
// for debugging
int Drawable::getType(){
	return myType;
}

// x,y are the center coordinates
// x,y,v are in pixels
void Drawable::draw(float x, float y){
	this->draw(x,y,0,ofColor(255,255,255));
}
void Drawable::draw(float x, float y, float v){
	this->draw(x,y,v,ofColor(255,255,255));
}
void Drawable::draw(float x, float y, ofColor c){
	this->draw(x,y,0,c);
}
void Drawable::draw(float x, float y, float v, ofColor c){
	float rad = 0;
	int alpha = 255;
	switch (myType) {
		case TYPE_CIRCLE:
			// turn into pixels
			rad = ofMap(mySize, 0,1, 20,400);
			alpha = (mySize<0.25)?255:(int)(ofMap(mySize,0.25,1,255,0));
			ofEnableAlphaBlending();
			ofSetColor(ofColor(c,alpha));
			ofFill();
			ofCircle(x+ofRandom(-v,v), y+ofRandom(-v,v), rad);
			ofDisableAlphaBlending();
			break;
		case TYPE_SQUARE:
			// turn into pixels
			rad = ofMap(mySize, 0,1, 25,200);
			alpha = (mySize<0.3)?255:(int)(ofMap(mySize,0.3,1,255,0));
			ofEnableAlphaBlending();
			ofSetColor(ofColor(c,alpha));
			ofFill();
			ofRect(x-rad+ofRandom(-v,v), y-rad+ofRandom(-v,v), 2*rad, 2*rad);
			ofDisableAlphaBlending();
			break;
		case TYPE_HOR_RECT:
			// turn into pixels
			rad = ofMap(mySize, 0,1, 50,400);
			alpha = (mySize<0.3)?255:(int)(ofMap(mySize,0.3,1,255,0));
			ofEnableAlphaBlending();
			ofSetColor(ofColor(c,alpha));
			ofFill();
			ofRect(x-rad+ofRandom(-v,v), y-(rad/2)+ofRandom(-v,v), 2*rad, rad);
			ofDisableAlphaBlending();
			break;
		case TYPE_VER_RECT:
			// turn into pixels
			rad = ofMap(mySize, 0,1, 50,400);
			alpha = (mySize<0.3)?255:(int)(ofMap(mySize,0.3,1,255,0));
			ofEnableAlphaBlending();
			ofSetColor(ofColor(c,alpha));
			ofFill();
			ofRect(x-(rad/2)+ofRandom(-v,v), y-rad+ofRandom(-v,v), rad, 2*rad);
			ofDisableAlphaBlending();
			break;
		case TYPE_CROSS:
			// turn into pixels
			rad = ofMap(mySize, 0,1, 50,400);
			alpha = (mySize<0.2)?255:(int)(ofMap(mySize,0.2,1,255,0));
			ofEnableAlphaBlending();
			ofSetColor(ofColor(c,alpha));
			ofFill();
			float rx = ofRandom(-v,v);
			float ry = ofRandom(-v,v);
			ofRect(x-(rad/2)+rx, y-rad+ry, rad, 2*rad);
			ofRect(x-rad+rx, y-(rad/2)+ry, 2*rad, rad);
			ofDisableAlphaBlending();
			break;
		default:
			// don't draw anything
			break;
	}
}