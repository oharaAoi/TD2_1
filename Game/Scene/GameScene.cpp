#include "GameScene.h"

GameScene::GameScene() {}
GameScene::~GameScene() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Init() {
	AdjustmentItem::GetInstance()->Init("GameScene");

	// -------------------------------------------------
	// ↓ カメラの更新
	// -------------------------------------------------
	camera_ = std::make_unique<Camera>();
	debugCamera_ = std::make_unique<DebugCamera>();

	// -------------------------------------------------
	// ↓ オブジェクトの初期化
	// -------------------------------------------------
	ground_ = std::make_unique<Ground>();
	waterSpace_ = std::make_unique<WaterSpace>();

	player_ = std::make_unique<Player>();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　読み込み
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Load() {
	ModelManager::LoadModel("./Game/Resources/Model/", "ground.obj");
	ModelManager::LoadModel("./Game/Resources/Model/", "waterSpace.obj");
	ModelManager::LoadModel("./Engine/Resources/Develop/", "skin.obj");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Update() {
	AdjustmentItem::GetInstance()->Update();

	// -------------------------------------------------
	// ↓ Cameraの更新
	// -------------------------------------------------
	if (!isDebug_) {
		camera_->Update();
		Render::SetEyePos(camera_->GetWorldTranslate());
		Render::SetViewProjection(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());
	} else {
		debugCamera_->Update();
		Render::SetEyePos(debugCamera_->GetWorldTranslate());
		Render::SetViewProjection(debugCamera_->GetViewMatrix(), debugCamera_->GetProjectionMatrix());
	}

	// -------------------------------------------------
	// ↓ オブジェクトの更新
	// -------------------------------------------------
	player_->Update();

	ground_->Update();
	waterSpace_->Update();

	// -------------------------------------------------
	// ↓ ParticleのViewを設定する
	// -------------------------------------------------
	EffectSystem::GetInstacne()->SetCameraMatrix(camera_->GetCameraMatrix());
	EffectSystem::GetInstacne()->SetViewProjectionMatrix(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());


#ifdef _DEBUG
	Debug_Gui();
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Draw() const {
#pragma region NormalPipeline

	Engine::SetPipeline(PipelineKind::kNormalPipeline);
	ground_->Draw();
	player_->Draw();

	// このクラスは一番最後に描画
	waterSpace_->Draw();

#pragma endregion

#pragma region PBR

	Engine::SetPipeline(PipelineKind::kPBRPipeline);
	//Engine::DrawModel(sphereModel_.get());
#pragma endregion
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
void GameScene::Debug_Gui() {
	ImGui::Begin("GameScene");
	player_->Debug_Gui();
	ImGui::End();
}
#endif