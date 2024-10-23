#include "GameStartUI.h"

GameStartUI::GameStartUI() {
}

GameStartUI::~GameStartUI() {
}

void GameStartUI::Init() {
	uiPos_ = { -700, 250.0f };
	start_UI_ = Engine::CreateSprite("gameStart.png");
	start_UI_->SetTextureCenterPos(uiPos_);

	isFadeIn_ = true;
	time_ = 0.0f;
	moveTime_ = 1.5f;

	fadeInStartPos_ = { -700, 250.0f };
	fadeOutPos_ = { 2000, 250.0f };
	isUiMove_ = false;
	isFinish_ = false;
	isFadeIn_ = true;
}

void GameStartUI::Update() {
	if (isFinish_) { return; }
	if (!isUiMove_) { return; }
	Move();

	start_UI_->SetTextureCenterPos(uiPos_);
	start_UI_->Update();
}

void GameStartUI::Draw() const {
	if (isFinish_) { return; }
	start_UI_->Draw();
}

void GameStartUI::Move() {
	// fadeがtrueだったら画面外から画面ないへ
	time_ += GameTimer::DeltaTime();
	float t = time_ / moveTime_;
	if (isFadeIn_) {
		uiPos_ = Vector2::Lerp(fadeInStartPos_, Vector2(640, fadeInStartPos_.y), EaseOutElastic(t));
	} else {
		uiPos_ = Vector2::Lerp(Vector2(640, fadeInStartPos_.y), fadeOutPos_, EaseInOutBack(t));
	}

	// 時間を過ぎたら
	if (time_ >= moveTime_) {
		time_ = 0.0f;

		if (!isFadeIn_) {
			isFinish_ = true;
		}
		isFadeIn_ = !isFadeIn_;
	}
}

void GameStartUI::SetUI() {
	isUiMove_ = true;
	AudioPlayer::SinglShotPlay("timeRemaining.mp3", 0.3f);
}
