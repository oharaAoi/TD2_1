#pragma once
#include "Engine.h"

/*---- game object ---*/
#include "Game/GameObject/Ground.h"
#include "Game/GameObject/WaterSpace.h"
#include "Game/GameObject/WorldWall.h"
#include "Game/GameObject/Player.h"
#include "Game/GameObject/Waterweed.h"

/*------ manager,editor ------*/
#include "Engine/ParticleSystem/EffectSystem.h"
#include "Engine/Manager/CollisionManager.h"
#include "Game/Editer/PlacementObjectEditer.h"
#include "Game/Manager/ObstaclesManager.h"

/*------- other -------*/
#include "Game/Scene/BaseScene.h"
#include "Game/Camera/Camera.h"
#include "Game/Camera/DebugCamera.h"
#include "Game/Effect/Trail.h"
#include "Game/Effect/Splash.h"
#include "Game/Information/StageInformation.h"
#include "Game/Information/GamePlayTimer.h"
/*------ UI ------*/
#include "Game/UI/FlyingTimerUI.h"

class TutorialScene : public BaseScene {
public:

	TutorialScene();
	~TutorialScene();

	void Finalize() override;
	void Init() override;
	void Load()  override;
	void Update() override;
	void Draw() const override;

	void CheckAddSplash();

#ifdef _DEBUG
	void Debug_Gui();
#endif 

private:

	// ---------- camera ---------- //
	std::unique_ptr<Camera> camera_ = nullptr;
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;
	bool isDegugCameraActive_ = false;

	// ---------- gameobject ---------- //
	std::unique_ptr<Player> player_ = nullptr;

	// ---------- worldobject ---------- //
	std::unique_ptr<WorldWall> worldWall_ = nullptr;
	std::unique_ptr<Waterweed> waterWeed_ = nullptr;

	std::list<std::unique_ptr<Ground>> grounds_;
	std::list<std::unique_ptr<WaterSpace>> waterSpaces_;

	// ------------ effect ------------ //
	std::list<std::unique_ptr<Splash>>splash_;
};
