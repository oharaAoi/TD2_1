#pragma once
#include "Game/GameObject/BasePlacementObject.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Lib/GameTimer.h"
#include "Easing.h"

class Mountain : public BaseGameObject {
public:

	Mountain();
	~Mountain();

	void Init() override;
	void Update() override;
	void Draw() const override;

private:

	Vector3 offset_;
};

