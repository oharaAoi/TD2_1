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

		// ------------------------------------ //
		// シーン変更があるか
		// ------------------------------------ //
		#ifdef _DEBUG
		ImGui::Begin("SceneManager");
		if (ImGui::Combo("Scene", &selectSceneNum_, "Game\0Test\0")) {
			isSceneChange_ = true;

		}
		ImGui::End();

		// ------------------------------------ //
		// EffectEditerの処理
		// ------------------------------------ //

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
		gameTimer_.WaitNextFrame();

		// ------------------------------------ //
		// シーン変更があったら
		// ------------------------------------ //
		#ifdef _DEBUG
		if (isSceneChange_) {
			switch (selectSceneNum_) {
			case SceneType::Scene_Game:
				scene_.reset(new GameScene);
				scene_->Load();
				scene_->Init();
				break;
			case SceneType::Scene_Test:
				scene_.reset(new TestScene);
				scene_->Load();
				scene_->Init();
				break;
			}
			isSceneChange_ = false;
		}
		#endif
	}

	Finalize();
}
