#include "TutorialScene.h"

TutorialScene::TutorialScene() {}
TutorialScene::~TutorialScene() {}

void TutorialScene::Finalize() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialScene::Init() {
	AdjustmentItem::GetInstance()->Init("Tutorial");

	// -------------------------------------------------
	// ↓ cameraの初期化
	// -------------------------------------------------
	camera_ = std::make_unique<Camera>();
	debugCamera_ = std::make_unique<DebugCamera>();

	// -------------------------------------------------
	// ↓ gameObjectの初期化
	// -------------------------------------------------
	player_ = std::make_unique<Player>();
	player_->Init();


	// -------------------------------------------------
	// ↓ worldObjectの初期化
	// -------------------------------------------------
	// 壁
	auto& worldWall = worldWalls_.emplace_back(std::make_unique<WorldWall>());
	worldWall->Init();
	// 水草
	auto& waterWeed = waterWeeds_.emplace_back(std::make_unique<Waterweed>());
	waterWeed->Init();
	waterWeed->SetWorldWallPos(StageInformation::worldWallPos_);
	// 地面
	auto& ground = grounds_.emplace_back(std::make_unique<Ground>());
	ground->GetTransform()->SetTranslaion(Vector3(0.0f, StageInformation::groundDepth_, 0.0f));
	// 水面
	auto& waterSpace = waterSpaces_.emplace_back(std::make_unique<WaterSpace>());
	waterSpace->Init("./Game/Resources/Model/Watersurface/", "Watersurface.obj");

	// -------------------------------------------------
	// ↓ ターゲットの設定
	// -------------------------------------------------
	camera_->SetPlayerPtr(player_.get());

	// -------------------------------------------------
	// ↓ その他メンバ変数の初期化
	// -------------------------------------------------
	stageLoopCount_ = 0;
	stageDeleteCount_ = 0;

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　読み込み
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialScene::Load() {
	ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player.fbx");
	ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player_Head.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player_Torso.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player_Tail.obj");

	ModelManager::LoadModel("./Game/Resources/Model/", "Item.obj");
	ModelManager::LoadModel("./Game/Resources/Model/", "Rock.obj");
	ModelManager::LoadModel("./Game/Resources/Model/", "Bird.obj");
	ModelManager::LoadModel("./Game/Resources/Model/", "Driftwood.obj");

	ModelManager::LoadModel("./Game/Resources/Model/", "Ripple.obj");
	ModelManager::LoadModel("./Game/Resources/Model/", "WaterColmn.obj");
	ModelManager::LoadModel("./Game/Resources/Model/", "Splash.obj");

	ModelManager::LoadModel("./Game/Resources/Model/WorldWall/", "WorldWall.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Coin/", "Coin.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/Fish/", "Fish.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/WaterWeed/", "Ground_WaterPlant.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Ground/", "Riverbed1.obj");

	// 仕様上連続して読み込みたい物
	ModelManager::LoadModel("./Game/Resources/Model/Watersurface/", "Watersurface.obj");
	TextureManager::LoadTextureFile("./Game/Resources/Model/", "normalMap.png");

	ModelManager::LoadModel("./Game/Resources/Model/", "ground.obj");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "WaterLight.png");

	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "number.png");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialScene::Update() {
	AdjustmentItem::GetInstance()->Update();

	// -------------------------------------------------
	// ↓ Cameraの更新
	// -------------------------------------------------
	if (!isDegugCameraActive_) {
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

	EndlessStage();
	for (std::unique_ptr<WorldWall>& wall : worldWalls_) {
		wall->Update();
	}
	for (std::unique_ptr<Waterweed>& weed : waterWeeds_) {
		weed->Update();
	}
	for (std::unique_ptr<Ground>& ground : grounds_) {
		ground->Update();
	}
	for (std::unique_ptr<WaterSpace>& waterSpace : waterSpaces_) {
		waterSpace->Update();
	}

	// -------------------------------------------------
	// ↓ Effectの更新
	// -------------------------------------------------
	CheckAddSplash();
	for (auto& splash : splash_) {
		splash->Update();
	}

	// -------------------------------------------------
	// ↓ 不要になった要素などの削除
	// -------------------------------------------------
	splash_.remove_if([](auto& splash) {return splash->GetIsEndSplash(); });

#ifdef _DEBUG
	Debug_Gui();
#endif 
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialScene::Draw() const {
	/////////////////////////////////
	// 3Dオブジェクトなどの表示(基本ここ)
	/////////////////////////////////
	Engine::SetPipeline(PipelineType::NormalPipeline);
	for (const std::unique_ptr<WorldWall>& wall : worldWalls_) {
		wall->Draw();
	}
	for (const std::unique_ptr<Waterweed>& weed : waterWeeds_) {
		weed->Draw();
	}
	for (auto& splash : splash_) {
		splash->Draw();
	}

	Engine::SetPipeline(PipelineType::WaterLightingPipeline);
	for (const std::unique_ptr<Ground>& ground : grounds_) {
		ground->Draw();
	}

	Engine::SetPipeline(PipelineType::NotCullingPipeline);
	player_->Draw();

	/////////////////////////////////
	// 水の表示
	/////////////////////////////////
	Engine::SetPipeline(PipelineType::NotCullingPipeline);
	// このクラスは一番最後に描画
	for (const std::unique_ptr<WaterSpace>& waterSpace : waterSpaces_) {
		waterSpace->Draw();
	}

	/////////////////////////////////
	// スプライトの表示
	/////////////////////////////////
	Render::SetRenderTarget(Sprite2D_RenderTarget);
	Engine::SetPipeline(PipelineType::SpritePipeline);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　条件を満たしたら水しぶきエフェクトを追加する
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialScene::CheckAddSplash() {
	if (player_->GetIsSplash()) {

		Vector3 emitPos = player_->GetTransform()->GetTranslation();
		emitPos.y = 0.01f;

		splash_.emplace_back(
			std::make_unique<Splash>(emitPos, 5.0f * (player_->GetMoveSpeed() / 50.0f))
		);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　ステージをループさせる
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialScene::EndlessStage() {
	// playerが一定間隔進んだら新しいステージを生成する
	if (player_->GetWorldTranslation().x > (StageInformation::stageWidthEvery_ * (stageLoopCount_ + 1)) - 400.0f) {
		++stageLoopCount_;
		// 新しく設置する座標を求める
		Vector3 newPos = StageInformation::worldWallPos_;
		newPos.x += StageInformation::stageWidthEvery_ * (stageLoopCount_);

		auto& worldWall = worldWalls_.emplace_back(std::make_unique<WorldWall>());
		worldWall->Init();
		worldWall->GetTransform()->SetTranslaion(newPos);
		// 水草
		auto& waterWeed = waterWeeds_.emplace_back(std::make_unique<Waterweed>());
		waterWeed->Init();
		waterWeed->SetWorldWallPos(newPos);
		// 地面
		auto& ground = grounds_.emplace_back(std::make_unique<Ground>());
		ground->GetTransform()->SetTranslaion(Vector3(newPos.x, StageInformation::groundDepth_, 0.0f));
		// 水面
		auto& waterSpace = waterSpaces_.emplace_back(std::make_unique<WaterSpace>());
		waterSpace->Init("./Game/Resources/Model/Watersurface/", "Watersurface.obj");
		waterSpace->SetTranslate({ newPos.x, 0.0f, 0.0f });
	}

	// playerが一定間隔進んだら古いステージを削除する
	if (player_->GetWorldTranslation().x > (StageInformation::stageWidthEvery_ * (stageDeleteCount_ + 1)) + 120.0f) {
		++stageDeleteCount_;
		worldWalls_.pop_front();
		waterWeeds_.pop_front();
		grounds_.pop_front();
		waterSpaces_.pop_front();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void TutorialScene::Debug_Gui() {
	ImGui::Begin("TutorialScene");
	ImGui::Text("worldWallsCount: %d", (int)worldWalls_.size());
	// カメラ
	{
		ImGui::Checkbox("isDebugCameraActive", &isDegugCameraActive_);
		if (ImGui::TreeNode("camera")) {
			if (!isDegugCameraActive_) {
				camera_->Debug_Gui();
			}
			debugCamera_->Debug_Gui();
			ImGui::TreePop();
		}
	}

	{
		if (ImGui::TreeNode("player")) {
			player_->Debug_Gui();
			ImGui::TreePop();
		}
	}

	ImGui::End();
}
#endif 