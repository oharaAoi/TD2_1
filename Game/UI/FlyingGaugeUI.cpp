#include "FlyingGaugeUI.h"

FlyingGaugeUI::FlyingGaugeUI() {
}

FlyingGaugeUI::~FlyingGaugeUI() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void FlyingGaugeUI::Init() {
	outside_ = Engine::CreateSprite("RankBack.png");
	bar_ = Engine::CreateSprite("RankFront.png");
	rank_ = Engine::CreateSprite("Rank.png");
	icon_ = Engine::CreateSprite("RankIcon.png");

	bar_->SetIsDiscard(false);

	adjustmentItem_ = AdjustmentItem::GetInstance();
	groupName_ = "FlyingGaugeUI";
	// 登録
	adjustmentItem_->AddItem(groupName_, "outside_position", outside_->GetCenterPos());
	adjustmentItem_->AddItem(groupName_, "bar_position", bar_->GetCenterPos());
	adjustmentItem_->AddItem(groupName_, "rank_position", rank_->GetCenterPos());
	// 適応
	AdaptAdjustmentItem();

	rank_->SetTextureSize({ 384.0f, 100.0f });
	rank_->SetRectRange({ 384.0f, 100.0f });

	rankLenghtMin_ = 0.0f;
	rankLenghtMax_ = 100.0f;

	maxLenghtRaito_ = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void FlyingGaugeUI::Update(float currentLength) {
	CalculationRaito(currentLength);

	outside_->Update();
	bar_->Update();
	rank_->Update();
	icon_->Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void FlyingGaugeUI::Draw() const {
	outside_->Draw();
	bar_->Draw();

	if (static_cast<uint32_t>(nowMaxRank_) >= 1) {
		rank_->Draw();
	}

	if (nowMaxRank_ != FlyingRank::Rank_Master) {
		icon_->Draw();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　現在の割合を出す
//////////////////////////////////////////////////////////////////////////////////////////////////

void FlyingGaugeUI::CalculationRaito(float currentLength) {
	lengthRaito_ = currentLength / rankLenghtMax_;

	lengthRaito_ = std::clamp(lengthRaito_, 0.0f, 1.0f);

	Vector2 st = outside_->GetCenterPos() - (outside_->GetTextureSize() * 0.5f);
	Vector2 end = outside_->GetCenterPos() + (outside_->GetTextureSize() * 0.5f);
	st.y = outside_->GetCenterPos().y - 30.0f;
	end.y = outside_->GetCenterPos().y - 30.0f;
	Vector2 pos = Vector2::Lerp(st, end, lengthRaito_);
	icon_->SetCenterPos(pos);

	if (nowMaxRank_ != FlyingRank::Rank_Master) {
		if (maxLenghtRaito_ < lengthRaito_) {
			bar_->SetUvDrawRange({ (1.0f * lengthRaito_) ,1.0f });
			maxLenghtRaito_ = lengthRaito_;
		}

		if (lengthRaito_ >= 1.0f) {
			RankUp();
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　ランクの昇格を行う
//////////////////////////////////////////////////////////////////////////////////////////////////

void FlyingGaugeUI::RankUp() {
	maxLenghtRaito_ = 0.0f;
	switch (nowMaxRank_) {
	case FlyingRank::Rank_First:
		nowMaxRank_ = FlyingRank::Rank_B;
		rankLenghtMin_ = 101.0f;
		rankLenghtMax_ = 200.0f;
		rank_->SetLeftTop({ 0.0f, 0.0f });
		break;
	case FlyingRank::Rank_B:
		nowMaxRank_ = FlyingRank::Rank_A;
		rankLenghtMin_ = 501.0f;
		rankLenghtMax_ = 1000.0f;
		rank_->SetLeftTop({ 48.0f, 0.0f });
		break;
	case FlyingRank::Rank_A:
		nowMaxRank_ = FlyingRank::Rank_S;
		rankLenghtMin_ = 0.0f;
		rankLenghtMax_ = 1500.0f;
		rank_->SetLeftTop({ 96.0f, 0.0f });
		break;
	case FlyingRank::Rank_S:
		nowMaxRank_ = FlyingRank::Rank_Master;
		rank_->SetTexture("kari_UI_Rank_master.png");
		break;
	case FlyingRank::Rank_Master:

		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　調整項目の適応
//////////////////////////////////////////////////////////////////////////////////////////////////

void FlyingGaugeUI::AdaptAdjustmentItem() {
	outside_->SetTextureCenterPos(adjustmentItem_->GetValue<Vector2>(groupName_, "outside_position"));
	bar_->SetTextureCenterPos(adjustmentItem_->GetValue<Vector2>(groupName_, "bar_position"));
	rank_->SetTextureCenterPos(adjustmentItem_->GetValue<Vector2>(groupName_, "rank_position"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void FlyingGaugeUI::Debug_Gui() {
	if (ImGui::TreeNode("FlyingTGaugeUI")) {
		Vector2 outsidePos = outside_->GetCenterPos();
		Vector2 barPos = bar_->GetCenterPos();

		ImGui::DragFloat2("outsidePos", &outsidePos.x, 1.0f);
		ImGui::DragFloat2("barPos", &barPos.x, 1.0f);

		ImGui::DragFloat("raito", &lengthRaito_, 0.01f);

		bar_->Debug_Gui();

		outside_->SetCenterPos(outsidePos);
		bar_->SetCenterPos(barPos);

		// 再適応
		if (ImGui::Button("ReAdapt")) {
			AdaptAdjustmentItem();
		}

		ImGui::TreePop();
	}
}
#endif // _DEBUG
