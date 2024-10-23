#include "ResultScene.h"

bool ResultScene::isViewingRanking_ = false;

//-------------------------------------------
ResultScene::ResultScene(){}

ResultScene::~ResultScene(){ Finalize(); }

void ResultScene::Finalize(){
	AnimetionEffectManager::GetInstance()->Finalize();
}

/////////////////////////////////////////////////////////////////
//							初期化
/////////////////////////////////////////////////////////////////

void ResultScene::Init(){

	isViewingRanking_ = false;



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

	scoreData_ = std::make_unique<Score>();
	scoreData_->Init();
	scoreData_->WriteFile((float)score_);
	scoreData_->LoadJsonFile(gameScoreArray_);

	/*--------------- camera ---------------*/
	camera_ = std::make_unique<ResultCamera>();
	camera_->Init();

	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Init();
	debugCamera_->SetTranslate(camera_->GetTranslate());
	debugCamera_->SetRotate(camera_->GetRotate());
	debugCamera_->Update();

	/*---------------- object --------------*/
	player_ = std::make_unique<Player>();
	player_->GetTransform()->SetTranslaion({ 0.0f,0.0f,0.0f });
	player_->GetTransform()->SetQuaternion(
		Quaternion::AngleAxis(180.0f * toRadian, { 0.0f,1.0f,0.0f }) *
		Quaternion::AngleAxis(45.0f * toRadian, { 0.0f,0.0f,1.0f })
	);

	//animationPlayer_ = std::make_unique<BaseGameObject>();
	//animationPlayer_->Init();
	//animationPlayer_->SetObject("Player_result.fbx");
	//animationPlayer_->SetAnimater("./Game/Resources/Model/Player_result_2/", "Player_result.fbx", true);



	/*------------ audio -------------*/

	clearBGM_ = std::make_unique<AudioPlayer>();
	clearBGM_->Init("clearBGM.wav");
	clearBGM_->Play(true, 0.1f, true);

	/*---------------- string --------------*/

	//////////////////////////////////////////////////
	//現在のスコア
	//////////////////////////////////////////////////
	space_ = 2.7f;
	auto& m = scoreNumberModels_.emplace_back(std::make_unique_for_overwrite<BaseGameObject>());
	m->Init();
	m->SetObject("m.obj");
	m->GetTransform()->SetTranslaion({ -6.0f,0.0f,10.0f });
	m->GetTransform()->SetScale({ 5.0f,5.0f,5.0f });
	m->UpdateMatrix();

	auto& m3 = rankingModels_[3].emplace_back(std::make_unique_for_overwrite<BaseGameObject>());
	m3->Init();
	m3->SetObject("m.obj");
	m3->GetTransform()->SetTranslaion({ -6.0f,0.0f,10.0f });
	m3->GetTransform()->SetScale({ 5.0f,5.0f,5.0f });
	m3->SetIsLighting(false);
	m3->UpdateMatrix();

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

			auto& num3 = rankingModels_[3].emplace_back(std::make_unique_for_overwrite<BaseGameObject>());
			num3->Init();
			num3->SetObject(std::to_string((score_ / digit) % 10) + ".obj");
			translate_ = { -5.8f,0.4f,12.5f };
			num->GetTransform()->SetTranslaion(translate_);
			rotate_ = { 0.53f,-0.62f,-0.18f };
			num3->GetTransform()->SetQuaternion(Quaternion::EulerToQuaternion(rotate_));
			num3->GetTransform()->SetScale({ 5.0f,5.0f,5.0f });
			num3->SetIsLighting(false);
			num3->UpdateMatrix();


			digit *= 10;
		}
	}

	// 文字の更新
	for(int i = 0; i < scoreNumberModels_.size(); i++){
		scoreNumberModels_[i]->GetTransform()->SetQuaternion(Quaternion::EulerToQuaternion(rotate_));
		scoreNumberModels_[i]->GetTransform()->SetTranslaion(
			translate_ + Vector3(1.0f, 0.0f, 0.0f) * MakeRotateXYZMatrix(rotate_) * space_ * static_cast<float>(i)
		);

		scoreNumberModels_[i]->Update();
	}


	//////////////////////////////////////////////////
	//スコアランク
	//////////////////////////////////////////////////

	scoreRankModel_ = std::make_unique<BaseGameObject>();
	scoreRankModel_->Init();
	if(rank_ == SCORE_RANK::C){
		scoreRankModel_->SetObject("C.obj");
		scoreRankModel_->SetColor({0.0f,0.2f,1.0f,1.0f });
	} else if(rank_ == SCORE_RANK::B){
		scoreRankModel_->SetObject("B.obj");
		scoreRankModel_->SetColor({ 0.26f,1.0f,0.0f,1.0f });
	} else if(rank_ == SCORE_RANK::A){
		scoreRankModel_->SetObject("A.obj");
		scoreRankModel_->SetColor({ 1.0f,0.1f,0.0f,1.0f });
	} else if(rank_ == SCORE_RANK::S){
		scoreRankModel_->SetObject("S.obj");
		scoreRankModel_->SetColor({ 1.0f,0.5f,0.0f,1.0f });
	} else if(rank_ == SCORE_RANK::SS){
		scoreRankModel_->SetObject("SS.obj");
		scoreRankModel_->SetColor({ 1.0f,1.0f,0.0f,1.0f });
	} else if(rank_ == SCORE_RANK::SSS){
		scoreRankModel_->SetObject("SSS.obj");
		scoreRankModel_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	}

	scoreRankModel_->GetTransform()->SetScale({ 4.6f,4.26f,4.14f });
	scoreRankModel_->GetTransform()->SetTranslaion({ -3.93f,-2.67f,6.29f });
	scoreRankModel_->GetTransform()->SetQuaternion(Quaternion::EulerToQuaternion({ -0.27f,2.58f,-0.16f }));
	scoreRankModel_->Update();

	//////////////////////////////////////////////////
	//ランキング
	//////////////////////////////////////////////////

	for(int32_t i = 0; i < 4; i++){

		if(i != 3){

			auto& m2 = rankingModels_[i].emplace_back(std::make_unique_for_overwrite<BaseGameObject>());
			m2->Init();
			m2->SetObject("m.obj");
			m2->GetTransform()->SetTranslaion({ -6.0f,0.0f,10.0f });
			m2->GetTransform()->SetScale({ 5.0f,5.0f,5.0f });
			m2->SetColor(rankingColors_[i]);
			m2->UpdateMatrix();
			rankingModels_[i].back()->SetIsLighting(false);

			int digit2 = 1;
			while(true){

				if((int)gameScoreArray_[i] / digit2 <= 0){

					rankingModels_[i].emplace_back(std::make_unique_for_overwrite<BaseGameObject>());
					rankingModels_[i].back()->Init();
					rankingModels_[i].back()->SetObject("dot.obj");
					rankingModels_[i].back()->GetTransform()->SetScale({ 5.0f,5.0f,5.0f });
					rankingModels_[i].back()->SetIsLighting(false);
					rankingModels_[i].back()->SetColor(rankingColors_[i]);

					rankingModels_[i].emplace_back(std::make_unique_for_overwrite<BaseGameObject>());
					rankingModels_[i].back()->Init();
					rankingModels_[i].back()->SetObject(std::to_string(i + 1) + ".obj");
					rankingModels_[i].back()->GetTransform()->SetScale({ 5.0f,5.0f,5.0f });
					rankingModels_[i].back()->SetIsLighting(false);
					rankingModels_[i].back()->SetColor(rankingColors_[i]);

					std::reverse(rankingModels_[i].begin(), rankingModels_[i].end());
					break;
				} else{
					auto& num = rankingModels_[i].emplace_back(std::make_unique_for_overwrite<BaseGameObject>());
					num->Init();
					num->SetObject(std::to_string(((int)gameScoreArray_[i] / digit2) % 10) + ".obj");
					num->GetTransform()->SetTranslaion(translate_);
					num->GetTransform()->SetQuaternion(Quaternion::EulerToQuaternion(rotate_));
					num->GetTransform()->SetTranslaion({ -6.0f,0.0f,10.0f });
					num->GetTransform()->SetScale({ 5.0f,5.0f,5.0f });
					rankingModels_[i].back()->SetIsLighting(false);
					num->SetColor(rankingColors_[i]);
					num->UpdateMatrix();

					digit2 *= 10;
				}
			}
		} else{
			rankingModels_[3].emplace_back(std::make_unique_for_overwrite<BaseGameObject>());
			rankingModels_[3].back()->Init();
			rankingModels_[3].back()->SetObject("dot.obj");
			rankingModels_[3].back()->GetTransform()->SetScale({ 5.0f,5.0f,5.0f });
			rankingModels_[3].back()->SetIsLighting(false);

			rankingModels_[3].emplace_back(std::make_unique_for_overwrite<BaseGameObject>());
			rankingModels_[3].back()->Init();
			rankingModels_[3].back()->SetObject("playerIcon.obj");
			rankingModels_[3].back()->GetTransform()->SetScale({ 5.0f,5.0f,5.0f });
			rankingModels_[3].back()->SetIsLighting(false);

			std::reverse(rankingModels_[3].begin(), rankingModels_[3].end());
		}

		// 文字の更新
		for(int j = 0; j < rankingModels_[i].size(); j++){
			rankingModels_[i][j]->GetTransform()->SetQuaternion(Quaternion::EulerToQuaternion(rankingRotate_));
			rankingModels_[i][j]->GetTransform()->SetTranslaion(
				rankingTranslate_ + Vector3(1.0f, 0.0f, 0.0f) * MakeRotateXYZMatrix(rankingRotate_) * space_ * static_cast<float>(j) +
				Vector3(0.0f, -1.0f, 0.0f) * MakeRotateXYZMatrix(rankingRotate_) * 
				(i != 3 ? (space_ * 2.0f): (space_ * 2.4f)) * static_cast<float>(i)
			);

			rankingModels_[i][j]->Update();
		}
	}
	/*---------------- effect ---------------*/
	tickerTapeEmitter_ = std::make_unique<ParticleManager<TickerTape>>();
	tickerTapeEmitter_->SetEmitRange({ 0.0f,-kWindowHeight_ * 0.2f }, { kWindowWidth_,0.0f });
	tickerTapeEmitter_->SetEmitCountEvery(4);

	/*--------------- sprite ---------------*/
	backgroundSprite_ = Engine::CreateSprite("white.png");
	backgroundSprite_->SetTextureSize({ kWindowWidth_ + 300.0f,kWindowHeight_ + 300.0f });
	backgroundSprite_->SetCenterPos({ kWindowWidth_ * 0.5f,kWindowHeight_ * 0.5f });
	backgroundSprite_->SetColor({ 0.0f,0.0f,0.0f,1.0f });
	backgroundSprite_->Update();

	fade_ = Engine::CreateSprite("white.png");
	fade_->SetTextureSize({ kWindowWidth_ + 300.0f,kWindowHeight_ + 300.0f });
	fade_->SetCenterPos({ kWindowWidth_ * 0.5f,kWindowHeight_ * 0.5f });
	fade_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	fade_->Update();

	if(rank_ == SCORE_RANK::SSS){
		comment_ = Engine::CreateSprite("resultComment.png");
	} else{
		comment_ = Engine::CreateSprite("resultComment2.png");
	}
	comment_->SetTextureSize({ kWindowWidth_,kWindowHeight_ });
	comment_->SetCenterPos({ kWindowWidth_ * 0.5f,kWindowHeight_ * 0.5f });
	comment_->SetColor({ 1.0f,1.0f,1.0f,0.0f });
	comment_->Update();

	debugScale_ = scoreRankModel_->GetTransform()->GetScale();
	debugTranslate_ = scoreRankModel_->GetTransform()->GetTranslation();

	/*---------------- UI ---------------*/
	guideUI_ = std::make_unique<GuideUI>();
	guideUI_->Init();
	guideUI_->SetResult();
}

