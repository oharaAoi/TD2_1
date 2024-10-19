#pragma once
#include "Game/GameObject/BasePlacementObject.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Lib/GameTimer.h"
#include "Easing.h"

class Coin : public BasePlacementObject {
public:

	Coin();
	~Coin();

	void Init() override;
	void Update() override;
	void Draw() const override;

	void OnCollision([[maybe_unused]] Collider* other)override;

	void IndividualFromCommon(const SubAttributeType& subType = SubAttributeType::NONE) override;

private:


};

