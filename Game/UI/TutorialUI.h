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

	void Init();
	void Update();
	void Draw() const;

	void LineUpUI(const Vector3& playerPos);

	void AdaptAdjustment();

#ifdef _DEBUG
	void Debug_Gui();
#endif

	void SetPlayerPos(const Vector3& playerPos) { playerPos_ = playerPos; }

	Vector3 GetSessionFishPos();
	Vector3 GetSessionBirdPos();
	Vector3 GetStartPos();
	
private:

	AdjustmentItem* adjust_;
	std::string groupName_;

	std::unordered_map<std::string, std::unique_ptr<BaseGameObject>> tutorialUI_;

	Vector3 offsetPos_ = { 0.0f, 0.0f, 0.0f };
	float interval_ = 500.0f;

	Vector3 playerPos_;
	float offsetLnegth_;

	bool isLineUp_;;
};

