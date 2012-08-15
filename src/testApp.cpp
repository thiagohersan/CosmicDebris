#include "testApp.h"

// return true if it sets up a serial communication port
bool testApp::setupSerial(){
	vector<ofSerialDeviceInfo> serialList = mySerial.getDeviceList();
	for(int i=0; i<serialList.size(); i++){
		string thisDevicePath = serialList.at(i).getDevicePath();
		string thisDeviceName = serialList.at(i).getDeviceName();
		int thisDeviceID = serialList.at(i).getDeviceID();
		//printf("%s, at %s, has ID %d\n", thisDeviceName.c_str(), thisDevicePath.c_str(), thisDeviceID);
		
		// find a usb port
		if((thisDeviceName.find("usb") != string::npos) || (thisDeviceName.find("USB") != string::npos) || (thisDeviceName.find("Usb") != string::npos)){
			mySerial.setup(thisDeviceID, 19200);
			return true;
		}
	}
	// did not find a usb port
	return false;
}

bool testApp::readSerial(){
	// read 4 bytes at a time
	//   use while()?
	if((!autoPilot) && (mySerial.available() >= 4)) {
		// read 4 bytes
		unsigned char tmpSerialBuf[4];
		mySerial.readBytes(tmpSerialBuf, 4);
		// check hello signal
		if(tmpSerialBuf[0] == 0xab){
			// load event args and notify
			//     gotta notify, come up in the spot looking extra fly
			//     for the day I die, I'mma touch the sky
			static serialEventArgs mySerialEventArgs;
			mySerialEventArgs.type_ = tmpSerialBuf[1];
			mySerialEventArgs.id_ = tmpSerialBuf[2];
			mySerialEventArgs.value_ = tmpSerialBuf[3];
			
			ofNotifyEvent(serialEvent, mySerialEventArgs, this);
			return true;
		}
	}
	return false;
}

//--------------------------------------------------------------
void testApp::setup(){
	// determine autoPilot and Serial port
	autoPilot = !setupSerial();
	// add a listener to the staticScene object
	ofAddListener(serialEvent, &sss, &StaticScene::onSerialEvent);
	
	// screen setup
	ofSetCircleResolution(100);
	//ofSetVerticalSync(true);
}

//--------------------------------------------------------------
void testApp::update(){
	// read serial
	this->readSerial();
	
	// test
	d.setType((ofGetFrameNum()/300));
	d.setSize(0.5 + (ofNoise((float)(ofGetFrameNum())/1000,(float)(ofGetFrameNum())/500)-0.5));
	if(ofGetFrameNum()%100 == 0){
		printf("%d:= %f\n",d.getType(),ofGetFrameRate());
	}	
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0,0,0);
	// test
	for(int i=100; i<ofGetHeight(); i+=200){
		for(int j=100; j<ofGetWidth(); j+=200){
			d.draw(j,i,ofRandom(20));
		}
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	if((key <= '9') && (key >= '0')){
		unsigned char tmpDigit = (key-'0')&0xff;
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
	
}