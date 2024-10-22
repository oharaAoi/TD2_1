#include "GameScene.h"

/*---------- static initialize -----------*/
float GameScene::groundDepth_ = -44.0f;
GAME_STATE GameScene::currentState_ = GAME_STATE::TITLE;
GAME_STATE GameScene::preState_ = currentState_;


/*-------------- コンストラクタ・デストラクタ ---------------*/
GameScene::GameScene() {}
GameScene::~GameScene() { Finalize(); }

void GameScene::Finalize() {
	AnimetionEffectManager::GetInstance()->Finalize();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Init() {

	AdjustmentItem::GetInstance()->Init("GameScene");

	gamePlayTimer_ = std::make_unique<GamePlayTimer>();
	gamePlayTimer_->Init(40.0f);

	// -------------------------------------------------
	// ↓ editorの初期化
	// -------------------------------------------------
	player_ = std::make_unique<Player>();

	obstaclesManager_ = std::make_unique<ObstaclesManager>();
	obstaclesManager_->Init(player_.get());
	if (StageInformation::GetStageNumMax() != 0) {
		obstaclesManager_->SetObstacles(StageInformation::GetStage());
	}

	placementObjectEditor_ = std::make_unique<PlacementObjectEditer>();
	placementObjectEditor_->Init(obstaclesManager_.get());

	animationEffectManager_ = AnimetionEffectManager::GetInstance();

	// -------------------------------------------------
	// ↓ cameraの初期化
	// -------------------------------------------------
	camera_ = std::make_unique<Camera>();
	debugCamera_ = std::make_unique<DebugCamera>();

	// -------------------------------------------------
	// ↓ gameObjectの初期化
	// -------------------------------------------------
	// trail
	trail_ = std::make_unique<Trail>();
	trail_->Init();

	// worldObject
	worldObjects_ = std::make_unique<WorldObjects>();
	worldObjects_->Init();

	// 仕切り
	partition_ = std::make_unique<BaseGameObject>();
	partition_->Init();
	partition_->SetObject("Partition.obj");
	partition_->GetTransform()->SetScale({ 100.0f, 100.0f, 1.0f });
	partition_->GetTransform()->SetQuaternion(Quaternion::AngleAxis(3.14f, { 0.0f,1.0f,0.0f }));
	partition_->SetColor({ 0.0f,0.266f,0.349f,0.0f });
	partition_->SetIsLighting(false);
	partition_->Update();

	// -------------------------------------------------
	// ↓ managerの初期化
	// -------------------------------------------------
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->AddCollider(player_.get());

	// -------------------------------------------------
	// ↓ UI・スプライトの初期化
	// -------------------------------------------------
	flyingTimerUI_ = std::make_unique<FlyingTimerUI>();
	flyingTimerUI_->Init();

	flyingGaugeUI_ = std::make_unique<FlyingGaugeUI>();
	flyingGaugeUI_->Init();

	playerSpeedCounter_ = std::make_unique<PlayerSpeedCounter>();
	playerSpeedCounter_->Init();

	// 桜の花を散らすやつ
	cherryEmitter_ = std::make_unique<ParticleManager<Cherry>>();
	cherryEmitter_->SetEmitRange({ kWindowWidth_ * 0.5f,-kWindowHeight_ * 0.2f }, { kWindowWidth_,0.0f });
	cherryEmitter_->SetEmitCountEvery(2);

	// 終了時泡をだすやつ
	bubbleEmitter_ = std::make_unique<ParticleManager<Bubble>>();
	bubbleEmitter_->SetEmitRange({ 0.0f,kWindowHeight_}, { kWindowWidth_,kWindowHeight_ * 2.0f});
	bubbleEmitter_->SetEmitCountEvery(16);

	// 青空
	sky_ = Engine::CreateSprite("sky.png");
	sky_->SetLeftTop({ 0.0f,0.0f });
	sky_->SetCenterPos({ 640.0f, 360.0f });
	sky_->SetTextureSize({ 1280.0f,720.0f });
	sky_->SetRectRange({ 1280.0f,720.0f });

	// タイトルロゴ
	titleLogo_ = Engine::CreateSprite("titleLogo.png");
	titleLogo_->SetLeftTop({ 0.0f,0.0f });
	titleLogo_->SetCenterPos({ 640.0f, 360.0f });

	fade_ = Engine::CreateSprite("white.png");
	fade_->SetLeftTop({ 0.0f,0.0f });
	fade_->SetCenterPos({ 640.0f, 360.0f });
	fade_->SetTextureSize({ 2000.0f,2000.0f });

	// -------------------------------------------------
	// ↓ ターゲットの設定
	// -------------------------------------------------
	camera_->SetPlayerPtr(player_.get());

	// -------------------------------------------------
	// ↓ Audioの生成
	// -------------------------------------------------

	mainBGM_ = std::make_unique<AudioPlayer>();
	mainBGM_->Init("mainBGM_tobenaikoi.wav");

	// -------------------------------------------------
	// ↓ 背景のモデルの生成
	// -------------------------------------------------
	/*backgroundObjects_["mountain"] = std::make_unique<BaseGameObject>();
	backgroundObjects_["mountain"]->Init();
	backgroundObjects_["mountain"]->SetObject("Mountain.obj");


	backgroundObjects_["tree"] = std::make_unique<BaseGameObject>();
	backgroundObjects_["tree"]->Init();
	backgroundObjects_["tree"]->SetObject("MountenTree.obj");

	backgroundObjects_["grass"] = std::make_unique<BaseGameObject>();
	backgroundObjects_["grass"]->Init();
	backgroundObjects_["grass"]->SetObject("MountainGrass.obj");

	backgroundObjects_["cloud"] = std::make_unique<BaseGameObject>();
	backgroundObjects_["cloud"]->Init();
	backgroundObjects_["cloud"]->SetObject("Cloud.obj");
	backgroundObjects_["cloud"]->SetColor({ 1.0f,1.0f,1.0f,0.5f });
	backgroundObjects_["cloud"]->SetIsLighting(false);*/

	backgroundObjects_["moai"] = std::make_unique<BaseGameObject>();
	backgroundObjects_["moai"]->Init();
	backgroundObjects_["moai"]->SetObject("Moai.obj");

	backgroundObjects_["Nico"] = std::make_unique<BaseGameObject>();
	backgroundObjects_["Nico"]->Init();
	backgroundObjects_["Nico"]->SetObject("Nico.obj");

	//モデル確認用
	backgroundObjects_["wing"] = std::make_unique<BaseGameObject>();
	backgroundObjects_["wing"]->Init();
	backgroundObjects_["wing"]->SetObject("Wing.obj");
	backgroundObjects_["wing"]->SetIsLighting(false);

	backgroundObjects_["UFO"] = std::make_unique<BaseGameObject>();
	backgroundObjects_["UFO"]->Init();
	backgroundObjects_["UFO"]->SetObject("MountainUFO.obj");

	debugModel_ = std::make_unique<BaseGameObject>();
	debugModel_->Init();
	debugModel_->SetObject("DriftWoodDestroy.gltf");
	debugModel_->SetAnimater("./Game/Resources/Model/DriftWoodDestroy/", "DriftWoodDestroy.gltf", true);
	debugModel_->SetIsLighting(false);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　読み込み
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Load() {
	ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player.fbx");
	ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player_Head.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player_Torso.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player_Tail.obj");

	ModelManager::LoadModel("./Game/Resources/Model/Wood/", "Wood.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Grass/", "Grass.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Grass/", "Grass2.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Partition/", "Partition.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/Driftwood/", "Driftwood.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Driftwood/", "Driftwood2.obj");

	ModelManager::LoadModel("./Game/Resources/Model/", "Item.obj");
	ModelManager::LoadModel("./Game/Resources/Model/", "Rock.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Bird/", "Bird.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/", "Waterweed.obj");

	ModelManager::LoadModel("./Game/Resources/Model/", "Ripple.obj");
	ModelManager::LoadModel("./Game/Resources/Model/", "WaterColmn.obj");
	ModelManager::LoadModel("./Game/Resources/Model/", "Splash.obj");

	ModelManager::LoadModel("./Engine/Resources/Develop/", "skin.obj");

	ModelManager::LoadModel("./Game/Resources/Model/WorldWall/", "WorldWall.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Coin/", "Coin.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/Fish/", "Fish.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/WaterWeed/", "Ground_WaterPlant.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Ground/", "Riverbed.obj");

	ModelManager::LoadModel("./Game/Resources/Model/Trail/", "waterTrail.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Trail/", "skyTrail.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Effect/", "staer.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Effect/", "HighSpeedEffect.gltf");

	// 仕様上連続して読み込みたい物
	ModelManager::LoadModel("./Game/Resources/Model/Watersurface/", "Watersurface.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/", "waterSpace.obj");
	TextureManager::LoadTextureFile("./Game/Resources/Model/", "normalMap.png");

	ModelManager::LoadModel("./Game/Resources/Model/", "ground.obj");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "WaterLight.png");

	// Texture
	TextureManager::LoadTextureFile("./Engine/Resources/Develop/", "uvChecker.png");
	TextureManager::LoadTextureFile("./Engine/Resources/Develop/", "sample.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "softLight.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "bubble.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "cherry.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "titleLogo.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "sky.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "number.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "RankBack.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "RankFront.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "Rank.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "kari_UI_Rank_master.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "RankIcon.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "speedMeterBack.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "tani.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "MaterStaple.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "Mater.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "timer.png");

	TextureManager::LoadTextureFile("./Game/Resources/Model/", "FishCanEat.png");


	//デバッグ用、モデル確認
	ModelManager::LoadModel("./Game/Resources/Model/Mountain/", "Mountain.obj");
	ModelManager::LoadModel("./Game/Resources/Model/MountenTree/", "MountenTree.obj");
	ModelManager::LoadModel("./Game/Resources/Model/MountainGrass/", "MountainGrass.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Cloud/", "Cloud.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Moai/", "Moai.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Nico/", "Nico.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Wing/", "Wing.obj");
	ModelManager::LoadModel("./Game/Resources/Model/MountainUFO/", "MountainUFO.obj");

	// animationEffect
	ModelManager::LoadModel("./Game/Resources/Model/DriftWoodDestroy/", "DriftWoodDestroy.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/BirdJumpEffect/", "BirdJumpEffect.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/Effect1/", "Effect1.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/FishDestroy/", "FishDestroy.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/JumpEffect/", "JumpEffect.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/AddTorso/", "AddTorso.gltf");

	// Adio
	AudioManager::LoadAudio("./Game/Resources/Audio/", "test.wav");
	AudioManager::LoadAudio("./Game/Resources/Audio/", "kari.wav");
	AudioManager::LoadAudio("./Game/Resources/Audio/", "kari_coinGet.wav");

	// ○がついていない物はまだ使用していない
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "boost.mp3");				// ブースト音		○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "decrementBody.mp3");		// 体の数を減らす		○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "eat.mp3");				// エサを食べる		○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "eatAccel.wav");			// エサを食べた時加速する		○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "gameFinish.wav");		// gameFinish音
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "goalTarget.wav");		// 目標の距離を達成した音
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "hitedBird.wav");			// 鳥に当たった時の音		○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "incrementBody.mp3");		// 体の数を増やす			○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "inWaterSurface.wav");	// 水面に入った時の音			○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "outWaterSurface.wav");	// 水面から出た時の音			○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "jumpBird.wav");			// 鳥を踏んでジャンプ			○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "overTime.wav");			// オーバータイムの音
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "timeLeft_10s.wav");		// タイムアップ10秒前			○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "timeLeft_60s.wav");		// タイムアップ60秒前			○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "timeUp.wav");			// タイムアップの音			○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "updateFlyingLength.wav");// 飛行距離を伸ばした時の音

	AudioManager::LoadAudio("./Game/Resources/Audio/BGM/", "mainBGM_tobenaikoi.wav");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Update() {

	AdjustmentItem::GetInstance()->Update();


	if (Input::IsTriggerKey(DIK_SPACE)) {
		if (currentState_ == GAME_STATE::TITLE) {
			currentState_ = GAME_STATE::TUTORIAL;

		} else if (currentState_ == GAME_STATE::TUTORIAL) {
			currentState_ = GAME_STATE::GAME;
		}
	}

#ifdef _DEBUG

#endif // _DEBUG


	// -------------------------------------------------
	// ↓ Cameraの更新
	// -------------------------------------------------

	if (!isDegugCameraActive_) {
		camera_->Update();
		Render::SetEyePos(camera_->GetWorldTranslate());
		Render::SetViewProjection(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());
		Render::SetViewProjection2D(camera_->GetViewMatrix2D(), camera_->GetProjectionMatrix2D());
	} else {
		debugCamera_->Update();
		Render::SetEyePos(debugCamera_->GetWorldTranslate());
		Render::SetViewProjection(debugCamera_->GetViewMatrix(), debugCamera_->GetProjectionMatrix());
		Render::SetViewProjection2D(debugCamera_->GetViewMatrix2D(), debugCamera_->GetProjectionMatrix2D());
	}

	// -------------------------------------------------
	// ↓ 一時停止時の処理
	// -------------------------------------------------
	if (Input::IsTriggerKey(DIK_ESCAPE)) {
		isPause_ = true;
	}
	if (isPause_) {
		isStepFrame_ = false;

#ifdef _DEBUG
		Debug_Gui();
		placementObjectEditor_->Update();
		obstaclesManager_->Update();
#endif
		// stepフラグが立っていたら1フレームだけ進める
		if (!isStepFrame_) {
			return;
		}
	}

	// -------------------------------------------------
	// ↓ オブジェクトの更新
	// -------------------------------------------------
	/*-------------- object -------------*/
	player_->Update();
	debugModel_->Update();

	worldObjects_->LoopStage();
	worldObjects_->Update(player_->GetWorldTranslation().x);

	for (auto& backgroundObject : backgroundObjects_) {
		backgroundObject.second->Update();
	}


	if (currentState_ == GAME_STATE::TITLE) {

#ifdef _DEBUG

		ImGui::Begin("color");
		if (ImGui::ColorEdit4("color", &editColor_.x)) {
			partition_->SetColor(editColor_);
		};
		ImGui::End();

#endif // 

		Vector3 pos = player_->GetWorldTranslation();
		pos.z += 10.0f;
		partition_->GetTransform()->SetTranslaion(pos);
		partition_->Update();
	}

	/*------------- manager -------------*/
	obstaclesManager_->SetPlayerPosition(player_->GetWorldTranslation());
	
#ifdef _DEBUG
	obstaclesManager_->Debug_Gui();
#endif // 
	obstaclesManager_->Update();

	/*-------------- effect -------------*/
	trail_->Update();
	trail_->AddTrail(player_->GetTransform()->GetTranslation(), player_->GetSlerpRotate(), player_->GetIsFlying());
	trail_->SetPlayerPosition(player_->GetTransform()->GetTranslation());

	if(currentState_ == GAME_STATE::TITLE){
		cherryEmitter_->Update();
	} else{

		if(gamePlayTimer_->GetTimeLinit() <= 0.0f){
			float t = std::clamp(gamePlayTimer_->GetOutgameTime() / 3.0f, 0.0f, 1.0f);
			bubbleEmitter_->SetInterval(0.5f - 0.49f * t);
			bubbleEmitter_->Update();
		}
	}

	CheckAddSplash();
	for (auto& splash : splash_) {
		splash->Update();
	}

	animationEffectManager_->Update();


#ifdef _DEBUG
	if (isGuiDraw_) {
		if (!isStepFrame_) {
			Debug_Gui();

			// editorの処理
			placementObjectEditor_->Update();
		}
	}
#endif

	// -------------------------------------------------
	// ↓ 開始時にコライダーのリストを更新する
	// -------------------------------------------------

	UpdateColliderList();

	// -------------------------------------------------
	// ↓ 当たり判定を取る
	// -------------------------------------------------
	worldObjects_->CollisionPlayerToWaterSpace(player_.get());
	collisionManager_->SetPlayerPosition(player_->GetWorldTranslation());
	collisionManager_->CheckAllCollision();

	// -------------------------------------------------
	// ↓ 不要になった要素などの削除
	// -------------------------------------------------

	splash_.remove_if([](auto& splash) {return splash->GetIsEndSplash(); });

	// -------------------------------------------------
	// ↓ UIの更新
	// -------------------------------------------------
	if (currentState_ != GAME_STATE::TITLE) {
		flyingTimerUI_->Update(player_->GetFlyingTime(), player_->GetMaxFlyingTime());
		flyingGaugeUI_->Update(player_->GetFlyingTime());
		playerSpeedCounter_->Update(player_->GetMoveSpeed(), player_->GetTotalSpeedRatio());
	}

	// -------------------------------------------------
	// ↓ ParticleのViewを設定する
	// -------------------------------------------------
	EffectSystem::GetInstacne()->SetCameraMatrix(camera_->GetCameraMatrix());
	EffectSystem::GetInstacne()->SetViewProjectionMatrix(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());

	if (player_->GetIsMove()) {
		gamePlayTimer_->Update(player_->GetIsFlying());

		//if (gamePlayTimer_->GetIsFinish()) {
		//	isPause_ = true;
		//}
	}

	if (Input::IsTriggerKey(DIK_RSHIFT)) {
		Init();
	}

	if (Input::IsTriggerKey(DIK_ESCAPE)) {
		isGuiDraw_ = !isGuiDraw_;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Draw() const{

	mainBGM_->Play(true, 0.4f, true);

	Engine::SetPipeline(PipelineType::SpritePipeline);
	sky_->Draw(true);


	/////////////////////////////////
	// 3Dオブジェクトなどの表示(基本ここ)
	/////////////////////////////////
	worldObjects_->Draw();

	//if (currentState_ == GAME_STATE::TITLE) {
	//	partition_->Draw();
	//}

	/////////////////////////////////
	// 線の描画
	/////////////////////////////////

	obstaclesManager_->Draw();
	Engine::SetPipeline(PipelineType::NormalPipeline);
	for(auto& splash : splash_) {
		splash->Draw();
	}

#ifdef _DEBUG
	Engine::SetPipeline(PipelineType::PrimitivePipeline);
	// コライダーの表示
	if(Collider::isColliderBoxDraw_) {
		if(!isDegugCameraActive_) {
			collisionManager_->Draw(camera_->GetViewMatrix() * camera_->GetProjectionMatrix());
		} else {
			collisionManager_->Draw(debugCamera_->GetViewMatrix() * debugCamera_->GetProjectionMatrix());
		}
	}
	// editorの描画
	placementObjectEditor_->Draw();
#endif // _DEBUG

	Engine::SetPipeline(PipelineType::NotCullingPipeline);
	player_->Draw();

	Engine::SetPipeline(PipelineType::SkinningPipeline);
	player_->DrawAnimetor();

	/////////////////////////////////
	// Effectの描画
	/////////////////////////////////
	Engine::SetPipeline(PipelineType::AddPipeline);
	trail_->Draw();
	Engine::SetPipeline(PipelineType::SkinningPipeline);
	animationEffectManager_->Draw();
	debugModel_->Draw();

	/////////////////////////////////
	// 水の表示
	/////////////////////////////////
	worldObjects_->DrawWater();

	Engine::SetPipeline(PipelineType::NormalPipeline);
	for(auto& backgroundObject : backgroundObjects_) {
		backgroundObject.second->Draw();
	}

	/////////////////////////////////
	// 3Dオブジェクトに対してCsを実行する
	/////////////////////////////////
	//Engine::RunCS();

	/////////////////////////////////
	// スプライトの表示
	/////////////////////////////////
	//Render::SetRenderTarget(Sprite2D_RenderTarget);


	if(currentState_ != GAME_STATE::TITLE) {

		if(gamePlayTimer_->GetTimeLinit() <= 0.0f){
			Engine::SetPipeline(PipelineType::AddBlendSpritePipeline);
			bubbleEmitter_->Draw();
		}
		Engine::SetPipeline(PipelineType::NormalBlendSpritePipeline);
		gamePlayTimer_->Draw();
		flyingTimerUI_->Draw();
		flyingGaugeUI_->Draw();
		playerSpeedCounter_->Draw();

	} else {
		Engine::SetPipeline(PipelineType::NormalBlendSpritePipeline);
		titleLogo_->Draw();
		cherryEmitter_->Draw();
	}

	// フェードの描画
	if(gamePlayTimer_->GetTimeLinit() <= 0.0f){
		float t = std::clamp((gamePlayTimer_->GetOutgameTime() - 2.0f) / 6.0f, 0.0f, 1.0f);
		fade_->SetColor({1.0f,1.0f,1.0f,EaseOutExpo(t)});
		fade_->Draw();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::UpdateColliderList() {

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
// ↓　条件を満たしたら水しぶきエフェクトを追加する
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::CheckAddSplash() {
	if (player_->GetIsSplash()) {

		Vector3 emitPos = player_->GetTransform()->GetTranslation();
		emitPos.y = 0.01f;

		splash_.emplace_back(
			std::make_unique<Splash>(emitPos, 5.0f * (player_->GetMoveSpeed() / 50.0f))
		);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
void GameScene::Debug_Gui() {
	if (isGuiDraw_) {
	
		ImGui::Begin("GameScene");
		ImGui::Text("particle %d", cherryEmitter_->GetParticleCount());

		//ImGui::DragFloat3()
		if (ImGui::Button("NextScene")) {
			SetNextScene(SceneType::Scene_Result);
		}

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
				gamePlayTimer_->Debug_Gui();
				flyingTimerUI_->Debug_Gui();
				flyingGaugeUI_->Debug_Gui();
				playerSpeedCounter_->Debug_Gui();
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
}
#endif
