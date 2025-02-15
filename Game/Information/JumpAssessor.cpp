#include "JumpAssessor.h"

//////////////////////////////////////////////////////////////////////////////////////
// static変数の初期化
//////////////////////////////////////////////////////////////////////////////////////
JumpAssessor* JumpAssessor::instance_ = nullptr;

//////////////////////////////////////////////////////////////////////////////////////
// コンストラクタ・デストラクタ
//////////////////////////////////////////////////////////////////////////////////////
JumpAssessor::JumpAssessor(){
	isUIvisible_ = false;
}

JumpAssessor::~JumpAssessor(){}

//////////////////////////////////////////////////////////////////////////////////////
// インスタンスの取得
//////////////////////////////////////////////////////////////////////////////////////
JumpAssessor* JumpAssessor::GetInstance(){
	if(!instance_){
		instance_ = new JumpAssessor();
	}
	return instance_;
}


//////////////////////////////////////////////////////////////////////////////////////
// 初期化
//////////////////////////////////////////////////////////////////////////////////////
void JumpAssessor::Initialize(){
	instance_->isUIvisible_ = false;
	for(int i = 0; i < 3; ++i){
		instance_->percentageSprite_[i] = Engine::CreateSprite("number.png");
		instance_->percentageSprite_[i]->SetRectRange(instance_->numberSpriteSize_);
		instance_->percentageSprite_[i]->SetTextureSize(instance_->numberSpriteSize_);
		instance_->percentageSprite_[i]->SetScale(Vector2(0.5f, 0.5f));
		instance_->percentageSprite_[i]->SetTextureCenterPos({ 0.0f, 0.0f });
		instance_->percentageSprite_[i]->SetLeftTop({ 0.0f, 0.0f });
		instance_->percentageSprite_[i]->SetRectRange(instance_->numberSpriteSize_);
		instance_->percentageSprite_[i]->Update();
	}

	instance_->evaluationSprite_ = Engine::CreateSprite("evaluationPERFECT.png");
}


//////////////////////////////////////////////////////////////////////////////////////
// 更新
//////////////////////////////////////////////////////////////////////////////////////
void JumpAssessor::Update(){
	static const float kVisibleTime = 1.0f;
	static float visibleTimer = 0.0f;
	static Vector3 offset = { 0.5f,0.5f,0.5f };
	if(!isUIvisible_){ return; }

	// 表示時間を更新
	visibleTimer = std::clamp(visibleTimer += GameTimer::DeltaTime(), 0.0f, kVisibleTime);
	// 媒介変数
	float t = visibleTimer / kVisibleTime;

	// 表示2D座標の計算
	Matrix4x4 parentMat = parentWT_->GetWorldMatrix();
	parentMat *= MakeTranslateMatrix(offset);// オフセットを加える
	Vector3 screenPos = Transform({ 0.0f, 0.0f, 0.0f }, parentMat * pCamera_->GetVpvpMatrix());



}


//////////////////////////////////////////////////////////////////////////////////////
// 描画
//////////////////////////////////////////////////////////////////////////////////////
void JumpAssessor::Draw(){


	if(!isUIvisible_){ return; }
}


//////////////////////////////////////////////////////////////////////////////////////
// ジャンプの評価を行う
//////////////////////////////////////////////////////////////////////////////////////
void JumpAssessor::SetJumpData(float speed, int32_t bodyCount, WorldTransform* parentWT, Camera* pCamera){

	isUIvisible_ = true;
	parentWT_ = parentWT;
	pCamera_ = pCamera;

	if(speed >= 150.0f && bodyCount == 8){// prefect
		AnimetionEffectManager::AddListEffect(
			"./Game/Resources/Model/evaluationPERFECT/", "evaluationPERFECT.gltf",
			parentWT,
			false,
			Vector3(0.5f, 0.5f, 0.5f), Quaternion(), Vector3()
		);
		AudioPlayer::SinglShotPlay("ParfectSE.mp3", 0.3f);


	} else if(speed < 150.f or bodyCount >= 0){// 速度不足
		AnimetionEffectManager::AddListEffect(
			"./Game/Resources/Model/evaluationNice/", "evaluationNice.gltf",
			parentWT,
			false,
			Vector3(1, 1, 1), Quaternion(), Vector3()
		);
		AudioPlayer::SinglShotPlay("NiceSE.mp3", 0.15f);

	} else if(speed >= 0 or bodyCount < 8){// 胴体不足
		AnimetionEffectManager::AddListEffect(
			"./Game/Resources/Model/evaluationGood/", "evaluationGood.gltf",
			parentWT,
			false,
			Vector3(0.5f, 0.5f, 0.5f), Quaternion(), Vector3()
		);
		AudioPlayer::SinglShotPlay("GoodSE.mp3", 0.2f);


	} else{

	}


	// パーセンテージの計算
	float speedPercentage = std::clamp(speed / 150.0f, 0.0f, 1.0f);
	float bodyCountPercentage = std::clamp((float)bodyCount / 8.0f, 0.0f, 1.0f);
	int percentage = int((speedPercentage + bodyCountPercentage) * 50.0f);

	// 数字の切り抜き
	int digits[3] = { percentage / 100, (percentage / 10) % 10, percentage % 10 };
	for(int i = 0; i < 3; ++i){
		instance_->percentageSprite_[i]->SetLeftTop({ widthEvery_ * digits[i] ,0.0f });
	}
}