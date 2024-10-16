#include "TestScene.h"

TestScene::TestScene() {}
TestScene::~TestScene() {}

void TestScene::Finalize() {
}

void TestScene::Init() {
	// カメラ -------------------------------------------------------------------
	debugCamera_ = std::make_unique<DebugCamera>();
	
	// object -------------------------------------------------------------------
	testObj_ = std::make_unique<BaseGameObject>();
	testObj_->Init();
	testObj_->SetObject("test2.fbx");
	//testObj_->SetAnimater("./Engine/Resources/Develop/", "test2.fbx");

	testObj2_ = std::make_unique<BaseGameObject>();
	testObj2_->Init();
	testObj2_->SetObject("walk.gltf");
	testObj2_->SetAnimater("./Engine/Resources/Animation/", "walk.gltf", true);
	testObj2_->GetTransform()->SetTranslaion({ 1.0f, 0.0f, 0.0f });

	waterSpace_ = std::make_unique<WaterSpace>();
	waterSpace_->Init("./Game/Resources/Model/", "waterPlane.obj");

	triangle_ = Engine::CreateTriangle({
		{-1.0f, 0.0f, 0.0f, 1.0f},
		{0.0f, 1.0f, 0.0f, 1.0f},
		{1.0f, 0.0f, 0.0f, 1.0f},
									   }, "uvChecker.png");
	
	// effect -------------------------------------------------------------------
	trail_ = std::make_unique<Trail>();
	trail_->Init();

	// manager -------------------------------------------------------------------

	collisionManager_ = std::make_unique<CollisionManager>();

	adjustment_->GetInstance()->Init("testScene");
}

void TestScene::Load() {
	ModelManager::LoadModel("./Engine/Resources/Develop/", "test2.fbx");
	ModelManager::LoadModel("./Engine/Resources/Animation/", "walk.gltf");
	// modelのload
	ModelManager::LoadModel("./Engine/Resources/Develop/", "SquarePyramid.obj");
	ModelManager::LoadModel("./Engine/Resources/Develop/", "skin.obj");
	ModelManager::LoadModel("./Engine/Resources/Develop/", "teapot.obj");
	ModelManager::LoadModel("./Engine/Resources/Develop/", "multiMaterial.obj");
	ModelManager::LoadModel("./Engine/Resources/Develop/", "Test_World.obj");

	ModelManager::LoadModel("./Game/Resources/Model/", "waterSpace.obj");
	ModelManager::LoadModel("./Game/Resources/Model/", "waterPlane.obj");
	TextureManager::LoadTextureFile("./Game/Resources/Model/", "normalMap3.png");
	
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
	testObj2_->Update();
	trail_->Update();
	trail_->AddTrail(testObj_->GetTransform()->GetTranslation());
	trail_->SetPlayerPosition(testObj_->GetTransform()->GetTranslation());

	triangle_->Update();

	waterSpace_->Update();
	
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
	
	Engine::SetPipeline(PipelineType::NormalPipeline);
	
	testObj_->Draw();
	testObj2_->Draw();
	Engine::SetPipeline(PipelineType::NormalPipeline);
	Render::DrawTriangle(triangle_.get());

	Engine::SetPipeline(PipelineType::AddPipeline);
	trail_->Draw();

	Engine::SetPipeline(PipelineType::PBRPipeline);
	waterSpace_->Draw();

	Render::SetRenderTarget(Sprite2D_RenderTarget);
	Engine::SetPipeline(PipelineType::SpritePipeline);

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
	if (isPause_) {
		ImGui::SameLine();
		if (ImGui::Button("step")) {
			isStepFrame_ = true;
		}
	}

	testObj_->Debug_Gui();

	waterSpace_->Debug_Gui();

	debugCamera_->Debug_Gui();

	if (ImGui::TreeNode("triangle")) {
		triangle_->Debug_Gui();
		ImGui::TreePop();
	}
	ImGui::End();
}
#endif
