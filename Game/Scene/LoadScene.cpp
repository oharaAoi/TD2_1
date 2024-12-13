#include "LoadScene.h"
#include "Engine/Utilities/AdjustmentItem.h"
#include "Engine/Audio/AudioPlayer.h"
#include "Engine/Math/MyRandom.h"
#include "Engine/Math/Easing.h"

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
	preCarpIconPos_ = carpIconPos_;
	birdIconPos_[3] = { 1280.0f, -50.0f };

	moveTime_ = 0.0f;
	moveLimit_ = 0.7f;

	movePosY_ = 0.0f;
	moveTheta_ = 0.0f;			// sin波
	moveAmplitude_ = 120.0f;	// 振幅

	moveIndex_ = 0;				// 移動するindex

	isMove_ = true;
	isLoadFinish_ = false;

	effectMoveTime_ = 0.5f;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void LoadScene::Update() {
	if (isLoadFinish_) {
		return;
	}
	// panelの更新
	fadePanel_->Update();

	// fadeが完了していたら
	if (fadePanel_->GetIsFinished()) {
		isMove_ = true;
		if (carpIconPos_.y <= -30.0f) {
			isLoadFinish_ = true;
			return;
		}
	}

	// 動いているかつsceneが終わって入なかったら
	if (isMove_ && !isLoadFinish_) {
		CarpIconMove();
	}

	// -------------------------------------------------
	// ↓ effectを更新する
	// -------------------------------------------------
	for (std::list<EffectData>::iterator it = birdJumpEffectList_.begin(); it != birdJumpEffectList_.end();) {
		(*it).moveTime_ += GameTimer::DeltaTime();
		float t = (*it).moveTime_ / effectMoveTime_;

		if ((*it).moveTime_ > effectMoveTime_) {
			it = birdJumpEffectList_.erase(it);
			continue;
		}

		(*it).effectSprite_->SetScale(Vector2::Lerp({ 0.0f, 0.0f }, { 0.5f, 0.5f }, EaseOutExpo(t)));
		(*it).alpha_ = std::lerp(1.0f, 0.0f, (t));
		(*it).color_ = { 1.0f, 1.0f, 1.0f, (*it).alpha_ };
		(*it).effectSprite_->SetColor((*it).color_);
		(*it).effectSprite_->Update();

		++it;
	}

	background_->Update();

	for (uint32_t oi = 0; oi < 3; ++oi) {
		birdIcon_[oi]->SetCenterPos(birdIconPos_[oi]);
		birdIcon_[oi]->Update();
	}
	
	carpIcon_->SetCenterPos(carpIconPos_);
	carpIcon_->Update();

	preCarpIconPos_ = carpIconPos_;

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

	for (std::list<EffectData>::const_iterator it = birdJumpEffectList_.begin(); it != birdJumpEffectList_.end();) {
		(*it).effectSprite_->Draw();
		++it;
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

		EmiteEffect();

		if (moveIndex_ == 3) {
			AudioPlayer::SinglShotPlay("AddSpeed.mp3", 0.5f);
		} else {
			AudioPlayer::SinglShotPlay("BirdJump_3.mp3", 0.4f);
		}
	}

	if (carpIconPos_.y <= -30.0f) {
		if (moveIndex_ >= 3) {
			if (isMove_) {
				isMove_ = false;
				fadePanel_->SetFadeOut(0.5f);
			}
		}
	}

	// 鯉の角度を求める
	Vector2 diffCarpPos = (carpIconPos_ - preCarpIconPos_);
	float angle = std::atan2f(diffCarpPos.y, diffCarpPos.x);
	carpIcon_->SetRotate(angle);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Effectの追加を行う
//////////////////////////////////////////////////////////////////////////////////////////////////

void LoadScene::EmiteEffect() {
	auto& effect = birdJumpEffectList_.emplace_back(EffectData());
	effect.effectSprite_->SetCenterPos(birdIconPos_[moveIndex_ - 1]);
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
	if (ImGui::TreeNode("fade")) {
		fadePanel_->Debug_Gui();
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("backGround")) {
		background_->Debug_Gui();
		ImGui::TreePop();
	}

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