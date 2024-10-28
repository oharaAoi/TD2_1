#include "PlayerBodyCountUI.h"

PlayerBodyCountUI::PlayerBodyCountUI() {}
PlayerBodyCountUI::~PlayerBodyCountUI() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerBodyCountUI::Init() {
	uiPos_ = { -200, 250.0f };
	backPos_UI_ = { 165, 90.0f };
	
	frontSize_UI_ = { 0.0f, 1.0f };

	tailUIPos_ = { 100.0f, 100.0f };

	maxBody_UI_ = Engine::CreateSprite("bodyMax.png");
	maxBody_UI_->SetTextureCenterPos(uiPos_);

	gaugeBack_UI_ = Engine::CreateSprite("TorsoGaugeBack.png");
	gaugeFront_UI_ = Engine::CreateSprite("TorsoGaugeFront.png");
	gaugeBack_UI_->SetTextureCenterPos(backPos_UI_);
	gaugeFront_UI_->SetTextureCenterPos(backPos_UI_);

	head_UI_ = Engine::CreateSprite("KoiGeuge_Head.png");
	tail_UI_ = Engine::CreateSprite("KoiGeuge_Tail.png");
	head_UI_->SetCenterPos(headUIPos_);
	tail_UI_->SetCenterPos(tailUIPos_);

	isFadeIn_ = true;
	time_ = 0.0f;
	moveTime_ = 1.0f;

	fadeInStartPos_ = { -200, 250.0f };
	fadeOutPos_ = { 1500, 250.0f };

	effectMoveTime_ = 0.6f;

	interval_bodyUI_ = 60.0f;

	// -------------------------------------------------
	// ↓ Effectの初期化
	// -------------------------------------------------

	bodyReleseEffect_ = Engine::CreateSprite("KoiGeuge_Torso.png");

	dropVelocity_ = {0.0f, 0.0f};
	dropStartVelocity_ = {-2.0f, 1.0f};
	dropEndVelocity_ = {-0.5f, 2.0f};

	dropBodyColor_ = {1.0f, 1.0f, 1.0f, 1.0f};

	dropDownCount_ = 0.0f;
	dropDownTime_ = 0.6f;

	dropRotate_ = 6.0f;

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

	uint32_t index = 1;
	for (std::list<std::unique_ptr<Sprite>>::iterator it = body_UI_List_.begin(); it != body_UI_List_.end();) {
		(*it)->SetCenterPos({
		tailUIPos_.x + (interval_bodyUI_ * index),
		tailUIPos_.y
						  });
		(*it)->Update();
		++it;
		++index;
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

		(*it).effectSprite_->SetScale(Vector2::Lerp({ 0.0f, 0.0f }, { 1.5f, 1.5f }, EaseOutExpo(t)));
		(*it).alpha_ = std::lerp(1.0f, 0.0f, (t));

		(*it).color_ = { 1.0f, 1.0f, 1.0f, (*it).alpha_ };

		(*it).effectSprite_->SetColor((*it).color_);

		(*it).effectSprite_->Update();

		++it;
	}

	if (isDrop_) {
		DropBody();
		bodyReleseEffect_->Update();
	}
	
	if (!isUiMove_) { return; }

	Move();

	maxBody_UI_->SetTextureCenterPos(uiPos_);
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

	for (std::list<std::unique_ptr<Sprite>>::const_iterator it = body_UI_List_.begin(); it != body_UI_List_.end();) {
		(*it)->Draw();
		++it;
	}

	if (isDrop_) {
		bodyReleseEffect_->Draw();
	}

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
	auto& add = body_UI_List_.emplace_back(Engine::CreateSprite("KoiGeuge_Torso.png"));
	add->SetCenterPos({
		tailUIPos_.x + (interval_bodyUI_ * (body_UI_List_.size())),
		tailUIPos_.y
	});

	// 頭も同時に移動させておく
	headUIPos_ = add->GetCenterPos();
	headUIPos_.x += interval_bodyUI_;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　胴体のリストを追加
//////////////////////////////////////////////////////////////////////////////////////////////////

void PlayerBodyCountUI::ReleseBody() {
	Vector2 pos = body_UI_List_.back()->GetCenterPos();
	SetDrop(pos);

	// リストに最後に追加された要素を削除する
	body_UI_List_.pop_back();

	const auto& end = body_UI_List_.back();
	headUIPos_ = end->GetCenterPos();
	headUIPos_.x += interval_bodyUI_;
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
	rotate -= dropRotate_ * toRadian;
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