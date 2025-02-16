#include "BodyUpEffect.h"
#include "Engine/Math/Easing.h"

BodyUpEffect::BodyUpEffect() {
}

BodyUpEffect::~BodyUpEffect() {
	upEffects_.clear();
}

///////////////////////////////////////////////////////////////////////////////////////////////
// ↓ 初期化処理
///////////////////////////////////////////////////////////////////////////////////////////////
void BodyUpEffect::Init() {

}

///////////////////////////////////////////////////////////////////////////////////////////////
// ↓ 更新処理
///////////////////////////////////////////////////////////////////////////////////////////////
void BodyUpEffect::Update(const Vector2& screenPos) {
	playerScreenPos_ = screenPos;
	playerScreenPos_.x -= 90;
	upEffects_.remove_if([&](const EffectData& effect) {
		return effect.disapperCount > disapperTime_;
					  });

	for (auto& effect : upEffects_) {
		effect.lifeTimeCount += GameTimer::DeltaTime();

		// 座標の移動
		Vector2 pos = effect.sprite->GetCenterPos();
		pos.y -= (velocityY_ * (1.0f - (decay_ * effect.lifeTimeCount))) * GameTimer::DeltaTime();
		effect.sprite->SetCenterPos(pos);

		if (effect.lifeTimeCount > lifeTime_ * .5f) {
			// 色の更新
			effect.disapperCount += GameTimer::DeltaTime();
			float t = (1.0f - effect.disapperCount) / disapperTime_;
			effect.sprite->SetColor(Vector4(1, 1, 1, t));

		}

		// 更新
		effect.sprite->Update();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
// ↓ 描画処理
///////////////////////////////////////////////////////////////////////////////////////////////
void BodyUpEffect::Draw() const {
	for (const auto& effect : upEffects_) {
		effect.sprite->Draw();
	}
}

void BodyUpEffect::AddEffect() {
	auto& newEffect = upEffects_.emplace_back();
	newEffect.sprite = Engine::CreateSprite("1up.png");
	newEffect.sprite->SetCenterPos(playerScreenPos_);
	newEffect.lifeTimeCount = 0.0f;
	newEffect.disapperCount = 0.0f;
}
