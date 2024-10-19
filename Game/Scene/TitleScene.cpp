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

	UI_title_ = std::make_unique<TitleUI>();
	UI_title_->Init();
}

void TitleScene::Load() {
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "title.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "kari.png");
}

void TitleScene::Update() {
	camera_->Update();
	Render::SetEyePos(camera_->GetWorldTranslate());
	Render::SetViewProjection(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());

	UI_title_->Update();

	if (Input::IsTriggerKey(DIK_1)) {
		SetNextScene(SceneType::Scene_Game);
	}

	if (Input::IsTriggerKey(DIK_2)) {
		SetNextScene(SceneType::Scene_Tutorial);
	}

#ifdef _DEBUG
	Debug_Gui();
#endif 
}

void TitleScene::Draw() const {

	Engine::SetPipeline(PipelineType::NormalPipeline);

	Render::SetRenderTarget(Sprite2D_RenderTarget);
	Engine::SetPipeline(PipelineType::SpritePipeline);
	UI_title_->Draw();
}

#ifdef _DEBUG
void TitleScene::Debug_Gui() {
	ImGui::Begin("TitleScene");
	ImGui::Text("tirggerKey : 1 [goGame]");
	ImGui::Text("tirggerKey : 2 [goTutorial]");

	{
		if (ImGui::TreeNode("title")) {
			UI_title_->Debug_Gui();
			ImGui::TreePop();
		}
	}
	ImGui::End();
}
#endif 