#pragma once
#include "Game/Effect/BaseParticle2D.h"
#include "Enviroment.h"

class Bubble : public BaseParticle2D{

public:
	Bubble(const Vector2& emitPos);
	~Bubble()override;

	void Update()override;
	void RandomInit()override;

public:

	bool GetIsActive()const override;

private:

	float addAngle_;
	float addMoveRotateRate_;
	Vector2 rotateCenter_;
	float sizeRate_;
	float moveRadius_;
};