/////////////////////////////////////////////////////////////////
//							読み込み
/////////////////////////////////////////////////////////////////

void ResultScene::Load(){


	// スコア表示用の数字モデル
	//ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "0.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "1.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "2.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "3.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "4.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "5.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "6.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "7.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "8.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "9.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "m.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "SSS.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "SS.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "S.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "A.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "B.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "C.obj");

	///* ---------------- audio ----------------*/
	//AudioManager::LoadAudio("./Game/Resources/Audio/", "test.wav");
	//AudioManager::LoadAudio("./Game/Resources/Audio/", "kari_coinGet.wav");

	///*----------------- model ----------------*/
	//ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player_Head.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player_Torso.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player_Tail.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/Effect/", "HighSpeedEffect.gltf");
	//ModelManager::LoadModel("./Game/Resources/Model/Wing/", "Wing.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/AddTorso/", "AddTorso.gltf");

	///*------------- animation ---------------*/
	//ModelManager::LoadModel("./Game/Resources/Model/DriftWoodDestroy/", "DriftWoodDestroy.gltf");
	//ModelManager::LoadModel("./Game/Resources/Model/BirdJumpEffect/", "BirdJumpEffect.gltf");
	//ModelManager::LoadModel("./Game/Resources/Model/Effect1/", "Effect1.gltf");
	//ModelManager::LoadModel("./Game/Resources/Model/FishDestroy/", "FishDestroy.gltf");
	//ModelManager::LoadModel("./Game/Resources/Model/JumpEffect/", "JumpEffect.gltf");
	//ModelManager::LoadModel("./Game/Resources/Model/AddTorso/", "AddTorso.gltf");


	///*---------------- texture ---------------*/
	//TextureManager::LoadTextureFile("./Engine/Resources/Develop/", "white.png");

	//// Adio
	//AudioManager::LoadAudio("./Game/Resources/Audio/", "test.wav");
	//AudioManager::LoadAudio("./Game/Resources/Audio/", "kari.wav");
	//AudioManager::LoadAudio("./Game/Resources/Audio/", "kari_coinGet.wav");

	//// ○がついていない物はまだ使用していない
	//AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "boost.mp3");				// ブースト音		○
	//AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "decrementBody.mp3");		// 体の数を減らす		○
	//AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "eat.mp3");				// エサを食べる		○
	//AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "eatAccel.wav");			// エサを食べた時加速する		○
	//AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "gameFinish.wav");		// gameFinish音
	//AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "goalTarget.wav");		// 目標の距離を達成した音
	//AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "hitedBird.wav");			// 鳥に当たった時の音		○
	//AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "incrementBody.mp3");		// 体の数を増やす			○
	//AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "inWaterSurface.mp3");	// 水面に入った時の音			○
	//AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "outWaterSurface.mp3");	// 水面から出た時の音			○
	//AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "BirdJump_3.mp3");			// 鳥を踏んでジャンプ			○
	//AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "overTime.wav");			// オーバータイムの音
	//AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "timeLeft_10s.wav");		// タイムアップ10秒前			○
	//AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "timeLeft_60s.wav");		// タイムアップ60秒前			○
	//AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "timeUp.wav");			// タイムアップの音			○
	//AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "updateFlyingLength.wav");// 飛行距離を伸ばした時の音
	//
	//AudioManager::LoadAudio("./Game/Resources/Audio/BGM/", "mainBGM_tobenaikoi.wav");
	//AudioManager::LoadAudio("./Game/Resources/Audio/BGM/", "mainBGM_tobenaikoi_in_water.wav");
}

