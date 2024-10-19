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
	void Draw() const;

	void CalculationRaito(float currentLength);

	void RankUp();

	void AdaptAdjustmentItem();

#ifdef _DEBUG
	void Debug_Gui();
#endif // _DEBUG

private:

	AdjustmentItem* adjustmentItem_;
	std::string groupName_;

	std::unique_ptr<Sprite> outside_;
	std::unique_ptr<Sprite> bar_;
	std::unique_ptr<Sprite> rank_;
	std::unique_ptr<Sprite> icon_;

	FlyingRank nowMaxRank_;

	float rankLenghtMin_;
	float rankLenghtMax_;

	float lengthRaito_;
	float maxLenghtRaito_;
};

