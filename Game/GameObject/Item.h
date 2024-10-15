#pragma once
#include "Game/GameObject/BasePlacementObject.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Input/Input.h"
#include "Engine/Lib/GameTimer.h"
#include "Engine/Collider/Collider.h"
#include "Easing.h"

class Item : public BasePlacementObject{

public:
	Item();
	~Item();

	void Init() override;
	void Update() override;
	void Draw() const override;

	void OnCollision([[maybe_unused]] Collider* other)override;

private:

};