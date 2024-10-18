#include "Splash.h"
#include "Easing.h"

// static Initialize
float Splash::defaultVelocity_ = 3.0f;

////////////////////////////////////////////////////////////////////
// 更新
////////////////////////////////////////////////////////////////////

Splash::Splash(const Vector3& emitPos, float velocity){

	emitPos_ = emitPos;
	maxHeight_ = velocity * 3.0f;
	radius_ = velocity * 6.0f;
	kLifetime_ = 0.8f;//(velocity / defaultVelocity_);
	lifetime_ = kLifetime_;

	///////////////////////////////
	// モデルを追加ゾーン
	///////////////////////////////

	//////////////////////////////////////////////////////
	//                      水しぶき
	//////////////////////////////////////////////////////
	for(int i = 0; i < kSplashCount_; i++){
		auto& splash = splashModels_.emplace_back(std::make_unique<BaseGameObject>());
		splash->Init();
		splash->SetObject("Splash.obj");
		splash->SetIsLighting(false);

		// 
		splash->GetTransform()->SetTranslaion(emitPos_);

		Vector3 rotate = {
			0.0f,//RandomFloat(0.0f,3.14f),
			RandomFloat(0.0f,3.14f * 2.0f),
			RandomFloat(0.0f,3.14f),
		};

		splash->GetTransform()->SetQuaternion(Quaternion::EulerToQuaternion(rotate));
		splash->GetTransform()->SetScale({ 0.5f,0.5f,0.5f });
		splash->Update();
	}

	//////////////////////////////////////////////////////
	//                      水柱
	//////////////////////////////////////////////////////
	for(int i = 0; i < kWaterColmnCount_; i++){
		auto& waterColmn = waterColmns_.emplace_back(std::make_unique<BaseGameObject>());
		waterColmn->Init();
		waterColmn->SetObject("WaterColmn.obj");
		waterColmn->SetIsLighting(false);

		Vector3 rotate = {
			0.0f,
			RandomFloat(0.0f,3.14f * 2.0f),
			0.0f,
		};

		waterColmn->GetTransform()->SetTranslaion(emitPos_);
		waterColmn->GetTransform()->SetQuaternion(Quaternion::EulerToQuaternion(rotate));
		waterColmn->Update();
	}



	//////////////////////////////////////////////////////
	//                      波紋
	//////////////////////////////////////////////////////
	for(int i = 0; i < kPlaneCount_; i++){
		auto& ripple = ripples_.emplace_back(std::make_unique<BaseGameObject>());
		ripple->Init();
		ripple->SetObject("Ripple.obj");
		ripple->SetIsLighting(false);

		Vector3 rotate = {
			0.0f,
			RandomFloat(0.0f,3.14f * 2.0f),
			0.0f,
		};

		ripple->GetTransform()->SetTranslaion(emitPos_);
		ripple->GetTransform()->SetQuaternion(Quaternion::EulerToQuaternion(rotate));
		ripple->Update();
	}
}

////////////////////////////////////////////////////////////////////
// 更新
////////////////////////////////////////////////////////////////////

void Splash::Update(){

	//////////////////////////////////////////////////////
	//                      水しぶき
	//////////////////////////////////////////////////////
	for(int i = 0; i < splashModels_.size(); i++){

		// 色の更新
		float t = lifetime_ / kLifetime_;
		splashModels_[i]->SetColor({ 1.0f,1.0f,1.0f,t });

		// 大きさの更新
		splashModels_[i]->GetTransform()->SetScale({ t,t,t });

		// 座標の更新
		Vector3 vec = Vector3(0.0f, 0.0f, 1.0f) * splashModels_[i]->GetTransform()->GetQuaternion().MakeMatrix();
		splashModels_[i]->GetTransform()->SetTranslaion(
			splashModels_[i]->GetTransform()->GetTranslation()
			+ vec * radius_ * 2.0f * GameTimer::DeltaTime()
		);

		// 行列更新
		splashModels_[i]->Update();
	}


	//////////////////////////////////////////////////////
	//                      水柱
	//////////////////////////////////////////////////////
	for(int i = 0; i < waterColmns_.size(); i++){
		float index_t = float(i) / float(waterColmns_.size() - 1);
		float time_t = lifetime_ / kLifetime_;

		// ザバアーンってする
		float magn = radius_ * 0.5f * index_t + radius_ * (1.0f - time_t);
		waterColmns_[i]->GetTransform()->SetScale({
			magn,
			maxHeight_ * (1.0f - index_t) * std::sinf(3.14f * time_t),
			magn
		});

		// 行列更新
		waterColmns_[i]->Update();
	}


	//////////////////////////////////////////////////////
	//                      波紋
	//////////////////////////////////////////////////////
	for(int i = 0; i < ripples_.size(); i++){
		float index_t = float(i) / float(ripples_.size() - 1);
		float time_t = lifetime_ / kLifetime_;

		// だんだん広げていく
		ripples_[i]->GetTransform()->SetScale({
			radius_ * 4.0f *index_t * (1.0f - time_t),
			1.0f,
			radius_ * 4.0f *index_t * (1.0f - time_t),
		});

		// 透明度を下げていく
		ripples_[i]->SetColor({ 1.0f,1.0f,1.0f,time_t });

		// 行列更新
		ripples_[i]->Update();
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
