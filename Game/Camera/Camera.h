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
	void Shake();

#ifdef _DEBUG
	void Debug_Gui() override;
#endif

public:

	void SetPlayerPtr(Player* pPlayer){ pPlayer_ = pPlayer; }
	static void ShakeStart(float shakeTime, float shakeRadius){
		shakeTime_ = shakeTime;
		shakeRadius_ = shakeRadius_;
	}


private:

	Player* pPlayer_ = nullptr;
	Vector3 offsetVec_;
	float offsetLength_;
	Vector3 baseRotate = { 0.0f, 0.91f, 0.0f };
	float velocityRate_;
	float defaultVelocity_ = 50.0f;
	float adjustX_;
	bool followOnly;
	float follwOfsetlength=4;

	Vector3 shakeTranslate_;
	static float shakeRadius_;
	static float  shakeTime_;

	bool isTitleToGameCamera_;

};