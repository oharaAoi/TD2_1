#include "Camera.h"
#include "Game/Scene/GameScene.h"

Camera::Camera(){
	Init();
}

Camera::~Camera(){}

void Camera::Finalize(){}

void Camera::Init(){
	BaseCamera::Init();

	fov_ = 0.55f;

	transform_ = {
		{1.0f, 1.0f, 1.0f},
		{0.0f, 0.91f, 0.0f},
		{0.0f, 0.0f, 0.0f}
	};

	offsetVec_ = Vector3(-20.4f, 0.3f, -25.7f).Normalize();

	isTitleToGameCamera_ = false;
}

void Camera::Update(){

	//if (isTitleToGameCamera_) {
	//	offsetLength_ = std::clamp(offsetLength_, 80.0f, 1000.0f);
	//	Vector3 dif = (pPlayer_->GetTransform()->GetTranslation() + offsetVec_ * offsetLength_ + Vector3(adjustX_, 0.0f, 0.0f)) - transform_.translate;

	//	// 少し遅らせて追従
	//	transform_.translate += dif * 0.04f * GameTimer::TimeRate();
	//	BaseCamera::Update();
	//	return;
	//}

	//プレイヤーが水面に出ているかどうかでターゲットを変更
	if(pPlayer_->GetTransform()->GetTranslation().y < 10.0f){
		target_ = pPlayer_->GetAboveSurfaceTransform();
		// ターゲットからの距離を水深に応じて更新
		offsetLength_ = 80.0f + pPlayer_->GetSwimmingDepth() * 3.0f;

	} else{
		target_ = pPlayer_->GetTransform();
		offsetLength_ = 80.0f;

		if(pPlayer_->GetTransform()->GetTranslation().y > 20.0f){
			offsetLength_ = 80.0f +std::clamp(pPlayer_->GetTransform()->GetTranslation().y - 20.0f / 40.0f, 0.0f, 1.0f) * 30.0f;
			offsetLength_ = std::clamp(offsetLength_, 80.0f, 1000.0f);
		}
	}

	// -------------------------------------------------
	// ↓ Targetがいたら
	// -------------------------------------------------
	if(target_ != nullptr) {

		Vector3 dif{};
		Vector3 addVec{};
		velocityRate_ = pPlayer_->GetMoveVelocity().x / defaultVelocity_;
		float moveSpeedRate = pPlayer_->GetMoveSpeed() / defaultVelocity_;

		if(pPlayer_->GetIsFlying()){

			// 飛んでいるとき
			adjustX_ -= 0.5f * GameTimer::TimeRate();
			adjustX_ = std::clamp(adjustX_, -15.0f, 0.0f);

			if(pPlayer_->GetTransform()->GetTranslation().y > 10.0f){

				// 高さの割合を計算
				float heightRatio = std::clamp((pPlayer_->GetTransform()->GetTranslation().y - 1.0f) / 40.0f, 0.0f, 1.0f);

				// 飛行中のプレイヤーの高さに応じた追加の調整ベクトルを計算
				addVec.y = 7.0f * std::clamp((pPlayer_->GetTransform()->GetTranslation().y - 10.0f) / 40.0f,0.0f,1.0f);
				addVec.x = -10.0f * heightRatio;
				addVec.x += moveSpeedRate * 10.0f;
				addVec.y += pPlayer_->GetVelocity().y * 30.0f;

				if(pPlayer_->GetIsCloseWing()){
					addVec.y += pPlayer_->GetDropSpeed() * 25.0f;
				}

				// 飛行中の定位置まで回転を加算
				transform_.rotate.x += 0.005f * GameTimer::TimeRate() * velocityRate_;
				transform_.rotate.x = std::clamp(transform_.rotate.x, baseRotate.x, baseRotate.x + 0.15f);
				transform_.rotate.y += 0.005f * GameTimer::TimeRate() * velocityRate_;
				transform_.rotate.y = std::clamp(transform_.rotate.y, baseRotate.y, baseRotate.y + 0.15f);

			}

		} else{

			// 移動をもとに戻す
			adjustX_ += 0.5f * GameTimer::TimeRate();
			adjustX_ = std::clamp(adjustX_, -15.0f, 0.0f);

			// 回転を徐々にもとに戻す
			transform_.rotate.x -= 0.005f * GameTimer::TimeRate();
			transform_.rotate.x = std::clamp(transform_.rotate.x, baseRotate.x, baseRotate.x + 0.15f);
			transform_.rotate.y -= 0.005f * GameTimer::TimeRate();
			transform_.rotate.y = std::clamp(transform_.rotate.y, baseRotate.y, baseRotate.y + 0.15f);
		}


		// 目的座標への差分
		offsetLength_ = std::clamp(offsetLength_, 80.0f, 1000.0f);
		dif = (pPlayer_->GetTransform()->GetTranslation() + offsetVec_ * offsetLength_ + addVec + Vector3(adjustX_,0.0f,0.0f)) - transform_.translate;

		// 少し遅らせて追従
		transform_.translate += dif* 0.04f * GameTimer::TimeRate();
	}

	if(followOnly){
		transform_.translate = pPlayer_->GetTransform()->GetTranslation() + Vector3(-20.4f* follwOfsetlength, 0.3f* follwOfsetlength, -25.7f* follwOfsetlength);
		transform_.rotate = { 0.0f, 0.91f, 0.0f };
	}


	if(GameScene::GetGameState() == GAME_STATE::TITLE){
		transform_.translate = pPlayer_->GetWorldTranslation() + Vector3(3.0f, 0.0f, -15.0f);
		transform_.rotate = { 0.0f,0.0f,0.0f };
	}

	// 前のシーンと違っていたら
	if (GameScene::GetPreGameState() != GameScene::GetGameState()) {
		if (GameScene::GetGameState() == GAME_STATE::TUTORIAL) {
			isTitleToGameCamera_ = true;
		}
	}

	BaseCamera::Update();
}



void Camera::Shake(){}





#ifdef _DEBUG
#include "Engine/Manager/ImGuiManager.h"
void Camera::Debug_Gui(){
	ImGui::Begin("Camera");

	ImGui::Text("%f", &offsetLength_);

	if(ImGui::Button("Reset")) {
		transform_ = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -9.0f} };
		transform_.rotate = { 0.0f, 0.0f, 0.0f };
		rotateMat_ = MakeRotateXYZMatrix(transform_.rotate);
	}

	ImGui::DragFloat3("translate", &transform_.translate.x, 0.1f);
	ImGui::DragFloat3("rotate", &transform_.rotate.x, 0.1f);

	if(ImGui::Button("FollowOnly")){
		followOnly = !followOnly;
	}
	ImGui::DragFloat("follwOfsetlength", &follwOfsetlength, 0.03f);

	ImGui::End();
}
#endif
