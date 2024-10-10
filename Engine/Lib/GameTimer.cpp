#include "GameTimer.h"

float GameTimer::deletaTime_ = 0.0f;
float GameTimer::fps_ = 60.0f;
float GameTimer::timeRate_ = 0.0f;

GameTimer::GameTimer(const uint32_t& fps) {
	frameDuration_ = std::chrono::milliseconds(1000 / fps);
	preFrameTime_ = std::chrono::steady_clock::now();
	deletaTime_ = 1.0f / static_cast<float>(fps);
}

GameTimer::~GameTimer() {
}

void GameTimer::WaitNextFrame() {
	auto currentTime = std::chrono::steady_clock::now();
	//std::chrono::duration<float> elapsedTime = currentTime - preFrameTime_;
	deletaTime_ = std::chrono::duration<float>(currentTime - preFrameTime_).count();
	timeRate_ = deletaTime_ / kDeletaTime_;
	preFrameTime_ = std::chrono::steady_clock::now();
}
