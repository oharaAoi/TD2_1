#include "ResultScene.h"

ResultScene::ResultScene(){}

ResultScene::~ResultScene(){}

void ResultScene::Finalize(){}

/////////////////////////////////////////////////////////////////
//							初期化
/////////////////////////////////////////////////////////////////

void ResultScene::Init(){

	// 読み込み
	Load();

	// スコアの決定
	if(score_ < (int)SCORE_RANK::B){
		rank_ = SCORE_RANK::C;
	} else if(score_ < (int)SCORE_RANK::A){
		rank_ = SCORE_RANK::B;
	} else if(score_ < (int)SCORE_RANK::S){
		rank_ = SCORE_RANK::A;
	} else if(score_ < (int)SCORE_RANK::SS){
		rank_ = SCORE_RANK::S;
	} else if(score_ < (int)SCORE_RANK::SSS){
		rank_ = SCORE_RANK::SS;
	} else {
		rank_ = SCORE_RANK::SSS;
	}

	/*--------------- camera ---------------*/
	camera_ = std::make_unique<ResultCamera>();
	camera_->Init();

	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Init();

	/*---------------- object --------------*/
	player_ = std::make_unique<Player>();
	player_->GetTransform()->SetTranslaion({ 0.0f,0.0f,0.0f });
	player_->GetTransform()->SetQuaternion(
		Quaternion::AngleAxis(180.0f * toRadian, { 0.0f,1.0f,0.0f }) *
		Quaternion::AngleAxis(45.0f * toRadian, { 0.0f,0.0f,1.0f })
	);


	/*---------------- string --------------*/
	space_ = 2.7f;
	auto& m = scoreNumberModels_.emplace_back(std::make_unique_for_overwrite<BaseGameObject>());
	m->Init();
	m->SetObject("m.obj");
	m->GetTransform()->SetTranslaion({ -6.0f,0.0f,10.0f });
	m->GetTransform()->SetScale({ 5.0f,5.0f,5.0f });
	m->UpdateMatrix();

	int digit = 1;
	while(true){

		if(score_ / digit <= 0){
			std::reverse(scoreNumberModels_.begin(), scoreNumberModels_.end());
			break;
		} else{
			auto& num = scoreNumberModels_.emplace_back(std::make_unique_for_overwrite<BaseGameObject>());
			num->Init();
			num->SetObject(std::to_string((score_ / digit) % 10) + ".obj");
			translate_ = { -5.8f,0.4f,12.5f };
			num->GetTransform()->SetTranslaion(translate_);
			rotate_ = { 0.53f,-0.62f,-0.18f };
			num->GetTransform()->SetQuaternion(Quaternion::EulerToQuaternion(rotate_));
			num->GetTransform()->SetScale({ 5.0f,5.0f,5.0f });
			num->UpdateMatrix();

			digit *= 10;
		}
	}

	// 文字の更新
	for(int i = 0; i < scoreNumberModels_.size(); i++){
		scoreNumberModels_[i]->GetTransform()->SetQuaternion(Quaternion::EulerToQuaternion(rotate_));
		scoreNumberModels_[i]->GetTransform()->SetTranslaion(
			translate_ + Vector3(1.0f, 0.0f, 0.0f) * MakeRotateXYZMatrix(rotate_) * space_ * i
		);

		scoreNumberModels_[i]->Update();
	}

	scoreRankModel_ = std::make_unique<BaseGameObject>();
	scoreRankModel_->Init();
	if(rank_ == SCORE_RANK::C){
		scoreRankModel_->SetObject("C.obj");
	} else if(rank_ == SCORE_RANK::B){
		scoreRankModel_->SetObject("B.obj");
	} else if(rank_ == SCORE_RANK::A){
		scoreRankModel_->SetObject("B.obj");
	} else if(rank_ == SCORE_RANK::S){
		scoreRankModel_->SetObject("S.obj");
	} else if(rank_ == SCORE_RANK::SS){
		scoreRankModel_->SetObject("SS.obj");
	} else if(rank_ == SCORE_RANK::SSS){
		scoreRankModel_->SetObject("SSS.obj");
	}

	scoreRankModel_->GetTransform()->SetTranslaion({ -4.2f,1.2f,-5.3f });
	scoreRankModel_->GetTransform()->SetQuaternion(Quaternion::EulerToQuaternion({ -0.27f,2.58f,-0.16f }));
	scoreRankModel_->Update();

	/*---------------- effect ---------------*/
	tickerTapeEmitter_ = std::make_unique<ParticleManager<TickerTape>>();
	tickerTapeEmitter_->SetEmitRange({ 0.0f,-kWindowHeight_ * 0.2f }, { kWindowWidth_,0.0f });
	tickerTapeEmitter_->SetEmitCountEvery(4);

	/*--------------- sprite ---------------*/
	backgroundSprite_ = Engine::CreateSprite("white.png");
	backgroundSprite_->SetTextureSize({ kWindowWidth_,kWindowHeight_ });
	backgroundSprite_->SetCenterPos({ kWindowWidth_ * 0.5f,kWindowHeight_ * 0.5f });
	backgroundSprite_->SetColor({ 0.0f,0.0f,0.0f,1.0f });
	backgroundSprite_->Update();


}

/////////////////////////////////////////////////////////////////
//							読み込み
/////////////////////////////////////////////////////////////////

