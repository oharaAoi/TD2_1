#pragma once
#include "Engine.h"
#include "Game/Scene/BaseScene.h"
#include "Game/Camera/BaseCamera.h"
#include "Game/Scene/TutorialScene.h"
#include "Game/Scene/GameScene.h"
#include "Game/UI/TitleUI.h"

class TitleScene 
	: public BaseScene {
public:

	TitleScene();
	~TitleScene();

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

	// ---------- UI ---------- //
	std::unique_ptr<TitleUI> UI_title_ = nullptr;

};

