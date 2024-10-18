#include "Player.h"
#include "Game/Scene/GameScene.h"

Player::Player(){
	Init();
}

Player::~Player(){}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Init(){

	typeID_ = (int)ObjectType::PLAYER;

	BaseGameObject::Init();
	SetObject("Player_Head.obj");
	aboveWaterSurfacePos = Engine::CreateWorldTransform();

	//
	BaseGameObject* pTarget = this;
	for(int i = 0; i < kMinBodyCount_; i++){

		// 胴体を追加
		AddBody(pTarget);

		// 自身を後続のモデルのターゲットに設定
		pTarget = followModels_.back().get();
  
	}


	animetor_ = std::make_unique<PlayerAnimator>();
	animetor_->Init();
	animetor_->LoadAnimation(model_);

	adjustmentItem_ = AdjustmentItem::GetInstance();
	const char* groupName = "Player";
	adjustmentItem_->AddItem(groupName, "position", transform_->GetTranslation());
	adjustmentItem_->AddItem(groupName, "velocity", velocity_);
	adjustmentItem_->AddItem(groupName, "moveSpeed", moveSpeed_);
	adjustmentItem_->AddItem(groupName, "lookAtT", lookAtT_);
	adjustmentItem_->AddItem(groupName, "radius", radius_);

	AdaptAdjustmentItem();
	restPoseRotation_ = Quaternion::AngleAxis(90.0f * toRadian, Vector3(0.0f, 1.0f, 0.0f));
	transform_->SetQuaternion(restPoseRotation_);

	obb_.size = { 1.0f, 1.0f, 1.0f };
	obb_.center = GetWorldTranslation();

	isMove_ = false;
	moveSpeed_ = defaultSpeed;//0.7f / (1.0f / 60.0f);
	radius_ = 2.0f;

	getCoinNum_ = 0;

	slerpRotation_ = Quaternion();

	hitSe_ = std::make_unique<AudioPlayer>();
	coinGetSe_ = std::make_unique<AudioPlayer>();
	hitSe_->Init("./Game/Resources/Audio/test.wav");
	coinGetSe_->Init("./Game/Resources/Audio/kari_coinGet.wav");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Update(){

	// 毎フレームの初期化・保存
	isSplash_ = false;
	preFlying_ = isFlying_;

	// 移動
	if(isMove_) {
		Move();
	} else {
		velocity_ = { 0.0f,0.0f,0.0f };
	}

	// 飛行フラグ更新
	if(transform_->GetTranslation().y > 0.0f) {
		isFlying_ = true;

		if (!preFlying_) {
			timer_.Measurement(transform_->GetTranslation().x);
			isSplash_ = true;
		}
	} else {
		isFlying_ = false;

		if (preFlying_) {
			timer_.Finish(transform_->GetTranslation().x);
			isSplash_ = true;
		}
	}

	// 体の更新
	UpdateBody();
	for(auto& body : followModels_){
		body->Update();
	}

	animetor_->Update();
	timer_.Update(transform_->GetTranslation().x);


	if(Input::IsTriggerKey(DIK_UP)){
		AddBody(followModels_.back().get());
	} else if(Input::IsTriggerKey(DIK_DOWN)){
		EraseBody();
	}

	obb_.center = GetWorldTranslation();
	obb_.MakeOBBAxis(transform_->GetQuaternion());

	BaseGameObject::Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Draw() const{
	BaseGameObject::Draw();

	// 体の描画
	for(auto& body : followModels_){
		body->Draw();
	}

	//Render::DrawAnimationModels(model_, animetor_->GetSkinnings(), transform_.get(), materials);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　移動関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Move(){

	// 前フレームの値を保存
	prePos_ = transform_->GetTranslation();

	// 飛んでいないときにだけ操作を受け付ける
	if(!isFlying_){// 水中 -----------------------------------

		if(!isDiving_){// 着水直後ではないとき

			// 押すと上昇、離すと沈む
			if(Input::IsPressKey(DIK_SPACE)) {
				pressTime_ += 0.025f * GameTimer::TimeRate();

			} else {
				pressTime_ -= 0.025f * GameTimer::TimeRate();
			}

		} else{// 着水直後

			diveTime_ -= GameTimer::DeltaTime();// 着水後の猶予時間を減らす

			// 潜る
			float t = diveTime_ / kDiveTime_;
			Vector3 diveVec = Vector3(0.0f, divingSpeed_ * 1.2f, 0.0f) * GameTimer::TimeRate() * t;
			transform_->SetTranslaion(transform_->GetTranslation() + diveVec * t);

			// 猶予時間が0になったら通常状態へ
			if(diveTime_ <= 0.0f){
				isDiving_ = false;
				diveTime_ = kDiveTime_;
			}

		}

		isFalling_ = false;

	} else{// 飛行中-------------------------------------------

		// 上昇を徐々に遅くする
		pressTime_  = std::clamp(pressTime_ - 0.01f * GameTimer::TimeRate(),-0.1f,1.0f);

		////////////////////////////// 上昇中 /////////////////////////////////
		if(!isFalling_){

			isCloseWing_ = false;
			dropSpeed_ = 0.0f;

		} else {//////////// 上昇がある程度弱まったら下降を開始する /////////////////


			// SPACE押して翼の開閉
			if(!isFacedBird_){
				if(Input::IsTriggerKey(DIK_SPACE)) {
					isCloseWing_ == false ? isCloseWing_ = true : isCloseWing_ = false;
				}
			}

			// 下降ベクトルを格納する変数
			Vector3 dropVec{};

			if(!isCloseWing_){//////// 翼を広げている際 ////////

				// 下降ベクトル
				dropSpeed_ = 0.0f;

			} else{//////// 翼を閉じている際 ////////

				dropSpeed_ += gravity_ * GameTimer::DeltaTime();
				dropVec = Vector3(0.0f, dropSpeed_, 0.0f) * GameTimer::TimeRate();
			}

			// 座標の更新
			transform_->SetTranslaion(transform_->GetTranslation() + dropVec);

			// 水に触れたらダイブのフラグをオンにする
			if(transform_->GetTranslation().y < 0.0f){
				isDiving_ = true;
				isFalling_ = false;
				isFacedBird_ = false;
				divingSpeed_ = transform_->GetTranslation().y - prePos_.y;
				// 潜水速度を一定範囲に保つ
				divingSpeed_ = std::clamp(divingSpeed_, -1.0f, -0.5f);
			}
		}
	}

	// 角度を加算
	pressTime_ = std::clamp(pressTime_, -1.0f, 1.0f);
	currentAngle_ = kMaxAngle_ * pressTime_;
	LookAtDirection(currentAngle_);

	// 移動量を加算
	velocity_ = Vector3(1.0f, 0.0f, 0.0f) * MakeRotateZMatrix(currentAngle_);
	velocity_ *= moveSpeed_ * std::fabsf(GameTimer::DeltaTime());
	transform_->SetTranslaion(transform_->GetTranslation() + velocity_);

	// プレイヤー上部の水面の座標を取得
	aboveWaterSurfacePos->SetTranslaion({ transform_->GetTranslation().x, 10.0f,0.0f });
	aboveWaterSurfacePos->Update();

	// 深さを更新
	swimmigDepth_ = 10.0f - transform_->GetTranslation().y;

	// 下降フラグの更新
	if(!isFalling_){
		// ある程度上昇が収まったら下降フラグをオンに
		if(currentAngle_ <= 0.1f){
			isFalling_ = true;
		}
	}

	MoveLimit();
}


void Player::MoveLimit(){

	// 地面に接触したら
	if(transform_->GetTranslation().y - radius_ < GameScene::GetGroundDepth()){
		Vector3 translate = transform_->GetTranslation();
		transform_->SetTranslaion({ translate.x,GameScene::GetGroundDepth() + radius_,translate.z });
		pressTime_ = 0.0f;
		// 減速させる
		moveSpeed_ += (kMinMoveSpeed_ - moveSpeed_) * 0.5f * GameTimer::DeltaTime();
		moveSpeed_ = std::clamp(moveSpeed_, kMinMoveSpeed_, kMaxMoveSpeed_);
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　進行方向を向かせる
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::LookAtDirection(const float& angle){
	Quaternion moveRotation = Quaternion::EulerToQuaternion(Vector3(0.0f, 0.0f, angle)) * restPoseRotation_;//.Normalize()
	slerpRotation_ = Quaternion::Slerp(transform_->GetQuaternion().Normalize(), moveRotation.Normalize(), lookAtT_).Normalize();
	transform_->SetQuaternion(moveRotation);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　調整項目を適応させる
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::AdaptAdjustmentItem(){
	const char* groupName = "Player";
	transform_->SetTranslaion(adjustmentItem_->GetValue<Vector3>(groupName, "position"));
	moveSpeed_ = adjustmentItem_->GetValue<float>(groupName, "moveSpeed");
	velocity_ = adjustmentItem_->GetValue<Vector3>(groupName, "velocity");
	lookAtT_ = adjustmentItem_->GetValue<float>(groupName, "lookAtT");
	radius_ = adjustmentItem_->GetValue<float>(groupName, "radius");
}

const Vector3 Player::GetWorldTranslation() const{
	return Transform(Vector3(0.0f, 0.0f, 0.0f), transform_->GetWorldMatrix());
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　胴体の追加
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::UpdateBody(){
	float division = 1.0f / (kMaxBodyCount_ - kMinBodyCount_);
	int bodyCount = kMinBodyCount_ + int(chargePower_ / division);

	if(bodyCount > bodyCount_){
		AddBody(followModels_.back().get());
	} else if(bodyCount < bodyCount_){
		EraseBody();
	}
}

void Player::AddBody(BaseGameObject* pTarget){

	if(bodyCount_ >= kMaxBodyCount_){ return; }

	if(followModels_.size() != 0){
		auto& preBody = followModels_.back();
		preBody->SetObject("Player_Torso.obj");
	}

	auto& body = followModels_.emplace_back(std::make_unique<PlayerBody>());
	body->Init();
	body->SetObject("Player_Tail.obj");
	body->SetTarget(pTarget);
	body->SetSpace(3.0f);
	body->GetTransform()->SetTranslaion(pTarget->GetTransform()->GetTranslation());
	body->GetTransform()->SetQuaternion(Quaternion::AngleAxis(90.0f * toRadian, Vector3(0.0f, 1.0f, 0.0f)));

	// 更新
	body->Update();

	bodyCount_++;
}

// 最後尾を削除
void Player::EraseBody(){
	if(followModels_.size() > kMinBodyCount_){
		followModels_.pop_back();
	}

	followModels_.back()->SetObject("Player_Tail.obj");
	bodyCount_--;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void Player::Debug_Gui(){
	ImGui::Begin("Player");
	transform_->Debug_Gui();
	ImGui::Separator();
	ImGui::Text("Parameter");
	ImGui::DragFloat3("velocity", &velocity_.x, 0.1f);
	ImGui::DragFloat("moveSpeed", &moveSpeed_, 0.1f);
	ImGui::DragFloat("lookAtT", &lookAtT_, 0.01f);
	
	ImGui::DragFloat("radius", &radius_, 0.1f);
	ImGui::DragFloat("currentAngle_", &currentAngle_, 0.1f);

	if(ImGui::Button("ReAdapt")) {
		AdaptAdjustmentItem();
	}


	ImGui::Checkbox("isMove", &isMove_);

	if(hitWaterSurface_) {
		ImGui::Text("Hit");
	} else {
		ImGui::Text("not Hit");
	}

	/*if (ImGui::TreeNode("FlyingTimer")) {*/
		timer_.Debug_Gui();
	/*	ImGui::TreePop();
	}*/

	ImGui::End();
}


#endif // _DEBUG

//////////////////////////////////////////////////////
//  衝突時処理
//////////////////////////////////////////////////////

void Player::OnCollision(Collider* other){

	//障害物に当たった場合
	if(other->GetObjectType() == (int)ObjectType::FISH){

		// fish型に変換
		Fish* pFish = dynamic_cast<Fish*>(other);
		float fishSizeDivision = 1.0f / (float)FISH_SIZE::kFishSizeCount;

		// 魚を食べられたとき
		if(chargePower_ / fishSizeDivision >= (float)pFish->GetFishSize()){

			chargePower_ += pFish->GetEnergy();
			chargePower_ = std::clamp(chargePower_, 0.0f, 1.0f);

		} else{// 食べられなかったとき

			chargePower_ -= 0.05f;
			chargePower_ = std::clamp(chargePower_, 0.0f, 1.0f);

			moveSpeed_ -= 10.0f;
			moveSpeed_ = std::clamp(moveSpeed_, kMinMoveSpeed_, kMaxMoveSpeed_);

			hitSe_->Play(false, true);
		}
	}

	if(other->GetObjectType() == (int)ObjectType::ITEM){
		moveSpeed_ += 10.0f;
		moveSpeed_ = std::clamp(moveSpeed_, kMinMoveSpeed_, kMaxMoveSpeed_);
	}

	if(other->GetObjectType() == (int)ObjectType::COIN) {
		coinGetSe_->Play(false, 0.5f, true);
		getCoinNum_++;
	}

	if(other->GetObjectType() == (int)ObjectType::BIRD) {

		if(isFlying_ && !isFalling_){ return; }

		if(isCloseWing_){
			pressTime_ = 1.0f;
			isFalling_ = false;
			isCloseWing_ = false;
		} else{
			moveSpeed_ *= 0.5f;
			isCloseWing_ = true;
			isFacedBird_ - true;
		}
	}
}