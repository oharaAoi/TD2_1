#include "RankUp.h"

RankUp::RankUp(const Vector2& emitPos){

	int rand = RandomInt(0, 3);
	filename_ = "A.png";

	kLifetime_ = 1.0f;
	lifetime_ = kLifetime_;

	speedRange_ = { 1000.0f,2000.0f };
	sizeRange_ = { 16.0f,50.0f };
	directionRange_ = { 80.0f * toRadian,100.0f * toRadian };

	// 色をランダムに決定
	color_ = { 1.0f,1.0f,1.0f,1.0f };

	particle_ = Engine::CreateSprite(filename_);
	translate_ = emitPos;
	rotateCenter_ = translate_;
}


RankUp::~RankUp(){}



void RankUp::Update(){

	translate_.y -= speed_ * GameTimer::DeltaTime() * sizeRate_;
	rotate_ += addAngle_ * GameTimer::DeltaTime();
	color_.w = lifetime_ / kLifetime_;

	particle_->SetScale(scale_);
	particle_->SetCenterPos(translate_);
	particle_->SetColor(color_);
	particle_->Update();

	lifetime_ -= GameTimer::DeltaTime();
}



void RankUp::RandomInit(){

	addAngle_ = RandomFloat(-6.28f, 6.28f);
	addMoveRotateRate_ = RandomFloat(1.0f, 2.0f);
	BaseParticle2D::RandomInit();

	rotate_ = 0.0f;
	sizeRate_ = size_ / sizeRange_.y;
	moveRadius_ = size_ * 0.2f;
}