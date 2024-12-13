#pragma once
#include <memory>
#include <vector>
#include <initializer_list>
#include "Engine/2D/Sprite.h"
#include "Engine/Lib/GameTimer.h"
#include "Engine/Math/MyRandom.h"
#include "Engine/Engine.h"

class BaseParticle2D{

public:
	BaseParticle2D() = default;
	BaseParticle2D(const Vector2& emitPos);
	virtual ~BaseParticle2D();
	virtual void Update();
	virtual void Draw();
	virtual void RandomInit();

public:
	virtual bool GetIsActive()const{ return lifetime_ > 0.0f; }
	float GetLifeTime()const{ return lifetime_; }
	void InitRandomColor(const std::initializer_list<Vector4>& colorList);
	void SetTextureName(const std::string& filename){ 
		filename_ = filename;
		particle_->SetTexture(filename_);
	}

protected:

	std::unique_ptr<Sprite>particle_ = nullptr;
	std::string filename_ = "white.png";

	Vector2 scale_ = {1.0f,1.0f};
	float rotate_;
	Vector2 translate_;

	float kLifetime_;
	float lifetime_;

	Vector2 sizeRange_;
	float size_;

	Vector2 speedRange_;
	float speed_;

	Vector2 directionRange_;//{minAngle,maxAngle};
	Vector2 direction_;

	Vector4 color_;
};