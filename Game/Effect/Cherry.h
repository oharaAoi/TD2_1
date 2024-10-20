#pragma once
#include "Game/Effect/BaseParticle2D.h"
#include "Enviroment.h"

class Cherry : public BaseParticle2D{

public:
	Cherry(const Vector2& emitPos);
	~Cherry()override;

	void Update()override;
	void RandomInit()override;

public:

	bool GetIsActive()const override;

private:

	float addAngle_;
	float addScaleTheta_;
	float dropSpeed_;
};