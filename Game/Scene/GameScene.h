#pragma once
#include "Engine.h"
#include "Game/Scene/BaseScene.h"
#include "Game/Camera/Camera.h"
#include "Game/Camera/DebugCamera.h"
#include "Engine/ParticleSystem/EffectSystem.h"
#include "Game/GameObject/Ground.h"
#include "Game/GameObject/WaterSpace.h"
#include "Game/GameObject/Player.h"
#include "Engine/Manager/CollisionManager.h"
#include "Game/Editer/PlacementObjectEditer.h"
#include "Game/Manager/ObstaclesManager.h"
#include  "Game/Effect/Trail.h"


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

	void PlayerWaveCollision();
	void EndlessStage();

private:

	std::unique_ptr<Camera> camera_ = nullptr;
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;
	bool isDegugCameraActive_ = false;

	// ---------- gameobject ---------- //
	static const int kStageModelCount_ = 1;
	std::unique_ptr<Ground> ground_[kStageModelCount_];
	std::unique_ptr<WaterSpace> waterSpace_[kStageModelCount_];
	std::unique_ptr<Player> player_ = nullptr;
	std::unique_ptr<Trail> trail_ = nullptr;

	// --------- parameter ----------- //
	float stageWidthEvery_ = 32.0f;

	// ---------- manager ---------- //
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;
	std::unique_ptr<ObstaclesManager> obstaclesManager_ = nullptr;

	// ---------- editor ----------- //

	std::unique_ptr<PlacementObjectEditer> placementObjectEditor_ = nullptr;

	// ---------- sound ---------- //
	SeData soundData_;
	BgmData bgmData_;
};
