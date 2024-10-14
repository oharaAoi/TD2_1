#include "GameScene.h"

float GameScene::groundDepth_ = -40.5f;

GameScene::GameScene(){}
GameScene::~GameScene(){}

void GameScene::Finalize(){
	for(auto& waterSpace : waterSpace_){
		waterSpace->Finalize();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Init(){

	AdjustmentItem::GetInstance()->Init("GameScene");

	// -------------------------------------------------
	// ↓ editorの初期化
	// -------------------------------------------------
	obstaclesManager_ = std::make_unique<ObstaclesManager>();
	obstaclesManager_->Init();
	if (StageInformation::GetStageNumMax() != 0) {
		obstaclesManager_->SetObstacles(StageInformation::GetStage());
	}
	
	placementObjectEditor_ = std::make_unique<PlacementObjectEditer>();
	placementObjectEditor_->Init(obstaclesManager_.get());

	// -------------------------------------------------
	// ↓ cameraの初期化
	// -------------------------------------------------
	camera_ = std::make_unique<Camera>();
	debugCamera_ = std::make_unique<DebugCamera>();

	// -------------------------------------------------
	// ↓ gameObjectの初期化
	// -------------------------------------------------

	for(int32_t i = 0; i < kStageModelCount_; i++){

		// 地面
		ground_[i] = std::make_unique<Ground>();
		ground_[i]->GetTransform()->SetTranslaion(Vector3(i * stageWidthEvery_, groundDepth_, 0.0f));

		// 水
		waterSpace_[i] = std::make_unique<WaterSpace>();
		waterSpace_[i]->Init("./Game/Resources/Model/", "waterSpace.obj");
		waterSpace_[i]->SetTranslate(Vector3(i * stageWidthEvery_, 0.0f, 0.0f));
	}

	player_ = std::make_unique<Player>();

	// trail
	trail_ = std::make_unique<Trail>();
	trail_->Init();


	// -------------------------------------------------
	// ↓ managerの初期化
	// -------------------------------------------------
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->AddCollider(player_.get());

	// -------------------------------------------------
	// ↓ ターゲットの設定
	// -------------------------------------------------
	camera_->SetPlayerPtr(player_.get());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　読み込み
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Load(){
	//ModelManager::LoadModel("./Engine/Resources/Develop/", "test3.fbx");
	ModelManager::LoadModel("./Engine/Resources/Develop/", "test2.fbx");
	ModelManager::LoadModel("./Game/Resources/Model/", "ground.obj");
	ModelManager::LoadModel("./Game/Resources/Model/", "Item.obj");
	ModelManager::LoadModel("./Game/Resources/Model/", "Fish.obj");
	ModelManager::LoadModel("./Game/Resources/Model/", "Rock.obj");
	ModelManager::LoadModel("./Engine/Resources/Develop/", "skin.obj");
	ModelManager::LoadModel("./Engine/Resources/Develop/", "teapot.obj");
	ModelManager::LoadModel("./Game/Resources/Model/", "waterSpace.obj");
	TextureManager::LoadTextureFile("./Game/Resources/Model/", "normalMap.png");

	TextureManager::LoadTextureFile("./Engine/Resources/Develop/", "uvChecker.png");
	TextureManager::LoadTextureFile("./Engine/Resources/Develop/", "sample.png");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Update(){
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
	// ↓ 一時停止時の処理
	// -------------------------------------------------
	if (Input::IsTriggerKey(DIK_ESCAPE)) {
		isPause_ = true;
	}
	if(isPause_) {
		isStepFrame_ = false;

	#ifdef _DEBUG
		Debug_Gui();
		placementObjectEditor_->Update();
		obstaclesManager_->Update();
	#endif
		// stepフラグが立っていたら1フレームだけ進める
		if(!isStepFrame_) {
			return;
		}
	}

	// -------------------------------------------------
	// ↓ オブジェクトの更新
	// -------------------------------------------------
	player_->Update();

	for(auto& ground : ground_){
		ground->SetPlayerVelocityX(player_->GetMoveVelocity().x);
		ground->Update();
	}

	for(auto& waterSpace : waterSpace_){
		waterSpace->Update();
	}

	//EndlessStage();

	obstaclesManager_->SetPlayerPosition(player_->GetWorldTranslation());
	obstaclesManager_->Update();

	// trail
	trail_->Update();
	trail_->AddTrail(player_->GetTransform()->GetTranslation());
	trail_->SetPlayerPosition(player_->GetTransform()->GetTranslation());

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
	if(!isStepFrame_) {
		Debug_Gui();

		// editorの処理
		placementObjectEditor_->Update();
	}

#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Draw() const{
#pragma region Primitive
	/////////////////////////////////
	// 線の描画
	/////////////////////////////////
	Engine::SetPipeline(PipelineType::PrimitivePipeline);

	// コライダーの表示
	if(Collider::isColliderBoxDraw_) {
		if(!isDegugCameraActive_) {
			collisionManager_->Draw(camera_->GetViewMatrix() * camera_->GetProjectionMatrix());
		} else {
			collisionManager_->Draw(debugCamera_->GetViewMatrix() * debugCamera_->GetProjectionMatrix());
		}
	}

#pragma endregion

#pragma region NormalPipeline

	/////////////////////////////////
	// 3Dオブジェクトなどの表示(基本ここ)
	/////////////////////////////////
	Engine::SetPipeline(PipelineType::NormalPipeline);

#ifdef _DEBUG

	// editorの描画
	placementObjectEditor_->Draw();

#endif // _DEBUG

	obstaclesManager_->Draw();

	for(auto& ground : ground_){
		ground->Draw();
	}
	Engine::SetPipeline(PipelineType::SkinningPipeline);
	player_->Draw();

	// effectの描画
	Engine::SetPipeline(PipelineType::AddPipeline);
	trail_->Draw();

#pragma endregion

#pragma region WaterSpace
	/////////////////////////////////
	// 水の表示
	/////////////////////////////////
	Engine::SetPipeline(PipelineType::PBRPipeline);
	// このクラスは一番最後に描画
	for(auto& waterSpace : waterSpace_){
		waterSpace->Draw();
	}

#pragma endregion

	// CSを実行する
	//Engine::RunCS();

#pragma region Sprite

	/////////////////////////////////
	// スプライトの表示
	/////////////////////////////////
	Render::SetRenderTarget(Sprite2D_RenderTarget);
	Engine::SetPipeline(PipelineType::SpritePipeline);

#pragma endregion
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　波とPlayerの判定
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::PlayerWaveCollision(){
	for(auto& waterSpace : waterSpace_){

		// デカい距離で初期化
		float minLength = 999;

		// 水面との距離を求める
		for(size_t oi = 0; oi < waterSpace->GetWorldTopFaceList().size(); ++oi) {
			// playerのY座標と波の面のY座標との最短の距離を求める
			Vector3 distans = player_->GetTransform()->GetTranslation() - waterSpace->GetWorldTopFaceList()[oi];
			distans.z = 0;
			float length = std::abs(distans.Length());
			if(length < minLength) {
				minLength = length;
			}
		}

		// 距離に応じた水との接触判定
		if(minLength < player_->GetRadius()) {
			player_->SetHitWaterSurface(true);
			break;
		} else {
			player_->SetHitWaterSurface(false);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　ステージをループさせる
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::EndlessStage(){

	for(int32_t i = 0; i < kStageModelCount_; i++){

		// 通り過ぎたかどうか判定
		if(ground_[i]->GetTransform()->GetTranslation().x <
			player_->GetTransform()->GetTranslation().x - stageWidthEvery_
			){

			Vector3 nextPos{};

			// 次の設置場所を求める
			if(i - 1 >= 0){
				nextPos = ground_[i - 1]->GetTransform()->GetTranslation()
					+ Vector3(stageWidthEvery_, 0.0f, 0.0f);
			} else{
				nextPos = ground_[kStageModelCount_ - 1]->GetTransform()->GetTranslation()
					+ Vector3(stageWidthEvery_, 0.0f, 0.0f);
			}

			// 配置
			ground_[i]->GetTransform()->SetTranslaion(nextPos);
			waterSpace_[i]->SetTranslate({ nextPos.x,0.0f,nextPos.z });
		}
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
void GameScene::Debug_Gui(){
	ImGui::Begin("GameScene");
	if (ImGui::Button("NextScene")) {
		SetNextScene(SceneType::Scene_Result);
	}
	ImGui::SameLine();
	if (ImGui::Button("GameOverScene")) {
		SetNextScene(SceneType::Scene_GameOver);
	}

	if(ImGui::Button("stop")) {
		isPause_ = true;
	}
	ImGui::SameLine();
	if(ImGui::Button("play")) {
		isPause_ = false;
	}
	if (isPause_) {
		ImGui::SameLine();
		if (ImGui::Button("step")) {
			isStepFrame_ = true;
		}
	}
	ImGui::End();

	ImGui::Begin("GameObjects");
	ImGui::Checkbox("isDebugCameraActive", &isDegugCameraActive_);
	ImGui::Checkbox("debugColliderDraw", &Collider::isColliderBoxDraw_);
	player_->Debug_Gui();

	for (auto& waterSpace : waterSpace_) {
		waterSpace->Debug_Gui();
	}

	if(!isDegugCameraActive_) {
		camera_->Debug_Gui();
	}

	debugCamera_->Debug_Gui();

	obstaclesManager_->Debug_Gui();

	ImGui::End();
}
#endif