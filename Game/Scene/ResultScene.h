#pragma once
#include "Engine.h"
#include "Game/Information/StageInformation.h"
#include <unordered_map>
#include <algorithm> 

/*---- game object ---*/
#include "Game/GameObject/Player.h"

/*------ manager,editor ------*/
#include "Engine/Manager/CollisionManager.h"
#include "Game/Editer/PlacementObjectEditer.h"
#include "Engine/Manager/AudioManager.h"
#include "Game/Effect/ParticleManager.h"
#include "Game/Manager/AnimetionEffectManager.h"

/*--------- lib ----------*/
#include "Engine/Math/MyRandom.h"

/*----------- effect -----------*/
#include "Game/Effect/TickerTape.h"

/*------- other -------*/
#include "Game/Scene/BaseScene.h"
#include "Game/Camera/Camera.h"
#include "Game/Camera/DebugCamera.h"
#include "Game/Camera/ResultCamera.h"

/*------ UI ------*/

/*------ Sprite ------*/


enum class SCORE_RANK : int32_t{
	SSS = 634,
	SS = 500,
	S = 365,
	A = 200,
	B = 100,
	C = 0
};


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

	/*---------- parameter ----------*/
	Vector3 rotate_;
	Vector3 translate_;
	float space_ = 2.0f;
	int score_ = 634;
	SCORE_RANK rank_;

private:

	/*----------- effect -----------*/
	std::unique_ptr<ParticleManager<TickerTape>>tickerTapeEmitter_;

	// ----------- object --------- //
	std::unique_ptr<Player> player_ = nullptr;
	std::vector<std::unique_ptr<BaseGameObject>>scoreNumberModels_;
	std::unique_ptr<BaseGameObject>scoreRankModel_ = nullptr;

	// ---------- camera ---------- //
	bool isDebugCameraActive_;
	std::unique_ptr<BaseCamera> camera_ = nullptr;
	std::unique_ptr<BaseCamera> debugCamera_ = nullptr;

	// ---------- Sprite --------- //
	std::unique_ptr<Sprite> backgroundSprite_ = nullptr;

};

