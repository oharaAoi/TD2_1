#pragma once
#include "Engine.h"
#include "Game/Scene/BaseScene.h"
#include "Game/Camera/BaseCamera.h"
#include "Game/Information/StageInformation.h"

class ResultScene 
	: public BaseScene {
public:

	ResultScene();
	~ResultScene();

	void Finalize() override;
	void Init() override;
	void Load()  override;
	void Update() override;
	void Draw() const override;

#ifdef _DEBUG
	void Debug_Gui();
#endif 

private:

	// ---------- camera ---------- //
	std::unique_ptr<BaseCamera> camera_ = nullptr;

};
