#include "SelectScene.h"

SelectScene::SelectScene() {}
SelectScene::~SelectScene() {}

void SelectScene::Finalize() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void SelectScene::Init() {
	stageInformation_.Init();

	camera_ = std::make_unique<BaseCamera>();
	camera_->Init();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　読み込み
//////////////////////////////////////////////////////////////////////////////////////////////////

void SelectScene::Load() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新
//////////////////////////////////////////////////////////////////////////////////////////////////

void SelectScene::Update() {
	camera_->Update();

	ChangeStage();

#ifdef _DEBUG
	Debug_Gui();
#endif 
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画
//////////////////////////////////////////////////////////////////////////////////////////////////

void SelectScene::Draw() const {
}

///////////////////////////////////dddd///////////////////////////////////////////////////////////////
// ↓　シーンを変更する
//////////////////////////////////////////////////////////////////////////////////////////////////

void SelectScene::ChangeStage() {
	if (Input::IsTriggerKey(DIK_LEFT)) {
		selectStageNum_--;
	}
	if (Input::IsTriggerKey(DIK_RIGHT)) {
		selectStageNum_++;
	}
	selectStageNum_ = std::clamp(static_cast<int>(selectStageNum_), 0, static_cast<int>(stageInformation_.GetStageNumMax() - 1));

	StageInformation::SetNowStage(selectStageNum_);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　debug
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void SelectScene::Debug_Gui() {
	ImGui::Begin("ScelectScene");
	if (ImGui::Button("PreScene")) {
		SetNextScene(SceneType::Scene_Title);
	}
	ImGui::SameLine();
	if (ImGui::Button("NextScene")) {
		SetNextScene(SceneType::Scene_Game);
		StageInformation::SetNowStage(selectStageNum_);
	}

	ImGui::Text("nowStageNum:%d", selectStageNum_);
	ImGui::SameLine();
	if (ImGui::ArrowButton("##left", ImGuiDir_Left)) { selectStageNum_--; }
	ImGui::SameLine();
	if (ImGui::ArrowButton("##right", ImGuiDir_Right)) { selectStageNum_++; }
	selectStageNum_ = std::clamp(static_cast<int>(selectStageNum_), 0, static_cast<int>(stageInformation_.GetStageNumMax() - 1));

	ImGui::End();
}
#endif 