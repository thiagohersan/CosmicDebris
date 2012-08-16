/*
 *
 */

#ifndef SERIALEVENTARGS_H
#define SERIALEVENTARGS_H

#include "ofEvents.h"

// sort of pedantic example of event handling...
//    but, hey! I learned something!

class serialEventArgs : public ofEventArgs{
public:
	unsigned char type_;
	unsigned char id_;
	unsigned char value_;
};

#endif