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
class TutorialUI{
public:

	TutorialUI();
	~TutorialUI();

	void Init();
	void Update();
	void Draw() const;

	void LineUpUI(const Vector3& playerPos);

	void AdaptAdjustment();

	// チュートリアルテキスト用関数
	void UpdateTutorialText();
	void DrawTutorialText();

#ifdef _DEBUG
	void Debug_Gui();
#endif

	void SetPlayerPos(const Vector3& playerPos){ playerPos_ = playerPos; }

	Vector3 GetSessionFishPos();
	Vector3 GetSessionBirdPos();
	Vector3 GetStartPos();
	bool GetIsTextShowing() const{ return isTextShowing_; }


private:
	AdjustmentItem* adjust_;
	std::string groupName_;

	std::unordered_map<std::string, std::unique_ptr<BaseGameObject>> tutorialUI_;

	std::unique_ptr<BaseGameObject> jumpTutorialUI_;
	Vector3 jumpTutorialPos_;
	Vector3 jumpTutorialOffsetPos_ = { 0.0f, 0.0f, 0.0f };

	Vector3 offsetPos_ = { 0.0f, 0.0f, 0.0f };
	float interval_ = 500.0f;

	Vector3 playerPos_;
	float offsetLnegth_;

	bool isLineUp_;

	Vector3 scaleUp_;
	float scaleUpStrength_;

	static const int32_t tutorialCount_ = 3;
	bool isShownText_[tutorialCount_];
	bool isTextShowing_ = false;
	std::unique_ptr<Sprite> tutorialText_;
	std::unique_ptr<Sprite> textBackSprite_;
};

