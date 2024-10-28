#pragma once
#include <cstdint>
#include <memory>
#include "Engine/GameObject/BaseGameObject.h"
#include "Game/Manager/AnimetionEffectManager.h"
#include "Engine/Audio/AudioPlayer.h"

// ジャンプの評価を行うクラス
class JumpAssessor{

public:
	JumpAssessor();
	~JumpAssessor();

	void Update();
	void Draw();

public:
	static void SetJumpData(float speed,int32_t bodyCount, WorldTransform* parentWT);

private:
	bool isUIvisible_;
};