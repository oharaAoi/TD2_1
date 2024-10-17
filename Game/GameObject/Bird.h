#pragma once
#include "Game/GameObject/BasePlacementObject.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Lib/GameTimer.h"
#include "Easing.h"

class Bird : public BasePlacementObject {
public:

	Bird();
	~Bird();

	void Init() override;
	void Update() override;
	void Draw() const override;

	void OnCollision([[maybe_unused]] Collider* other)override;

private:

	bool isHitPlayer_ = false;
	float wipeOutTime_ = 2.0f;
};

