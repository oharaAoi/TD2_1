#pragma once
#include "Engine.h"
#include "Engine/Input/Input.h"
#include "Game/Scene/BaseScene.h"
#include "Engine/Utilities/DrawUtils.h"
#include "Engine/GameObject/BaseGameObject.h"
#include "Game/Camera/Camera.h"
#include "Game/Camera/DebugCamera.h"
#include "Engine/ParticleSystem/EffectSystem.h"
#include "Engine/Manager/CollisionManager.h"

#include "Game/Editer/PlacementObjectEditer.h"
#include "Engine/Utilities/AdjustmentItem.h"


class TestScene 
: public BaseScene {
public:


public:

	TestScene();
	~TestScene() override;

	void Finalize() override;
	void Init() override;
	void Load() override;
	void Update() override;
	void Draw() const override;

#ifdef _DEBUG
	void ImGuiDraw();
#endif

private:

	// camera ----------------------------------------------
	std::unique_ptr<Camera> camera_ = nullptr;
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;

	// gameObject -----------------------------------
	std::unique_ptr<BaseGameObject> testObj_ = nullptr;
	std::unique_ptr<BaseGameObject> testObj2_ = nullptr;
	
	// sprite -----------------------------------
	std::unique_ptr<Sprite> sprite_;
	Vector2 range_;
	Vector2 leftTop_;
	
	// Manager ------------------------------------------
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;

	// editer ------------------------------------------
	std::unique_ptr<PlacementObjectEditer> placementObjEditer_ = nullptr;
	std::unique_ptr<AdjustmentItem> adjustment_ = nullptr;
};

