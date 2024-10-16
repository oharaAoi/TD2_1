#include "GameScene.h"

float GameScene::groundDepth_ = -44.0f;

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

	gamePlayTimer_ = std::make_unique<GamePlayTimer>();
	gamePlayTimer_->Init(999.0f);

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
		waterSpace_[i]->Init("./Game/Resources/Model/Watersurface/", "Wotersurface.obj");
		waterSpace_[i]->SetTranslate(Vector3(i * stageWidthEvery_, 0.0f, 0.0f));
	}

	player_ = std::make_unique<Player>();

	// trail
	trail_ = std::make_unique<Trail>();
	trail_->Init();

	worldWall_ = std::make_unique<WorldWall>();
	worldWall_->Init();

	waterWeed_ = std::make_unique<BaseGameObject>();
	waterWeed_->Init();
	waterWeed_->SetObject("Ground_WaterPlant.obj");
	waterWeed_->GetTransform()->SetTranslaion(worldWall_->GetTransform()->GetTranslation());
	waterWeed_->SetColor({ 0.0f, 1.0f, 0.0f, 1.0f });

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
	ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player.fbx");
	ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player_Head.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player_Torso.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player_Tail.obj");

	ModelManager::LoadModel("./Game/Resources/Model/", "Item.obj");
	ModelManager::LoadModel("./Game/Resources/Model/", "Rock.obj");
	ModelManager::LoadModel("./Game/Resources/Model/", "Bird.obj");
	ModelManager::LoadModel("./Game/Resources/Model/", "Driftwood.obj");
	ModelManager::LoadModel("./Game/Resources/Model/", "Waterweed.obj");
	
	ModelManager::LoadModel("./Game/Resources/Model/", "Ripple.obj");
	ModelManager::LoadModel("./Game/Resources/Model/", "WaterColmn.obj");
	ModelManager::LoadModel("./Game/Resources/Model/", "Splash.obj");

	ModelManager::LoadModel("./Engine/Resources/Develop/", "test2.fbx");
	ModelManager::LoadModel("./Engine/Resources/Develop/", "skin.obj");
	ModelManager::LoadModel("./Engine/Resources/Develop/", "teapot.obj");

	ModelManager::LoadModel("./Game/Resources/Model/WorldWall/", "WorldWall.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Coin/", "Coin.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/Fish/", "Fish.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/WaterWeed/", "Ground_WaterPlant.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Ground/", "Riverbed1.obj");

	// 仕様上連続して読み込みたい物
	ModelManager::LoadModel("./Game/Resources/Model/Watersurface/", "Wotersurface.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/", "waterSpace.obj");
	TextureManager::LoadTextureFile("./Game/Resources/Model/", "normalMap.png");

	ModelManager::LoadModel("./Game/Resources/Model/", "ground.obj");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "WaterLight.png");

	// Texture
	TextureManager::LoadTextureFile("./Engine/Resources/Develop/", "uvChecker.png");
	TextureManager::LoadTextureFile("./Engine/Resources/Develop/", "sample.png");

	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "number.png");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Update(){

	AdjustmentItem::GetInstance()->Update();

	// -------------------------------------------------
	// ↓ Cameraの更新
	// -------------------------------------------------
	if(!isDegugCameraActive_) {
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

	/*-------------- object -------------*/
 	player_->Update();

	for(auto& ground : ground_){
		ground->SetPlayerVelocityX(player_->GetMoveVelocity().x);
		ground->Update();
	}

	for(auto& waterSpace : waterSpace_){
		waterSpace->Update();
	}

	worldWall_->Update();
	waterWeed_->Update();
	
	/*------------- manager -------------*/
	obstaclesManager_->Debug_Gui();
	obstaclesManager_->SetPlayerPosition(player_->GetWorldTranslation());
	obstaclesManager_->Update();

	/*-------------- effect -------------*/
	trail_->Update();
	trail_->AddTrail(player_->GetTransform()->GetTranslation());
	trail_->SetPlayerPosition(player_->GetTransform()->GetTranslation());

	CheckAddSplash();
	for(auto& splash : splash_){
		splash->Update();
	}


#ifdef _DEBUG
	if(!isStepFrame_) {
		Debug_Gui();

		// editorの処理
		placementObjectEditor_->Update();
	}
#endif

	// -------------------------------------------------
	// ↓ 開始時にコライダーのリストを更新する
	// -------------------------------------------------

	UpdateColliderList();

	// -------------------------------------------------
	// ↓ 当たり判定を取る
	// -------------------------------------------------
	PlayerWaveCollision();
	collisionManager_->SetPlayerPosition(player_->GetWorldTranslation());
	collisionManager_->CheckAllCollision();

	// -------------------------------------------------
	// ↓ 不要になった要素などの削除
	// -------------------------------------------------

	splash_.remove_if([](auto& splash){return splash->GetIsEndSplash(); });

	// -------------------------------------------------
	// ↓ ParticleのViewを設定する
	// -------------------------------------------------
	EffectSystem::GetInstacne()->SetCameraMatrix(camera_->GetCameraMatrix());
	EffectSystem::GetInstacne()->SetViewProjectionMatrix(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());


	if (player_->GetIsMove()) {
		gamePlayTimer_->Update();

		if (gamePlayTimer_->GetIsFinish()) {
			isPause_ = true;
		}
	}

	if (Input::IsTriggerKey(DIK_R)) {
		Init();
	}
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
	Engine::SetPipeline(PipelineType::NormalPipeline);//----------------//

	worldWall_->Draw();
	waterWeed_->Draw();

	Engine::SetPipeline(PipelineType::WaterLightingPipeline);//---------//
	for (auto& ground : ground_) {
		ground->Draw();
	}

	Engine::SetPipeline(PipelineType::NormalPipeline);//----------------//

#ifdef _DEBUG

	// editorの描画
	placementObjectEditor_->Draw();

#endif // _DEBUG

	obstaclesManager_->Draw();
	player_->Draw();

	for(auto& splash : splash_){
		splash->Draw();
	}

	/*------- effect -------*/
	Engine::SetPipeline(PipelineType::AddPipeline);//--------------------//
	trail_->Draw();


#pragma endregion

#pragma region WaterSpace
	/////////////////////////////////
	// 水の表示
	/////////////////////////////////
	Engine::SetPipeline(PipelineType::NormalPipeline);
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
	gamePlayTimer_->Draw();

#pragma endregion
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::UpdateColliderList(){

	collisionManager_->Reset();

	collisionManager_->AddCollider(player_.get());

	for (auto& obstacle : obstaclesManager_->GetPlacementObject()) {
		float lenght = std::abs((player_->GetWorldTranslation() - obstacle->GetWorldTranslation()).Length());
		if (lenght < obstaclesManager_->GetUpdateLenght()) {
			collisionManager_->AddCollider(obstacle.get());
		}
	}

#ifdef _DEBUG

	for (auto& obstacle : placementObjectEditor_->GetDebugPlacementObjs()) {
		float lenght = std::abs((player_->GetWorldTranslation() - obstacle.object_->GetWorldTranslation()).Length());
		if (lenght < obstaclesManager_->GetUpdateLenght()) {
			collisionManager_->AddCollider(obstacle.object_.get());
		}
	}

	for (auto& obstacle : placementObjectEditor_->GetInportPlacementObjs()) {
		float lenght = std::abs((player_->GetWorldTranslation() - obstacle.object_->GetWorldTranslation()).Length());
		if (lenght < obstaclesManager_->GetUpdateLenght()) {
			collisionManager_->AddCollider(obstacle.object_.get());
		}
	}

#endif // _DEBUG

	/*for(auto& fish : fish_){
		float lenght = std::abs((player_->GetWorldTranslation() - fish->GetWorldTranslation()).Length());
		if (lenght < 200.0f) {
			collisionManager_->AddCollider(fish.get());
		}
	}

	for(auto& item : items_){
		float lenght = std::abs((player_->GetWorldTranslation() - item->GetWorldTranslation()).Length());
		if (lenght < 200.0f) {
			collisionManager_->AddCollider(item.get());
		}
	}*/
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
// ↓　条件を満たしたら水しぶきエフェクトを追加する
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::CheckAddSplash(){
	if(player_->GetIsSplash()){

		Vector3 emitPos = player_->GetTransform()->GetTranslation();
		emitPos.y = 0.01f;

		splash_.emplace_back(
			std::make_unique<Splash>(emitPos, 5.0f * (player_->GetMoveSpeed()/50.0f))
		);
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
	if(isPause_) {
		ImGui::SameLine();
		if(ImGui::Button("step")) {
			isStepFrame_ = true;
		}
	}

	ImGui::Text("GetCoinNum: %d", player_->GetCoinNum());
	ImGui::SameLine();
	ImGui::Text(" / %d", obstaclesManager_->GetMaxCoins());

	for (auto& ground : ground_) {
		ground->Debug_Gui();
	}

	worldWall_->GetTransform()->Debug_Gui();

	ImGui::End();

	ImGui::Begin("GameObjects");
	ImGui::Checkbox("isDebugCameraActive", &isDegugCameraActive_);
	ImGui::Checkbox("debugColliderDraw", &Collider::isColliderBoxDraw_);
	player_->Debug_Gui();

	for(auto& waterSpace : waterSpace_) {
		waterSpace->Debug_Gui();
	}

	if(!isDegugCameraActive_) {
		camera_->Debug_Gui();
	}

	debugCamera_->Debug_Gui();

	gamePlayTimer_->Debug_Gui();

	ImGui::End();
}
#endif