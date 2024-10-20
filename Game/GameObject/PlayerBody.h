#pragma once
#include "Engine/GameObject/BaseGameObject.h"

class PlayerBody : public BaseGameObject{

public:
	PlayerBody();
	~PlayerBody();
	void Update()override;
	void Debug(int num);

public:

	void SetTarget(BaseGameObject* target){ pTargetObject_ = target; }
	void SetSpace(float spaceLength){ spaceLength_ = spaceLength; }

private:
	// 追従する対象のポインタ
	BaseGameObject* pTargetObject_ = nullptr;

	// 前のオブジェクトとの間にあける距離
	float spaceLength_;

	float angle_;
	Quaternion newRotate;
};