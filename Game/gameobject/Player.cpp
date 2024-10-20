#include "Player.h"
#include "Game/Scene/GameScene.h"
#include "Game/Attachment/PlayerAnimator.h"

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
	SetIsLighting(false);

	//
	BaseGameObject* pTarget = this;
	for(int i = 0; i < kMinBodyCount_; i++){

		// 胴体を追加
		AddBody(pTarget);

		// 自身を後続のモデルのターゲットに設定
		pTarget = followModels_.back().get();

	}

	animetor_ = std::make_unique<PlayerAnimator>();
	animetor_->Init(this);

	wings_ = std::make_unique<PlayerWings>();
	wings_->Init();

	adjustmentItem_ = AdjustmentItem::GetInstance();
	const char* groupName = "Player";
	adjustmentItem_->AddItem(groupName, "position", transform_->GetTranslation());
	adjustmentItem_->AddItem(groupName, "velocity", velocity_);
	adjustmentItem_->AddItem(groupName, "temporaryAcceleration_", temporaryAcceleration_);
	adjustmentItem_->AddItem(groupName, "lookAtT", lookAtT_);
	adjustmentItem_->AddItem(groupName, "radius", radius_);

	AdaptAdjustmentItem();
	//restPoseRotation_ = Quaternion::AngleAxis(90.0f * toRadian, Vector3(0.0f, 1.0f, 0.0f));
	//transform_->SetQuaternion(restPoseRotation_);

	obb_.size = { 1.0f, 1.0f, 1.0f };
	obb_.center = GetWorldTranslation();

	isMove_ = false;
	baseSpeed_ = defaultSpeed;//0.7f / (1.0f / 60.0f);
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
		if(Input::IsTriggerKey(DIK_SPACE)) {
			isMove_ = true;
		}
	}

	// 飛行フラグ更新
	if(transform_->GetTranslation().y > 0.0f) {
		isFlying_ = true;

		// 水から出た瞬間
		if(!preFlying_) {
			timer_.Measurement(transform_->GetTranslation().x);
			isSplash_ = true;
			isEnableLaunch_ = true;
		}
	} else {
		//isFlying_ = false;

		// 着水した瞬間
		if(preFlying_) {
			timer_.Finish(transform_->GetTranslation().x);
			isSplash_ = true;
		}
	}

	// 体の更新
	UpdateBody();
	for(auto& body : followModels_){
		body->Update();
	}

	timer_.Update(transform_->GetTranslation().x);
	totalSpeedRatio = GetMoveSpeed() / kMaxMoveSpeed_;

	if(Input::IsTriggerKey(DIK_UP)){
		AddBody(followModels_.back().get());
	} else if(Input::IsTriggerKey(DIK_DOWN)){
		EraseBody();
	}

	// -------------------------------------------------
	// ↓ obbの更新
	// -------------------------------------------------
	obb_.center = transform_->GetTranslation();
	obb_.MakeOBBAxis(transform_->GetQuaternion());

	// -------------------------------------------------
	// ↓ 羽根の更新
	// -------------------------------------------------
	if (isFlying_) {
		const PlayerBody* topBody = followModels_.begin()->get();
		if (isFalling_ && isCloseWing_) {
			wings_->Update(topBody->GetTranslation(), topBody->GetQuaternion(), true);
		} else {
			wings_->Update(topBody->GetTranslation(), topBody->GetQuaternion(), false);
		}
	} else {
		wings_->NotFlying();
	}
	
	BaseGameObject::Update();

	// -------------------------------------------------
	// ↓ Effectの更新
	// -------------------------------------------------
	animetor_->Update();
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
	if (isFlying_) {
		wings_->Draw();
	}
	//Render::DrawAnimationModels(model_, animetor_->GetSkinnings(), transform_.get(), materials);
}

