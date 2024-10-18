#include "ResultScene.h"

ResultScene::ResultScene() {
}

ResultScene::~ResultScene() {
}

void ResultScene::Finalize() {
}

void ResultScene::Init() {
	camera_ = std::make_unique<BaseCamera>();
	camera_->Init();
}

void ResultScene::Load() {
}

void ResultScene::Update() {
	camera_->Update();

#ifdef _DEBUG
	Debug_Gui();
#endif 
}

void ResultScene::Draw() const {
}

#ifdef _DEBUG
void ResultScene::Debug_Gui() {
	ImGui::Begin("GameScene");
	if (ImGui::Button("ReTry")) {
		SetNextScene(SceneType::Scene_Game);
	}
	if (StageInformation::GetNowStage() < StageInformation::GetStageNumMax() - 1) {
		if (ImGui::Button("Next")) {
			SetNextScene(SceneType::Scene_Game);
			StageInformation::SetNowStage(StageInformation::GetNowStage() + 1);
		}
	}


	ImGui::End();
}
#endif 