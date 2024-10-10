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
	static const int kStageModelCount_ = 4;
	std::unique_ptr<Ground> ground_[kStageModelCount_];
	std::unique_ptr<WaterSpace> waterSpace_[kStageModelCount_];
	std::unique_ptr<Player> player_ = nullptr;

	// --------- parameter ----------- //
	float stageWidthEvery_ = 32.0f;

	// ---------- manager ---------- //
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;

	// ---------- sound ---------- //
	SeData soundData_;
	BgmData bgmData_;
};
