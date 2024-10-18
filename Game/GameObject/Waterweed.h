#pragma once
#include "Game/GameObject/BasePlacementObject.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Lib/GameTimer.h"
#include "Easing.h"

class Waterweed : public BasePlacementObject {
public:

	Waterweed();
	~Waterweed();

	void Init() override;
	void Update() override;
	void Draw() const override;

	void OnCollision([[maybe_unused]] Collider* other)override;

	void SetWorldWallPos(const Vector3& pos);

private:


};

