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

	if(GameScene::GetGameState() == GAME_STATE::TITLE){
		transform_->SetTranslaion({ 0.0f,emitHeight_,0.0f });
	}

	obb_.size = { 1.0f, 1.0f, 1.0f };
	obb_.center = GetWorldTranslation();

	isMove_ = true;
	baseSpeed_ = defaultSpeed;//0.7f / (1.0f / 60.0f);
	radius_ = 2.0f;

	getCoinNum_ = 0;

	slerpRotation_ = Quaternion();

	hitSe_ = std::make_unique<AudioPlayer>();
	coinGetSe_ = std::make_unique<AudioPlayer>();
	hitSe_->Init("test.wav");
	coinGetSe_->Init("kari_coinGet.wav");
	//初期位置をずらす
	transform_.get()->SetTranslationX(600);
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
		// 水から出た瞬間
		if(!preFlying_) {
			timer_.Measurement(transform_->GetTranslation().x);
			isFlying_ = true;
			isSplash_ = true;
			isEnableLaunch_ = true;
			if(waterSurfaceCoolTime <= 0) {
				AudioPlayer::SinglShotPlay("inWaterSurface.mp3", 0.5f);
				AudioPlayer::SinglShotPlay("outWaterSurface.mp3", 0.5f);
				waterSurfaceCoolTime = 0.25f;
			}
		}

	} else {
		// 着水した瞬間
		if(preFlying_) {
			timer_.Finish(transform_->GetTranslation().x);
			isSplash_ = true;
			//isFlying_ = false;//コメントアウト外さない
			if(waterSurfaceCoolTime <= 0) {
				AudioPlayer::SinglShotPlay("inWaterSurface.mp3", 0.5f);
				AudioPlayer::SinglShotPlay("outWaterSurface.mp3", 0.5f);
				waterSurfaceCoolTime = 0.25f;
			}
		}
	}
	waterSurfaceCoolTime -= GameTimer::DeltaTime();
	// 体の更新
	UpdateBody();


	timer_.Update(transform_->GetTranslation().x);
	totalSpeedRatio = GetMoveSpeed() / kMaxMoveSpeed_;

#ifdef _DEBUG
	if(Input::IsTriggerKey(DIK_UP)){
		if(followModels_.size() != 0){
			AddBody(followModels_.back().get());
		} else{
			AddBody(this);
		}
	} else if(Input::IsTriggerKey(DIK_DOWN)){
		EraseBody();
	}
