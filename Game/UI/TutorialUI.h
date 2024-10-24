#pragma once
#include <vector>
#include <memory>
#include <map>
#include "Engine/Engine.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/GameObject/BaseGameObject.h"


/// <summary>
/// Tutorial用に使用するUI
/// </summary>
class TutorialUI {
public:

	TutorialUI();
	~TutorialUI();

	void Init(const Vector3& playerPos);
	void Update();
	void Draw() const;

	void LineUpUI();

	void AdaptAdjustment();

#ifdef _DEBUG
	void Debug_Gui();
#endif

private:

	AdjustmentItem* adjust_;
	std::string groupName_;

	std::unordered_map<std::string, std::unique_ptr<BaseGameObject>> tutorialUI_;

	Vector3 offsetPos_ = { 0.0f, 0.0f, 0.0f };
	float interval_ = 300.0f;
};

