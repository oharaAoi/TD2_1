#pragma once
#include <vector>
#include "Engine.h"
#include "Engine/Utilities/AdjustmentItem.h"

class FlyingGaugeUI {
public:

	FlyingGaugeUI();
	~FlyingGaugeUI();

	void Init();
	void Update(float currentLength, float maxLength);
	void Draw() const;

	void AdaptAdjustmentItem();

#ifdef _DEBUG
	void Debug_Gui();
#endif // _DEBUG

private:

	AdjustmentItem* adjustmentItem_;
	std::string groupName_;

	std::unique_ptr<Sprite> outside_;
	std::unique_ptr<Sprite> bar_;

};

