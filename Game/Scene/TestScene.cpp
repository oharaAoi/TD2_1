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
	testObj_->SetObject("skin.obj");
	//testObj_->SetAnimater("./Engine/Resources/Animation/", "walk.gltf");

	testObj2_ = std::make_unique<BaseGameObject>();
	testObj2_->Init();
	testObj2_->SetObject("walk.gltf");
	testObj2_->SetAnimater("./Engine/Resources/Animation/", "walk.gltf");
	testObj2_->GetTransform()->SetTranslaion({ 1.0f, 0.0f, 0.0f });

	waterSpace_ = std::make_unique<WaterSpace>();
	waterSpace_->Init("./Game/Resources/Model/", "waterPlane.obj");

	// effect -------------------------------------------------------------------
	trail_ = std::make_unique<Trail>();
	trail_->Init();

	// manager -------------------------------------------------------------------
	placementObjEditer_ = std::make_unique<PlacementObjectEditer>();
	placementObjEditer_->Init();

	collisionManager_ = std::make_unique<CollisionManager>();

	adjustment_->GetInstance()->Init("testScene");
}

void TestScene::Load() {
	// modelのload
	ModelManager::LoadModel("./Engine/Resources/Develop/", "SquarePyramid.obj");
	ModelManager::LoadModel("./Engine/Resources/Develop/", "skin.obj");
	ModelManager::LoadModel("./Engine/Resources/Develop/", "teapot.obj");
	ModelManager::LoadModel("./Engine/Resources/Develop/", "multiMaterial.obj");
	ModelManager::LoadModel("./Engine/Resources/Develop/", "Test_World.obj");

	ModelManager::LoadModel("./Game/Resources/Model/", "waterSpace.obj");
	ModelManager::LoadModel("./Game/Resources/Model/", "waterPlane.obj");
	TextureManager::LoadTextureFile("./Game/Resources/Model/", "normalMap3.png");

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
	testObj2_->Update();
	trail_->Update();
	trail_->AddTrail(testObj_->GetTransform()->GetTranslation());
	trail_->SetPlayerPosition(testObj_->GetTransform()->GetTranslation());

	waterSpace_->Update(0.0f);
	
	placementObjEditer_->Update();

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
	placementObjEditer_->Draw();
	testObj_->Draw();
	testObj2_->Draw();

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

	testObj_->SetColor({1.0f, 0.0f, 0.0f, 1.0f});

	testObj_->Debug_Gui();

	waterSpace_->Debug_Gui();

	debugCamera_->Debug_Gui();

	Vector3 point = Vector3(2.1f, -0.9f, 1.3f);
	Quaternion q = Quaternion::AngleAxis(0.45f, Vector3(1.0f, 0.4f, -0.2f).Normalize());
	Matrix4x4 qMat = q.MakeMatrix();

	Vector3 rottateByA = q * point;
	Vector3 B = Transform(point, qMat);

	ImGui::DragFloat3("A", &rottateByA.x, 1.0f);
	ImGui::DragFloat3("B", &B.x, 1.0f);

	ImGui::End();
}
#endif
