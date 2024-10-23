#include "FinishUI.h"

FinishUI::FinishUI() {}
FinishUI::~FinishUI() {}

void FinishUI::Init() {
	uiPos_ = { -200, 250.0f };
	finish_UI_ = Engine::CreateSprite("finish.png");
	finish_UI_->SetTextureCenterPos(uiPos_);

	isFadeIn_ = true;
	time_ = 0.0f;
	moveTime_ = 1.5f;

	fadeInStartPos_ = { -400, 250.0f };
	fadeOutPos_ = { 2000, 250.0f };
	isUiMove_ = false;
	onlyMove_ = false;
}

void FinishUI::Update() {
	if (!isUiMove_) { return; }

	Move();

	finish_UI_->SetTextureCenterPos(uiPos_);
	finish_UI_->Update();
}

void FinishUI::Draw() const {
	if (!isUiMove_) { return; }
	finish_UI_->Draw();
}

void FinishUI::Move() {
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
		isFadeIn_ = !isFadeIn_;
	}
}

void FinishUI::SetUI(bool isOverTime) {
	if (onlyMove_) {
		return;
	}
	if (isOverTime) {
		finish_UI_->SetTexture("overTime.png");
		finish_UI_->SetTextureCenterPos(uiPos_);
	}

	AudioPlayer::SinglShotPlay("Finish.mp3", 0.3f);
	isUiMove_ = true;
	onlyMove_ = true;
}
