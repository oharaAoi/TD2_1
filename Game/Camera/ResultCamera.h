#pragma once
#include "Game/Camera/BaseCamera.h"
#include "Engine/Input/Input.h"
#include "Engine/Math/MyRandom.h"
#include "Engine/Math/MyMath.h"

class ResultCamera : public BaseCamera{
public:

	ResultCamera();
	~ResultCamera() override;

	void Finalize() override;
	void Init() override;
	void Update() override;

#ifdef _DEBUG
	void Debug_Gui() override;
#endif

	void Shake(float radius);

public:


private:

	// ベースのトランスフォーム
	Vector3 baseRotate_ = { 0.13f, 0.22f, 0.0f };
	Vector3 baseTranslate_ = { -4.62f, 2.56f, -9.63f };

	// 追加するトランスフォーム
	Vector3 shakeTranslate_;
	Vector3 shakeRotate_;
	Vector3 addCameraRotate_ = {0.0f,3.14f,0.0f};

	// 時間管理系
	float startWaitTime_ = 0.0f;
	float startTime_ = 2.0f;
	float time_;

	float transitionTimeToRanking_ = 2.0f;
	float time2_;

};