#pragma once
#include "Engine.h"
#include <unordered_map>

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
#include "Game/Effect/Trail.h"
#include "Game/Effect/Splash.h"
#include "Game/Information/StageInformation.h"
#include  "Game/Effect/Trail.h"
#include "Game/Information/GamePlayTimer.h"
/*------ UI ------*/
#include "Game/UI/FlyingTimerUI.h"
#include "Game/UI/FlyingGaugeUI.h"
#include "Game/UI/PlayerSpeedCounter.h"
#include "Game/UI/TutorialUI.h"
/*------ Sprite ------*/
#include "Game/Effect/Cherry.h"
#include "Game/Effect/Bubble.h"

// ゲームシーン内での状態分けのenum
enum class GAME_STATE : int32_t{
	TITLE,// タイトルなのにゲームシーンに入れてごめん♡
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

	// ---------- gameobject ---------- //
	std::unique_ptr<Player> player_ = nullptr;

	// ------------ effect ------------ //
	std::unique_ptr<Trail> trail_ = nullptr;
	std::list<std::unique_ptr<Splash>>splash_;

	// ---------- worldobject ---------- //
	std::unique_ptr<WorldObjects> worldObjects_;

	std::unordered_map<std::string, std::unique_ptr<BaseGameObject>>backgroundObjects_;

	Vector4 editColor_;
	std::unique_ptr<BaseGameObject>partition_ = nullptr;

	// --------- parameter ----------- //
	float stageWidth_ = 8000.0f;
	float stageWidthEvery_ = 32.0f;
	static float groundDepth_;
	float fadeWaitTime_ = 0.5f;
	float outgameWaitTime_ = 3.0f;
	float BGM_volumeT_ = 1.0f;

	// ---------- manager ---------- //
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;
	std::unique_ptr<ObstaclesManager> obstaclesManager_ = nullptr;
	std::unique_ptr<ParticleManager<Cherry>> cherryEmitter_ = nullptr;
	std::unique_ptr<ParticleManager<Bubble>> bubbleEmitter_ = nullptr;

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

	// ------------ Sprite ---------- //
	std::unique_ptr<Sprite>titleLogo_ = nullptr;
	std::unique_ptr<Sprite>sky_ = nullptr;
	std::unique_ptr<Sprite>fade_ = nullptr;

	// ------------ Audio ---------- //
	std::unique_ptr<AudioPlayer> mainBGM_;
	std::unique_ptr<AudioPlayer> mainBGM_inWater_;
	std::unique_ptr<AudioPlayer> windSound_;
	std::unique_ptr<AudioPlayer> swimSound_;

	//デバッグ用
	std::unique_ptr<BaseGameObject> debugModel_;
	bool isGuiDraw_ = true;
};
