#include "ResultScene.h"

ResultScene::ResultScene(){
	Init();
}

ResultScene::~ResultScene(){}

void ResultScene::Finalize(){}

/////////////////////////////////////////////////////////////////
//							初期化
/////////////////////////////////////////////////////////////////

void ResultScene::Init(){

	// 読み込み
	Load();

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


	int digit = 1;
	while(true){

		if(score_ / digit <= 0){
			break;
		} else{
			auto& num = scoreNumberModels_.emplace_back(std::make_unique_for_overwrite<BaseGameObject>());
			num->Init();
			num->SetObject(std::to_string((score_ / digit) % 10) + ".obj");
			num->GetTransform()->SetTranslaion(camera_->GetWorldTranslate());
			num->GetTransform()->SetScale({ 5.0f,5.0f,5.0f });
			num->UpdateMatrix();
			digit *= 10;
		}
	}

	/*---------------- effect ---------------*/
	tickerTapeEmitter_ = std::make_unique<ParticleManager<TickerTape>>();
	tickerTapeEmitter_->SetEmitRange({ 0.0f,-kWindowHeight_ * 0.2f }, { kWindowWidth_,0.0f });
	tickerTapeEmitter_->SetEmitCountEvery(4);

	/*--------------- sprite ---------------*/
	backgroundSprite_ = Engine::CreateSprite("white.png");
	backgroundSprite_->SetTextureSize({kWindowWidth_,kWindowHeight_});
	backgroundSprite_->SetCenterPos({kWindowWidth_ * 0.5f,kWindowHeight_ * 0.5f});
	backgroundSprite_->SetColor({ 0.0f,0.0f,0.0f,1.0f });
	backgroundSprite_->Update();


}

/////////////////////////////////////////////////////////////////
//							読み込み
/////////////////////////////////////////////////////////////////

void ResultScene::Load(){

	//ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player.fbx");
	//ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player_Head.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player_Torso.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player_Tail.obj");

	//ModelManager::LoadModel("./Game/Resources/Model/Wood/", "Wood.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/Grass/", "Grass.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/Grass/", "Grass2.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/Partition/", "Partition.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/", "Item.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/", "Rock.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/Bird/", "Bird.gltf");
	//ModelManager::LoadModel("./Game/Resources/Model/", "Waterweed.obj");

	//ModelManager::LoadModel("./Game/Resources/Model/", "Ripple.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/", "WaterColmn.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/", "Splash.obj");

	//ModelManager::LoadModel("./Engine/Resources/Develop/", "skin.obj");

	//ModelManager::LoadModel("./Game/Resources/Model/WorldWall/", "WorldWall.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/Coin/", "Coin.gltf");
	//ModelManager::LoadModel("./Game/Resources/Model/Fish/", "Fish.gltf");
	//ModelManager::LoadModel("./Game/Resources/Model/WaterWeed/", "Ground_WaterPlant.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/Ground/", "Riverbed.obj");

	//ModelManager::LoadModel("./Game/Resources/Model/Trail/", "waterTrail.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/Trail/", "skyTrail.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/Effect/", "staer.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/Effect/", "HighSpeedEffect.gltf");

	// 仕様上連続して読み込みたい物
	ModelManager::LoadModel("./Game/Resources/Model/Watersurface/", "Watersurface.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/", "waterSpace.obj");
	TextureManager::LoadTextureFile("./Game/Resources/Model/", "normalMap.png");

	ModelManager::LoadModel("./Game/Resources/Model/", "ground.obj");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "WaterLight.png");

	// Texture
	TextureManager::LoadTextureFile("./Engine/Resources/Develop/", "uvChecker.png");
	TextureManager::LoadTextureFile("./Engine/Resources/Develop/", "sample.png");

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


	//デバッグ用、モデル確認
	ModelManager::LoadModel("./Game/Resources/Model/Mountain/", "Mountain.obj");
	ModelManager::LoadModel("./Game/Resources/Model/MountenTree/", "MountenTree.obj");
	ModelManager::LoadModel("./Game/Resources/Model/MountainGrass/", "MountainGrass.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Cloud/", "Cloud.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Moai/", "Moai.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Nico/", "Nico.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Wing/", "Wing.obj");
	ModelManager::LoadModel("./Game/Resources/Model/MountainUFO/", "MountainUFO.obj");
	ModelManager::LoadModel("./Game/Resources/Model/FishDestroy/", "FishDestroy.gltf");

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
	/*---------------- texture ---------------*/
	TextureManager::LoadTextureFile("./Engine/Resources/Develop/", "white.png");
}

/////////////////////////////////////////////////////////////////
//							更新
/////////////////////////////////////////////////////////////////

void ResultScene::Update(){

	// ====================== object ====================== //

	player_->ResultSceneUpdate();

	for(auto& num : scoreNumberModels_){
		num->Update();
	}

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