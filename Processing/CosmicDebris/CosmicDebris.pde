import processing.serial.*;
import ddf.minim.*;
import ddf.minim.signals.*;

import processing.video.*;

// globals
private final static boolean WEDEBUG = true;

// audio variables
Minim minim;
AudioOutput out;
Oscillator myOscillator;

//
Movie myMovie;

// communication variables
Serial mySerial;

// input variables
int NUM_ANALOG = 6;
int NUM_DIGITAL = 6;
int digitalVals[] = new int[NUM_DIGITAL];
int analogVals[] = new int[NUM_ANALOG];

// period variables 
long lastUpdate = 0;
boolean turnOn = true;

// auto-pilot variables
long noiseVar = 1;
boolean autoPilot = false;

// state keepers
int currType = 0;

// drawable object
Objectable myObjectable;

void setup() {
  size(1600, 1000);
  frameRate(60);
  smooth();

  colorMode(HSB, 255);
  ellipseMode(CENTER);
  rectMode(CENTER);

  // detect serial port
  for (int i=0; i<Serial.list().length; i++) {
    String thisSerial = Serial.list()[i].toLowerCase();
    if (thisSerial.matches(".*tty.*(arduino|usb).*")) {
      mySerial = new Serial(this, Serial.list()[i], 19200);
    }
    println("!!!!: "+Serial.list()[i]);
  }

  autoPilot = (mySerial == null);
  println("auto pilot is "+((mySerial==null)?"ON":"OFF"));

  //////
  // setup audio
  minim = new Minim(this);
  // get a line out from Minim, default bufferSize is 1024, default sample rate is 44100, bit depth is 16
  out = minim.getLineOut(Minim.STEREO);
  // create an Oscillator, set to 60 Hz, sample rate from line out
  myOscillator = new TriangleWave(80, 0.8, out.sampleRate());
  // set the portamento speed on the oscillator
  myOscillator.noPortamento();
  // add the oscillator to the line out
  out.addSignal(myOscillator);

  //////
  // drawables
  //myObjectable = new Drawable();
  myObjectable = new Morphable();

  ////////
  /// if auto pilot, then set initial conditions
  if (autoPilot == true) {
    for (int i=0; i<NUM_DIGITAL; i++) {
      digitalVals[i] = 255;
    }
    for (int i=0; i<NUM_ANALOG; i++) {
      analogVals[i] = 0;
    }
  }

  ///////////
  //myMovie = new Movie(this, "a.mov");
  //myMovie.loop();
}



void draw() {
  //if(myMovie.available()){
  //myMovie.read();
  //}

  // get input values
  if (autoPilot == true) {
    for (int i=0; i<NUM_ANALOG; i++) {    
      analogVals[i] = int(noise(i, noiseVar*0.003-i*PI)*255);
    }
  }

  // automatically pick shapes
  int myRandom0 = (int) random(0, 60);
  int myRandom1 = (int) random(0, 60);
  if (myRandom0 == myRandom1)
    println(myRandom0+"  "+myRandom1);
  if (autoPilot) {
    currType = (myRandom0 == myRandom1)?(currType+myRandom0):currType;
    myObjectable.setType(currType);
  }
  noiseVar++;

  // figure out flashing frequency
  float bgndPeriod = map(analogVals[0], 30, 254, 250, 20);

  // get time
  long currTime = millis();

  // update if necessary
  if ((currTime-lastUpdate) > bgndPeriod/2) {
    // read variables
    int bgndColor = (int)map(analogVals[1], 30, 254, 0, 255);
    float sVar = map(analogVals[2], 30, 254, 0, 1);
    float rVar = map(analogVals[3], 30, 254, 0, 20);

    ////////////////////////

    // differentiate the two half-period, turning on and off
    if (turnOn == true) {
      // color background
      background(bgndColor, 255, 255);
      //image(myMovie, 0,0);

      // red objects
      myObjectable.setSize(sVar); 

      for (int i=100; i<height; i+=200) {
        for (int j=100; j<width; j+=200) {
          myObjectable.draw((float)j, (float)i, rVar, color(255, 255, 255));
        }
      }

      // sound on
      myOscillator.setAmp(0.8);
    }
    /////
    else {
      // black background
      background(0, 255, 0);
      //image(myMovie, 0,0);

      // color objects
      myObjectable.setSize(sVar);

      for (int i=100; i<height; i+=200) {
        for (int j=100; j<width; j+=200) {
          myObjectable.draw((float)j, (float)i, rVar, color(bgndColor, 255, 255));
        }
      }

      // sound off
      myOscillator.setAmp(0.0);
    }

    // update period variables
    lastUpdate = currTime;
    turnOn = !turnOn;

    if (WEDEBUG) println(frameRate);
  }
}


void keyReleased() {
  if ((key >= '0') && (key <= '9')) {
    currType = (int)(key-'0');
    myObjectable.setType(currType);
  }
}

void serialEvent(Serial myPort) {
  // read 3 bytes from the serial port:
  if (myPort.available() >= 4) {
    if (myPort.read() == 0xab) {
      char inType = (char)myPort.read();
      int inPort = myPort.read();
      int inVal = myPort.read();
      if (WEDEBUG) println(inType+" "+inPort+" "+inVal);

      if (inType == 'D') {
        digitalVals[inPort%NUM_DIGITAL] = inVal;
      }
      else if (inType == 'A') {
        analogVals[inPort%NUM_ANALOG] = inVal;
      }
    }
    myPort.clear();
  }
}

// function to set refresh rate
/*
void setVsync(boolean b) {
 pgl = (PGraphicsOpenGL) g;
 gl = pgl.beginGL();
 gl.setSwapInterval((b?1:0));
 pgl.endGL();
 }
 */
