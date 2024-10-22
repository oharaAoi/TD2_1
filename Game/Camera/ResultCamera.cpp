#include "ResultCamera.h"

ResultCamera::ResultCamera(){
	Init();
}

ResultCamera::~ResultCamera(){}

void ResultCamera::Finalize(){}

void ResultCamera::Init(){
	BaseCamera::Init();

	transform_ = {
		{1.0f, 1.0f, 1.0f},
		{0.13f, 0.22f, 0.0f},
		{-4.32f, 2.56f, -9.63f}
	};

}

void ResultCamera::Update(){

	//if(startWaitTime_ >= 0.0f){
	//	startWaitTime_ -= GameTimer::DeltaTime();
	//} else{
	//
	//if(time_ +)
	//}


	BaseCamera::Update();
}

#ifdef _DEBUG
void ResultCamera::Debug_Gui(){}
#endif // _DEBUG

