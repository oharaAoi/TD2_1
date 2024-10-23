#include "TutorialUI.h"

TutorialUI::TutorialUI() {
}

TutorialUI::~TutorialUI() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialUI::Init() {
	offsetLnegth_ = 300.0f;	// この距離にするといい感じの距離になる
	adjust_ = AdjustmentItem::GetInstance();
	groupName_ = "TutorialUI";
	adjust_->AddItem(groupName_, "offsetPos", offsetPos_);
	adjust_->AddItem(groupName_, "interval", interval_);
	adjust_->AddItem(groupName_, "offsetLnegth", offsetLnegth_);

	AdaptAdjustment();

	tutorialUI_.clear();

	tutorialUI_.try_emplace("kari", std::make_unique<BaseGameObject>());
	tutorialUI_["kari"]->Init();
	tutorialUI_["kari"]->SetObject("UI_Plane.obj");
	tutorialUI_["kari"]->SetTexture("Tutorial_1.png");
	tutorialUI_["kari"]->SetIsLighting(false);

	tutorialUI_.try_emplace("kari2", std::make_unique<BaseGameObject>());
	tutorialUI_["kari2"]->Init();
	tutorialUI_["kari2"]->SetObject("UI_Plane.obj");
	tutorialUI_["kari2"]->SetTexture("Tutorial_2.png");
	tutorialUI_["kari2"]->SetIsLighting(false);

	tutorialUI_.try_emplace("kari3", std::make_unique<BaseGameObject>());
	tutorialUI_["kari3"]->Init();
	tutorialUI_["kari3"]->SetObject("UI_Plane.obj");
	tutorialUI_["kari3"]->SetTexture("sky.png");
	tutorialUI_["kari3"]->SetIsLighting(false);

	tutorialUI_.try_emplace("kari4", std::make_unique<BaseGameObject>());
	tutorialUI_["kari4"]->Init();
	tutorialUI_["kari4"]->SetObject("TutorialGuide.obj");
	//tutorialUI_["kari4"]->SetTexture("sky.png");
	tutorialUI_["kari4"]->SetIsLighting(false);

	tutorialUI_.try_emplace("start", std::make_unique<BaseGameObject>());
	tutorialUI_["start"]->Init();
	tutorialUI_["start"]->SetObject("UI_Plane.obj");
	tutorialUI_["start"]->SetTexture("sky.png");
	tutorialUI_["start"]->SetIsLighting(false);

	float index = 0;
	for (auto& ui : tutorialUI_) {
		Vector3 pos = offsetPos_;
		pos.x += (interval_ * index)+offsetLnegth_;
		ui.second->GetTransform()->SetTranslaion(pos);
		ui.second->GetTransform()->SetQuaternion(Quaternion::AngleAxis(180.0f * toRadian, { 0.0f, 1.0f, 0.0f }));

		index++;
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

void TutorialUI::LineUpUI(const Vector3& playerPos) {
	// この処理はプレイヤーが
	if (!isLineUp_) {
		float index = 0;
		for (auto& ui : tutorialUI_) {
			Vector3 pos = offsetPos_;
			pos.x += (interval_ * index) + offsetLnegth_ + playerPos.x;
			ui.second->GetTransform()->SetTranslaion(pos);
			ui.second->GetTransform()->SetQuaternion(Quaternion::AngleAxis(180.0f * toRadian, { 0.0f, 1.0f, 0.0f }));

			index++;
		}
		isLineUp_ = true;
	} else {
		return;
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　調整項目の適応
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialUI::AdaptAdjustment() {
	offsetPos_ = adjust_->GetValue<Vector3>(groupName_, "offsetPos");
	interval_ = adjust_->GetValue<float>(groupName_, "interval");
	offsetLnegth_ = adjust_->GetValue<float>(groupName_, "offsetLnegth");
}

Vector3 TutorialUI::GetSessionFishPos() {
	return tutorialUI_["kari3"]->GetTransform()->GetTranslation();
}

Vector3 TutorialUI::GetSessionBirdPos() {
	return tutorialUI_["kari4"]->GetTransform()->GetTranslation();
}

Vector3 TutorialUI::GetStartPos() {
	return tutorialUI_["start"]->GetTransform()->GetTranslation();
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

		float index = 0;
		for (auto& ui : tutorialUI_) {
			Vector3 pos = offsetPos_;
			pos.x += (interval_ * index) + offsetLnegth_;
			ui.second->GetTransform()->SetTranslaion(pos);
			ui.second->GetTransform()->SetQuaternion(Quaternion::AngleAxis(180.0f * toRadian, { 0.0f, 1.0f, 0.0f }));

			index++;
		}
		ImGui::TreePop();
	}
}


#endif