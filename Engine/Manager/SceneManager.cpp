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
		if (ImGui::Combo("Scene", &selectSceneNum_, "Title\0Select\0Game\0Result\0GameOver\0Test\0")) {
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
		gameTimer_.FPS();

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

		// fpsの計算
		gameTimer_.CalculationFrame();

		if (scene_->GetType()) {
			SetChange();
			scene_->SetNextScene(std::nullopt);
		}

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

void SceneManager::SetChange() {
	GetScene();
	scene_->Load();
	scene_->Init();
}

void SceneManager::GetScene() {
	switch (scene_->GetType().value()) {
	case SceneType::Scene_Title:
		scene_.reset(new TitleScene);
		break;
	case SceneType::Scene_Select:
		scene_.reset(new SelectScene);
		break;
	case SceneType::Scene_Game:
		scene_.reset(new GameScene);
		break;
	case SceneType::Scene_Result:
		scene_.reset(new ResultScene);
		break;
	case SceneType::Scene_GameOver:
		scene_.reset(new GameOverScene);
		break;
	case SceneType::Scene_Test:
		scene_.reset(new TestScene);
		break;
	}
}
