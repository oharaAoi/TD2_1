#pragma once
#include "Engine/GameObject/BaseGameObject.h"

class PlayerBody : public BaseGameObject{

public:
	PlayerBody();
	~PlayerBody();
	void Update()override;
	void Debug(int num);

public:

	void ScaleUpBody();

	void UpdateMatrix();
	void SetTarget(BaseGameObject* target){ pTargetObject_ = target; }
	void SetSpace(float spaceLength){ spaceLength_ = spaceLength; }

	void SetScaleUp();

	const Vector3 GetTranslation() const { return transform_->GetTranslation(); }
	const Quaternion GetQuaternion() const { return transform_->GetQuaternion(); }

private:
	// 追従する対象のポインタ
	BaseGameObject* pTargetObject_ = nullptr;

	// 前のオブジェクトとの間にあける距離
	float spaceLength_;

	float angle_;
	Vector3 rotate_;
	Quaternion newRotate;

	bool isNewCreate_ = true;
	bool isScaleDown_ = false;

	float scaleUpTimeCount_ = 0.0f;
	float scaleUpTime_ = 1.0f;

	float scaleDownTimeCount_ = 0.0f;
	float scaleDownTime_ = 1.0f;

	Vector3 upScale_ = {14.0f, 0.2f,2.0f};
};