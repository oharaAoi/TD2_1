#include "LoadScene.h"
#include "Engine/Engine.h"
#include "Engine/Utilities/AdjustmentItem.h"

LoadScene::LoadScene() {}
LoadScene::~LoadScene() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void LoadScene::Init() {
	// -------------------------------------------------
	// ↓ 調整項目の登録
	// -------------------------------------------------
	AdjustmentItem* adjustItem = AdjustmentItem::GetInstance();
	adjustItem->AddItem(groupName_, "carpIconPos", carpIconPos_);
	adjustItem->AddItem(groupName_, "birdIconPos0", birdIconPos_[0]);
	adjustItem->AddItem(groupName_, "birdIconPos1", birdIconPos_[1]);
	adjustItem->AddItem(groupName_, "birdIconPos2", birdIconPos_[2]);

	AdaptAdjustmentItem();

	// -------------------------------------------------
	// ↓ objectの初期化
	// -------------------------------------------------
	// 背景
	background_ = Engine::CreateSprite("LoadBackGround.png");
	background_->SetCenterPos({ 640.0f, 360.0f });
	// 鯉
	carpIcon_ = Engine::CreateSprite("carpIcon.png");
	carpIcon_->SetCenterPos(carpIconPos_);
	// 鳥
	for (uint32_t oi = 0; oi < 3; ++oi) {
		birdIcon_[oi] = Engine::CreateSprite("birdIcon.png");
		birdIcon_[oi]->SetCenterPos(birdIconPos_[oi]);
	}
	// パネル
	fadePanel_ = std::make_unique<FadePanel>();
	fadePanel_->Init();
	// fadeInさせる
	if (fadePanel_->GetDoNoting()) {
		fadePanel_->SetFadeOutOpen(0.6f);
	}

	// -------------------------------------------------
	// ↓ Parameterの初期化
	// -------------------------------------------------
	firstCarpIconPos_ = carpIconPos_;
	birdIconPos_[3] = { 1280.0f, -30.0f };

	moveTime_ = 0.0f;
	moveLimit_ = 1.0f;

	movePosY_ = 0.0f;
	moveTheta_ = 0.0f;			// sin波
	moveAmplitude_ = 120.0f;	// 振幅

	moveIndex_ = 0;				// 移動するindex

	isMove_ = false;
	isLoadFinish_ = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void LoadScene::Update() {
	if (isLoadFinish_) {
		return;
	}

	fadePanel_->Update();

	if (fadePanel_->GetIsFinished()) {
		isMove_ = true;
		if (carpIconPos_.y <= -20.0f) {
			isLoadFinish_ = true;
			return;
		}
	}

	if (isMove_ && !isLoadFinish_) {
		CarpIconMove();
	}

	background_->Update();

	for (uint32_t oi = 0; oi < 3; ++oi) {
		birdIcon_[oi]->SetCenterPos(birdIconPos_[oi]);
		birdIcon_[oi]->Update();
	}
	carpIcon_->SetCenterPos(carpIconPos_);
	carpIcon_->Update();

#ifdef _DEBUG
	Debug_Gui();
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void LoadScene::Draw() const {
	background_->Draw();

	for (uint32_t oi = 0; oi < 3; ++oi) {
		birdIcon_[oi]->Draw();
	}

	carpIcon_->Draw();

	fadePanel_->Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　調整項目の適応
//////////////////////////////////////////////////////////////////////////////////////////////////

void LoadScene::AdaptAdjustmentItem() {
	AdjustmentItem* adjustItem = AdjustmentItem::GetInstance();
	carpIconPos_ = adjustItem->GetValue<Vector2>(groupName_, "carpIconPos");
	birdIconPos_[0] = adjustItem->GetValue<Vector2>(groupName_, "birdIconPos0");
	birdIconPos_[1] = adjustItem->GetValue<Vector2>(groupName_, "birdIconPos1");
	birdIconPos_[2] = adjustItem->GetValue<Vector2>(groupName_, "birdIconPos2");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　鯉のアイコンを移動させる
//////////////////////////////////////////////////////////////////////////////////////////////////

void LoadScene::CarpIconMove() {
	moveTime_ += GameTimer::DeltaTime();
	float t = moveTime_ / moveLimit_;
	carpIconPos_ = Vector2::Lerp(firstCarpIconPos_, birdIconPos_[moveIndex_], t);
	moveTheta_ = std::lerp(0.0f, PI, t);

	movePosY_ = std::sinf(moveTheta_) * moveAmplitude_;
	carpIconPos_.y -= movePosY_;

	if (moveTime_ >= moveLimit_) {
		firstCarpIconPos_ = birdIconPos_[moveIndex_];
		moveAmplitude_ += 60.0f;
		moveIndex_++;
		moveTime_ = 0.0f;
	}

	if (carpIconPos_.y <= -20.0f) {
		if (moveIndex_ >= 3) {
			if (isMove_) {
				isMove_ = false;
				fadePanel_->SetFadeOut(0.5f);
			}
		}
	}

	/*if (moveIndex_ >= 4) {
		isMove_ = false;
		fadePanel_->SetFadeOut(0.5f);
	}*/
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　編集処理
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG

void LoadScene::Debug_Gui() {
	ImGui::Begin("loadScene");
	ImGui::DragFloat2("carpIconPos", &carpIconPos_.x, 2.0f);
	ImGui::DragFloat2("birdIconPos0", &birdIconPos_[0].x, 2.0f);
	ImGui::DragFloat2("birdIconPos1", &birdIconPos_[1].x, 2.0f);
	ImGui::DragFloat2("birdIconPos2", &birdIconPos_[2].x, 2.0f);
	fadePanel_->Debug_Gui();

	if (ImGui::Button("Reset")) {
		AdjustmentItem* adjustItem = AdjustmentItem::GetInstance();
		firstCarpIconPos_ = adjustItem->GetValue<Vector2>(groupName_, "carpIconPos");
		moveAmplitude_ = 120.0f;
		movePosY_ = 0.0f;
		moveIndex_ = 0.0f;
		moveTime_ = 0.0f;
		isMove_ = true;
	}

	ImGui::End();
}
#endif