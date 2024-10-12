#pragma once
#include "Game/GameObject/BasePlacementObject.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Input/Input.h"
#include "Engine/Lib/GameTimer.h"
#include "Engine/Collider/Collider.h"
#include "Easing.h"

class Fish : 
	public BasePlacementObject,
	public Collider
{
public:
	Fish();
	~Fish();

	void Init() override;
	void Update() override;
	void Draw() const override;

private:

};