#include "testApp.h"

// inits object declared in the header file
//testApp::testApp() : c(this){}

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
	// add a listener for the Canvas object
	ofAddListener(serialEvent, &c, &Canvas::onSerialEvent);
	
	// screen setup
	//ofSetCircleResolution(100);
	// TODO: bug?? only on nvidia geforce 320m
	ofSetBackgroundAuto(false);
	ofSetVerticalSync(true);
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	// other setup
	ofSetRectMode(OF_RECTMODE_CENTER);
	
	// for manual control without box
	digitalVal = 0;
	for(int i=0; i<6; i++){
		analogVals[i] = 0;
		digitalVals[i] = 0;
	}
}

//--------------------------------------------------------------
void testApp::update(){
	// read serial
	this->readSerial();
	c.update();
	
	/*** autopilot keep this !!!
	 d.setType((ofGetFrameNum()/300));
	 d.setSize(0.5 + (ofNoise(ofGetFrameNum()/1000.0, ofGetFrameNum()/500.0)-0.5));
	 d.setSize(0.2); // just touching
	 float hue = ofMap(ofNoise(ofGetFrameNum()/500.0, ofGetFrameNum()/2000.0), 0,1, 0,255);
	 ofColor cc = ofColor::fromHsb(hue, 255, 255);
	 ***/
	
	// test
	if(ofGetFrameNum()%100 == 0){
		printf("%f\n",ofGetFrameRate());
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	c.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	// for manual control without box
	if((key <= '9') && (key >= '0')){
		unsigned char tmpDigit = (key-'0')&0xff;
		bNum = tmpDigit%6;
	}
	if((key == 'a') || (key == 'A')){
		bType = 'A';
	}
	if((key == 'd') || (key == 'D')){
		bType = 'D';
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
	// for manual control without box
	bVal = ofMap(y, 5,ofGetHeight()-5, 255,0, true);
	
	if(bType == 'A'){
		analogVals[bNum] = bVal;
	}
	if(bType == 'D'){
		digitalVals[bNum] = (bVal>128)?255:0;
		
		unsigned char mask0 =   ((digitalVals[bNum]==0xff)&0x1)<<(5-bNum);    // all zeros and v
		unsigned char mask1 = ~(((digitalVals[bNum]!=0xff)&0x1)<<(5-bNum));   // all ones and v
		
		digitalVal = (mask0|digitalVal)&mask1;
	}
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
