#include "TutorialUI.h"

TutorialUI::TutorialUI() {
}

TutorialUI::~TutorialUI() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialUI::Init(const Vector3& playerPos) {
	adjust_ = AdjustmentItem::GetInstance();

	groupName_ = "TutorialUI";
	adjust_->AddItem(groupName_, "offsetPos", offsetPos_);
	adjust_->AddItem(groupName_, "interval", interval_);

	tutorialUI_.try_emplace("kari", std::make_unique<BaseGameObject>());
	tutorialUI_["kari"]->Init();
	tutorialUI_["kari"]->SetObject("UI_Plane.obj");
	tutorialUI_["kari"]->SetTexture("sky.png");

	tutorialUI_.try_emplace("kari2", std::make_unique<BaseGameObject>());
	tutorialUI_["kari2"]->Init();
	tutorialUI_["kari2"]->SetObject("UI_Plane.obj");
	tutorialUI_["kari2"]->SetTexture("sky.png");

	tutorialUI_.try_emplace("kari3", std::make_unique<BaseGameObject>());
	tutorialUI_["kari3"]->Init();
	tutorialUI_["kari3"]->SetObject("UI_Plane.obj");
	tutorialUI_["kari3"]->SetTexture("sky.png");

	tutorialUI_.try_emplace("kari4", std::make_unique<BaseGameObject>());
	tutorialUI_["kari4"]->Init();
	tutorialUI_["kari4"]->SetObject("UI_Plane.obj");
	tutorialUI_["kari4"]->SetTexture("sky.png");

	for (auto& ui : tutorialUI_) {
		Vector3 pos = offsetPos_;
		pos.x += interval_;
		ui.second->GetTransform()->SetTranslaion(pos);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialUI::Update() {
	for (auto& ui : tutorialUI_) {
		ui.second->Update();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialUI::Draw() const {
	for (const auto& ui : tutorialUI_) {
		ui.second->Draw();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　編集
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialUI::LineUpUI() {
	tutorialUI_["kari"]->GetTransform()->SetTranslaion(offsetPos_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　調整項目の適応
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialUI::AdaptAdjustment() {
	offsetPos_ = adjust_->GetValue<Vector3>(groupName_, "offsetPos");
	interval_ = adjust_->GetValue<float>(groupName_, "interval");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　編集
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void TutorialUI::Debug_Gui() {
	if (ImGui::TreeNode("TutorialUI")) {
		ImGui::DragFloat3("offsetPos", &offsetPos_.x, 0.1f);
		ImGui::DragFloat("interval", &interval_, 1.0f);
		/*tutorialUI_["kari"]->Debug_Gui();*/

		for (auto& ui : tutorialUI_) {
			Vector3 pos = offsetPos_;
			pos.x += interval_;
			ui.second->GetTransform()->SetTranslaion(pos);
		}
		ImGui::TreePop();
	}
}
#endif