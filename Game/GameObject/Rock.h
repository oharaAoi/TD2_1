#pragma once
#include "Game/GameObject/BasePlacementObject.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Lib/GameTimer.h"
#include "Easing.h"

enum class RockSize : int32_t {
	SMALL = 6,
	MIDIUM = 12,
	LARGE = 18,
};

class Rock : public BasePlacementObject {
public:

	Rock();
	~Rock();

	void Init() override;
	void Update() override;
	void Draw() const override;

	void OnCollision([[maybe_unused]] Collider* other)override;

	void IndividualFromCommon(const SubAttributeType& subType = SubAttributeType::NONE) override;

private:


};

