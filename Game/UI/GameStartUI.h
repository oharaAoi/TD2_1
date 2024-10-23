#pragma once
#include <memory>
#include <string>
#include "Engine/Engine.h"
#include "Engine/2d/Sprite.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Math/Easing.h"
#include "Engine/Audio/AudioPlayer.h"

class GameStartUI {
public:

	GameStartUI();
	~GameStartUI();

	void Init();
	void Update();
	void Draw() const;

	void Move();

	void SetUI();

private:

	std::unique_ptr<Sprite> start_UI_ = nullptr;

	Vector2 uiPos_;

	// Parameter
	float time_;
	float moveTime_ = 1.8f;

	Vector2 fadeInStartPos_;
	Vector2 fadeOutPos_;

	// フラグ
	bool isUiMove_;
	bool isFadeIn_;

	bool isFinish_;

};

