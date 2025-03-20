#include "FishEnergy.h"
#include "Engine/Engine.h"
#include "Engine/Math/MyRandom.h"

void FishEnergy::Init(const Vector2& pos) {
	energySprite_ = Engine::CreateSprite("fishEnergy.png");
	energySprite_->SetCenterPos(pos);

	Vector2 direction = Vector2( RandomFloat(-1.0f, 1.0f),RandomFloat(-1.0f, 1.0f) );
	float scaleSize = RandomFloat(0.5f, 1.0f);
	Vector2 scale = Vector2(scaleSize, scaleSize);
	energySprite_->SetScale(scale);
	velocity_ = direction * firstSpeed_;

	isAbsorption_ = false;

	isAlive_ = true;
}

void FishEnergy::Update() {
	// 最初の散乱
	if (!isAbsorption_) {
		Vector2 pos = energySprite_->GetCenterPos();
		
		velocity_ *= decelerationRaito_;
		pos += velocity_ * firstSpeed_ * GameTimer::DeltaTime();
		energySprite_->SetCenterPos(pos);

		// velocityの長さが1以下になったら左上に向かうようにする
		if (velocity_.Length() < 2.0f) {
			isAbsorption_ = true;
			absorptioVelocity_ = pos.Normalize() * absorptionSpeed_;
			absorptioAcceleration_ = absorptioVelocity_;
		}

	} else {
		// ゲージの方へ向かっていく
		Vector2 pos = energySprite_->GetCenterPos();

		absorptioAcceleration_ = (pos - targetPos_).Normalize() * absorptionSpeed_;

		absorptioVelocity_ += absorptioAcceleration_ * absorptionSpeed_ * GameTimer::DeltaTime();
		pos -= absorptioVelocity_ * GameTimer::DeltaTime();
		energySprite_->SetCenterPos(pos);

		if ((targetPos_ - pos).Length() < 64.0f) {
			isAlive_ = false;
		}
	}


	energySprite_->Update();

}

void FishEnergy::Draw() const {
	energySprite_->Draw();
}
