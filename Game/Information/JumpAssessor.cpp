#include "JumpAssessor.h"
#include "Engine/Math/Easing.h"
#include "Engine/Math/MyMath.h"

//////////////////////////////////////////////////////////////////////////////////////
// static変数の初期化
//////////////////////////////////////////////////////////////////////////////////////
JumpAssessor* JumpAssessor::instance_ = nullptr;

//////////////////////////////////////////////////////////////////////////////////////
// コンストラクタ・デストラクタ
//////////////////////////////////////////////////////////////////////////////////////
JumpAssessor::JumpAssessor(){}

JumpAssessor::~JumpAssessor(){}

//////////////////////////////////////////////////////////////////////////////////////
// インスタンスの取得
//////////////////////////////////////////////////////////////////////////////////////
JumpAssessor* JumpAssessor::GetInstance(){
	if(!instance_){
		instance_ = new JumpAssessor();
		instance_->Initialize();
	}
	return instance_;
}


//////////////////////////////////////////////////////////////////////////////////////
// 初期化
//////////////////////////////////////////////////////////////////////////////////////
void JumpAssessor::Initialize(){
	instance_->isUIvisible_ = false;
	for(int i = 0; i < 4; ++i){

		instance_->percentageSprite_[i] = Engine::CreateSprite("number2.png");
		instance_->percentageSprite_[i]->SetRectRange(instance_->numberSpriteSize_);
		instance_->percentageSprite_[i]->SetTextureSize(instance_->numberSpriteSize_);
		instance_->percentageSprite_[i]->SetRectRange(instance_->numberSpriteSize_);
		instance_->percentageSprite_[i]->SetTextureSize(instance_->numberSpriteSize_);
		instance_->percentageSprite_[i]->SetAnchorPoint({ 0.5f, 0.5f });
		instance_->percentageSprite_[i]->Update();
	}

	for(int i = 0; i < 2; i++){
		instance_->evaluationSprite_[i] = Engine::CreateSprite("evaluation3.png");
		instance_->evaluationSprite_[i]->SetScale(Vector2(0.5f, 0.5f));
		instance_->evaluationSprite_[i]->Update();
	}
}


//////////////////////////////////////////////////////////////////////////////////////
// 更新
//////////////////////////////////////////////////////////////////////////////////////
void JumpAssessor::Update(){
	static  float kApeearTime = 0.2f;
	static float appearTimer = 0.0f;
	static bool isAlreadyAppeared = false;
	static  float kDisplayTime = 1.0f;
	static float displayTimer = 0.0f;
	static Vector3 offset = { 0.5f,0.5f,0.5f };
	static Vector2 percentageFieldSize = { 209.0f, 120.0f };
	static float spriteTheta = 0.0f;
	static float evalutionMaxScale = 0.3f;
	static float digitMaxScale = 1.0f;
	static Vector2 screenOffset = { 50.0f,0.0f };
	static float evalutionSeparetorSize = 40.0f;
	static float h = 0.0f;

#ifdef _DEBUG
	ImGui::Begin("jumpAssesor");
	ImGui::DragFloat("appearTimer", &kApeearTime, 0.01f);
	ImGui::DragFloat("displayTimer", &kDisplayTime, 0.01f);
	ImGui::DragFloat("digitMaxScale", &digitMaxScale, 0.01f);
	ImGui::DragFloat("evalutionMaxScale", &evalutionMaxScale, 0.01f);
	ImGui::DragFloat3("offset", &offset.x, 0.01f);
	ImGui::DragFloat2("percentageFieldSize", &percentageFieldSize.x, 0.01f);
	ImGui::End();
#endif // _DEBUG

	if(!isUIvisible_){ return; }

	// 表示時間を更新
	if(!isAlreadyAppeared){
		appearTimer = std::clamp(appearTimer += GameTimer::DeltaTime(), 0.0f, kApeearTime);
		if(appearTimer >= kApeearTime){
			isAlreadyAppeared = true;
		}
	} else{
		if(displayTimer < kDisplayTime){
			displayTimer += GameTimer::DeltaTime();
		} else{
			appearTimer = std::clamp(appearTimer -= GameTimer::DeltaTime(), 0.0f, kApeearTime);
		}

		if(appearTimer <= 0.0f){
			isUIvisible_ = false;
			isAlreadyAppeared = false;
			displayTimer = 0.0f;
		}
	}

	// 媒介変数
	float t = appearTimer / kApeearTime;
	float ease = EaseInBack(t);

	// 表示2D座標の計算
	Matrix4x4 parentMat = parentWT_->GetWorldMatrix();
	parentMat *= MakeTranslateMatrix(offset);// オフセットを加える
	Vector3 screenPos = Transform({ 0.0f, 0.0f, 0.0f }, parentMat * pCamera_->GetVpvpMatrix());
	spriteTheta = -6.28f * t;

	// 評価の最大表示に虹色にする
	if(isEvalutionMax_){
		h += GameTimer::DeltaTime();
	}

	// スプライトのトランスフォームを更新
	for(int i = 0; i < 4; ++i){
		Vector3 numberOffset = { i * (percentageFieldSize.x / 4) - percentageFieldSize.x * 0.5f,0.0f,0.0f };
		numberOffset *= ease;
		numberOffset = numberOffset * MakeRotateZMatrix(spriteTheta);
		instance_->percentageSprite_[i]->SetCenterPos(Vector2(screenPos.x + numberOffset.x, screenPos.y + numberOffset.y) + screenOffset);
		instance_->percentageSprite_[i]->SetRotate(spriteTheta);
		instance_->percentageSprite_[i]->SetScale(Vector2(t, t) * digitMaxScale);
		if(isEvalutionMax_){
			instance_->percentageSprite_[i]->SetColor(FloatColor(HSV_to_RGB(h, 1.0f, 1.0f, 1.0f)));
		}
		instance_->percentageSprite_[i]->Update();
	}

	for(int i = 0; i < 2; i++){
		Vector2 evalutionOffset = Vector2(percentageFieldSize.x * 0.5f, -percentageFieldSize.y * 0.5f + (i * -evalutionSeparetorSize));
		evalutionOffset *= ease;
		instance_->evaluationSprite_[i]->SetCenterPos(Vector2(screenPos.x, screenPos.y) + evalutionOffset + screenOffset);
		instance_->evaluationSprite_[i]->SetScale(Vector2(t, t) * evalutionMaxScale);
		instance_->evaluationSprite_[i]->Update();
	}
}


