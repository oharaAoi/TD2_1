#include "SceneManager.h"

SceneManager::SceneManager() {}

SceneManager::~SceneManager() {}

void SceneManager::Finalize() {
	effectSystem_->Finalize();
	MeshManager::GetInstance()->Finalize();
	ModelManager::GetInstance()->Finalize();
	Engine::Finalize();
}

void SceneManager::Init() {
	Engine::Initialize(kWindowWidth_, kWindowHeight_);
	ModelManager::GetInstance()->Init();

	Load();
	
	scene_ = std::make_unique<GameScene>();
	scene_->Init();

	effectSystem_ = EffectSystem::GetInstacne();
	effectSystem_->Init();
}

void SceneManager::Run() {
	while (Engine::ProcessMessage()) {
		Engine::BeginFrame();

		// ------------------------------------ //
		// 更新処理
		// ------------------------------------ //
		scene_->Update();

		effectSystem_->Update();

		Render::Update();

		// ------------------------------------ //
		// 描画処理
		// ------------------------------------ //

		scene_->Draw();

		Engine::SetPipeline(PipelineType::ParticlePipeline);
		effectSystem_->Draw();

		// ------------------------------------ //
		// シーン変更があるか
		// ------------------------------------ //
		#ifdef _DEBUG
		/*ImGui::Begin("SceneManager");
		if (ImGui::Combo("Scene", &selectSceneNum_, "Title\0Tutorial\0Game\0Result\0Test\0")) {
			isSceneChange_ = true;

		}
		ImGui::End();*/

		// ------------------------------------ //
		// EffectEditerの処理
		// ------------------------------------ //

		if (effectSystem_->GetIsEffectEditer()) {
			effectSystem_->BeginEditer();
			effectSystem_->UpdateEditer();
			Engine::SetPipeline(PipelineType::ParticlePipeline);
			effectSystem_->DrawEditer();
		}
		#endif
		gameTimer_.FPS();

		// ------------------------------------ //
		// フレーム終了処理
		// ------------------------------------ //
		Engine::DrawRenderTexture();
		Engine::EndImGui();

		#ifdef _DEBUG
		/*if (effectSystem_->GetIsEffectEditer()) {
			effectSystem_->EndEditer();
		}*/
		#endif

		Engine::EndFrame();

		// fpsの計算
		gameTimer_.CalculationFrame();

		if (scene_->GetType()) {
			SetChange();
			scene_->SetNextScene(std::nullopt);
		}

		// ------------------------------------ //
		// シーン変更があったら
		// ------------------------------------ //
		#ifdef _DEBUG
		if (isSceneChange_) {
			switch (selectSceneNum_) {
			case SceneType::Scene_Title:
				scene_.reset(new TitleScene);
				break;
			case SceneType::Scene_Tutorial:
				scene_.reset(new TutorialScene);
				break;
			case SceneType::Scene_Game:
				scene_.reset(new GameScene);
				break;
			case SceneType::Scene_Result:
				scene_.reset(new ResultScene);
				break;
			case SceneType::Scene_Test:
				scene_.reset(new TestScene);
				break;
			}
	
			scene_->Init();
			isSceneChange_ = false;
		}
		#endif
	}

	Finalize();
}

