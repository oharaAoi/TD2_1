#pragma once
#include "Engine/GameObject/BaseGameObject.h"

class PlayerBody : public BaseGameObject{

public:
	PlayerBody();
	~PlayerBody();
	void Update()override;
	void Debug(int num);

public:

	void UpdateMatrix();
	void SetTarget(BaseGameObject* target){ pTargetObject_ = target; }
	void SetSpace(float spaceLength){ spaceLength_ = spaceLength; }

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
};