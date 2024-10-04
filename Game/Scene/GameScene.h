#pragma once
#include "Engine.h"
#include "Game/Scene/BaseScene.h"
#include "Engine/Lib/Transform.h"
#include "Game/Camera/Camera.h"
#include "Game/Camera/DebugCamera.h"
#include "Engine/ParticleSystem/EffectSystem.h"

class GameScene 
	: public BaseScene {
public:

	GameScene();
	~GameScene();

	void Init() override;
	void Load()  override;
	void Update() override;
	void Draw() const override;

private:

	std::unique_ptr<Camera> camera_ = nullptr;

	// ---------- sound ---------- //
	SeData soundData_;
	BgmData bgmData_;
};