/////////////////////////////////////////////////////////////////
//							更新
/////////////////////////////////////////////////////////////////

void ResultScene::Update(){

	// 調整項目の更新
	AdjustmentItem::GetInstance()->Update();

	// ====================== object ====================== //

	player_->ResultSceneUpdate();
	//animationPlayer_->Update();

	// 文字の更新
	//for(int i = 0; i < 3; i++){
	//	for(int j = 0; j < rankingModels_[i].size(); j++){
	//		rankingModels_[i][j]->GetTransform()->SetQuaternion(Quaternion::EulerToQuaternion(rankingRotate_));
	//		rankingModels_[i][j]->GetTransform()->SetTranslaion(
	//			rankingTranslate_ + Vector3(1.0f, 0.0f, 0.0f) * MakeRotateXYZMatrix(rankingRotate_) * space_ * static_cast<float>(j) +
	//			Vector3(0.0f, -1.0f, 0.0f) * MakeRotateXYZMatrix(rankingRotate_) * (space_ * 3.0f) * static_cast<float>(i)
	//		);
	//
	//		rankingModels_[i][j]->Update();
	//	}
	//}

	scoreRankModel_->Update();

	// ====================== effect ====================== //
	tickerTapeEmitter_->Update();

	// ====================== effect ====================== //
	guideUI_->Update();

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


	if(!isStartScene_){
		if(Input::IsTriggerKey(DIK_SPACE)){

			if(!isViewingRanking_){
				isViewingRanking_ = true;
				guideUI_->SetScore();
			} else{
				isEndScene_ = true;
			}
		}
	}

	if(isViewingRanking_){
		commentSpriteAlpha_ += GameTimer::DeltaTime() * 0.5f;
		commentSpriteAlpha_ = std::clamp(commentSpriteAlpha_, 0.0f, 1.0f);
		comment_->SetColor({ 1.0f,1.0f,1.0f,commentSpriteAlpha_ });
		comment_->Update();
	}


	if(rank_ == SCORE_RANK::SSS){
		color_h_ += GameTimer::DeltaTime() * 0.5f;
		color_h_ > 1.0f ? color_h_ = 0.0f : color_h_;
		scoreRankModel_->SetColor(FloatColor(HSV_to_RGB(color_h_, 1.0f, 1.0f, 1.0f)));
	}


	Fade();

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

	Engine::SetPipeline(PipelineType::SkinningPipeline);
	//animationPlayer_->Draw();

	Engine::SetPipeline(PipelineType::NotCullingPipeline);
	for(auto& num : scoreNumberModels_){
		num->Draw();
	}

	for(int i = 0; i < 4; i++){
		for(auto& ranking : rankingModels_[i]){
			ranking->Draw();
		}
	}

	scoreRankModel_->Draw();

	// =======================  sprite ======================= //

	Engine::SetPipeline(PipelineType::NormalBlendSpritePipeline);
	backgroundSprite_->Draw(true);
	comment_->Draw();

	// ====================== effect ====================== //
	tickerTapeEmitter_->Draw();

	// ====================== effect ====================== //
	guideUI_->Draw();

	// =======================  fade ======================= //
	fade_->Draw();
}


