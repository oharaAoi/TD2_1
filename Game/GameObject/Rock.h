#pragma once
#include "Game/GameObject/BasePlacementObject.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Lib/GameTimer.h"
#include "Easing.h"

class Rock : public BasePlacementObject {
public:

	Rock();
	~Rock();

	void Init() override;
	void Update() override;
	void Draw() const override;

	void OnCollision([[maybe_unused]] Collider* other)override;

private:


};

