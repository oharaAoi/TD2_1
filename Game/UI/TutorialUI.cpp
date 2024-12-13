#include "TutorialUI.h"

TutorialUI::TutorialUI() {
}

TutorialUI::~TutorialUI() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialUI::Init() {

	// このスケールで全体のUIのスケールを統一する
	scaleUpStrength_ = 1.2f;
	scaleUp_ = { scaleUpStrength_, scaleUpStrength_, scaleUpStrength_ };

	// この位置を調整することで鳥を踏むUIを移動できる
	jumpTutorialOffsetPos_ = { 40.0f, 85.0f, 0.0f };

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
	tutorialUI_["kari3"]->SetTexture("Tutorial_4.png");
	tutorialUI_["kari3"]->SetIsLighting(false);

	tutorialUI_.try_emplace("kari4", std::make_unique<BaseGameObject>());
	tutorialUI_["kari4"]->Init();
	tutorialUI_["kari4"]->SetObject("TutorialGuide.obj");
	//tutorialUI_["kari4"]->SetTexture("sky.png");
	tutorialUI_["kari4"]->SetIsLighting(false);

	tutorialUI_.try_emplace("start", std::make_unique<BaseGameObject>());
	tutorialUI_["start"]->Init();
	tutorialUI_["start"]->SetObject("UI_Plane.obj");
	//tutorialUI_["start"]->SetTexture("UI_none.png");
	tutorialUI_["start"]->SetIsLighting(false);
	tutorialUI_["start"]->SetColor({ 0.0, 0.0,0.0,0.0f });

	// これがjumpUIのオブジェクト
	jumpTutorialUI_ = std::make_unique<BaseGameObject>();
	jumpTutorialUI_->Init();
	jumpTutorialUI_->SetObject("UI_Plane.obj");
	jumpTutorialUI_->SetIsLighting(false);
	jumpTutorialUI_->SetTexture("Tutorial_3.png");
	jumpTutorialUI_->GetTransform()->SetQuaternion(Quaternion::AngleAxis(180.0f * toRadian, { 0.0f, 1.0f, 0.0f }));

	jumpTutorialPos_ = tutorialUI_["kari4"]->GetTransform()->GetTranslation();

	float index = 0;
	for (auto& ui : tutorialUI_) {
		Vector3 pos = offsetPos_;
		pos.x += (interval_ * index) + offsetLnegth_;
		ui.second->GetTransform()->SetTranslaion(pos);
		ui.second->GetTransform()->SetQuaternion(Quaternion::AngleAxis(180.0f * toRadian, { 0.0f, 1.0f, 0.0f }));

		index++;
	}
	tutorialUI_["kari3"]->GetTransform()->SetTranslaion(tutorialUI_["kari3"]->GetTransform()->GetTranslation()+Vector3(100,0,0));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialUI::Update() {
	scaleUp_ = { scaleUpStrength_, scaleUpStrength_, scaleUpStrength_ };

	jumpTutorialPos_ = tutorialUI_["kari4"]->GetTransform()->GetTranslation() + jumpTutorialOffsetPos_;
	jumpTutorialUI_->GetTransform()->SetTranslaion(jumpTutorialPos_);
	jumpTutorialUI_->GetTransform()->SetScale(scaleUp_);
	jumpTutorialUI_->Update();
	
	for (auto& ui : tutorialUI_) {
		ui.second->GetTransform()->SetScale(scaleUp_);
		ui.second->Update();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialUI::Draw() const {
	jumpTutorialUI_->Draw();
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
		tutorialUI_["kari3"]->GetTransform()->SetTranslaion(tutorialUI_["kari3"]->GetTransform()->GetTranslation() + Vector3(100, 0, 0));

	} else {
		return;
	}
	tutorialUI_["start"]->GetTransform()->SetQuaternion(Quaternion::AngleAxis((-180.0f * toRadian), Vector3{ 0,1,0 }));
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
		ImGui::DragFloat3("jumpTutorialOffsetPos", &jumpTutorialOffsetPos_.x, 0.1f);
		ImGui::DragFloat3("offsetPos", &offsetPos_.x, 0.1f);
		ImGui::DragFloat("interval", &interval_, 1.0f);
		ImGui::DragFloat("scaleUpStrength", &scaleUpStrength_, 1.0f);
		
		jumpTutorialUI_->Debug_Gui();
		/*for (auto& ui : tutorialUI_) {
			Vector3 pos = offsetPos_;
			pos.x += (interval_ * index) + offsetLnegth_;
			ui.second->GetTransform()->SetTranslaion(pos);
			ui.second->GetTransform()->SetQuaternion(Quaternion::AngleAxis(180.0f * toRadian, { 0.0f, 1.0f, 0.0f }));

			index++;
		}*/
		ImGui::TreePop();
	}
}


#endif