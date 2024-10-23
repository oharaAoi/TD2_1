#include "ResultCamera.h"
#include "Easing.h"
#include "Game/Scene/ResultScene.h"

ResultCamera::ResultCamera(){
	Init();
}

ResultCamera::~ResultCamera(){}

void ResultCamera::Finalize(){}

void ResultCamera::Init(){
	BaseCamera::Init();

	transform_ = {
		{1.0f, 1.0f, 1.0f},
		baseRotate_,
		baseTranslate_
	};

}

void ResultCamera::Update(){

	// =============================== 最初のスコア表示 ================================= //
	if(ResultScene::GetIsViewRanking() == false){

		if(startWaitTime_ >= 0.0f){
			startWaitTime_ -= GameTimer::DeltaTime();

		} else{

			if(time_ < startTime_){
				time_ = std::clamp(time_ + GameTimer::DeltaTime(), 0.0f, startTime_);
				float ease = EaseOutBack(time_ / startTime_, 0.5f);
				addCameraRotate_.y = 3.14f + (-3.14f * ease);
			}
		}

	} else{// ======================== ランキング画面の表示へ移行 =============================== //

		time2_ = std::clamp(time2_ + GameTimer::DeltaTime(), 0.0f, transitionTimeToRanking_);
		float ease = EaseInOutBack(time2_ / transitionTimeToRanking_);
		addCameraRotate_.x = -1.57f * ease;

	}

	// 座標の更新
	Shake(2.0f);
	transform_.rotate = baseRotate_ + addCameraRotate_ + shakeRotate_;
	transform_.translate = baseTranslate_ + shakeTranslate_;
	BaseCamera::Update();
}


void ResultCamera::Shake(float radius){

	static float time = 0.0f;
	static float lengthEvery = 0.0f;
	static Vector3 targetPoint{};
	static Vector3 startPoint{};
	static Vector3 prePos{};
	static Vector3 dif{};
	static Vector3 interpolationEvery{};
	float interpolationTime = 3.0;

	// 時間を満たしたら新しいシェイクの目標座標を求める
	time += GameTimer::DeltaTime();
	prePos = shakeRotate_;

	if(time >= 0.3f){
		Vector3 rotate = {
			RandomFloat(-0.08f,0.08f),
			RandomFloat(-0.08f,0.08f),
			0.0f//RandomFloat(-0.18f,0.18f)
		};

		startPoint = shakeRotate_;
		targetPoint = rotate;
		if((targetPoint - startPoint).Length() > radius){
			targetPoint = (targetPoint - startPoint).Normalize() * radius;
		}

		interpolationEvery = ((targetPoint - startPoint) / (60.0f * interpolationTime));
		time = 0.0f;
	}



	// シェイクを補間
	Vector3 interpolation = ((dif * 0.7f) + (interpolationEvery * 0.3f)) * 0.5f;
	shakeRotate_ += interpolation * GameTimer::TimeRate();
	if(shakeRotate_.Length() > radius){
		shakeRotate_ = shakeRotate_.Normalize() * radius;
	}

	// 
	dif = shakeRotate_ - prePos;
}


#ifdef _DEBUG
void ResultCamera::Debug_Gui(){}
#endif // _DEBUG

