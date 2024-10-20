#pragma once
#include <memory>
#include "Engine.h"
#include "Engine/GameObject/BaseGameObject.h"
#include "Game/GameObject/Player.h"

class PlayerAnimator {
public:

	PlayerAnimator();
	~PlayerAnimator();

	void Init(Player* pPlayer);
	void Update();
	void Draw() const;
	
private:

	Player* pPlayer_ = nullptr;

	std::unique_ptr<BaseGameObject> playerHiSpeedEffect_;

};

