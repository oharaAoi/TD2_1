#include "BaseParticle2D.h"

BaseParticle2D::BaseParticle2D(const Vector2& emitPos){
	particle_ = Engine::CreateSprite(filename_);
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	translate_ = emitPos;
}

BaseParticle2D::~BaseParticle2D(){}

void BaseParticle2D::Update(){

	particle_->SetScale(scale_);
	particle_->SetCenterPos(translate_);
	particle_->SetRotate(rotate_);
	particle_->SetColor(color_);
	particle_->Update();

	lifetime_ -= GameTimer::DeltaTime();
}

void BaseParticle2D::Draw(){
	particle_->Draw();
}

void BaseParticle2D::RandomInit(){
	size_ = RandomFloat(sizeRange_.x, sizeRange_.y);
	speed_ = RandomFloat(speedRange_.x, speedRange_.y);
	Vector3 tmpDirection = Vector3(1.0f, 0.0f,0.0f) * MakeRotateZMatrix(RandomFloat(directionRange_.x, directionRange_.y));
	direction_ = { tmpDirection.x,tmpDirection.y };

	particle_->SetLeftTop({0.0f,0.0f});
	particle_->SetTextureSize({ size_,size_ });
	particle_->Update();
}
