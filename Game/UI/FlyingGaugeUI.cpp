#include "FlyingGaugeUI.h"

FlyingGaugeUI::FlyingGaugeUI() {
}

FlyingGaugeUI::~FlyingGaugeUI() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void FlyingGaugeUI::Init() {
	outside_ = Engine::CreateSprite("UI_flyingGaugeOut.png");
	bar_ = Engine::CreateSprite("kari_UI_bar.png");

	bar_->SetPivot({ 1.0f, 1.0f }); 

	adjustmentItem_ = AdjustmentItem::GetInstance();
	groupName_ = "FlyingGaugeUI";
	// 登録
	adjustmentItem_->AddItem(groupName_, "outside_position", outside_->GetCenterPos());
	adjustmentItem_->AddItem(groupName_, "bar_position", bar_->GetCenterPos());
	// 適応
	AdaptAdjustmentItem();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void FlyingGaugeUI::Update(float currentLength, float maxLength) {
	outside_->Update();
	bar_->Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void FlyingGaugeUI::Draw() const {
	outside_->Draw();
	bar_->Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　調整項目の適応
//////////////////////////////////////////////////////////////////////////////////////////////////

void FlyingGaugeUI::AdaptAdjustmentItem() {
	outside_->SetTextureCenterPos(adjustmentItem_->GetValue<Vector2>(groupName_, "outside_position"));
	bar_->SetTextureCenterPos(adjustmentItem_->GetValue<Vector2>(groupName_, "bar_position"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

void FlyingGaugeUI::Debug_Gui() {
	if (ImGui::TreeNode("FlyingTGaugeUI")) {
		Vector2 outsidePos = outside_->GetCenterPos();
		Vector2 barPos = bar_->GetCenterPos();

		ImGui::DragFloat2("outsidePos", &outsidePos.x, 1.0f);
		ImGui::DragFloat2("barPos", &barPos.x, 1.0f);

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
