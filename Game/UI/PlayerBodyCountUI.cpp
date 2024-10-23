#include "PlayerBodyCountUI.h"

PlayerBodyCountUI::PlayerBodyCountUI() {}
PlayerBodyCountUI::~PlayerBodyCountUI() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerBodyCountUI::Init() {
	uiPos_ = { -200, 250.0f };
	backPos_UI_ = { 1100, 110.0f };
	
	frontSize_UI_ = { 0.0f, 1.0f };

	maxBody_UI_ = Engine::CreateSprite("bodyMax.png");
	maxBody_UI_->SetTextureCenterPos(uiPos_);

	gaugeBack_UI_ = Engine::CreateSprite("TorsoGaugeBack.png");
	gaugeFront_UI_ = Engine::CreateSprite("TorsoGaugeFront.png");
	gaugeBack_UI_->SetTextureCenterPos(backPos_UI_);
	gaugeFront_UI_->SetTextureCenterPos(backPos_UI_);

	isFadeIn_ = true;
	time_ = 0.0f;
	moveTime_ = 1.0f;

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

	float raito = playerBodyCount / 8.0f;
	frontSize_UI_.x = raito;
	gaugeFront_UI_->SetUvDrawRange(frontSize_UI_);
	gaugeBack_UI_->SetTextureCenterPos(backPos_UI_);
	gaugeFront_UI_->SetTextureCenterPos(backPos_UI_);

	gaugeBack_UI_->Update();
	gaugeFront_UI_->Update();

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
	gaugeBack_UI_->Draw();
	gaugeFront_UI_->Draw();
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
		ImGui::DragFloat3("backPos_UI", &backPos_UI_.x, 1.0f);
		ImGui::TreePop();
	}
}
#endif