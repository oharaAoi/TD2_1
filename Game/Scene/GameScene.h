#pragma once
#include "Engine.h"
#include "Game/Scene/BaseScene.h"
// lib
#include "Engine/Lib/Transform.h"
#include "Engine/Utilities/DrawUtils.h"
// gameObject
#include "Game/Camera/Camera.h"

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
	std::unique_ptr<Sphere> sphere_ = nullptr;

	// ---------- sound ---------- //
	SeData soundData_;
	BgmData bgmData_;
};
