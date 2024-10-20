#include "Cherry.h"

Cherry::Cherry(const Vector2& emitPos){

	filename_ = "cherry.png";
	speedRange_ = { 100.0f,240.0f };
	sizeRange_ = { 10.0f,25.0f };
	directionRange_ = { 225.0f * toRadian,315.0f * toRadian };
	color_ = { 1.0f,1.0f,1.0f,1.0f };

	particle_ = Engine::CreateSprite(filename_);
	translate_ = emitPos;
}


Cherry::~Cherry(){}



void Cherry::Update(){

	translate_ += direction_ * -speed_ * GameTimer::DeltaTime();
	rotate_ += addAngle_ * GameTimer::DeltaTime();
	scale_.x = std::fabsf(std::sinf(GameTimer::TotalTime() * addScaleTheta_));

	particle_->SetScale(scale_);
	particle_->SetCenterPos(translate_);
	particle_->SetRotate(rotate_);
	particle_->SetColor(color_);
	particle_->Update();

	lifetime_ -= GameTimer::DeltaTime();
}



void Cherry::RandomInit(){

	addAngle_ = RandomFloat(-6.28f, 6.28f);
	addScaleTheta_ = RandomFloat(0.125f, 6.28f);
	BaseParticle2D::RandomInit();
}



bool Cherry::GetIsActive() const{

	if(translate_.x > 0.0f - size_ && translate_.x < kWindowWidth_ + size_){
		if(translate_.y > 0.0f - size_ && translate_.y < kWindowHeight_ + size_){ return true; }
	}

	return false;
}
