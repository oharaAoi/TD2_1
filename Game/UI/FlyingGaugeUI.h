#pragma once
#include <vector>
#include <algorithm>
#include "Engine.h"
#include "Engine/Utilities/AdjustmentItem.h"

enum class FlyingRank {
	Rank_First,
	Rank_B,
	Rank_A,
	Rank_S,
	Rank_Master
};

class FlyingGaugeUI {
public:

	FlyingGaugeUI();
	~FlyingGaugeUI();

	void Init();
	void Update(float currentLength);
	void Draw(float alpha);

	void CalculationRaito(float currentLength);

	void RankUp();

	void AdaptAdjustmentItem();

#ifdef _DEBUG
	void Debug_Gui();
#endif // _DEBUG

private:

	AdjustmentItem* adjustmentItem_;
	std::string groupName_;

	std::unique_ptr<Sprite> icon_;
	std::unique_ptr<Sprite> icon_maxHeight_;
	std::unique_ptr<Sprite> tower_;

	FlyingRank nowMaxRank_;

	float rankLenghtMin_;
	float rankLenghtMax_;

	float lengthRaito_;
	float maxLenghtRaito_;

	float height_t;
	float maxHeight_t;
	float halfHeight_t;
	float skytreeHeight_ = 634.0f;
	float halfWindowHeight_ = 158.0f;
	Vector2 skytreeBasePos_ = {100.0f,100.0f};
	Vector2 iconBasePos_ = { 200.0f,kWindowHeight_ };
	Vector2 maxIconBasePos_ = {-10000.0f,-10000.0f};
};

