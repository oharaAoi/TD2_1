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

	triangle_ = Engine::CreateTriangle({
		{-1.0f, 0.0f, 0.0f, 1.0f},
		{0.0f, 1.0f, 0.0f, 1.0f},
		{1.0f, 0.0f, 0.0f, 1.0f},
	}, "uvChecker.png");

	sprite_ = std::make_unique<Sprite>();
	sprite_ = Engine::CreateSprite("uvChecker.png");
	sprite_->SetCenterPos({ 300.0f, 300.0f });
	
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

	triangle_->Update();

	sprite_->Update();

	
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
	
	Engine::SetPipeline(PipelineType::NormalPipeline);
	Render::DrawTriangle(triangle_.get());

	Engine::SetPipeline(PipelineType::AddPipeline);
	
	Engine::SetPipeline(PipelineType::PBRPipeline);
	
	Render::SetRenderTarget(Sprite2D_RenderTarget);
	Engine::SetPipeline(PipelineType::SpritePipeline);
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
	if (isPause_) {
		ImGui::SameLine();
		if (ImGui::Button("step")) {
			isStepFrame_ = true;
		}
	}

	debugCamera_->Debug_Gui();

	if (ImGui::TreeNode("triangle")) {
		triangle_->Debug_Gui();
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("sprite")) {
		ImGui::Begin("Sprite");
		sprite_->Debug_Gui();
		ImGui::End();
		ImGui::TreePop();
	}
	ImGui::End();
}
#endif
