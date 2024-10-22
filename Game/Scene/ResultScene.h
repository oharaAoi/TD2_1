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
#include "Game/Information/FlyingTimer.h"
#include "Game/Information/Score.h"

/*------ UI ------*/

/*------ Sprite ------*/


enum class SCORE_RANK : int32_t{
	SSS = 5000,
	SS = 3000,
	S = 2000,
	A = 1500,
	B = 500,
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

	void Fade();

public:

	static bool GetIsViewRanking() { return isViewingRanking_; }

private:

	/*---------- parameter ----------*/
	Vector3 rotate_;
	Vector3 translate_;
	float space_ = 2.0f;

	float kFadeTime_ = 2.0f;
	float fade_t_ = 1.0f;
	bool isEndScene_ = false;
	bool isStartScene_ = true;
	static bool isViewingRanking_;

	int score_ = FlyingTimer::flyingScor;
	SCORE_RANK rank_;

	/*---------- debug ----------*/
	Vector3 debugScale_;
	Vector3 debugRotate_;
	Vector3 debugTranslate_;

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
	std::unique_ptr<Sprite> fade_ = nullptr;

	// ---------- Score --------- //
	std::unique_ptr<Score> scoreData_;
	float gameScoreArray_[3];
};

