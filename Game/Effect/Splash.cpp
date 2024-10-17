#include "Splash.h"

// static Initialize
float Splash::defaultVelocity_ = 3.0f;

////////////////////////////////////////////////////////////////////
// 更新
////////////////////////////////////////////////////////////////////

Splash::Splash(const Vector2& emitPos, float velocity){

	emitPos_ = emitPos_;
	maxHeight_ = velocity;
	radius_ = velocity;
	kLifetime_ = 2.0f * (velocity / defaultVelocity_);
	lifetime_ = kLifetime_;

	///////////////////////////////
	// モデルを追加ゾーン
	///////////////////////////////
	for(int i = 0; i < kSplashCount_; i++){
		auto& splash = splashModels_.emplace_back(std::make_unique<BaseGameObject>());
		splash->SetObject("Splash.obj");
		// 
		splash->GetTransform()->SetTranslaion(emitPos_);
		Quaternion quaternion;
		quaternion.x = Quaternion::AngleAxis(RandomFloat(0.0f, 3.14f), { 1.0f,0.0f,0.0f }).x;

		Vector3 rotate = {
			RandomFloat(0.0f,3.14f),
			RandomFloat(0.0f,3.14f * 2.0f),
			RandomFloat(0.0f,3.14f),
		};

		splash->GetTransform()->SetQuaternion(Quaternion::EulerToQuaternion(rotate));
		splash->GetTransform()->SetScale({ 0.5f,0.5f,0.5f });
		splash->Update();
	}

	for(int i = 0; i < kWaterColmnCount_; i++){
		auto& waterColmn = waterColmns_.emplace_back(std::make_unique<BaseGameObject>());
		waterColmn->SetObject("WaterColmn.obj");
		waterColmn->Update();

		Vector3 rotate = {
			0.0f,
			RandomFloat(0.0f,3.14f * 2.0f),
			0.0f,
		};

		waterColmn->GetTransform()->SetQuaternion(Quaternion::EulerToQuaternion(rotate));
		waterColmn->Update();
	}

	for(int i = 0; i < kPlaneCount_; i++){
		auto& ripple = ripples_.emplace_back(std::make_unique<BaseGameObject>());
		ripple->SetObject("Ripple.obj");
		ripple->Update();

		Vector3 rotate = {
			0.0f,
			RandomFloat(0.0f,3.14f * 2.0f),
			0.0f,
		};

		ripple->GetTransform()->SetQuaternion(Quaternion::EulerToQuaternion(rotate));
		ripple->Update();
	}
}

////////////////////////////////////////////////////////////////////
// 更新
////////////////////////////////////////////////////////////////////

void Splash::Update(){

	for(int i = 0; i < splashModels_.size(); i++){
		//splashModels_[i]->GetTransform()->Se
	}

	for(int i = 0; i < waterColmns_.size(); i++){
		float t = float(i) / float(waterColmns_.size() - 1);
	}

	for(int i = 0; i < ripples_.size(); i++){
		float t = float(i) / float(ripples_.size() - 1);
	}


	lifetime_ -= GameTimer::DeltaTime();
}

////////////////////////////////////////////////////////////////////
// 描画
////////////////////////////////////////////////////////////////////

void Splash::Draw(){

	// 水柱の描画
	for(auto& waterColmn : waterColmns_){
		waterColmn->Draw();
	}

	// しぶきの描画
	for(auto& splash : splashModels_){
		splash->Draw();
	}

	// しぶきの下で水面に映ってる波を平面で描画
	for(auto& ripple : ripples_){
		ripple->Draw();
	}
}