#endif // _DEBUG



	// -------------------------------------------------
	// ↓ obbの更新
	// -------------------------------------------------
	obb_.center = transform_->GetTranslation();
	obb_.MakeOBBAxis(transform_->GetQuaternion());

	// -------------------------------------------------
	// ↓ 羽根の更新
	// -------------------------------------------------
	if(isFlying_) {
		const PlayerBody* topBody = followModels_.begin()->get();
		if(isFalling_ && isCloseWing_) {
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
	if(isFlying_) {
		wings_->Draw();
	}
	//Render::DrawAnimationModels(model_, animetor_->GetSkinnings(), transform_.get(), materials);
}

void Player::DrawAnimetor() const{
	animetor_->Draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　移動関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Move(){

	if(GameScene::GetGameState() == GAME_STATE::TITLE){
		pressTime_ = std::sinf(6.28f * GameTimer::TotalTime()) * 0.2f;

		// クランプ
		float height = GetWorldTranslation().y;
		height = std::clamp(height, emitHeight_ - 10.0f, emitHeight_ + 10.0f);
		transform_->SetTranslationY(height);

		// transformの更新
		UpdateTransform();
		MoveLimit();
		return;
	}


	// 前フレームの値を保存
	prePos_ = transform_->GetTranslation();

	MoveWater();
	MoveSky();

	// 角度を加算
	pressTime_ = std::clamp(pressTime_, -1.0f, 1.0f);
	currentAngle_ = kMaxAngle_ * pressTime_;

	// 移動量を加算
	velocity_ = Vector3(1.0f, 0.0f, 0.0f) * MakeRotateZMatrix(currentAngle_);
	velocity_ *= GetMoveSpeed() * std::fabsf(GameTimer::DeltaTime());


	transform_->SetTranslaion(transform_->GetTranslation() + velocity_ + dropVec + diveVec);

	float newAngle = std::atan2(-(velocity_.y + dropVec.y + diveVec.y), -(velocity_.x + dropVec.x + diveVec.x));
	newAngle += 3.141592f;
	LookAtDirection(newAngle);
	dropVec = { 0,0,0 };
	diveVec = { 0,0,0 };

	// 深さを更新
	swimmigDepth_ = 10.0f - transform_->GetTranslation().y;


	// プレイヤー上部の水面の座標を取得
	aboveWaterSurfacePos->SetTranslaion({ transform_->GetTranslation().x, 10.0f,0.0f });
	aboveWaterSurfacePos->Update();

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
						AudioPlayer::SinglShotPlay("boost.mp3", 0.5f);
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
		temporaryAcceleration_ = std::clamp(temporaryAcceleration_, kMinAcceleration_, kMaxAcceleration_);
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

	if(bodyCount > bodyCount_ or bodyCount_ < kMinBodyCount_){
		if(followModels_.size() != 0){
			AddBody(followModels_.back().get());
		} else{
			AddBody(this);
		}

		if(bodyCount_ > kMinBodyCount_){
			AudioPlayer::SinglShotPlay("incrementBody.mp3", 0.1f);
		}

	} else if(bodyCount < bodyCount_){
		EraseBody();
		AudioPlayer::SinglShotPlay("decrementBody.mp3", 0.1f);
	}

	for(auto& body : followModels_){
		body->Update();
	}
}

void Player::AddBody(BaseGameObject* pTarget){

	pTarget->UpdateMatrix();
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
	AnimetionEffectManager::AddListEffect("./Game/Resources/Model/AddTorso/", "AddTorso.gltf", body->GetTransform(), false,
		Vector3(1, 1, 1), Quaternion(), body->GetTranslation());
	// 更新
	body->Update();

	bodyCount_++;
}

// 最後尾を削除
void Player::EraseBody(){
	if(followModels_.size() > kMinBodyCount_){
		const auto& popObj = followModels_.back();
		AnimetionEffectManager::AddListEffect("./Game/Resources/Model/JumpEffect/", "JumpEffect.gltf", transform_.get(), false,
			{ 1.0f, 1.0f, 1.0f }, Quaternion(), popObj->GetTransform()->GetTranslation());
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
	ImGui::SliderInt("bodyCount", &bodyCount_, 0, 1);
	ImGui::DragFloat("lookAtT", &lookAtT_, 0.01f);
	ImGui::Text("totalSpeedRatio=%f", totalSpeedRatio);
	ImGui::DragFloat("addPressTime_=%f", &addPressTime_, 0.001f);

	ImGui::DragFloat("radius", &radius_, 0.1f);
	ImGui::DragFloat("reflection", &reflection_, 0.1f);
	ImGui::DragFloat("dontInputTime", &dontInputTime_, 0.1f);
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
			temporaryAcceleration_ = std::clamp(temporaryAcceleration_, kMinAcceleration_, kMaxAcceleration_);
			//基礎速度の変動
			baseSpeed_ = std::clamp(baseSpeed_ + kAddSpeed_, kMinBaseSpeed_, kMaxBaseSpeed_);
			AudioPlayer::SinglShotPlay("eat.mp3", 0.5f);
			//AudioPlayer::SinglShotPlay("eatAccel.wav", 0.5f);
			AudioPlayer::SinglShotPlay("AddSpeed.mp3", 0.5f);
			float scale = (float)pFish->GetFishSize() + 2.0f;
			AnimetionEffectManager::AddListEffect("./Game/Resources/Model/EatEffect/", "EatEffect.gltf",
				nullptr, false, Vector3(scale, scale, scale), Quaternion(), transform_->GetTranslation());



		} else{// 食べられなかったとき

			if(!isDiving_){

				// チャージが減る
				//chargePower_ -= 0.05f;
				//chargePower_ = std::clamp(chargePower_, 0.0f, 1.0f);

				float scale = (float)pFish->GetFishSize() + 1.0f;
				AnimetionEffectManager::AddListEffect("./Game/Resources/Model/FishDestroy/", "FishDestroy.gltf",
					nullptr, false, Vector3(scale, scale, scale), Quaternion(), GetWorldTranslation());
				SpeedDown();
			}
		}
	}

	if(other->GetObjectType() == (int)ObjectType::BIRD) {

		if(isFlying_ && !isFalling_){ return; }

		// 翼を閉じて落下しているときだけ踏みつけられる
		if(isCloseWing_){

			// ある程度上から踏みつけないといけない
			if(dropSpeed_ < gravity_ * 0.25f){//dropSpeed_ < gravity_ * 0.25f//transform_->GetTranslation().y>other->GetWorldTranslation().y+ other->GetObb().size.y*0.25f
				if(!isFacedBird_) {
					pressTime_ = 1.0f;
					isFalling_ = false;
					isCloseWing_ = false;
					AudioPlayer::SinglShotPlay("BirdJump_3.mp3", 0.5f);
					AnimetionEffectManager::AddListEffect("./Game/Resources/Model/BirdJumpEffect/", "BirdJumpEffect.gltf", transform_.get(), false,
						Vector3(1.0f, 1.0f, 1.0f), Quaternion(), transform_->GetTranslation());
				}
			} else{
				// 正面衝突の場合
				isFacedBird_ = true;
				isCloseWing_ = true;
				SpeedDown();
			}

		} else{

			if(collisionAllowableTime_ <= 0.0f){
				isFacedBird_ = true;
				isCloseWing_ = true;
				SpeedDown();
			}
		}
	}

	if(other->GetObjectType() == (int)ObjectType::DRIFTWOOD) {
		pressTime_ *= (-1.0f) * reflection_;
		dontInputPressTime_ = dontInputTime_;
		AnimetionEffectManager::AddListEffect("./Game/Resources/Model/DriftWoodDestroy/", "DriftWoodDestroy.gltf", nullptr, true,
			Vector3(1, 1, 1), Quaternion(), other->GetWorldTranslation());
		SpeedDown();

	} else if(other->GetObjectType() == (int)ObjectType::ROCK) {
		SpeedDown();
	}
}




void Player::Move_TITLE(){}
void Player::Move_TUTORIAL(){}
void Player::Move_GAME(){}



void Player::ResultSceneUpdate(){

	float scale = 1.0f + 0.1f * EaseOutSine(std::fabsf(std::sinf(3.14f * GameTimer::TotalTime()) * 1.5f));
	transform_->SetScale({ scale,scale,scale });

	UpdateMatrix();
	UpdateBody();
}

void Player::SpeedDown(){
	// 一時減速する
	temporaryAcceleration_ += decreaseVelocity_;
	temporaryAcceleration_ = std::clamp(temporaryAcceleration_, kMinAcceleration_, kMaxAcceleration_);
	//基礎速度の変動
	baseSpeed_ = std::clamp(baseSpeed_ - kDecreaseSpeed_, kMinBaseSpeed_, kMaxBaseSpeed_);
	AudioPlayer::SinglShotPlay("confusion.mp3", 0.5f);
	AudioPlayer::SinglShotPlay("Damage.mp3", 3.0f);

	AnimetionEffectManager::AddListEffect("./Game/Resources/Model/Effect1/", "Effect1.gltf",
		nullptr, false, Vector3(1.0f, 1.0f, 1.0f), Quaternion(), GetWorldTranslation());
	//ピヨピヨエフェクト
	AnimetionEffectManager::AddListEffect("./Game/Resources/Model/SlowEffect/", "SlowEffect.gltf",
		transform_.get(), false, Vector3(3, 3, 3), Quaternion(), GetWorldTranslation());

}
// 水中 -----------------------------------
void Player::MoveWater(){
	if(isFlying_){ return; }

	if(!isDiving_){// 着水直後ではないとき

		addPressTime_ = std::lerp(maxAddPress, minAddPress, totalSpeedRatio);

		// 入力を受け付けない時間がプラスだったら入力しない
		if(dontInputPressTime_ < 0) {
			// 押すと上昇、離すと沈む
			if(Input::IsPressKey(DIK_SPACE)) {
				pressTime_ += addPressTime_ * GameTimer::TimeRate();

			} else {
				pressTime_ -= addPressTime_ * GameTimer::TimeRate();
			}
			seCoolTime -= GameTimer::DeltaTime();
			if(seCoolTime <= 0 && Input::IsTriggerKey(DIK_SPACE)) {
				AudioPlayer::SinglShotPlay("MoveChangeUp.mp3", 0.2f);
				seCoolTime = 0.25f;
			}/*
			if(seCoolTime <= 0 && Input::IsReleaseKey(DIK_SPACE)) {
				AudioPlayer::SinglShotPlay("MoveChangeDown.mp3", 0.2f);
				seCoolTime = 0.25f;
			}*/
		} else {
			dontInputPressTime_ -= GameTimer::DeltaTime();
		}

		// 一時加速、減速を徐々に元に戻す
		if(temporaryAcceleration_ > 0.0f){
			temporaryAcceleration_ -= (increaseVelocity_ * 0.5f) * GameTimer::DeltaTime();
		} else{
			temporaryAcceleration_ += (increaseVelocity_ * 0.5f) * GameTimer::DeltaTime();
		}
		temporaryAcceleration_ = std::clamp(temporaryAcceleration_, kMinAcceleration_, kMaxAcceleration_);


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


}

void Player::MoveSky(){

	if(!isFlying_){ return; }
	// 飛んでいないときにだけ操作を受け付ける
	// 飛行中-------------------------------------------

		// 上昇を徐々に遅くする
		float moveSpeedRate = GetMoveSpeed() / kMaxMoveSpeed_;
		pressTime_ = std::clamp(
			pressTime_ - 0.01f * GameTimer::TimeRate() * (3.0f - (2.0f * moveSpeedRate)),
			-0.1f * (3.0f - (2.0f * moveSpeedRate)),
			1.0f
		);

		// SPACE押して翼の開閉
		//if(!isFacedBird_){
		//	if(Input::IsTriggerKey(DIK_SPACE)) {
		//		isCloseWing_ == false ? isCloseWing_ = true : isCloseWing_ = false;
		//		isFalling_ = true;
		//		isEnableLaunch_ = false;// 再発射できないようにする
		//	}
		//}

		////////////////////////////// 上昇中 /////////////////////////////////
		if(!isFalling_){

			isCloseWing_ = false;
			isFacedBird_ = false;
			dropSpeed_ = 0.0f;
			collisionAllowableTime_ = kAllowableTime;
			//diveTime_ = kDiveTime_;

		} else {//////////// 上昇がある程度弱まったら下降を開始する /////////////////

			collisionAllowableTime_ -= GameTimer::DeltaTime();

			//////////////////////////////////////////////////////////////////////////////

			// ---------------- 滑空状態にするか下降状態にするかを判定するための処理 ----------------------- //
			// 飛行中は押していると滑空する
			// pressタイムがプラスの時は上を向いているので受け付けない
			if(pressTime_ <= 0) {
				if(!isFacedBird_) {
					if(Input::IsPressKey(DIK_SPACE)) {
						// 押している時は滑空する
						// 下降ベクトル
						dropSpeed_ = 0.0f;
						// 羽根が広がっている
						isCloseWing_ = false;
						// baseSpeedを上げて下降速度を上げている
						baseSpeed_ = defaultSpeed * 2.0f;

					} else {
						// 離しているので下降する
						isCloseWing_ = true;
						baseSpeed_ = defaultSpeed;
					}
				}
			}
			////////////////////////////////////////////////////////////////////////////////

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
			if(transform_->GetTranslation().y + dropVec.y < 0.0f){
				isDiving_ = true;
				isFalling_ = false;
				isFlying_ = false;//isFlying_をfalseにするのはここだけ
				isFacedBird_ = false;
				divingSpeed_ = transform_->GetTranslation().y - prePos_.y;
				// 潜水速度を一定範囲に保つ
				divingSpeed_ = std::clamp(divingSpeed_, -1.0f, -0.5f);
				baseSpeed_ = kMinBaseSpeed_ + 10;//std::clamp(baseSpeed_ - kDecreaseSpeed_, kMinBaseSpeed_, kMaxBaseSpeed_);
				temporaryAcceleration_ = std::clamp(temporaryAcceleration_ + decreaseVelocity_, kMinAcceleration_, kMaxAcceleration_);
				diveTime_ = kDiveTime_;
			}
		}
	
}



void Player::UpdateTransform(){
	// 角度を加算
	pressTime_ = std::clamp(pressTime_, -1.0f, 1.0f);
	currentAngle_ = kMaxAngle_ * pressTime_;
	LookAtDirection(currentAngle_);

	// 移動量を加算
	velocity_ = Vector3(1.0f, 0.0f, 0.0f) * MakeRotateZMatrix(currentAngle_);
	velocity_ *= GetMoveSpeed() * std::fabsf(GameTimer::DeltaTime());
	transform_->SetTranslaion(transform_->GetTranslation() + velocity_);

	// プレイヤー上部の水面の座標を取得
	aboveWaterSurfacePos->SetTranslaion({ transform_->GetTranslation().x, 10.0f,0.0f });
	aboveWaterSurfacePos->Update();

	// 深さを更新
	swimmigDepth_ = 10.0f - transform_->GetTranslation().y;
}

void Player::UpdateMatrix(){
	BaseGameObject::Update();
}
