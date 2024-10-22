#include "Bubble.h"

Bubble::Bubble(const Vector2& emitPos){

	int rand = RandomInt(0, 3);
	if(rand == 0){
		filename_ = "softLight.png";
	} else{
		filename_ = "bubble.png";
	}

	speedRange_ = { 1000.0f,2000.0f };
	sizeRange_ = { 16.0f,500.0f };
	directionRange_ = { 80.0f * toRadian,100.0f * toRadian };

	// 色をランダムに決定
	color_ = { 1.0f,1.0f,1.0f,1.0f };

	particle_ = Engine::CreateSprite(filename_);
	translate_ = emitPos;
	rotateCenter_ = translate_;
}


Bubble::~Bubble(){}



void Bubble::Update(){

	rotateCenter_ += direction_ * -speed_ * GameTimer::DeltaTime() * sizeRate_;
	translate_.x = rotateCenter_.x + moveRadius_ * std::sinf(3.14f * GameTimer::TotalTime() * 4.0f * addMoveRotateRate_);
	translate_.y = rotateCenter_.y;
	rotate_ += addAngle_ * GameTimer::DeltaTime();

	particle_->SetScale(scale_);
	particle_->SetCenterPos(translate_);
	particle_->SetRotate(rotate_);
	particle_->SetColor(color_);
	particle_->Update();

	lifetime_ -= GameTimer::DeltaTime();
}



void Bubble::RandomInit(){

	addAngle_ = RandomFloat(-6.28f, 6.28f);
	addMoveRotateRate_ = RandomFloat(1.0f, 2.0f);
	BaseParticle2D::RandomInit();

	sizeRate_ = size_ / sizeRange_.y;
	moveRadius_ = size_ * 0.2f;
}



bool Bubble::GetIsActive() const{
	if(translate_.y > -size_){ return true; }
	return false;
}
