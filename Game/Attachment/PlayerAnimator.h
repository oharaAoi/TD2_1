#pragma once
#include <memory>
#include "Engine.h"
#include "Engine/GameObject/BaseGameObject.h"
#include "Game/GameObject/Player.h"


class PlayerAnimator{
public:

	PlayerAnimator();
	~PlayerAnimator();

	void Init(Player* pPlayer);
	void Update();
	void Draw() const;

private:

	Player* pPlayer_ = nullptr;

	std::unique_ptr<BaseGameObject> playerHiSpeedEffect_;
	float drawSpeed=80;
	float easeT;
	Vector3 sizeMin = { 0.1f,0.1f,0.1f };
	Vector3 sizeMax = { 1.0f,1.0f,1.0f };

};

