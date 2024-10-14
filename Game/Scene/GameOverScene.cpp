#include "GameOverScene.h"

GameOverScene::GameOverScene() {
}

GameOverScene::~GameOverScene() {
}

void GameOverScene::Finalize() {
}

void GameOverScene::Init() {
	camera_ = std::make_unique<BaseCamera>();
	camera_->Init();
}

void GameOverScene::Load() {
}

void GameOverScene::Update() {
	camera_->Update();

#ifdef _DEBUG
	Debug_Gui();
#endif 
}

void GameOverScene::Draw() const {
}

#ifdef _DEBUG
void GameOverScene::Debug_Gui() {
	ImGui::Begin("GameOverScene");
	if (ImGui::Button("ReTry")) {
		SetNextScene(SceneType::Scene_Game);
	}
	ImGui::End();
}
#endif 