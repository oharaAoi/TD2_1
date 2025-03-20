#pragma once
#include <memory>
#include "Engine/2d/Sprite.h"

/// <summary>
/// 魚が食べられた時に出る2dのエフェクト
/// </summary>
class FishEnergy {
public:

	FishEnergy() = default;
	~FishEnergy() = default;

	void Init(const Vector2& pos);
	void Update();
	void Draw() const;

public:

	void SetTargetPos(const Vector2& pos) { targetPos_ = pos; }

	bool GetIsAlive() const { return isAlive_; }

private:

	std::unique_ptr<Sprite> energySprite_;

	Vector2 absorptioVelocity_;
	Vector2 absorptioAcceleration_;

	Vector2 velocity_;
	float firstSpeed_ = 30.0f;
	float decelerationRaito_ = 0.9f;

	bool isAbsorption_ = false;
	float absorptionSpeed_ = 40.0f;

	bool isAlive_ = true;

	Vector2 targetPos_;

};

