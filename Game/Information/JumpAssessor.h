#pragma once
#include <cstdint>
#include <memory>
#include "Engine/GameObject/BaseGameObject.h"
#include "Game/Manager/AnimetionEffectManager.h"
#include "Engine/Audio/AudioPlayer.h"
#include "Game/Camera/Camera.h"

// ジャンプの評価を行うクラス
class JumpAssessor{
private:
	JumpAssessor();
public:
	~JumpAssessor();
	static JumpAssessor* GetInstance();
	static void Initialize();
	void Update();
	void Draw();

public:
	void SetJumpData(float speed,int32_t bodyCount, WorldTransform* parentWT,Camera* pCamera);

private:
	static JumpAssessor* instance_;
	bool isUIvisible_;

	// 評価に応じて表示するスプライト
	std::array<std::unique_ptr<Sprite>, 4> percentageSprite_;
	std::unique_ptr<Sprite> evaluationSprite_;
	WorldTransform* parentWT_ = nullptr;
	Camera* pCamera_ = nullptr;

	// 数字画像の切り抜きサイズ
	Vector2 numberSpriteSize_ = { 48.0f, 72.0f };
	float widthEvery_ = 48.0f;
};