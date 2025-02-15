#include "PlayerBodyCountUI.h"
#include "Engine/Audio/AudioPlayer.h"

PlayerBodyCountUI::PlayerBodyCountUI() {}
PlayerBodyCountUI::~PlayerBodyCountUI() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerBodyCountUI::Init() {
	maxUIPos = { -200, 160.0f };
	backPos_UI_ = { 165, 90.0f };

	frontSize_UI_ = { 0.0f, 1.0f };

	tailUIPos_ = { 52, 100.0f };

	maxBody_UI_ = Engine::CreateSprite("bodyMax.png");
	maxBody_UI_->SetTextureCenterPos(maxUIPos);
	maxBody_UI_->SetScale(Vector2(.7f, .7f));

	gaugeBack_UI_ = Engine::CreateSprite("TorsoGaugeBack.png");
	gaugeFront_UI_ = Engine::CreateSprite("TorsoGaugeFront.png");
	gaugeBack_UI_->SetTextureCenterPos(backPos_UI_);
	gaugeFront_UI_->SetTextureCenterPos(backPos_UI_);

	head_UI_ = Engine::CreateSprite("KoiGeuge_Head.png");
	tail_UI_ = Engine::CreateSprite("KoiGeuge_Tail.png");
	headUIPos_.x = 5;

	head_UI_->SetCenterPos(headUIPos_);
	tail_UI_->SetCenterPos(tailUIPos_);

	head_UI_->SetScale(bodyScale_);
	tail_UI_->SetScale(bodyScale_);

	isFadeIn_ = true;
	time_ = 0.0f;
	moveTime_ = 1.0f;

	fadeInStartPos_ = { -200, 160.0f };
	fadeOutPos_ = { 1500, 160.0f };

	effectMoveTime_ = 0.6f;

	interval_bodyUI_ = 36;

	for (std::list<BodyUIData>::iterator it = backBody_UI_List_.begin(); it != backBody_UI_List_.end();) {
		(*it).sprite_->SetScale(bodyScale_);
		(*it).sprite_->SetColor({ 1.0f, 1.0f, 1.0f, 0.4f });
		(*it).sprite_->Update();
		++it;
	}

	for (uint32_t oi = 0; oi < 7; ++oi) {
		auto& add = backBody_UI_List_.emplace_back(BodyUIData());
		add.sprite_->SetScale(bodyScale_);
		add.sprite_->SetCenterPos({
					tailUIPos_.x + (interval_bodyUI_ * (backBody_UI_List_.size()) + 6),
					tailUIPos_.y
		});
		headUIPos_ = { tailUIPos_.x + (interval_bodyUI_ * (backBody_UI_List_.size()) + 6),
					tailUIPos_.y };
	}

	headUIPos_.x += interval_bodyUI_ + 5;

	
	// -------------------------------------------------
	// ↓ Effectの初期化
	// -------------------------------------------------

	bodyReleseEffect_ = Engine::CreateSprite("KoiGeuge_Torso.png");

	dropVelocity_ = { 0.0f, 0.0f };
	dropStartVelocity_ = { -2.0f, 1.0f };
	dropEndVelocity_ = { -0.5f, 14.0f };

	dropBodyColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	dropDownCount_ = 0.0f;
	dropDownTime_ = 0.9f;

	dropRotate_ = 375.0f;

	isDrop_ = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerBodyCountUI::Update(int playerBodyCount) {
	// 8以上かつ動いて入なかったら
	if (playerBodyCount == 8) {
		if (preCount_ != 8) {
			isUiMove_ = true;
			AudioPlayer::SinglShotPlay("missionClear.mp3", 0.3f);
		}
	}

	if (preCount_ != playerBodyCount) {
		if (playerBodyCount > 1) {
			// 前フレームより少なかったら追加されている
			if (preCount_ < playerBodyCount) {
				AddBody();
			} else {
				ReleseBody();
			}
		}
	}

	float raito = playerBodyCount / 8.0f;
	frontSize_UI_.x = raito;
	gaugeFront_UI_->SetUvDrawRange(frontSize_UI_);
	gaugeBack_UI_->SetTextureCenterPos(backPos_UI_);
	gaugeFront_UI_->SetTextureCenterPos(backPos_UI_);

	gaugeBack_UI_->Update();
	gaugeFront_UI_->Update();

	head_UI_->SetTextureCenterPos(headUIPos_);
	tail_UI_->SetTextureCenterPos(tailUIPos_);

	head_UI_->Update();
	tail_UI_->Update();

	// -------------------------------------------------
	// ↓ bodyUIの更新を行う
	// -------------------------------------------------
	for (std::list<BodyUIData>::iterator it = body_UI_List_.begin(); it != body_UI_List_.end();) {
		// タイムが0.5f以下だったら
		if ((*it).time_ < 0.5f) {
			(*it).time_ += GameTimer::DeltaTime();
			float t = (*it).time_ / 0.5f;

			Vector2 scale;
			scale = Vector2::Lerp({ 0.0f, 0.0f }, bodyScale_, EaseOutBack(t));
			scale.x = std::clamp(scale.x, 0.0f, 1.4f);
			scale.y = std::clamp(scale.y, 0.0f, 1.4f);
			(*it).sprite_->SetScale(scale);
		}
		lastBodyPos_ = (*it).sprite_->GetCenterPos();
		lastBodyPos_.y = tailUIPos_.y;
		(*it).sprite_->Update();
		++it;
	}

	for (std::list<BodyUIData>::iterator it = backBody_UI_List_.begin(); it != backBody_UI_List_.end();) {
		(*it).sprite_->SetScale(bodyScale_);
		(*it).sprite_->SetColor({ 1.0f, 1.0f, 1.0f, 0.4f });
		(*it).sprite_->Update();
		++it;
	}

	preCount_ = playerBodyCount;

	// -------------------------------------------------
	// ↓ effectを更新する
	// -------------------------------------------------
	for (std::list<EffectData>::iterator it = effectList_.begin(); it != effectList_.end();) {
		(*it).moveTime_ += GameTimer::DeltaTime();
		float t = (*it).moveTime_ / effectMoveTime_;

		if ((*it).moveTime_ > effectMoveTime_) {
			it = effectList_.erase(it);
			continue;
		}

		(*it).effectSprite_->SetScale(Vector2::Lerp({ 0.0f, 0.0f }, { 0.5f, 0.5f }, EaseOutExpo(t)));
		(*it).alpha_ = std::lerp(1.0f, 0.0f, (t));

		(*it).color_ = { 1.0f, 1.0f, 1.0f, (*it).alpha_ };

		(*it).effectSprite_->SetColor((*it).color_);

		(*it).effectSprite_->SetCenterPos(lastBodyPos_);
		(*it).effectSprite_->Update();

		++it;
	}

	if (isDrop_) {
		DropBody();
		bodyReleseEffect_->Update();
	}

	if (isUiMove_) {
		Move();
	}

	maxBody_UI_->SetTextureCenterPos(maxUIPos);
	maxBody_UI_->Update();

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerBodyCountUI::Draw() const {

	// -------------------------------------------------
	// ↓ effectを描画する
	// -------------------------------------------------
	for (std::list<EffectData>::const_iterator it = effectList_.begin(); it != effectList_.end();) {
		(*it).effectSprite_->Draw();
		++it;
	}

	/*gaugeBack_UI_->Draw();
	gaugeFront_UI_->Draw();*/

	head_UI_->Draw();
	tail_UI_->Draw();

	for (std::list<BodyUIData>::const_iterator it = body_UI_List_.begin(); it != body_UI_List_.end();) {
		(*it).sprite_->Draw();
		++it;
	}

	for (std::list<BodyUIData>::const_iterator it = backBody_UI_List_.begin(); it != backBody_UI_List_.end();) {
		(*it).sprite_->Draw();
		++it;
	}

	if (isDrop_) {
		bodyReleseEffect_->Draw();
	}

	//if (!isUiMove_) { return; }
	maxBody_UI_->Draw();
}

void PlayerBodyCountUI::Move() {
	// fadeがtrueだったら画面外から画面ないへ
	time_ += GameTimer::DeltaTime();
	float t = time_ / moveTime_;
	if (isFadeIn_) {
		maxUIPos = Vector2::Lerp(fadeInStartPos_, Vector2(640, fadeInStartPos_.y), EaseOutElastic(t));
	} else {
		maxUIPos = Vector2::Lerp(Vector2(640, fadeInStartPos_.y), fadeOutPos_, EaseInOutBack(t));
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

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Effectを出現させる
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerBodyCountUI::EmiteEffect() {
	auto& effect = effectList_.emplace_back(EffectData());
	effect.effectSprite_->SetCenterPos(backPos_UI_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　胴体のリストを追加
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerBodyCountUI::AddBody() {
	auto& add = body_UI_List_.emplace_back(BodyUIData());
	add.sprite_->SetScale(bodyScale_);
	add.sprite_->SetCenterPos({
		tailUIPos_.x + (interval_bodyUI_ * (body_UI_List_.size()) + 6),
		tailUIPos_.y
							  });

	add.time_ = 0.0f;

	//// 頭も同時に移動させておく
	//headUIPos_ = add.sprite_->GetCenterPos();
	//headUIPos_.x += interval_bodyUI_ + 5;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　胴体のリストを追加
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerBodyCountUI::ReleseBody() {
	Vector2 pos = body_UI_List_.back().sprite_->GetCenterPos();
	SetDrop(pos);

	// リストに最後に追加された要素を削除する
	body_UI_List_.pop_back();

	/*const auto& end = body_UI_List_.back();
	headUIPos_ = end.sprite_->GetCenterPos();
	headUIPos_.x += interval_bodyUI_ + 5;*/
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　胴体が落ちる演出
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerBodyCountUI::DropBody() {
	dropDownCount_ += GameTimer::DeltaTime();
	float t = dropDownCount_ / dropDownTime_;

	// 座標を移動させる
	Vector2 pos = bodyReleseEffect_->GetCenterPos();
	pos += dropVelocity_;
	bodyReleseEffect_->SetCenterPos(pos);

	// 回転をさせる
	float rotate = bodyReleseEffect_->GetRotate();
	rotate -= dropRotate_ * toRadian * GameTimer::DeltaTime();
	bodyReleseEffect_->SetRotate(rotate);

	dropVelocity_ = Vector2::Lerp(dropStartVelocity_, dropEndVelocity_, EaseOutCubic(t));

	// 色を変える
	dropBodyAlpa_ = std::lerp(1.0f, 0.0f, EaseInQuint(t));
	dropBodyColor_.w = dropBodyAlpa_;
	bodyReleseEffect_->SetColor(dropBodyColor_);

	// 時間がたったら落ちるのをやめる
	if (dropDownCount_ > dropDownTime_) {
		isDrop_ = false;
	}
}

void PlayerBodyCountUI::SetDrop(const Vector2& pos) {
	bodyReleseEffect_->SetCenterPos(pos);
	dropDownCount_ = 0.0f;
	isDrop_ = true;
}

#ifdef _DEBUG
void PlayerBodyCountUI::Debug_Gui() {
	if (ImGui::TreeNode("playerBodyUI")) {
		ImGui::BulletText("maxUI");
		ImGui::DragFloat2("maxUI_position", &maxUIPos.x, 2.0f);

		ImGui::BulletText("bodyUI");
		ImGui::DragFloat3("backPos_UI", &backPos_UI_.x, 1.0f);
		ImGui::Spacing();
		ImGui::DragFloat3("dropStartVelocity", &dropStartVelocity_.x, 0.1f);
		ImGui::DragFloat3("dropEndVelocity", &dropEndVelocity_.x, 0.1f);
		ImGui::DragFloat("dropRotate", &dropRotate_, 1.0f, 0.0f, 360.0f);
		ImGui::DragFloat("interval_bodyUI", &interval_bodyUI_, 1.0f, 0.0f, 360.0f);
		ImGui::Spacing();
		ImGui::DragFloat2("headPos", &headUIPos_.x, 1.0f);
		ImGui::DragFloat2("tailPos", &tailUIPos_.x, 1.0f);

		if (ImGui::Button("isDrop")) {
			SetDrop(backPos_UI_);
		}
		ImGui::TreePop();
	}
}
#endif