#pragma once
#include "Game/Camera/BaseCamera.h"

class Player;

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
		kShakeTime_ = shakeTime_;
		shakeRadius_ = shakeRadius;
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
	float baseFov_ = 0.55f;

	Vector3 shakeTranslate_;
	static float shakeRadius_;
	static float  shakeTime_;
	static float  kShakeTime_;

	bool isTitleToGameCamera_;

};