//////////////////////////////////////////////////////////////////////////////////////
// 描画
//////////////////////////////////////////////////////////////////////////////////////
void JumpAssessor::Draw(){

	if(!isUIvisible_){ return; }

	for(int i = 0; i < 4; ++i){
		if(!isDigitVisible_[i]){ continue; }
		instance_->percentageSprite_[i]->Draw();
	}

	for(int i = 0; i < 2; i++){
		if(!isEvaluationVisible_[i]){ continue; }
		instance_->evaluationSprite_[i]->Draw();
	}
}


//////////////////////////////////////////////////////////////////////////////////////
// ジャンプの評価を行う
//////////////////////////////////////////////////////////////////////////////////////
void JumpAssessor::SetJumpData(float speed, int32_t bodyCount, WorldTransform* parentWT, Camera* pCamera){

	isUIvisible_ = true;
	parentWT_ = parentWT;
	pCamera_ = pCamera;

	if(speed >= 150.0f && bodyCount == 8){// prefect
		//AnimetionEffectManager::AddListEffect(
		//	"./Game/Resources/Model/evaluationPERFECT/", "evaluationPERFECT.gltf",
		//	parentWT,
		//	false,
		//	Vector3(0.5f, 0.5f, 0.5f), Quaternion(), Vector3()
		//);
		AudioPlayer::SinglShotPlay("ParfectSE.mp3", 0.3f);


	} else if(speed < 150.f or bodyCount >= 0){// 速度不足
		//AnimetionEffectManager::AddListEffect(
		//	"./Game/Resources/Model/evaluationNice/", "evaluationNice.gltf",
		//	parentWT,
		//	false,
		//	Vector3(1, 1, 1), Quaternion(), Vector3()
		//);
		AudioPlayer::SinglShotPlay("NiceSE.mp3", 0.15f);


	} else if(speed >= 70.0f or bodyCount >= 4){// nice
		//AnimetionEffectManager::AddListEffect(
		//	"./Game/Resources/Model/evaluationGood/", "evaluationGood.gltf",
		//	parentWT,
		//	false,
		//	Vector3(0.5f, 0.5f, 0.5f), Quaternion(), Vector3()
		//);
		AudioPlayer::SinglShotPlay("GoodSE.mp3", 0.2f);


	} else{

	}


	// パーセンテージの計算
	float playerDefaultSpeed = 38.0f;
	float speedPercentage = std::clamp((speed- playerDefaultSpeed) / (150.0f - playerDefaultSpeed), 0.0f, 1.0f);
	float bodyCountPercentage = std::clamp(((float)bodyCount -2.0f) / 6.0f, 0.0f, 1.0f);
	int percentage = int((speedPercentage + bodyCountPercentage) * 50.0f);

	for(int i = 0; i < 4; i++){
		isDigitVisible_[i] = true;
	}

	// 表示する桁の切り替え
	if(percentage < 100){
		isDigitVisible_[0] = false;
	}
	
	if(percentage < 10){
		isDigitVisible_[1] = false;
	}

	// 評価の切り替え
	int index = 0;
	isEvaluationVisible_[1] = false;

	if(percentage < 100){
		if(speedPercentage < 1.0f){
			evaluationSprite_[index]->SetTexture("evaluation1.png");// 速度不足
			index++;
		}

		if(bodyCountPercentage < 1.0f){
			evaluationSprite_[index]->SetTexture("evaluation2.png");// 胴体数不足
			index++;
		}

		isEvalutionMax_ = false;

	} else{
		evaluationSprite_[index]->SetTexture("evaluation3.png");// パーフェクト
		isEvalutionMax_ = true;
		index++;
	}

	if(index >= 2){
		isEvaluationVisible_[1] = true;
	}

	// 数字の切り抜き
	int digits[4] = { percentage / 100, (percentage / 10) % 10, percentage % 10,10 };
	for(int i = 0; i < 4; ++i){
		instance_->percentageSprite_[i]->SetLeftTop({ widthEvery_ * digits[i] ,0.0f });
		instance_->percentageSprite_[3]->SetLeftTop({ widthEvery_ * 10,0.0f });

		// ％に応じて色を変える(HSVのHを動かして青->赤へ)
		float h = 0.5f - 0.5f * (percentage / 100.0f);
		instance_->percentageSprite_[i]->SetColor(FloatColor(HSV_to_RGB(h, 1.0f, 1.0f,1.0f)));
	}

}