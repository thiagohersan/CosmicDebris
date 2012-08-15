#include "StaticScene.h"

/* TODO:
	- read values from serial and update frequency, colors, shape variables, etc
*/

StaticScene::StaticScene(){
	digitalVal = 0;
}

StaticScene::~StaticScene(){
}

void StaticScene::setup(){
}

void StaticScene::update(){
}

void StaticScene::draw(){
}

/**/
void StaticScene::onSerialEvent(serialEventArgs &a){
	unsigned char type_ = a.type_;
	unsigned char id_ = a.id_;
	unsigned char value_ = a.value_;

	if(type_ == 'A'){
		analogVals[id_%NUM_ANALOG] = value_;
		printf("Seeting analog[%d] to %d\n", (int)(id_%NUM_ANALOG), value_);
	}
	else if(type_ == 'D'){
		digitalVals[id_%NUM_DIGITAL] = (value_==0xff);
		printf("Seeting digital[%d] to %d\n", (int)(id_%NUM_DIGITAL), (value_==0xff));
		// set id_-th bit of digitalVal based on value_
		//   on box, the buttons are numbered: [0,1,...,5]
		//   but here I want them to read as bits on a number, so I have to flip their order with NUM_DIGITAL-1-id_
		//   leet shit
		char mask0 =   ((value_==0xff)&0x1)<<(NUM_DIGITAL-1-id_);    // all zeros and v
		char mask1 = ~(((value_!=0xff)&0x1)<<(NUM_DIGITAL-1-id_));   // all ones and v
		
		digitalVal = (mask0|digitalVal)&mask1;
		printf("digital val = %d\n", digitalVal);
	}
}
/**/
