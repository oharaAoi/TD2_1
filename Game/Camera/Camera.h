#pragma once
#include "Game/Camera/BaseCamera.h"
#include "Game/GameObject/Player.h"

class Camera : public BaseCamera {
public:

	Camera();
	~Camera() override;

	void Finalize() override;
	void Init() override;
	void Update() override;

#ifdef _DEBUG
	void Debug_Gui() override;
#endif

public:

	void SetPlayerPtr(Player* pPlayer){ pPlayer_ = pPlayer; }

private:

	Player* pPlayer_ = nullptr;
	Vector3 offsetVec_;
	float offsetLength_;
};