void ResultScene::Load(){


	// スコア表示用の数字モデル
	ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "0.obj");
	ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "1.obj");
	ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "2.obj");
	ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "3.obj");
	ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "4.obj");
	ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "5.obj");
	ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "6.obj");
	ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "7.obj");
	ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "8.obj");
	ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "9.obj");
	ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "m.obj");
	ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "SSS.obj");
	ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "SS.obj");
	ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "S.obj");
	ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "A.obj");
	ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "B.obj");
	ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "C.obj");

	/* ---------------- audio ----------------*/
	AudioManager::LoadAudio("./Game/Resources/Audio/", "test.wav");
	AudioManager::LoadAudio("./Game/Resources/Audio/", "kari_coinGet.wav");

	/*----------------- model ----------------*/
	ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player.fbx");
	ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player_Head.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player_Torso.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player_Tail.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Effect/", "HighSpeedEffect.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/Wing/", "Wing.obj");
	ModelManager::LoadModel("./Game/Resources/Model/AddTorso/", "AddTorso.gltf");

	/*------------- animation ---------------*/
	ModelManager::LoadModel("./Game/Resources/Model/DriftWoodDestroy/", "DriftWoodDestroy.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/BirdJumpEffect/", "BirdJumpEffect.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/Effect1/", "Effect1.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/FishDestroy/", "FishDestroy.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/JumpEffect/", "JumpEffect.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/AddTorso/", "AddTorso.gltf");


	/*---------------- texture ---------------*/
	TextureManager::LoadTextureFile("./Engine/Resources/Develop/", "white.png");

	// Adio
	AudioManager::LoadAudio("./Game/Resources/Audio/", "test.wav");
	AudioManager::LoadAudio("./Game/Resources/Audio/", "kari.wav");
	AudioManager::LoadAudio("./Game/Resources/Audio/", "kari_coinGet.wav");

	// ○がついていない物はまだ使用していない
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "boost.wav");				// ブースト音		○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "decrementBody.wav");		// 体の数を減らす		○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "eat.wav");				// エサを食べる		○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "eatAccel.wav");			// エサを食べた時加速する		○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "gameFinish.wav");		// gameFinish音
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "goalTarget.wav");		// 目標の距離を達成した音
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "hitedBird.wav");			// 鳥に当たった時の音		○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "incrementBody.wav");		// 体の数を増やす			○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "inWaterSurface.wav");	// 水面に入った時の音			○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "outWaterSurface.wav");	// 水面から出た時の音			○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "jumpBird.wav");			// 鳥を踏んでジャンプ			○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "overTime.wav");			// オーバータイムの音
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "timeLeft_10s.wav");		// タイムアップ10秒前			○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "timeLeft_60s.wav");		// タイムアップ60秒前			○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "timeUp.wav");			// タイムアップの音			○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "updateFlyingLength.wav");// 飛行距離を伸ばした時の音
}

/////////////////////////////////////////////////////////////////
//							更新
/////////////////////////////////////////////////////////////////

void ResultScene::Update(){

	// ====================== object ====================== //

	player_->ResultSceneUpdate();



	scoreRankModel_->Update();

	// ====================== effect ====================== //
	tickerTapeEmitter_->Update();

	//==================== cameraの更新 ====================//
	if(!isDebugCameraActive_){
		camera_->Update();
		Render::SetEyePos(camera_->GetWorldTranslate());
		Render::SetViewProjection(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());
		Render::SetViewProjection2D(camera_->GetViewMatrix2D(), camera_->GetProjectionMatrix2D());

	} else{
		debugCamera_->Update();
		Render::SetEyePos(debugCamera_->GetWorldTranslate());
		Render::SetViewProjection(debugCamera_->GetViewMatrix(), debugCamera_->GetProjectionMatrix());
		Render::SetViewProjection2D(debugCamera_->GetViewMatrix2D(), debugCamera_->GetProjectionMatrix2D());
	}


	if(Input::IsTriggerKey(DIK_SPACE)){
		SetNextScene(SceneType::Scene_Game);
	}

#ifdef _DEBUG
	Debug_Gui();
#endif 
}

/////////////////////////////////////////////////////////////////
//							描画
/////////////////////////////////////////////////////////////////

void ResultScene::Draw() const{

	// ======================== object ======================= //

	Engine::SetPipeline(PipelineType::NormalPipeline);


	Engine::SetPipeline(PipelineType::NotCullingPipeline);
	player_->Draw();


	for(auto& num : scoreNumberModels_){
		num->Draw();
	}

	scoreRankModel_->Draw();

	// =======================  sprite ======================= //

	Engine::SetPipeline(PipelineType::SpritePipeline);
	backgroundSprite_->Draw(true);

	// ====================== effect ====================== //
	tickerTapeEmitter_->Draw();
}


// ======================= GUI ========================= //
#ifdef _DEBUG
void ResultScene::Debug_Gui(){



	ImGui::Begin("ResultScene");

	ImGui::Checkbox("isDebugCameraActive", &isDebugCameraActive_);



	if(ImGui::Button("ReTry")) {
		SetNextScene(SceneType::Scene_Game);
	}

	if(StageInformation::GetNowStage() < StageInformation::GetStageNumMax() - 1) {
		if(ImGui::Button("Next")) {
			SetNextScene(SceneType::Scene_Game);
			StageInformation::SetNowStage(StageInformation::GetNowStage() + 1);
		}
	}


	ImGui::End();
}
#endif 