void Player::DrawAnimetor() const {
	animetor_->Draw();
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
				pressTime_ += addPressTime_ * GameTimer::TimeRate();

			} else {
				pressTime_ -= addPressTime_ * GameTimer::TimeRate();
			}

			// 一時加速、減速を徐々に元に戻す
			if(temporaryAcceleration_ > 0.0f){
				temporaryAcceleration_ -= (increaseVelocity_ * 0.5f) * GameTimer::DeltaTime();
			} else{
				temporaryAcceleration_ += (increaseVelocity_ * 0.5f) * GameTimer::DeltaTime();
			}
			temporaryAcceleration_ = std::clamp(temporaryAcceleration_, kMinMoveSpeed_ - baseSpeed_, kMaxMoveSpeed_ - baseSpeed_ + 20.0f);


		} else{// 着水直後

			diveTime_ -= GameTimer::DeltaTime();// 着水後の猶予時間を減らす

			// 潜る
			float t = diveTime_ / kDiveTime_;
			diveVec = { 0,0,0 };
			 diveVec = Vector3(0.0f, divingSpeed_ * 1.2f, 0.0f) * GameTimer::TimeRate() * t;
			//transform_->SetTranslaion(transform_->GetTranslation() + diveVec * t);

			// 猶予時間が0になったら通常状態へ
			if(diveTime_ <= 0.0f){
				isDiving_ = false;
				diveTime_ = kDiveTime_;
			}

		}

		isFalling_ = false;

	} else{// 飛行中-------------------------------------------

		// 上昇を徐々に遅くする
		float moveSpeedRate = GetMoveSpeed() / kMaxMoveSpeed_;
		pressTime_ = std::clamp(
			pressTime_ - 0.01f * GameTimer::TimeRate() * (3.0f - (2.0f * moveSpeedRate)),
			-0.1f * (3.0f - (2.0f * moveSpeedRate)),
			1.0f
		);

		// SPACE押して翼の開閉
		if(!isFacedBird_){
			if(Input::IsTriggerKey(DIK_SPACE)) {
				isCloseWing_ == false ? isCloseWing_ = true : isCloseWing_ = false;
				isFalling_ = true;
				isEnableLaunch_ = false;// 再発射できないようにする
			}
		}

		////////////////////////////// 上昇中 /////////////////////////////////
		if(!isFalling_){

			isCloseWing_ = false;
			isFacedBird_ = false;
			dropSpeed_ = 0.0f;

		} else {//////////// 上昇がある程度弱まったら下降を開始する /////////////////


			// 下降ベクトルを格納する変数
			//Vector3 dropVec{};
			dropVec = { 0,0,0 };
			if(!isCloseWing_){//////// 翼を広げている際 ////////

				// 下降ベクトル
				dropSpeed_ = 0.0f;

			} else{//////// 翼を閉じている際 ////////

				pressTime_ *= 0.5f * GameTimer::TimeRate();
				dropSpeed_ += gravity_ * GameTimer::DeltaTime();
				dropVec = Vector3(0.0f, dropSpeed_, 0.0f) * GameTimer::TimeRate();

			}

			// 座標の更新
			//transform_->SetTranslaion(transform_->GetTranslation() + dropVec);

			// 水に触れたらダイブのフラグをオンにする
			if(transform_->GetTranslation().y < 0.0f){
				isDiving_ = true;
				isFalling_ = false;
				isFlying_ = false;
				isFacedBird_ = false;
				divingSpeed_ = transform_->GetTranslation().y - prePos_.y;
				// 潜水速度を一定範囲に保つ
				divingSpeed_ = std::clamp(divingSpeed_, -1.0f, -0.5f);
				baseSpeed_ = std::clamp(baseSpeed_ / 2, kMinBaseSpeed_, kMaxBaseSpeed_);
				diveTime_ = kDiveTime_;
			}
		}
	}

	// 角度を加算
	pressTime_ = std::clamp(pressTime_, -1.0f, 1.0f);
	currentAngle_ = kMaxAngle_ * pressTime_;

	// 移動量を加算
	velocity_ = Vector3(1.0f, 0.0f, 0.0f) * MakeRotateZMatrix(currentAngle_);
	velocity_ *= GetMoveSpeed() * std::fabsf(GameTimer::DeltaTime());


	transform_->SetTranslaion(transform_->GetTranslation() + velocity_+ dropVec+ diveVec);
	
	float newAngle = std::atan2(-(velocity_.y + dropVec.y+ diveVec.y), -(velocity_.x + dropVec.x + diveVec.x));
	newAngle += 3.141592 ;
	LookAtDirection(newAngle);
	dropVec = { 0,0,0 };
	diveVec = { 0,0,0 };




	// プレイヤー上部の水面の座標を取得
	aboveWaterSurfacePos->SetTranslaion({ transform_->GetTranslation().x, 10.0f,0.0f });
	aboveWaterSurfacePos->Update();

	// 深さを更新
	swimmigDepth_ = 10.0f - transform_->GetTranslation().y;

	// 下降フラグの更新
	if(isFlying_){
		if(!isFalling_){
			// ある程度上昇が収まったら下降フラグをオンに
			if(bodyCount_ > kMinBodyCount_){
				// 胴体スタックがあるときは再上昇
				if(currentAngle_ <= 0.1f){
					if(isEnableLaunch_){
						float division = 1.0f / (kMaxBodyCount_ - kMinBodyCount_);
						chargePower_ = ((bodyCount_ - kMinBodyCount_) - 1) * division;
						pressTime_ = 0.7f;
					}
				}
			} else{

				if(currentAngle_ <= 0.1f){
					isFalling_ = true;
				}
			}
		}
	}

	MoveLimit();
}


