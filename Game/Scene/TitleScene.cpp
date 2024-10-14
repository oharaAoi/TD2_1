#include "TitleScene.h"

TitleScene::TitleScene() {
}

TitleScene::~TitleScene() {
}

void TitleScene::Finalize() {
}

void TitleScene::Init() {
	camera_ = std::make_unique<BaseCamera>();
	camera_->Init();
}

void TitleScene::Load() {
}

void TitleScene::Update() {
	camera_->Update();

#ifdef _DEBUG
	Debug_Gui();
#endif 
}

void TitleScene::Draw() const {
}

#ifdef _DEBUG
void TitleScene::Debug_Gui() {
	ImGui::Begin("TitleScene");
	if (ImGui::Button("NextScene")) {
		SetNextScene(SceneType::Scene_Select);
	}
	ImGui::End();
}
#endif 