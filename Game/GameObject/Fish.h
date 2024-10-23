#pragma once
#include "Game/GameObject/BasePlacementObject.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Input/Input.h"
#include "Engine/Lib/GameTimer.h"
#include "Engine/Collider/Collider.h"
#include "Easing.h"
#include "Game/Manager/AnimetionEffectManager.h"

enum class FISH_SIZE : int32_t{
	SMALL = 0,
	MIDIUM,
	LARGE,
	kFishSizeCount// 種類のカウント用。使ってはいけない
};

class Fish : public BasePlacementObject{

public:
	Fish();
	~Fish();

	void Init() override;
	void Update() override;
	void Draw() const override;

	void OnCollision([[maybe_unused]] Collider* other)override;

	void IndividualFromCommon(const SubAttributeType& subType = SubAttributeType::NONE) override;

	void Change();

public:

	float GetEnergy()const{ return energy_; }
	const FISH_SIZE& GetFishSize()const{ return fishSize_; }
	void SetFishSize(const FISH_SIZE& fishSize);
	const int GetEatCount(){ return eatCount; }
	void SetEatCount(int value){ eatCount = value; }

	void SetIsAte(bool isAte) { isAte_ = isAte; }

private:

	float energy_;// プレイヤーが食べた時に得るエネルギー
	FISH_SIZE fishSize_;
	Vector3 initSize = { 1,1,1 };

	bool isAte_;
	bool isPreAte_;
	int eatCount;//食べるのに必要な胴体の個数
};