// ======================= GUI ========================= //
#ifdef _DEBUG
void ResultScene::Debug_Gui(){



	ImGui::Begin("ResultScene");

	ImGui::DragFloat3("rotate", &rankingRotate_.x, 0.01f);
	//ImGui::DragFloat3("Rotate", &debugRotate_.x, 0.01f);
	ImGui::DragFloat3("Translate", &rankingTranslate_.x, 0.01f);


	scoreRankModel_->GetTransform()->SetTranslaion(debugTranslate_);
	scoreRankModel_->GetTransform()->SetScale(debugScale_);


	ImGui::Checkbox("isDebugCameraActive", &isDebugCameraActive_);
	ImGui::Text("fade : %f", fade_t_);

	if(ImGui::Button("ReTry")) {
		SetNextScene(SceneType::Scene_Game);
	}

	if(StageInformation::GetNowStage() < StageInformation::GetStageNumMax() - 1) {
		if(ImGui::Button("Next")) {
			SetNextScene(SceneType::Scene_Game);
			StageInformation::SetNowStage(StageInformation::GetNowStage() + 1);
		}
	}

	guideUI_->Debug_Gui();

	ImGui::End();
}
#endif 


void ResultScene::Fade(){

	if(isStartScene_){

		fade_t_ = std::clamp(fade_t_ - (GameTimer::DeltaTime() / kFadeTime_), 0.0f, 1.0f);

		if(fade_t_ <= 0.0f){
			isStartScene_ = false;
		}


	} else{
		if(isEndScene_){

			clearBGM_->SetVolume(0.08f * (1.0f - fade_t_));
			fade_t_ = std::clamp(fade_t_ + (GameTimer::DeltaTime() / kFadeTime_), 0.0f, 1.0f);

			if(fade_t_ >= 1.0f){
				clearBGM_->Stop();
				isEndScene_ = false;
				SetNextScene(SceneType::Scene_Game);
			}
		}
	}

	fade_->SetColor({ 1.0f,1.0f,1.0f,fade_t_ });

}