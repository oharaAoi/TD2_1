#include "GameScene.h"

GameScene::GameScene() {}
GameScene::~GameScene() {}

void GameScene::Finalize() {
	waterSpace_->Finalize();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Init() {
	AdjustmentItem::GetInstance()->Init("GameScene");

	// -------------------------------------------------
	// ↓ cameraの初期化
	// -------------------------------------------------
	camera_ = std::make_unique<Camera>();
	debugCamera_ = std::make_unique<DebugCamera>();

	// -------------------------------------------------
	// ↓ gameObjectの初期化
	// -------------------------------------------------
	ground_ = std::make_unique<Ground>();
	waterSpace_ = std::make_unique<WaterSpace>();
	waterSpace_->Init("./Game/Resources/Model/", "waterSpace.obj");

	player_ = std::make_unique<Player>();

	range_ = { 222, 222 };
	leftTop_ = {0,0};
	sprite_ = Engine::CreateSprite({200, 200} , {222, 222});
	sprite_->SetTexture("sample.png");

	// -------------------------------------------------
	// ↓ managerの初期化
	// -------------------------------------------------
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->AddCollider(player_.get());

	Engine::SetComputeShader(CSKind::GrayScale);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　読み込み
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Load() {
	ModelManager::LoadModel("./Game/Resources/Model/", "ground.obj");
	ModelManager::LoadModel("./Engine/Resources/Develop/", "skin.obj");

	TextureManager::LoadTextureFile("./Engine/Resources/Develop/", "uvChecker.png");
	TextureManager::LoadTextureFile("./Engine/Resources/Develop/", "sample.png");
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

	sprite_->Update();

	// -------------------------------------------------
	// ↓ 当たり判定を取る
	// -------------------------------------------------
	PlayerWaveCollision();

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
#pragma region Primitive
	Engine::SetPipeline(PipelineKind::kPrimitivePiPeline);
	if (Collider::isColliderBoxDraw_) {
		if (!isDebug_) {
			collisionManager_->Draw(camera_->GetViewMatrix() * camera_->GetProjectionMatrix());
		} else {
			collisionManager_->Draw(debugCamera_->GetViewMatrix() * debugCamera_->GetProjectionMatrix());
		}
	}
#pragma endregion

#pragma region NormalPipeline

	Engine::SetPipeline(PipelineKind::kNormalPipeline);
	ground_->Draw();
	player_->Draw();

#pragma endregion

#pragma region WaterSpace

	Engine::SetPipeline(PipelineKind::kWaterSpacePipeline);
	// このクラスは一番最後に描画
	waterSpace_->Draw();

#pragma endregion

	// CSを実行する
	Engine::RunCS();

#pragma region Sprite

	Render::SetRenderTarget(Sprite2D_RenderTarget);
	Engine::SetPipeline(PipelineKind::kSpritePipeline);
	sprite_->Draw();

#pragma endregion
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　波とPlayerの判定
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::PlayerWaveCollision() {
	float minLenght = 999;
	for (size_t oi = 0; oi < waterSpace_->GetWorldTopFaceList().size(); ++oi) {
		// playerのY座標と波の面のY座標との最短の距離を求める
		Vector3 distans = player_->GetTransform()->GetTranslation() - waterSpace_->GetWorldTopFaceList()[oi];
		distans.z = 0;
		float length = std::abs(distans.Length());
		if (length < minLenght) {
			minLenght = length;
		}
	}

	if (minLenght < player_->GetRadius()) {
		player_->SetHitWaterSurface(true);
	} else {
		player_->SetHitWaterSurface(false);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
void GameScene::Debug_Gui() {
	ImGui::Begin("GameScene");
	ImGui::Checkbox("isDebugCamera", &isDebug_);
	ImGui::Checkbox("debugColliderDraw", &Collider::isColliderBoxDraw_);
	player_->Debug_Gui();

	waterSpace_->Debug_Gui();

	if (!isDebug_) {
		camera_->Debug_Gui();
	}

	sprite_->Debug_Gui();
	ImGui::DragFloat2("range", &range_.x, 1.0f);
	ImGui::DragFloat2("leftTop", &leftTop_.x, 1.0f);

	ImGui::End();
}
#endif