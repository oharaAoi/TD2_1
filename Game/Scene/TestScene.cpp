#include "TestScene.h"

TestScene::TestScene() {}
TestScene::~TestScene() {}

void TestScene::Init() {
	// カメラ -------------------------------------------------------------------
	camera_ = std::make_unique<Camera>();
	
	testObj_ = std::make_unique<BaseGameObject>();
	testObj_->Init();
	testObj_->SetObject("walk.gltf");
	testObj_->SetAnimater("./Engine/Resources/Animation/", "walk.gltf");

	world_ = std::make_unique<BaseGameObject>();
	world_->Init();
	world_->SetObject("Test_World.obj");

	camera_->SetTarget(testObj_->GetTransform());

	collisionManager_ = std::make_unique<CollisionManager>();

	placementObjEditer_ = std::make_unique<PlacementObjectEditer>();
	placementObjEditer_->Init();

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
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 更新
//////////////////////////////////////////////////////////////////////////////////////////////////
void TestScene::Update() {
	// -------------------------------------------------
	// ↓ オブジェクトの更新
	// -------------------------------------------------
	testObj_->Update();
	world_->Update();
	
	placementObjEditer_->Update();

	// -------------------------------------------------
	// ↓ Cameraの更新
	// -------------------------------------------------
	camera_->Update();
	Render::SetEyePos(camera_->GetWorldTranslate());
	Render::SetViewProjection(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());
	Render::SetViewProjection2D(camera_->GetViewMatrix2D(), camera_->GetProjectionMatrix2D());

	// -------------------------------------------------
	// ↓ ParticleのViewを設定する
	// -------------------------------------------------
	EffectSystem::GetInstacne()->SetCameraMatrix(camera_->GetCameraMatrix());
	EffectSystem::GetInstacne()->SetViewProjectionMatrix(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());


	// -------------------------------------------------
	// ↓ Debugの表示
	// -------------------------------------------------
#ifdef _DEBUG
	ImGuiDraw();
#endif

}

void TestScene::Draw() const {
	
	Engine::SetPipeline(PipelineKind::kPrimitivePiPeline);

#pragma region NormalPipeline

	Engine::SetPipeline(PipelineKind::kNormalPipeline);
	placementObjEditer_->Draw();
	world_->Draw();

#pragma endregion

#pragma region PBR

	Engine::SetPipeline(PipelineKind::kPBRPipeline);

#pragma endregion

#pragma region Sprite

	Engine::SetPipeline(PipelineKind::kSpritePipeline);
	
#pragma endregion
}

#ifdef _DEBUG
void TestScene::ImGuiDraw() {
	ImGui::Begin("GameObjects");
	world_->Debug_Gui();
	ImGui::End();

	camera_->Debug_Gui();
}
#endif
