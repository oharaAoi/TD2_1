#include "GameScene.h"

float GameScene::groundDepth_ = -44.0f;

GameScene::GameScene(){}
GameScene::~GameScene(){}

void GameScene::Finalize(){
	for (std::unique_ptr<WaterSpace>& waterSpace : waterSpaces_) {
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
	player_ = std::make_unique<Player>();

	// trail
	trail_ = std::make_unique<Trail>();
	trail_->Init();

	for (uint32_t oi = 0; oi < kStageMax_; ++oi) {
		Vector3 newPos = StageInformation::worldWallPos_;
		newPos.x += StageInformation::stageWidthEvery_ * (oi);

		worldWalls_[oi] = std::make_unique<WorldWall>();
		worldWalls_[oi]->Init();
		// 水草
		waterWeeds_[oi] = std::make_unique<Waterweed>();
		waterWeeds_[oi]->Init();
		waterWeeds_[oi]->SetWorldWallPos(StageInformation::worldWallPos_);
		// 地面
		grounds_[oi] = std::make_unique<Ground>();
		grounds_[oi]->GetTransform()->SetTranslaion(Vector3(0.0f, StageInformation::groundDepth_, 0.0f));
		// 水面
		waterSpaces_[oi] = std::make_unique<WaterSpace>();
		waterSpaces_[oi]->Init("./Game/Resources/Model/Watersurface/", "Watersurface.obj");

		worldWalls_[oi]->GetTransform()->SetTranslaion(newPos);
		waterWeeds_[oi]->GetTransform()->SetTranslaion(newPos);
		grounds_[oi]->GetTransform()->SetTranslaion(Vector3(newPos.x, StageInformation::groundDepth_, 0.0f));
		waterSpaces_[oi]->SetTranslate({ newPos.x, 0.0f, 0.0f });
	}


	// -------------------------------------------------
	// ↓ managerの初期化
	// -------------------------------------------------
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->AddCollider(player_.get());

	// -------------------------------------------------
	// ↓ UIの初期化
	// -------------------------------------------------
	flyingTimerUI_ = std::make_unique<FlyingTimerUI>();
	flyingTimerUI_->Init();

	flyingGaugeUI_ = std::make_unique<FlyingGaugeUI>();
	flyingGaugeUI_->Init();

	// -------------------------------------------------
	// ↓ ターゲットの設定
	// -------------------------------------------------
 	camera_->SetPlayerPtr(player_.get());

	//デバッグ用、モデル確認
	debugModel_= std::make_unique<BaseGameObject>();
	debugModel_->Init();
	debugModel_->SetObject("Mountain.obj");

	debugModel2_ = std::make_unique<BaseGameObject>();
	debugModel2_->Init();
	debugModel2_->SetObject("MountenTree.obj");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　読み込み
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Load(){
	ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player.fbx");
	ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player_Head.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player_Torso.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player_Tail.obj");

	ModelManager::LoadModel("./Game/Resources/Model/Wood/", "Wood.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Grass/", "Grass.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Grass/", "Grass2.obj");

	ModelManager::LoadModel("./Game/Resources/Model/", "Item.obj");
	ModelManager::LoadModel("./Game/Resources/Model/", "Rock.obj");
	ModelManager::LoadModel("./Game/Resources/Model/", "Bird.obj");
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
	ModelManager::LoadModel("./Game/Resources/Model/Ground/", "Riverbed.obj");

	// 仕様上連続して読み込みたい物
	ModelManager::LoadModel("./Game/Resources/Model/Watersurface/", "Watersurface.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/", "waterSpace.obj");
	TextureManager::LoadTextureFile("./Game/Resources/Model/", "normalMap.png");

	ModelManager::LoadModel("./Game/Resources/Model/", "ground.obj");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "WaterLight.png");

	// Texture
	TextureManager::LoadTextureFile("./Engine/Resources/Develop/", "uvChecker.png");
	TextureManager::LoadTextureFile("./Engine/Resources/Develop/", "sample.png");

	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "number.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "UI_flyingGaugeOut.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "kari_UI_bar.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "kari_UI_Rank_1.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "kari_UI_Rank_master.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "kari_UI_icon.png");


	//デバッグ用、モデル確認
	ModelManager::LoadModel("./Game/Resources/Model/Mountain/", "Mountain.obj");
	ModelManager::LoadModel("./Game/Resources/Model/MountenTree/", "MountenTree.obj");

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
		//デバッグ用、モデル確認
	debugModel_->Update();
	debugModel2_->Update();
	/*-------------- object -------------*/
 	player_->Update();

	EndlessStage();
	for (uint32_t oi = 0; oi < kStageMax_; ++oi) {
		worldWalls_[oi]->Update();
		waterWeeds_[oi]->Update();
		grounds_[oi]->Update();
		waterSpaces_[oi]->Update();
	}
	
	/*------------- manager -------------*/
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
		obstaclesManager_->Debug_Gui();
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
	// ↓ UIの更新
	// -------------------------------------------------
	flyingTimerUI_->Update(player_->GetFlyingTime(), player_->GetMaxFlyingTime());
	flyingGaugeUI_->Update(player_->GetFlyingTime());

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


	/////////////////////////////////
	// 3Dオブジェクトなどの表示(基本ここ)
	/////////////////////////////////
	Engine::SetPipeline(PipelineType::NormalPipeline);
	for (uint32_t oi = 0; oi < kStageMax_; ++oi) {
		worldWalls_[oi]->Draw();
		waterWeeds_[oi]->Draw();
	}
	debugModel_->Draw();
	debugModel2_->Draw();

	/////////////////////////////////
	// 線の描画
	/////////////////////////////////
	

	Engine::SetPipeline(PipelineType::WaterLightingPipeline);
	for (const std::unique_ptr<Ground>& ground : grounds_) {
		ground->Draw();
	}

	obstaclesManager_->Draw();

	Engine::SetPipeline(PipelineType::NormalPipeline);
	for(auto& splash : splash_){
		splash->Draw();
	}
	Engine::SetPipeline(PipelineType::PrimitivePipeline);
	// コライダーの表示
	if (Collider::isColliderBoxDraw_) {
		if (!isDegugCameraActive_) {
			collisionManager_->Draw(camera_->GetViewMatrix() * camera_->GetProjectionMatrix());
		}
		else {
			collisionManager_->Draw(debugCamera_->GetViewMatrix() * debugCamera_->GetProjectionMatrix());
		}
	}
#ifdef _DEBUG
	// editorの描画
	placementObjectEditor_->Draw();
#endif // _DEBUG

	Engine::SetPipeline(PipelineType::NotCullingPipeline);
	player_->Draw();

	/////////////////////////////////
	// Effectの描画
	/////////////////////////////////
	Engine::SetPipeline(PipelineType::AddPipeline);
	trail_->Draw();

	/////////////////////////////////
	// 水の表示
	/////////////////////////////////
	Engine::SetPipeline(PipelineType::NotCullingPipeline);
	// このクラスは一番最後に描画
	for (const std::unique_ptr<WaterSpace>& waterSpace : waterSpaces_) {
		waterSpace->Draw();
	}

	/////////////////////////////////
	// 3Dオブジェクトに対してCsを実行する
	/////////////////////////////////
	//Engine::RunCS();

	/////////////////////////////////
	// スプライトの表示
	/////////////////////////////////
	Render::SetRenderTarget(Sprite2D_RenderTarget);
	Engine::SetPipeline(PipelineType::SpritePipeline);
	gamePlayTimer_->Draw();
	flyingTimerUI_->Draw();
	flyingGaugeUI_->Draw();
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
	for(auto& waterSpace : waterSpaces_){

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
	// playerが一定間隔進んだら新しいステージを生成する
	if (player_->GetWorldTranslation().x < 3000.0f) {
		return;
	}

	if (player_->GetWorldTranslation().x > (StageInformation::stageWidthEvery_ * (stageLoopCount_ + 1)) + 200.0f) {
		++stageLoopCount_;
		size_t index = static_cast<size_t>(nowStageIndex_);
		// 新しく設置する座標を求める
		Vector3 newPos = StageInformation::worldWallPos_;
		newPos.x += StageInformation::stageWidthEvery_ * (stageLoopCount_ + 1);

		worldWalls_[index]->GetTransform()->SetTranslaion(newPos);
		waterWeeds_[index]->GetTransform()->SetTranslaion(newPos);
		grounds_[index]->GetTransform()->SetTranslaion(Vector3(newPos.x, StageInformation::groundDepth_, 0.0f));
		waterSpaces_[index]->SetTranslate({ newPos.x, 0.0f, 0.0f });

		nowStageIndex_ = !nowStageIndex_;
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
	//ImGui::DragFloat3()
	debugModel2_->Debug_Gui();
	if (ImGui::Button("NextScene")) {
		SetNextScene(SceneType::Scene_Result);
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

	ImGui::Checkbox("debugColliderDraw", &Collider::isColliderBoxDraw_);

	{
		ImGui::Checkbox("isDebugCameraActive", &isDegugCameraActive_);
		if (ImGui::TreeNode("Camera")) {
			if (!isDegugCameraActive_) {
				camera_->Debug_Gui();
			} else {
				debugCamera_->Debug_Gui();
			}
			ImGui::TreePop();
		}
	}

	{
		if (ImGui::TreeNode("UI")) {
			ImGui::Begin("UI");
			flyingTimerUI_->Debug_Gui();
			flyingGaugeUI_->Debug_Gui();
			ImGui::End();
			ImGui::TreePop();
		}
	}

	{
		if (ImGui::TreeNode("Player")) {
			player_->Debug_Gui();
			gamePlayTimer_->Debug_Gui();
			ImGui::TreePop();
		}
	}
	ImGui::End();
}
#endif