void Player::MoveLimit(){

	// 地面に接触したら
	if(transform_->GetTranslation().y - radius_ < GameScene::GetGroundDepth()){
		// 移動制限
		Vector3 translate = transform_->GetTranslation();
		transform_->SetTranslaion({ translate.x,GameScene::GetGroundDepth() + radius_,translate.z });
		pressTime_ = 0.0f;
		// 減速させる
		temporaryAcceleration_ += ((kMinMoveSpeed_ - baseSpeed_) - temporaryAcceleration_) * 0.5f * GameTimer::DeltaTime();
		temporaryAcceleration_ = std::clamp(temporaryAcceleration_, kMinMoveSpeed_ - baseSpeed_, kMaxMoveSpeed_ - baseSpeed_ + 20.0f);
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　進行方向を向かせる
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::LookAtDirection(const float& angle){
	//Quaternion moveRotation = Quaternion::EulerToQuaternion(Vector3(0.0f, 0.0f, angle)) * restPoseRotation_;//.Normalize()
	//slerpRotation_ = Quaternion::Slerp(transform_->GetQuaternion().Normalize(), moveRotation.Normalize(), lookAtT_).Normalize();
	Quaternion newRotate = Quaternion::AngleAxis(angle, { 0 ,0,1 });
	transform_->SetQuaternion(newRotate);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　調整項目を適応させる
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::AdaptAdjustmentItem(){
	const char* groupName = "Player";
	transform_->SetTranslaion(adjustmentItem_->GetValue<Vector3>(groupName, "position"));
	temporaryAcceleration_ = adjustmentItem_->GetValue<float>(groupName, "temporaryAcceleration_");
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

void Player::Rounding(Vector3& velocity){
	if(velocity.Length() < 0.1f){
		velocity = { 0,0,0 };
	}
}



//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void Player::Debug_Gui(){
	

	ImGui::Begin("Player");
	transform_->Debug_Gui();
	ImGui::DragFloat3("worldPos", &worldPos_.x, 0.1f);
	ImGui::Separator();
	ImGui::Text("Parameter");
	ImGui::DragFloat3("velocity", &velocity_.x, 0.1f);
	ImGui::DragFloat("temporaryAcceleration_", &temporaryAcceleration_, 0.1f);
	ImGui::DragFloat("baseSpeed_", &baseSpeed_, 0.1f);
	ImGui::SliderFloat("chargePower_", &chargePower_, 0.0f, 1.0f);
	ImGui::DragFloat("lookAtT", &lookAtT_, 0.01f);
	ImGui::Text("totalSpeedRatio=%f", totalSpeedRatio);
	ImGui::DragFloat("addPressTime_=%f", &addPressTime_, 0.001f);

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

	wings_->Debug_Gui();

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

			// チャージを行う
			chargePower_ += pFish->GetEnergy();
			chargePower_ = std::clamp(chargePower_, 0.0f, 1.0f);
			// 一時加速する
			temporaryAcceleration_ += increaseVelocity_;
			temporaryAcceleration_ = std::clamp(temporaryAcceleration_, kMinMoveSpeed_ - baseSpeed_, kMaxMoveSpeed_ - baseSpeed_ + 20.0f);
			//基礎速度の変動
			baseSpeed_ = std::clamp(baseSpeed_ + kAddSpeed_, kMinBaseSpeed_, kMaxBaseSpeed_);

		} else{// 食べられなかったとき

			if(!isDiving_){

				// チャージが減る
				chargePower_ -= 0.05f;
				chargePower_ = std::clamp(chargePower_, 0.0f, 1.0f);

				// 一時減速する
				temporaryAcceleration_ += decreaseVelocity_;
				temporaryAcceleration_ = std::clamp(temporaryAcceleration_, kMinMoveSpeed_ - baseSpeed_, kMaxMoveSpeed_ - baseSpeed_ + 20.0f);
				//基礎速度の変動
				baseSpeed_ = std::clamp(baseSpeed_ - kDecreaseSpeed_, kMinBaseSpeed_, kMaxBaseSpeed_);
				hitSe_->Play(false, true);

			}
		}
	}

	if(other->GetObjectType() == (int)ObjectType::ITEM){

	}

	if(other->GetObjectType() == (int)ObjectType::COIN) {
		coinGetSe_->Play(false, 0.5f, true);
		getCoinNum_++;
	}

	if(other->GetObjectType() == (int)ObjectType::BIRD) {

		if(isFlying_ && !isFalling_){ return; }

		// 翼を閉じて落下しているときだけ踏みつけられる
		if(isCloseWing_){

			// ある程度上から踏みつけないといけない
			if(dropSpeed_ < gravity_ * 0.25f){//dropSpeed_ < gravity_ * 0.25f//transform_->GetTranslation().y>other->GetWorldTranslation().y+ other->GetObb().size.y*0.25f
				pressTime_ = 1.0f;
				isFalling_ = false;
				isCloseWing_ = false;

			} else{
				// 正面衝突の場合
				temporaryAcceleration_ -= (kMaxMoveSpeed_ - baseSpeed_) * 0.5f;
				temporaryAcceleration_ = std::clamp(temporaryAcceleration_, kMinMoveSpeed_ - baseSpeed_, kMaxMoveSpeed_ - baseSpeed_ + 20.0f);
				isFacedBird_ = true;
				isCloseWing_ = true;
			}

		} else{
			// 正面衝突の場合
			temporaryAcceleration_ -= (kMaxMoveSpeed_ - baseSpeed_) * 0.5f;
			temporaryAcceleration_ = std::clamp(temporaryAcceleration_, kMinMoveSpeed_ - baseSpeed_, kMaxMoveSpeed_ - baseSpeed_ + 20.0f);
			isFacedBird_ = true;
			isCloseWing_ = true;
		}
	}
}