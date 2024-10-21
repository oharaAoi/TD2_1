#pragma once
#include "Game/Effect/BaseParticle2D.h"
#include "Enviroment.h"

class TickerTape : public BaseParticle2D{

public:
	TickerTape(const Vector2& emitPos);
	~TickerTape()override;

	void Update()override;
	void RandomInit()override;

public:

	bool GetIsActive()const override;

private:

	float addAngle_;
	float addScaleTheta_;
	float dropSpeed_;
};