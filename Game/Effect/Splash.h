#pragma once
#include "Engine/GameObject/BaseGameObject.h"
#include "Engine/Math/MyRandom.h"
#include <vector>
#include <memory>

class Splash{

public:
	Splash() = default;
	Splash(const Vector3& emitPos, float velocity);

	void Update();
	void Draw();

public:

	bool GetIsEndSplash(){ return lifetime_ <= 0.0f; }

public:
	Vector3 emitPos_;
	float kLifetime_;
	float lifetime_;
	float radius_;
	float maxHeight_;

private:
	static const int kSplashCount_ = 12;
	static const int kWaterColmnCount_ = 8;
	static const int kPlaneCount_ = 8;
	static float defaultVelocity_;

private:
	std::vector<std::unique_ptr<BaseGameObject>>waterColmns_;
	std::vector<std::unique_ptr<BaseGameObject>>splashModels_;
	std::vector<std::unique_ptr<BaseGameObject>>ripples_;
};