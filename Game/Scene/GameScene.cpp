#include "GameScene.h"

GameScene::GameScene() {
}

GameScene::~GameScene() {
}

void GameScene::Init() {
	camera_ = std::make_unique<Camera>();

	soundData_ = Engine::LoadSE("Resources/Audio/fanfare.wav");
	bgmData_ = Engine::LoadBGM("Resources/Audio/fanfare.wav");
}

void GameScene::Load() {
}

void GameScene::Update() {
	// -------------------------------------------------
	// ↓ オブジェクトの更新
	// -------------------------------------------------

	// -------------------------------------------------
	// ↓ Cameraの更新
	// -------------------------------------------------
	camera_->Update();
	Render::SetEyePos(camera_->GetWorldTranslate());
	Render::SetViewProjection(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());

	// -------------------------------------------------
	// ↓ ParticleのViewを設定する
	// -------------------------------------------------
	EffectSystem::GetInstacne()->SetCameraMatrix(camera_->GetCameraMatrix());
	EffectSystem::GetInstacne()->SetViewProjectionMatrix(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());


#ifdef _DEBUG

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// ImGui
	//////////////////////////////////////////////////////////////////////////////////////////////////
	
#endif
}

void GameScene::Draw() const {
#pragma region NormalPipeline

	Engine::SetPipeline(PipelineKind::kNormalPipeline);

#pragma endregion

#pragma region Textureless

	Engine::SetPipeline(PipelineKind::kTexturelessPipeline);

#pragma endregion

#pragma region PBR

	Engine::SetPipeline(PipelineKind::kPBRPipeline);
	//Engine::DrawModel(sphereModel_.get());


#pragma endregion

#pragma region Particle

	Engine::SetPipeline(PipelineKind::kParticlePipeline);
	

#pragma endregion

	
}
