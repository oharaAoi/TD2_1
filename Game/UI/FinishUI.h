#pragma once
#include <memory>
#include <string>
#include "Engine/Engine.h"
#include "Engine/2d/Sprite.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Math/Easing.h"
#include "Engine/Audio/AudioPlayer.h"

class FinishUI {
public:

	FinishUI();
	~FinishUI();

	void Init();
	void Update();
	void Draw() const;

	void Move();

	void SetUI(bool isOverTime);

private:

	std::unique_ptr<Sprite> finish_UI_ = nullptr;

	Vector2 uiPos_;

	// Parameter
	float time_;
	float moveTime_ = 2.0f;

	Vector2 fadeInStartPos_;
	Vector2 fadeOutPos_;

	// フラグ
	bool isUiMove_;
	bool isFadeIn_;

	int preCount_;

	bool onlyMove_;

	bool isFinish_;

};

