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

	BaseCamera::Update();
}

void ResultCamera::Debug_Gui(){}