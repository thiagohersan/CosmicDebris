#include "StaticScene.h"

/* TODO:
	- test that we're pasing the Val pointers correctly...
*/

StaticScene::StaticScene(char* aVals_, char* dVals_, int* dVal_){
	analogVals = aVals_;
	digitalVals = dVals_;
	digitalVal = dVal_;
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
}
/**/