void SceneManager::Load() {
	TextureManager::LoadTextureFile("./Engine/Resources/Develop/", "white.png");

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
	ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "dot.obj");
	//ModelManager::LoadModel("./Game/Resources/Model/Player_result/", "Player_result.fbx");

	ModelManager::LoadModel("./Game/Resources/Model/EatFish/", "EatFish.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/AnimationWing/", "Wing.gltf");

	ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player_Head.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player_Torso.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Player/", "Player_Tail.obj");
	ModelManager::LoadModel("./Game/Resources/Model/ResultNumbers/", "playerIcon.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Driftwood/", "Driftwood2.obj");
	ModelManager::LoadModel("./Game/Resources/Model/", "Rock.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Bird/", "Bird.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/", "Waterweed.obj");
	ModelManager::LoadModel("./Game/Resources/Model/", "Ripple.obj");
	ModelManager::LoadModel("./Game/Resources/Model/", "WaterColmn.obj");
	ModelManager::LoadModel("./Game/Resources/Model/", "Splash.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Mountain/", "Mountain.obj");
	ModelManager::LoadModel("./Game/Resources/Model/MountenTree/", "MountenTree.obj");
	ModelManager::LoadModel("./Game/Resources/Model/MountainGrass/", "MountainGrass.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Cloud/", "Cloud.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Moai/", "Moai.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Nico/", "Nico.obj");
	ModelManager::LoadModel("./Game/Resources/Model/MountainUFO/", "MountainUFO.obj");
	ModelManager::LoadModel("./Game/Resources/Model/UFOSmoke/", "UFOSmoke.obj");
	ModelManager::LoadModel("./Game/Resources/Model/HeightMeter/", "HeightMeter.obj");
	// UI
	ModelManager::LoadModel("./Game/Resources/Model/evaluationNice/", "evaluationNice.obj");
	ModelManager::LoadModel("./Game/Resources/Model/UI_Plane/", "UI_Plane.obj");

	ModelManager::LoadModel("./Game/Resources/Model/WorldWall/", "WorldWall.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Fish/", "Fish.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/WaterWeed/", "Ground_WaterPlant.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Ground/", "Riverbed.obj");

	ModelManager::LoadModel("./Game/Resources/Model/Trail/", "waterTrail.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Trail/", "skyTrail.obj");
	ModelManager::LoadModel("./Game/Resources/Model/Effect/", "HighSpeedEffect.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/TutorialGuide/", "TutorialGuide.obj");
	ModelManager::LoadModel("./Game/Resources/Model/MaxHeight/", "MaxHeight.obj");

	// 仕様上連続して読み込みたい物
	ModelManager::LoadModel("./Game/Resources/Model/Watersurface/", "Watersurface.obj");
	TextureManager::LoadTextureFile("./Game/Resources/Model/", "normalMap.png");
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
	//TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "RankBack.png");
	//TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "RankFront.png");
	//TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "Rank.png");
	//TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "kari_UI_Rank_master.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "RankIcon.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "RankIcon_max.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "speedMeterBack.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "tani.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "MaterStaple.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "Mater.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "timer.png");
	//TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "UI_fly.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "UI_fly1.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "UI_fly2.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "bodyMax.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "finish.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "overTime.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "TorsoGaugeFront.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "TorsoGaugeBack.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "timer10.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "timer60.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "UI_Titlle.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "UI_space.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "UI_game.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "UI_tutorial.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "UI_arrow.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "gameStart.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "Tutorial_4.png");

	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "missionSpeed_1.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "missionSpeed_2.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "missionHeight_1.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "missionHeight_2.png");

	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "missionComplete.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "missionClear.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "missionClearAddTime.png");

	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "KoiGeuge_Head.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "KoiGeuge_Torso.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "KoiGeuge_Tail.png");

	// time
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "TestTimeGauge.png");
	// limitTime
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "BigNumber1.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "BigNumber2.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "BigNumber3.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "BigNumber4.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/UI/", "BigNumber5.png");

	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "Tutorial_1.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "Tutorial_2.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "resultComment.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "resultComment2.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "tower.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "blueSky_ORE.png");
	//TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "kari.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "tutorial_3.png");
	TextureManager::LoadTextureFile("./Game/Resources/Sprite/", "UIeffect.png");
	
	TextureManager::LoadTextureFile("./Game/Resources/Model/", "FishCanEat.png");

	// animationEffect
	ModelManager::LoadModel("./Game/Resources/Model/DriftWoodDestroy/", "DriftWoodDestroy.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/BirdJumpEffect/", "BirdJumpEffect.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/Effect1/", "Effect1.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/FishDestroy/", "FishDestroy.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/JumpEffect/", "JumpEffect.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/AddTorso/", "AddTorso.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/EatEffect/", "EatEffect.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/AddSpeedEffect/", "AddSpeedEffect.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/SlowEffect/", "SlowEffect.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/AddSpeedEffect/", "AddSpeedEffect.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/evaluationNice/", "evaluationNice.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/evaluationGood/", "evaluationGood.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/evaluationPERFECT/", "evaluationPERFECT.gltf");
	ModelManager::LoadModel("./Game/Resources/Model/FishWeakening/", "FishWeakening.gltf");


	// Adio
	//AudioManager::LoadAudio("./Game/Resources/Audio/", "kari.wav");
	AudioManager::LoadAudio("./Game/Resources/Audio/", "kari_coinGet.wav");

	// ○がついていない物はまだ使用していない
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "boost.mp3");				// ブースト音		○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "decrementBody.mp3");		// 体の数を減らす		○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "eat.mp3");				// エサを食べる		○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "AddSpeed.mp3");			// エサを食べた時加速する		○
	//AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "gameFinish.wav");		// gameFinish音
	//AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "goalTarget.wav");		// 目標の距離を達成した音
	//AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "hitedBird.wav");			// 鳥に当たった時の音		○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "incrementBody.mp3");		// 体の数を増やす			○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "inWaterSurface.mp3");	// 水面に入った時の音			○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "outWaterSurface.mp3");	// 水面から出た時の音			○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "BirdJump_3.mp3");			// 鳥を踏んでジャンプ			○
	//AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "overTime.wav");			// オーバータイムの音
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "timeRemaining.mp3");		// タイムアップ10秒前			○
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "timeRemaining.mp3");		// タイムアップ60秒前			○
	//AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "timeUp.wav");			// タイムアップの音			○
	//AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "updateFlyingLength.wav");// 飛行距離を伸ばした時の音
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "confusion.mp3");// 被弾した時の音
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "Damage.mp3");// 被弾した時の音2
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "Bubble.mp3");//　シーン遷移の音
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "MoveChangeUp.mp3");//　水中で上下移動を切り替えた時
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "MoveChangeDown.mp3");//　空中で上下移動を切り替えた時
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "WingOpen.mp3");//　空中で上下移動を切り替えた時
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "missionClear.mp3");//　ミッションクリア時の音
	AudioManager::LoadAudio("./Game/Resources/Audio/SystemSE/", "Finish.mp3");//　空中で上下移動を切り替えた時
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "test.wav");//被弾時の音
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "NiceSE.mp3");//Nice音
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "GoodSE.mp3");//Good音
	AudioManager::LoadAudio("./Game/Resources/Audio/GameSE/", "ParfectSE.mp3");//Parfect音
	
	AudioManager::LoadAudio("./Game/Resources/Audio/BGM/", "mainBGM_tobenaikoi.mp3");
	AudioManager::LoadAudio("./Game/Resources/Audio/BGM/", "mainBGM_tobenaikoi_in_water.mp3");
	AudioManager::LoadAudio("./Game/Resources/Audio/BGM/", "brow.mp3");
	AudioManager::LoadAudio("./Game/Resources/Audio/BGM/", "swim.mp3");
	AudioManager::LoadAudio("./Game/Resources/Audio/BGM/", "clearBGM.mp3");
}

void SceneManager::SetChange() {
	GetScene();
	scene_->Init();
}

void SceneManager::GetScene() {
	switch (scene_->GetType().value()) {
	case SceneType::Scene_Title:
		scene_.reset(new TitleScene);
		break;
	case Scene_Tutorial:
		scene_.reset(new TutorialScene);
		break;
	case SceneType::Scene_Game:
		scene_.reset(new GameScene);
		break;
	case SceneType::Scene_Result:
		scene_.reset(new ResultScene);
		break;
	case SceneType::Scene_Test:
		scene_.reset(new TestScene);
		break;
	}
}
