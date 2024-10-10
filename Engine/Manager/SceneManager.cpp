#include "SceneManager.h"

SceneManager::SceneManager() { Init();}

SceneManager::~SceneManager() {}

void SceneManager::Finalize() {
	effectSystem_->Finalize();
	MeshManager::GetInstance()->Finalize();
	ModelManager::GetInstance()->Finalize();
	Engine::Finalize();
}

void SceneManager::Init() {
	Engine::Initialize(kWindowWidth_, kWindowHeight_);
	ModelManager::GetInstance()->Init();
	
	scene_ = std::make_unique<GameScene>();
	scene_->Load();
	scene_->Init();

	effectSystem_ = EffectSystem::GetInstacne();
	effectSystem_->Init();
}

void SceneManager::Run() {
	while (Engine::ProcessMessage()) {
		Engine::BeginFrame();

		Input::GetInstance()->Update();

		// ------------------------------------ //
		// 更新処理
		// ------------------------------------ //
		scene_->Update();

		effectSystem_->Update();

		Render::Update();

		// ------------------------------------ //
		// 描画処理
		// ------------------------------------ //

		scene_->Draw();

		Engine::SetPipeline(PipelineType::ParticlePipeline);
		effectSystem_->Draw();

		#ifdef _DEBUG
		if (effectSystem_->GetIsEffectEditer()) {
			effectSystem_->BeginEditer();
			effectSystem_->UpdateEditer();
			Engine::SetPipeline(PipelineType::ParticlePipeline);
			effectSystem_->DrawEditer();
		}
		#endif

		// ------------------------------------ //
		// フレーム終了処理
		// ------------------------------------ //
		Engine::DrawRenderTexture();
		Engine::EndImGui();

		#ifdef _DEBUG
		if (effectSystem_->GetIsEffectEditer()) {
			effectSystem_->EndEditer();
		}
		#endif

		Engine::EndFrame();

		//gameTimer_.WaitNextFrame();
	}

	Finalize();
}
