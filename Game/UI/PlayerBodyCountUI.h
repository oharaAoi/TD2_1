#pragma once
#include <memory>
#include <string>
#include "Engine/Engine.h"
#include "Engine/2d/Sprite.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Math/Easing.h"

/// <summary>
/// Playerの体の数を数える
/// </summary>
class PlayerBodyCountUI {
public:

	PlayerBodyCountUI();
	~PlayerBodyCountUI();

	void Init();
	void Update(int playerBodyCount);
	void Draw() const;

	void Move();


#ifdef _DEBUG
	void Debug_Gui();
#endif

	void SetIsUiMove(bool isMove) { isUiMove_ = isMove; }

private:

	AdjustmentItem* adjust_;
	std::unique_ptr<Sprite> maxBody_UI_ = nullptr;

	std::unique_ptr<Sprite> gaugeBack_UI_ = nullptr;
	std::unique_ptr<Sprite> gaugeFront_UI_ = nullptr;
	
	Vector2 uiPos_;

	// Parameter
	float time_;
	float moveTime_ = 2.0f;

	Vector2 fadeInStartPos_;
	Vector2 fadeOutPos_;

	Vector2 backPos_UI_;
	Vector2 frontPos_UI_;

	Vector2 frontSize_UI_;

	// フラグ
	bool isUiMove_;
	bool isFadeIn_;

	int preCount_;
};

