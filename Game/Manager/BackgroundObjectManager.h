#pragma once
#include <list>
#include <memory>
#include "Engine/GameObject/BaseGameObject.h"
#include "Game/GameObject/Player.h"
#include "Engine/Math/MyRandom.h"

class BackgroundObjectManager{

public:
	BackgroundObjectManager() = default;
	~BackgroundObjectManager();

	void Initialize(Player* pPlayer);
	void Update();
	void Draw();

private:

	void AddObjects();
	void EraseObjects();

public:
	void SetPlayerPtr(Player* pPlayer){ pPlayer_ = pPlayer; }

private:

	/*------ parameter ------*/
	float baseZ_ = 20.0f;
	const Vector2 kGenerateRange_ = {100.0f,20.0f};
	float currentRightEnd_;
	int32_t subdivision_ = 5;

	/*--------- ptr ---------*/
	Player* pPlayer_ = nullptr;

	/*------- objects -------*/
	std::list<std::unique_ptr<BaseGameObject>>woods_;
	std::list<std::unique_ptr<BaseGameObject>>grasses_;
	std::list<std::unique_ptr<BaseGameObject>>rocks_;
};