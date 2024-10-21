#include "TickerTape.h"

TickerTape::TickerTape(const Vector2& emitPos){

	filename_ = "white.png";
	speedRange_ = { 150.0f,300.0f };
	sizeRange_ = { 16.0f,32.0f };
	directionRange_ = { 225.0f * toRadian,315.0f * toRadian };

	// 色をランダムに決定
	InitRandomColor({
		{1.0f,0.0f,0.0f,1.0f},
		{0.0f,1.0f,0.0f,1.0f},
		{0.0f,0.0f,1.0f,1.0f},
		{1.0f,1.0f,0.0f,1.0f},
		{0.0f,1.0f,0.0f,1.0f}
	});

	particle_ = Engine::CreateSprite(filename_);
	translate_ = emitPos;
}


TickerTape::~TickerTape(){}



void TickerTape::Update(){

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



void TickerTape::RandomInit(){

	addAngle_ = RandomFloat(-6.28f, 6.28f);
	addScaleTheta_ = RandomFloat(3.14f * 2.0f, 3.14f * 4.0f);
	BaseParticle2D::RandomInit();
}



bool TickerTape::GetIsActive() const{
	if(translate_.y < kWindowHeight_ + size_){ return true; }
	return false;
}
