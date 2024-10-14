#pragma once
#include "Engine.h"
#include "Game/Scene/BaseScene.h"
#include "Game/Camera/BaseCamera.h"
#include "Game/Information/StageInformation.h"
#include "Engine/Input/Input.h"

class SelectScene 
	: public BaseScene {
public:

	SelectScene();
	~SelectScene();

	void Finalize() override;
	void Init() override;
	void Load()  override;
	void Update() override;
	void Draw() const override;

	void ChangeStage();

#ifdef _DEBUG
	void Debug_Gui();
#endif 

private:
	// 
	uint32_t selectStageNum_ = 0;
	StageInformation stageInformation_;

	// ---------- camera ---------- //
	std::unique_ptr<BaseCamera> camera_ = nullptr;

};

