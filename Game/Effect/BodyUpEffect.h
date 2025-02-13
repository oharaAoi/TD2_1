#pragma once
#include <list>
#include <memory>
#include "Engine/Engine.h"
#include "Engine/2d/Sprite.h"

/// <summary>
/// Playerの体が出た時に出すEffect
/// </summary>
class BodyUpEffect {
public:

	struct EffectData {
		std::unique_ptr<Sprite> sprite;
		float lifeTimeCount;
		float disapperCount;
	};

public:

	BodyUpEffect();
	~BodyUpEffect();

	void Init();
	void Update(const Vector2& screenPos);
	void Draw() const;

	void AddEffect();

private:

	// ------------------- Sprite ------------------- //
	std::list<EffectData> upEffects_;

	// ------------------- playerPosition ------------------- //
	Vector2 playerScreenPos_;

	// ------------------- Parameter ------------------- //
	float lifeTime_ = .6f;
	float disapperTime_ = 1.0f;

	float decay_ = 0.4f; // 減衰率
	float velocityY_ = 120.0f;

};

