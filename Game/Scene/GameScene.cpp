#include "GameScene.h"

bool GamePlayTimer::isFinish_ = false;

/*---------- static initialize -----------*/
float GameScene::groundDepth_ = -44.0f;
GAME_STATE GameScene::currentState_ = GAME_STATE::TITLE;
GAME_STATE GameScene::preState_ = currentState_;


/*-------------- コンストラクタ・デストラクタ ---------------*/
GameScene::GameScene(){}
GameScene::~GameScene(){ Finalize(); }

void GameScene::Finalize(){
	mainBGM_->Finalize();
	mainBGM_inWater_->Finalize();
	windSound_->Finalize();
	swimSound_->Finalize();
	AnimetionEffectManager::GetInstance()->Finalize();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Init(){

	currentState_ = GAME_STATE::TITLE;

	AdjustmentItem::GetInstance()->Init("GameScene");

	gamePlayTimer_ = std::make_unique<GamePlayTimer>();
	gamePlayTimer_->Init(30.0f);


	// -------------------------------------------------
	// ↓ editorの初期化
	// -------------------------------------------------
	player_ = std::make_unique<Player>();

	obstaclesManager_ = std::make_unique<ObstaclesManager>();
	obstaclesManager_->Init(player_.get());
	if(StageInformation::GetStageNumMax() != 0) {
		obstaclesManager_->SetObstacles(StageInformation::GetStage());
	}
	obstaclesManager_->SetGameScene(this);

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

	startSceneChange_ = false;
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

	playerBodyCountUI_ = std::make_unique<PlayerBodyCountUI>();
	playerBodyCountUI_->Init();

	playerControlUI_ = std::make_unique<PlayerControlUI>();
	playerControlUI_->Init();

	guideUI_ = std::make_unique<GuideUI>();
	guideUI_->Init();
	guideUI_->SetTitle();

	finishUI_ = std::make_unique<FinishUI>();
	finishUI_->Init();

	gameStartUI_ = std::make_unique<GameStartUI>();
	gameStartUI_->Init();

	missionUI_ = std::make_unique<MissionUI>();
	missionUI_->Init(gamePlayTimer_.get());
	
	// チュートリアル
	tutorialUI_ = std::make_unique<TutorialUI>();
	tutorialUI_->Init();

	// 桜の花を散らすやつ
	cherryEmitter_ = std::make_unique<ParticleManager<Cherry>>();
	cherryEmitter_->SetEmitRange({ kWindowWidth_ * 0.5f,-kWindowHeight_ * 0.2f }, { kWindowWidth_,0.0f });
	cherryEmitter_->SetEmitCountEvery(2);

	// 終了時泡をだすやつ
	bubbleEmitter_ = std::make_unique<ParticleManager<Bubble>>();
	bubbleEmitter_->SetEmitRange({ 0.0f,kWindowHeight_ }, { kWindowWidth_,kWindowHeight_ * 2.0f });
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

	// panel
	fadePanel_ = std::make_unique<FadePanel>();
	fadePanel_->Init();

	// -------------------------------------------------
	// ↓ LoadSceneの設定
	// -------------------------------------------------
	loadScene_ = std::make_unique<LoadScene>();
	loadScene_->Init();

	// -------------------------------------------------
	// ↓ ターゲットの設定
	// -------------------------------------------------
	camera_->SetPlayerPtr(player_.get());

	// -------------------------------------------------
	// ↓ Audioの生成
	// -------------------------------------------------

	mainBGM_ = std::make_unique<AudioPlayer>();
	mainBGM_->Init("mainBGM_tobenaikoi.mp3");

	mainBGM_inWater_ = std::make_unique<AudioPlayer>();
	mainBGM_inWater_->Init("mainBGM_tobenaikoi_in_water.mp3");

	windSound_ = std::make_unique<AudioPlayer>();
	windSound_->Init("brow.mp3");

	swimSound_ = std::make_unique<AudioPlayer>();
	swimSound_->Init("swim.mp3");

	// -------------------------------------------------
	// ↓ 背景のモデルの生成
	// -------------------------------------------------
	//モデル確認用
	debugModel_ = std::make_unique<BaseGameObject>();
	debugModel_->Init();
	debugModel_->SetObject("FishWeakening.gltf");
	debugModel_->SetAnimater("./Game/Resources/Model/FishWeakening/", "FishWeakening.gltf", true);
	debugModel_->SetIsLighting(false);


}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　読み込み
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Load(){

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Update(){
#ifdef _DEBUG
	//プレイヤーの動きを停止
	if(Input::IsTriggerKey(DIK_1)) {

		player_->DebugFreeze();
	}
	//ブースト
	if(Input::IsTriggerKey(DIK_2)) {

		player_->DebugBoost();
	}
	//デバッグカメラをプレイヤーの座標に合わせる
	if(Input::IsTriggerKey(DIK_3)) {

		debugCamera_->SetPlayerPos(player_.get()->GetWorldTranslation());
		isDegugCameraActive_ = !isDegugCameraActive_;
	}
	//進める
	if(Input::IsTriggerKey(DIK_4)) {

		player_->DebugAdvancePos();
	}
	//進める
	if(Input::IsTriggerKey(DIK_5)) {

		player_->DebugResetPos();
	}
#endif // _DEBUG

	// 調整項目の更新
	AdjustmentItem::GetInstance()->Update();

	if (currentState_ == GAME_STATE::TITLE) {
		if (Input::IsTriggerKey(DIK_UP) || Input::IsTriggerKey(DIK_W)) {
			isGameStart_ = !isGameStart_;
		}

		if (Input::IsTriggerKey(DIK_DOWN) || Input::IsTriggerKey(DIK_S)) {
			isGameStart_ = !isGameStart_;
		}

		guideUI_->SetArrow(isGameStart_);
	}

	if(Input::IsTriggerKey(DIK_SPACE)) {
		if(currentState_ == GAME_STATE::TITLE) {
			if (fadePanel_->GetDoNoting()) {
				fadePanel_->SetFadeOut(0.6f);
				Input::SetNotAccepted(true);
			}
		} /*else if (currentState_ == GAME_STATE::TUTORIAL) {
			currentState_ = GAME_STATE::GAME;
		}*/
	}

	fadePanel_->Update();

	if (currentState_ == GAME_STATE::TITLE) {
		if (fadePanel_->GetIsFinished()) {
			currentState_ = GAME_STATE::LOAD;
		}
	}

	if (currentState_ == GAME_STATE::LOAD) {
		if (fadePanel_->GetIsFinished()) {
			loadScene_->Update();
		}

		if (loadScene_->GetIsLoadFinish()) {
			if (fadePanel_->GetDoNoting()) {
				fadePanel_->SetFadeOutOpen(0.6f);
				if (!isGameStart_) {
					currentState_ = GAME_STATE::TUTORIAL;
					tutorialUI_->LineUpUI(player_->GetWorldTranslation());
					obstaclesManager_->TutorialImport("tutorial_Fish", tutorialUI_->GetSessionFishPos(), 0);
					obstaclesManager_->TutorialImport("tutorial_bird", tutorialUI_->GetSessionBirdPos(), 0);
					obstaclesManager_->TutorialImport(start1, tutorialUI_->GetStartPos() + Vector3(20, 0, 0), 1);
					obstaclesManager_->TutorialImport(start2, tutorialUI_->GetStartPos() + Vector3(220, 0, 0), 1);
				} else {
					currentState_ = GAME_STATE::GAME;
					obstaclesManager_->TutorialImport(start1, player_->GetWorldTranslation() + Vector3(140, 0, 0), 1);
					obstaclesManager_->TutorialImport(start2, player_->GetWorldTranslation() + Vector3(250, 0, 0), 1);
					obstaclesManager_->TutorialImport(start1, player_->GetWorldTranslation() + Vector3(450, 0, 0), 1);
					gameStartUI_->SetUI();
					missionUI_->Start();
				}
			}
		}
		return;
	}

	if(currentState_ == GAME_STATE::TUTORIAL) {
		Update_TUTORIAL();
	}

#ifdef _DEBUG

#endif // _DEBUG


	// -------------------------------------------------
	// ↓ Cameraの更新
	// -------------------------------------------------

	if(!isDegugCameraActive_) {
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
	if(Input::IsTriggerKey(DIK_ESCAPE)) {
		SetNextScene(SceneType::Scene_Game);
	}
	if(isPause_) {
		isStepFrame_ = false;

	#ifdef _DEBUG
		Debug_Gui();
		placementObjectEditor_->Update();
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
	debugModel_->Update();

	worldObjects_->LoopStage();
	worldObjects_->Update(player_->GetWorldTranslation().x);

	if(currentState_ == GAME_STATE::TITLE) {
		Vector3 pos = player_->GetWorldTranslation();
		pos.z += 10.0f;
	}

	// -------------------------------------------------
	// ↓ manager
	// -------------------------------------------------

	obstaclesManager_->SetPlayerPosition(player_->GetWorldTranslation());

	if(player_->GetMaxFlyingTime() > level3){
		obstaclesManager_->SetLevel(3);


	} else if(player_->GetMaxFlyingTime() > level2){
		obstaclesManager_->SetLevel(2);

	}


#ifdef _DEBUG
	obstaclesManager_->Debug_Gui();
#endif // 

	obstaclesManager_->Update();

	// -------------------------------------------------
	// ↓ effect
	// -------------------------------------------------
	trail_->Update();
	trail_->AddTrail(player_->GetTransform()->GetTranslation(), player_->GetTransform()->GetQuaternion(), player_->GetIsFlying());
	trail_->SetPlayerPosition(player_->GetTransform()->GetTranslation());

	if(currentState_ == GAME_STATE::TITLE) {
		cherryEmitter_->Update();
	} else {

		if(gamePlayTimer_->GetTimeLinit() <= 0.0f){

			if(!player_->GetIsFlying()){
				isEndAndInWater_ = true;
				finishUI_->SetUI(false);
			} else {
				finishUI_->SetUI(true);
			}

			if(isEndAndInWater_){
				if(!startSceneChange_)
				{
					AudioPlayer::SinglShotPlay("Bubble.mp3", 0.3f);

				}
				startSceneChange_ = true;
				float t = std::clamp(gamePlayTimer_->GetOutgameTime() / 3.0f, 0.0f, 1.0f);
				bubbleEmitter_->SetInterval(0.5f - 0.49f * t);
				bubbleEmitter_->Update();
			}
		}
	}

	CheckAddSplash();
	for(auto& splash : splash_) {
		splash->Update();
	}

	animationEffectManager_->Update();

	// -------------------------------------------------
	// ↓ Editer
	// -------------------------------------------------

#ifdef _DEBUG
	if(isGuiDraw_) {
		if(!isStepFrame_) {
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
	if(currentState_ != GAME_STATE::TITLE) {
		//flyingTimerUI_->Update(player_->GetFlyingTime(), player_->GetMaxFlyingTime());
		flyingGaugeUI_->Update(player_->GetFlyingTime());
		playerSpeedCounter_->Update(player_->GetMoveSpeed(), player_->GetTotalSpeedRatio(),speedMeterAlpha_, player_->GetIsFlying());
	} else{
		startSceneTime_ = std::clamp(startSceneTime_ - GameTimer::DeltaTime(), 0.0f, 2.0f);
	}

	if (player_->GetIsJet()) {
		playerBodyCountUI_->EmiteEffect();
	}

	if (currentState_ != GAME_STATE::TITLE) {
		playerBodyCountUI_->Update(player_->GetBodyCount());
	}

	finishUI_->Update();

	if (player_->GetIsFlying()) {
		Vector3 playerScreenPos = Transform({ 0.0f, 0.0f, 0.0f },
											player_->GetTransform()->GetWorldMatrix() * camera_->GetVpvpMatrix());
		playerControlUI_->Update({playerScreenPos.x, playerScreenPos.y}, player_->GetIsCloseWing());
	}

	guideUI_->Update();

	gameStartUI_->Update();

	if (currentState_ == GAME_STATE::GAME) {
		missionUI_->Update(player_->GetMoveSpeed(), player_->GetWorldTranslation().y);
	}

	// -------------------------------------------------
	// ↓ スプライト
	// -------------------------------------------------

	if(player_->GetIsFlying()){
		if(currentState_ != GAME_STATE::TITLE){
			sky_->SetTexture("blueSky_ORE.png");
		}
	} else{
		sky_->SetTexture("sky.png");

		if(speedMeterAlpha_ >= 1.0f){
			isStartupScene_ = false;
		}
	}

	// -------------------------------------------------
	// ↓ audioの更新
	// -------------------------------------------------

	if(player_->GetIsFlying()){
		BGM_volumeT_ = std::clamp(BGM_volumeT_ + (0.05f * GameTimer::TimeRate()), 0.0f, 1.0f);
		speedMeterAlpha_ = std::clamp(speedMeterAlpha_ - (0.02f * GameTimer::TimeRate()), 0.0f, 1.0f);

	} else{
		BGM_volumeT_ = std::clamp(BGM_volumeT_ - (0.05f * GameTimer::TimeRate()), 0.0f, 1.0f);
		speedMeterAlpha_ = std::clamp(speedMeterAlpha_ + (0.02f * GameTimer::TimeRate()), 0.0f, 1.0f);
	}

	// ゲーム終了時にフェードアウトさせるための変数更新
	float end_t = std::clamp((gamePlayTimer_->GetOutgameTime()) / outgameWaitTime_, 0.0f, 1.0f);
	BGM_masterVolumeRate_ = 1.0f - end_t;

	if(BGM_masterVolumeRate_ <= 0.0f){
		mainBGM_->Stop();
		mainBGM_inWater_->Stop();
		windSound_->Stop();
		swimSound_->Stop();
	}

	// 水に入っているかどうかで音の割合を切り替える
	mainBGM_->SetVolume(0.1f * BGM_volumeT_ * BGM_masterVolumeRate_);
	mainBGM_inWater_->SetVolume(0.1f * (1.0f - BGM_volumeT_) * BGM_masterVolumeRate_);
	windSound_->SetVolume(0.1f * BGM_volumeT_ * BGM_masterVolumeRate_);
	swimSound_->SetVolume(0.07f * (1.0f - BGM_volumeT_) * BGM_masterVolumeRate_);

	// -------------------------------------------------
	// ↓ ParticleのViewを設定する
	// -------------------------------------------------
	EffectSystem::GetInstacne()->SetCameraMatrix(camera_->GetCameraMatrix());
	EffectSystem::GetInstacne()->SetViewProjectionMatrix(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());

	if(GetGameState() == GAME_STATE::GAME) {
		if(player_->GetIsMove()) {
			gamePlayTimer_->Update(player_->GetIsFlying());

			//if (gamePlayTimer_->GetIsFinish()) {
			//	isPause_ = true;
			//}
		}
	}

#ifdef _DEBUG
	if(Input::IsTriggerKey(DIK_RSHIFT)) {
		Init();
	}

	if(Input::IsTriggerKey(DIK_ESCAPE)) {
		isGuiDraw_ = !isGuiDraw_;
	}
#endif // _DEBUG


	if(gamePlayTimer_->GetOutgameTime() > outgameWaitTime_ + fadeWaitTime_) {
		SetNextScene(SceneType::Scene_Result);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Draw() const{


	if(BGM_masterVolumeRate_ > 0.0f){
		mainBGM_->Play(true, 0.1f, true);
		mainBGM_inWater_->Play(true, 0.1f, true);
		windSound_->Play(true, 0.1f, true);
		swimSound_->Play(true, 0.07f, true);
	}

	Engine::SetPipeline(PipelineType::SpritePipeline);
	sky_->Draw(true);


	/////////////////////////////////
	// 3Dオブジェクトなどの表示(基本ここ)
	/////////////////////////////////
	worldObjects_->Draw();

	/////////////////////////////////
	// tutorialUIの描画
	/////////////////////////////////
	if(currentState_ == GAME_STATE::TUTORIAL) {
		Engine::SetPipeline(PipelineType::NormalPipeline);
		tutorialUI_->Draw();
	}

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

	if(currentState_ != GAME_STATE::TITLE){
		trail_->Draw();
	}

	Engine::SetPipeline(PipelineType::SkinningPipeline);
	animationEffectManager_->Draw();
	debugModel_->Draw();

	/////////////////////////////////
	// 水の表示
	/////////////////////////////////
	worldObjects_->DrawWater();

	Engine::SetPipeline(PipelineType::NormalPipeline);
	if(currentState_ == GAME_STATE::GAME) {
		player_->DrawHeightMeter();
	}
	/////////////////////////////////
	// 3Dオブジェクトに対してCsを実行する
	/////////////////////////////////
	//Engine::RunCS();

	/////////////////////////////////
	// スプライトの表示
	/////////////////////////////////
	//Render::SetRenderTarget(Sprite2D_RenderTarget);

	// 一番最初に描画して他の物が描画されないようにする
	Engine::SetPipeline(PipelineType::NormalBlendSpritePipeline);
	if (currentState_ == GAME_STATE::LOAD) {
		//fadePanel_->Draw();
		if (fadePanel_->GetIsFinished()) {
			loadScene_->Draw();
		}
		return;
	}

	if(currentState_ != GAME_STATE::TITLE) {

		if(gamePlayTimer_->GetTimeLinit() <= 0.0f) {
			Engine::SetPipeline(PipelineType::AddBlendSpritePipeline);
			bubbleEmitter_->Draw();
		}
		Engine::SetPipeline(PipelineType::NormalBlendSpritePipeline);
		gamePlayTimer_->Draw();

		if(!isStartupScene_){
			//flyingTimerUI_->Draw(1.0f - speedMeterAlpha_);
			flyingGaugeUI_->Draw(1.0f - speedMeterAlpha_);
		}
		playerSpeedCounter_->Draw();
		playerBodyCountUI_->Draw();
		player_->DrawCutIn();
		missionUI_->Draw();
		finishUI_->Draw();
		gameStartUI_->Draw();
		
		playerControlUI_->Draw(player_->GetIsFlying());


	} else {
		Engine::SetPipeline(PipelineType::NormalBlendSpritePipeline);
		titleLogo_->Draw();
		cherryEmitter_->Draw();
		guideUI_->Draw();

		float t = std::clamp(startSceneTime_ / 2.0f, 0.0f, 1.0f);
		fade_->SetColor({ 1.0f,1.0f,1.0f,t });
		fade_->Draw();
	}

	// フェードの描画
	if(gamePlayTimer_->GetTimeLinit() <= 0.0f) {
		if(isEndAndInWater_) {
			float t = std::clamp((gamePlayTimer_->GetOutgameTime() - fadeWaitTime_) / outgameWaitTime_, 0.0f, 1.0f);
			fade_->SetColor({ 1.0f,1.0f,1.0f,t });
			fade_->Draw();
		}
	}

	fadePanel_->Draw();
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　チュートリアルの内容を行う
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::Update_TUTORIAL(){

#ifdef _DEBUG
	if(Input::IsTriggerKey(DIK_Z)) {
		currentState_ = GAME_STATE::GAME;
	}
#endif // _DEBUG




	if(player_->GetWorldTranslation().x > tutorialUI_->GetStartPos().x) {
		currentState_ = GAME_STATE::GAME;
		gameStartUI_->SetUI();
		missionUI_->Start();
	}

	// UIの更新
	tutorialUI_->Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Colliderの更新を行う
//////////////////////////////////////////////////////////////////////////////////////////////////

void GameScene::UpdateColliderList(){

	collisionManager_->Reset();

	collisionManager_->AddCollider(player_.get());

	for(auto& obstacle : obstaclesManager_->GetPlacementObject()) {
		float lenght = std::abs((player_->GetWorldTranslation() - obstacle->GetWorldTranslation()).Length());
		if(lenght < obstaclesManager_->GetUpdateLenght()) {
			collisionManager_->AddCollider(obstacle.get());
		}
	}

#ifdef _DEBUG

	for(auto& obstacle : placementObjectEditor_->GetDebugPlacementObjs()) {
		float lenght = std::abs((player_->GetWorldTranslation() - obstacle.object_->GetWorldTranslation()).Length());
		if(lenght < obstaclesManager_->GetUpdateLenght()) {
			collisionManager_->AddCollider(obstacle.object_.get());
		}
	}

	for(auto& obstacle : placementObjectEditor_->GetInportPlacementObjs()) {
		float lenght = std::abs((player_->GetWorldTranslation() - obstacle.object_->GetWorldTranslation()).Length());
		if(lenght < obstaclesManager_->GetUpdateLenght()) {
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

void GameScene::CheckAddSplash(){
	if(player_->GetIsSplash()) {

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
void GameScene::Debug_Gui(){
	if(isGuiDraw_) {

		ImGui::Begin("GameScene");
		ImGui::Text("particle %d", cherryEmitter_->GetParticleCount());

		// マスター音の追加
		float volume = Audio::GetMasterVolume();
		ImGui::DragFloat("masterVolume", &volume, 0.01f);
		Audio::SetMasterVolume(volume);

		//ImGui::DragFloat3()
		if(ImGui::Button("NextScene")) {
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

		ImGui::Checkbox("debugColliderDraw", &Collider::isColliderBoxDraw_);

		//==================================================================================
		// ↓　Camera
		//==================================================================================
		{
			ImGui::Checkbox("isDebugCameraActive", &isDegugCameraActive_);
			if(ImGui::TreeNode("Camera")) {
				if(!isDegugCameraActive_) {
					camera_->Debug_Gui();
				} else {
					debugCamera_->Debug_Gui();
				}
				ImGui::TreePop();
			}
		}

		//==================================================================================
		// ↓　UI
		//==================================================================================
		{
			if(ImGui::TreeNode("UI")) {
				ImGui::Begin("UI");
				gamePlayTimer_->Debug_Gui();
				flyingTimerUI_->Debug_Gui();
				flyingGaugeUI_->Debug_Gui();
				playerSpeedCounter_->Debug_Gui();
				playerControlUI_->Debug_Gui();
				playerBodyCountUI_->Debug_Gui();
				guideUI_->Debug_Gui();
				missionUI_->Debug_Gui();
				ImGui::End();
				ImGui::TreePop();
			}
		}

		//==================================================================================
		// ↓　Player
		//==================================================================================
		{
			if(ImGui::TreeNode("Player")) {
				player_->Debug_Gui();
				gamePlayTimer_->Debug_Gui();
				ImGui::TreePop();
			}
		}

		//==================================================================================
		// ↓　other
		//==================================================================================
		{
			if(currentState_ == GAME_STATE::TUTORIAL) {
				if(ImGui::TreeNode("Tutorial")) {
					tutorialUI_->Debug_Gui();
					ImGui::TreePop();
				}
			}
		}

		ImGui::End();
	}
}
#endif
