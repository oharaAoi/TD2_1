#include "FlyingTimer.h"

FlyingTimer::FlyingTimer() {}
FlyingTimer::~FlyingTimer() {}

void FlyingTimer::Init() {
	isMeasurement_ = false;
	currentFlyingLength_ = 0.0f;
	maxFlyingLength_ = 0.0f;
}

void FlyingTimer::Update(float posX) {
	if (isMeasurement_) {
		currentFlyingLength_ = posX - flyingStartPosX_;
	}
}

/// <summary>
/// 計測を行う
/// </summary>
void FlyingTimer::Measurement(float posX) {
	flyingStartPosX_ = posX;
	isMeasurement_ = true;
}

/// <summary>
/// 計測終了
/// </summary>
void FlyingTimer::Finish(float posX) {
	flyingFinalPosX_ = posX;

	currentFlyingLength_ = flyingFinalPosX_ - flyingStartPosX_;

	// maxyより大きかったら
	if (maxFlyingLength_ < currentFlyingLength_) {
		maxFlyingLength_ = currentFlyingLength_;
	}
	currentFlyingLength_ = 0.0f;
	isMeasurement_ = false; 
}

#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
void FlyingTimer::Debug_Gui() {
	ImGui::Begin("FlyingTimer");
	ImGui::Text("max : %f", maxFlyingLength_);
	ImGui::Text("current : %f", currentFlyingLength_);
	ImGui::End();
}
#endif