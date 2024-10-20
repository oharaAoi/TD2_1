#pragma once
#include <list>
#include <memory>
#include <string>
#include "Engine/2D/Sprite.h"
#include "Engine/Math/MyRandom.h"
#include "Engine/Lib/GameTimer.h"
#include "Engine/Engine.h"
#include "Game/Effect/BaseParticle2D.h"
#include <type_traits>

template <typename T>
class ParticleManager{

	// 指定した型がBaseParticle2Dを継承していなければアサート
	static_assert(std::is_base_of<BaseParticle2D, T>::value, "T must be derived from BaseParticle2D");

public:
	ParticleManager() = default;
	~ParticleManager();

	void Update();
	void Draw();

public:

	int32_t GetParticleCount(){ return (int32_t)particles_.size(); }
	void SetInterval(float interval){ interval_ = interval; }
	void SetEmitRange(const Vector2& LT, const Vector2& RB){
		emitRangeLT_ = LT;
		emitRangeRB_ = RB;
	}
	void SetEmitCountEvery(int32_t emitCountEvery){ emitCountEvery_ = emitCountEvery; }

private:

	// 出現させる範囲のrect
	Vector2 emitRangeLT_{};
	Vector2 emitRangeRB_{};

	// 出現までの間隔
	float interval_ = 0.5f;
	float toNext_ = interval_;

	// 一度にいくつ出現するか
	int32_t emitCountEvery_ = 1;

	// パーティクル一覧
	std::list<std::unique_ptr<T>>particles_;

};


/////////////////////////////////////////////////////////////////////////////////////

///										定義										  ///

/////////////////////////////////////////////////////////////////////////////////////


template <typename T>
ParticleManager<T>::~ParticleManager(){}


template <typename T>
void ParticleManager<T>::Update(){

	// 時間になったらパーティクル追加
	if(toNext_ <= 0.0f){

		for(int32_t i = 0; i < emitCountEvery_; i++){

			Vector2 emitPos = {
				RandomFloat(emitRangeLT_.x,emitRangeRB_.x),
				RandomFloat(emitRangeLT_.y,emitRangeRB_.y)
			};

			auto& particle = particles_.emplace_back(std::make_unique<T>(emitPos));
			particle->RandomInit();
			toNext_ = interval_;
		}
	}

	for(auto& particle : particles_){
		particle->Update();
	}

	// アクティブでないパーティクルの削除
	particles_.remove_if([](auto& particle){return !particle->GetIsActive(); });

	// 次のパーティクル追加までの時間
	toNext_ -= GameTimer::DeltaTime();

}

template <typename T>
void ParticleManager<T>::Draw(){
	for(auto& particle : particles_){
		particle->Draw();
	}
}