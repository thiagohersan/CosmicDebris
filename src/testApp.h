#ifndef TESTAPP_H
#define TESTAPP_H

#include "ofMain.h"
#include "StaticScene.h"
//#include "SerialEventArgs.h"

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	// serial comm and other setup variable
	bool autoPilot;
	ofSerial mySerial;
	
	ofEvent<serialEventArgs> serialEvent;
	StaticScene sss;
	
private:
	unsigned char tmpId, tmpType, tmpVal;
	bool setupSerial();	
};

#endif

