#include "PlayerBodyCountUI.h"

PlayerBodyCountUI::PlayerBodyCountUI() {}
PlayerBodyCountUI::~PlayerBodyCountUI() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerBodyCountUI::Init() {
	uiPos_ = { -200, 250.0f };
	maxBody_UI_ = Engine::CreateSprite("kari.png");
	maxBody_UI_->SetTextureCenterPos(uiPos_);

	isFadeIn_ = true;
	time_ = 0.0f;
	moveTime_ = 2.0f;

	fadeInStartPos_ = { -200, 250.0f };
	fadeOutPos_ = { 1500, 250.0f };
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerBodyCountUI::Update(int playerBodyCount) {
	// 8以上かつ動いて入なかったら
	if (playerBodyCount == 8) {
		if (preCount_ != 8) {
			isUiMove_ = true;
		}
	}

	preCount_ = playerBodyCount;
	
	if (!isUiMove_) { return; }

	Move();

	maxBody_UI_->SetTextureCenterPos(uiPos_);
	maxBody_UI_->Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerBodyCountUI::Draw() const {
	if (!isUiMove_) { return; }
	maxBody_UI_->Draw();
}

void PlayerBodyCountUI::Move() {
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
			isUiMove_ = false;
		}

		isFadeIn_ = !isFadeIn_;
	}
}

#ifdef _DEBUG
void PlayerBodyCountUI::Debug_Gui() {
	if (ImGui::TreeNode("playerBodyUI")) {

		ImGui::TreePop();
	}
}
#endif