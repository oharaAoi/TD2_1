#include "TestScene.h"

TestScene::TestScene() {}
TestScene::~TestScene() {}

void TestScene::Finalize() {
}

void TestScene::Init() {
	// カメラ -------------------------------------------------------------------
	debugCamera_ = std::make_unique<DebugCamera>();
	
	testObj_ = std::make_unique<BaseGameObject>();
	testObj_->Init();
	testObj_->SetObject("walk.gltf");
	testObj_->SetAnimater("./Engine/Resources/Animation/", "walk.gltf");

	collisionManager_ = std::make_unique<CollisionManager>();

	range_ = { 222, 222 };
	leftTop_ = { 0,0 };
	sprite_ = Engine::CreateSprite({ 200, 200 }, { 222, 222 });
	sprite_->SetTexture("sample.png");

	/*placementObjEditer_ = std::make_unique<PlacementObjectEditer>();
	placementObjEditer_->Init();*/

	adjustment_->GetInstance()->Init("testScene");
}

void TestScene::Load() {
	// modelのload
	ModelManager::LoadModel("./Engine/Resources/Develop/", "plane.obj");
	ModelManager::LoadModel("./Engine/Resources/Develop/", "SquarePyramid.obj");
	ModelManager::LoadModel("./Engine/Resources/Develop/", "skin.obj");
	ModelManager::LoadModel("./Engine/Resources/Develop/", "teapot.obj");
	ModelManager::LoadModel("./Engine/Resources/Develop/", "Test_World.obj");

	ModelManager::LoadModel("./Engine/Resources/Animation/", "walk.gltf");
	
	// textureのload
	TextureManager::LoadTextureFile("./Engine/Resources/Develop/", "uvChecker.png");
	TextureManager::LoadTextureFile("./Engine/Resources/Develop/", "sample.png");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 更新
//////////////////////////////////////////////////////////////////////////////////////////////////
void TestScene::Update() {
	// -------------------------------------------------
	// ↓ 一時停止時の処理
	// -------------------------------------------------
	if (isPause_) {
		isStepFrame_ = false;

#ifdef _DEBUG
		ImGuiDraw();
#endif
		// stepフラグが立っていたら1フレームだけ進める
		if (!isStepFrame_) {
			return;
		}
	}
		
	// -------------------------------------------------
	// ↓ オブジェクトの更新
	// -------------------------------------------------
	testObj_->Update();
	
	//placementObjEditer_->Update();

	sprite_->Update(range_, leftTop_);

	// -------------------------------------------------
	// ↓ Cameraの更新
	// -------------------------------------------------
	debugCamera_->Update();
	Render::SetEyePos(debugCamera_->GetWorldTranslate());
	Render::SetViewProjection(debugCamera_->GetViewMatrix(), debugCamera_->GetProjectionMatrix());
	Render::SetViewProjection2D(debugCamera_->GetViewMatrix2D(), debugCamera_->GetProjectionMatrix2D());

	// -------------------------------------------------
	// ↓ ParticleのViewを設定する
	// -------------------------------------------------
	EffectSystem::GetInstacne()->SetCameraMatrix(debugCamera_->GetCameraMatrix());
	EffectSystem::GetInstacne()->SetViewProjectionMatrix(debugCamera_->GetViewMatrix(), debugCamera_->GetProjectionMatrix());


	// -------------------------------------------------
	// ↓ Debugの表示
	// -------------------------------------------------
#ifdef _DEBUG
	if (!isStepFrame_) {
		ImGuiDraw();
	}
#endif

}

void TestScene::Draw() const {
	
	Engine::SetPipeline(PipelineKind::kPrimitivePiPeline);

#pragma region NormalPipeline

	Engine::SetPipeline(PipelineKind::kNormalPipeline);
	//placementObjEditer_->Draw();
	testObj_->Draw();
	
#pragma endregion

	Render::SetRenderTarget(Sprite2D_RenderTarget);
	Engine::SetPipeline(PipelineKind::kSpritePipeline);
	sprite_->Draw();

}

#ifdef _DEBUG
void TestScene::ImGuiDraw() {
	ImGui::Begin("GameObjects");
	if (ImGui::Button("stop")) {
		isPause_ = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("play")) {
		isPause_ = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("step")) {
		isStepFrame_ = true;
	}

	ImGui::DragFloat2("range", &range_.x, 1.0f);
	ImGui::DragFloat2("leftTop", &leftTop_.x, 1.0f);

	debugCamera_->Debug_Gui();
	ImGui::End();
}
#endif
