#pragma once
#include "Engine.h"
#include <unordered_map>
#include <queue>

/*---- game object ---*/
#include "Game/GameObject/Player.h"
#include "Game/GameObject/Fish.h"
#include "Game/GameObject/Bird.h"
#include "Game/GameObject/WorldObjects.h"
/*------ manager,editor ------*/
#include "Engine/ParticleSystem/EffectSystem.h"
#include "Engine/Manager/CollisionManager.h"
#include "Game/Editer/PlacementObjectEditer.h"
#include "Game/Manager/ObstaclesManager.h"
#include "Engine/Manager/AudioManager.h"
#include "Game/Effect/ParticleManager.h"
#include "Game/Manager/AnimetionEffectManager.h"

/*--------- lib ----------*/
#include "Engine/Math/MyRandom.h"

/*------- other -------*/
#include "Game/Scene/BaseScene.h"
#include "Game/Camera/Camera.h"
#include "Game/Camera/DebugCamera.h"
#include "Game/Information/StageInformation.h"
#include "Game/Information/GamePlayTimer.h"

/*------ Effect ------*/
#include "Game/Effect/Trail.h"
#include "Game/Effect/Splash.h"
#include "Game/Effect/BodyUpEffect.h"
#include  "Game/Effect/Trail.h"
/*------ UI ------*/
#include "Game/UI/FlyingTimerUI.h"
#include "Game/UI/FlyingGaugeUI.h"
#include "Game/UI/PlayerSpeedCounter.h"
#include "Game/UI/TutorialUI.h"
#include "Game/UI/PlayerBodyCountUI.h"
#include "Game/UI/PlayerControlUI.h"
#include "Game/UI/GuideUI.h"
#include "Game/UI/FinishUI.h"
#include "Game/UI/GameStartUI.h"
#include "Game/UI/MissionUI.h"
#include "Game/UI/FadePanel.h"

/*------ Sprite ------*/
#include "Game/Effect/Cherry.h"
#include "Game/Effect/Bubble.h"
#include "Game/Effect/RankUp.h"
/*------ LoadScene ------*/
#include "Game/Scene/LoadScene.h"

// ゲームシーン内での状態分けのenum
enum class GAME_STATE : int32_t{
	TITLE,// タイトルなのにゲームシーンに入れてごめん♡
	LOAD,
	TUTORIAL,
	GAME
};

class GameScene
	: public BaseScene {
public:

	GameScene();
	~GameScene();

	void Finalize() override;
	void Init() override;
	void Load()  override;
	void Update() override;
	void Draw() const override;

#ifdef _DEBUG
	void Debug_Gui();
	
#endif 

	void Update_TITLE();
	void Update_TUTORIAL();
	void Update_GAME();
	void UpdateColliderList();

public:
	static float GetGroundDepth() { return groundDepth_; }
	static GAME_STATE GetGameState() { return currentState_; }
	static GAME_STATE GetPreGameState() { return preState_; }
	void CheckAddSplash();

private:

	std::unique_ptr<Camera> camera_ = nullptr;
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;
	bool isDegugCameraActive_ = false;

	std::string start1 = "stage_1_1";
	std::string start2 = "stage_1_2";

	// ---------- gameobject ---------- //
	std::unique_ptr<Player> player_ = nullptr;

	// ------------ effect ------------ //
	std::unique_ptr<Trail> trail_ = nullptr;
	std::list<std::unique_ptr<Splash>>splash_;

	// ---------- worldobject ---------- //
	std::unique_ptr<WorldObjects> worldObjects_;

	Vector4 editColor_;
	
	// --------- parameter ----------- //
	float startSceneTime_ = 2.0f;
	float stageWidth_ = 8000.0f;
	float stageWidthEvery_ = 32.0f;
	static float groundDepth_;
	bool isEndAndInWater_ = false;
	float fadeWaitTime_ = 0.5f;
	float outgameWaitTime_ = 3.0f;
	bool startSceneChange_;//シーン遷移が始まっているか
	float BGM_volumeT_ = 1.0f;
	float BGM_masterVolumeRate_ = 1.0f;
	float level2=350;
	float level3=650;
	float speedMeterAlpha_;
	bool isStartupScene_ = true;
	float maxHeight_ = 0.0f;
	int rankCount_ = -1;
	std::queue<float>borders_;
	std::string rankTextureNames[4] = { "A.png","S.png","SS.png","SSS.png"};

	bool isGameStart_;

	// ---------- manager ---------- //
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;
	std::unique_ptr<ObstaclesManager> obstaclesManager_ = nullptr;
	std::unique_ptr<ParticleManager<Cherry>> cherryEmitter_ = nullptr;
	std::unique_ptr<ParticleManager<Bubble>> bubbleEmitter_ = nullptr;
	std::unique_ptr<ParticleManager<RankUp>> rankUpParticleEmitter_ = nullptr;

	AnimetionEffectManager* animationEffectManager_;

	// ---------- editor ----------- //

	std::unique_ptr<PlacementObjectEditer> placementObjectEditor_ = nullptr;

	// ---------- information ---------- //

	std::unique_ptr<GamePlayTimer> gamePlayTimer_;
	static GAME_STATE currentState_;
	static GAME_STATE preState_;

	// ---------- UI ---------- //
	std::unique_ptr<FlyingTimerUI> flyingTimerUI_ = nullptr;
	std::unique_ptr<FlyingGaugeUI> flyingGaugeUI_ = nullptr;
	std::unique_ptr<PlayerSpeedCounter> playerSpeedCounter_ = nullptr;
	std::unique_ptr<TutorialUI> tutorialUI_ = nullptr;
	std::unique_ptr<PlayerBodyCountUI> playerBodyCountUI_ = nullptr;
	std::unique_ptr<PlayerControlUI> playerControlUI_ = nullptr;
	std::unique_ptr<GuideUI> guideUI_ = nullptr;
	std::unique_ptr<FinishUI> finishUI_ = nullptr;
	std::unique_ptr<GameStartUI> gameStartUI_ = nullptr;
	std::unique_ptr<MissionUI> missionUI_ = nullptr;
	// パネル
	std::unique_ptr<FadePanel> fadePanel_ = nullptr;

	// ---------- Effect ---------- //
	std::unique_ptr<BodyUpEffect> bodyUpEffect_ = nullptr;

	// --------- Load ----------- //
	std::unique_ptr<LoadScene> loadScene_;
	
	// ------------ Sprite ---------- //
	std::unique_ptr<Sprite>titleLogo_ = nullptr;
	std::unique_ptr<Sprite>sky_ = nullptr;
	std::unique_ptr<Sprite>fade_ = nullptr;
	// ------------ Audio ---------- //
	std::unique_ptr<AudioPlayer> mainBGM_;
	std::unique_ptr<AudioPlayer> mainBGM_inWater_;
	std::unique_ptr<AudioPlayer> windSound_;
	std::unique_ptr<AudioPlayer> swimSound_;

	// ---------- nextSceneを変える用 ---------- //
	float notControlTime_;
	float notControlTimeLimit_ = 5.0f;

	//デバッグ用
	std::unique_ptr<BaseGameObject> debugModel_;
	bool isGuiDraw_ = true;
};
