#include "GamePlayTimer.h"

GamePlayTimer::GamePlayTimer() {
}

GamePlayTimer::~GamePlayTimer() {
}

void GamePlayTimer::Init(float limit) {
	isFinish_ = false;
	gameTimer_ = 0;
	gameTimeLimit_ = limit;
}

void GamePlayTimer::Update() {
	gameTimer_ += GameTimer::DeltaTime();

	// 制限時間を超えたら
	if (gameTimer_ > gameTimeLimit_) {
		isFinish_ = true;
	}
}

void GamePlayTimer::Measurement() {
	isFinish_ = false;
	gameTimer_ = 0;
}

#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
void GamePlayTimer::Debug_Gui() {
	ImGui::Begin("GamePlayTimer");
	ImGui::Text("gameTimer_ : %f", gameTimer_);
	ImGui::End();
}
#endif