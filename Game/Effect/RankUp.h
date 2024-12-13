#pragma once
#include "Game/Effect/BaseParticle2D.h"
#include "Enviroment.h"

class RankUp : public BaseParticle2D{

public:
	RankUp(const Vector2& emitPos);
	~RankUp()override;

	void Update()override;
	void RandomInit()override;


private:

	float addAngle_;
	float addMoveRotateRate_;
	Vector2 rotateCenter_;
	float sizeRate_;
	float moveRadius_;
};