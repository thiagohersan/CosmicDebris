#include "testApp.h"

// return true if it sets up a serial communication port
bool testApp::setupSerial(){
	vector<ofSerialDeviceInfo> serialList = mySerial.getDeviceList();
	for(int i=0; i<serialList.size(); i++){
		string thisDevicePath = serialList.at(i).getDevicePath();
		string thisDeviceName = serialList.at(i).getDeviceName();
		int thisDeviceID = serialList.at(i).getDeviceID();
		printf("%s, at %s, has ID %d\n", thisDeviceName.c_str(), thisDevicePath.c_str(), thisDeviceID);
		
		// find a usb port
		if((thisDeviceName.find("usb") != string::npos)){
			mySerial.setup(thisDeviceID, 19200);
			return true;
		}
	}
	// did not find a usb port
	return false;
}

//--------------------------------------------------------------
void testApp::setup(){
	// determine autoPilot and Serial port
	autoPilot = !setupSerial();
	
	//StaticScene sss;
	ofAddListener(serialEvent, &sss, &StaticScene::onSerialEvent);

	tmpId = 0x0;
	tmpType = 'A';
	tmpVal = 0x0;
}

//--------------------------------------------------------------
void testApp::update(){
	
}

//--------------------------------------------------------------
void testApp::draw(){
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	if((key <= '9') && (key >= '0')){
		tmpId = (key-'0')&0xff;
	}
	if((key == 'a') || (key == 'A')){
		tmpType = 'A';
	}
	if((key == 'd') || (key == 'D')){
		tmpType = 'D';
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
	tmpVal = (unsigned char)ofMap(y,ofGetHeight(),0,0,255);

	static serialEventArgs mySerialEventArgs;
	mySerialEventArgs.type_ = tmpType;
	mySerialEventArgs.id_ = tmpId;
	mySerialEventArgs.value_ = (tmpType == 'A')?(tmpVal):((tmpVal>0x7f)*0xff);
	
	ofNotifyEvent(serialEvent, mySerialEventArgs, this);
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