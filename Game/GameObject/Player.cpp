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



	jumpUI_transform_ = Engine::CreateWorldTransform();
	debugJumpUI_ = std::make_unique<BaseGameObject>();
	debugJumpUI_->Init();
	debugJumpUI_->SetObject("evaluationNice.obj");




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
	isHighSpeedMove = false;
	maxSpeedTimeCount = 0;

	wingAnimatinoKeyFrame_ = 0;
	heightMeterObj_ = std::make_unique<BaseGameObject>();
	heightMeterObj_->Init();
	heightMeterObj_->SetObject("HeightMeter.obj");
	heightMeterObj_->SetIsLighting(false);
	heightMeterObj_->GetTransform()->SetScale({ 0.1f,1,1 });
	heightMeterObj_->SetColor({ 1,1,1,0.6f });

	MaxHeightMeter_ = std::make_unique<BaseGameObject>();
	MaxHeightMeter_->Init();
	MaxHeightMeter_->SetObject("MaxHeight.obj");
	MaxHeightMeter_->SetIsLighting(false);
	MaxHeightMeter_->GetTransform()->SetScale({ 0.1f,1,1 });

	jumpUI_Scale_Offset_ = -0.5f;
	jumpUI_Rotate_Offset_ = 0.665f;
	jumpUI_Translate_Offset_ = { 65,-11.5f,17 };


	// カットインスプライト
	cutInSprite_[0] = Engine::CreateSprite("CutIn.png");
	cutInSprite_[0]->SetAnchorPoint({ 0.5f, 0.5f });
	cutInSprite_[0]->SetCenterPos({ 640.0f,360.0f });

	cutInSprite_[1] = Engine::CreateSprite("white.png");
	cutInSprite_[1]->SetTextureSize({ 1280.0f, 720.0f });
	cutInSprite_[1]->SetScale({ 10.0f, 10.0f });
	cutInSprite_[1]->SetColor({ 0,0,0,0 });
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Update(){
	// 毎フレームの初期化・保存
	isSplash_ = false;
	preFlying_ = isFlying_;
	isjet_ = false;

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
			isFlying_ = true;
			isSplash_ = true;
			isEnableLaunch_ = true;
			wingAnimatinoKeyFrame_ = 0;
			if(waterSurfaceCoolTime <= 0) {
				AudioPlayer::SinglShotPlay("inWaterSurface.mp3", 0.08f);
				AudioPlayer::SinglShotPlay("outWaterSurface.mp3", 0.08f);
				waterSurfaceCoolTime = 0.25f;
				Camera::ShakeStart(cameraShakeTime_, cameraShakeRadius_ * 0.25f);
			}

			//JumpAssessor::SetJumpData(GetMoveSpeed(), bodyCount_, jumpUI_transform_.get());
		}

	} else {
		// 着水した瞬間
		if(preFlying_) {
			isSplash_ = true;
			autoFlying_ = false;
			//isFlying_ = false;//コメントアウト外さない
			if(waterSurfaceCoolTime <= 0) {
				AudioPlayer::SinglShotPlay("inWaterSurface.mp3", 0.08f);
				AudioPlayer::SinglShotPlay("outWaterSurface.mp3", 0.08f);
				waterSurfaceCoolTime = 0.25f;
				Camera::ShakeStart(cameraShakeTime_, cameraShakeRadius_ * 0.5f);
			}
		}

		if(Input::GetNotAccepted()) {
			Input::SetNotAccepted(false);
		}
	}
	waterSurfaceCoolTime -= GameTimer::DeltaTime();
	// 体の更新
	UpdateBody();

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

	debugJumpUI_->GetTransform()->SetParent(jumpUI_transform_->GetWorldMatrix());
	debugJumpUI_->Update();

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
			wings_->Update(topBody->GetTranslation(), wingAnimatinoKeyFrame_);
		} else {
			wings_->Update(topBody->GetTranslation(), wingAnimatinoKeyFrame_);
		}
	} else {
		wings_->NotFlying();
	}

	//高さメーターの更新
	heightMeterObj_->GetTransform()->SetTranslationX(transform_->GetTranslation().x);
	heightMeterObj_->Update();

	if(MaxHeightMeter_->GetTransform()->GetTranslation().y <= transform_->GetTranslation().y){
		MaxHeightMeter_->GetTransform()->SetTranslationY(transform_->GetTranslation().y);
	}
	MaxHeightMeter_->GetTransform()->SetTranslationX(transform_->GetTranslation().x);
	MaxHeightMeter_->Update();
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


#ifdef _DEBUG
	if(isDebugUI_Visible_){
		debugJumpUI_->Draw();
	}
#endif // _DEBUG

	//Render::DrawAnimationModels(model_, animetor_->GetSkinnings(), transform_.get(), materials);
}

void Player::DrawAnimetor() const{
	animetor_->Draw();
	if(isFlying_) {
		wings_->Draw();
	}
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
	if(isCutIn_ && !autoFlying_){ pressTime_ = 0.0f; }

	currentAngle_ = kMaxAngle_ * pressTime_;

	// 移動量を加算
	velocity_ = Vector3(1.0f, 0.0f, 0.0f) * MakeRotateZMatrix(currentAngle_);

	//鳥ジャンプの速度をへらしていく
	maxSpeedTimeCount -= GameTimer::DeltaTime();
	if(maxSpeedTimeCount <= 0)
	{
		isHighSpeedMove = false;
	}

	if(maxSpeedTimeCount >= 0){
		birdJumpRaito_ = std::lerp(1.0f, curMaxSpeed, maxSpeedTimeCount);
	}


	velocity_ *= GetMoveSpeed() * fabsf(birdJumpRaito_ * GameTimer::DeltaTime());

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


	// ジャンプUIのトランスフォーム決定
	jumpUI_transform_->SetTranslaion({
		transform_->GetTranslation().x + jumpUI_Translate_Offset_.x,
		transform_->GetTranslation().y + jumpUI_Translate_Offset_.y,
		transform_->GetTranslation().z + jumpUI_Translate_Offset_.z
		});

	jumpUI_transform_->SetScale({
		transform_->GetScale().x + jumpUI_Scale_Offset_,
		transform_->GetScale().y + jumpUI_Scale_Offset_,
		transform_->GetScale().z + jumpUI_Scale_Offset_
		});

	jumpUI_transform_->SetQuaternion(Quaternion::AngleAxis(jumpUI_Rotate_Offset_, { 0.0f,1.0f,0.0f }));

	jumpUI_transform_->Update();

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
				isEnableLaunch_ = false;
				if(currentAngle_ <= 0.1f){
					isFalling_ = true;
				}
			}
		}
	}

	MoveLimit();



}


void Player::MoveLimit(){
	Vector3 translate = transform_->GetTranslation();
	// 地面に接触したら
	if(translate.y - radius_ <= GameScene::GetGroundDepth()){
		// 移動制限		
		//transform_->SetTranslaion({ translate.x,GameScene::GetGroundDepth() + radius_,translate.z });
		pressTime_ = 0.0f;
		// 減速させる
		temporaryAcceleration_ += ((kMinMoveSpeed_ - baseSpeed_) - temporaryAcceleration_) * 0.1f * GameTimer::DeltaTime();
		temporaryAcceleration_ = std::clamp(temporaryAcceleration_, kMinAcceleration_, kMaxAcceleration_);
	}
	transform_->SetTranslaion({ translate.x,std::clamp(translate.y,GameScene::GetGroundDepth() + 2.0f,2000.0f),translate.z });

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

		if (isTutorial_) {
			if (bodyCount_ > 2) {
				for (auto& body : followModels_) {
					body->Update();
				}
				return;
			}
		}

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
		preBody->SetScaleUp();
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
	if(baseSpeed_ + temporaryAcceleration_ >= 150.0f && bodyCount_ >= kMaxBodyCount_){
		if(!isCutIn_ && !autoFlying_){
			isCutIn_ = true;
			gamePlayTimer_->AddTime(5.0f);
			AudioPlayer::SinglShotPlay("CutIn1.mp3", 0.15f);
			AudioPlayer::SinglShotPlay("CutIn2.mp3", 0.15f);
		}
	}

	if (bodyUpEffect_ != nullptr) {
		bodyUpEffect_->AddEffect();
	}
}

// 最後尾を削除
void Player::EraseBody(){
	if(followModels_.size() > kMinBodyCount_){
		const auto& popObj = followModels_.back();
		AnimetionEffectManager::AddListEffect("./Game/Resources/Model/JumpEffect/", "JumpEffect.gltf", transform_.get(), false,
			{ 1.0f, 1.0f, 1.0f }, Quaternion(), popObj->GetTransform()->GetTranslation());
		followModels_.pop_back();

		isjet_ = true;
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

#ifdef _DEBUG

	ImGui::Begin("jumpUI");
	ImGui::Checkbox("isDegugUI_visible", &isDebugUI_Visible_);
	ImGui::DragFloat("scale", &jumpUI_Scale_Offset_, 0.005f);
	ImGui::DragFloat("rotate", &jumpUI_Rotate_Offset_, 0.005f);
	ImGui::DragFloat3("translate", &jumpUI_Translate_Offset_.x, 0.025f);
	ImGui::End();

#endif // _DEBUG


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
	ImGui::DragFloat("dropSpeed", &dropSpeed_, 0.1f);
	ImGui::DragFloat("kBirdJumpMaxRaito", &kBirdJumpMaxRaito_, 0.1f);

	if(ImGui::Button("ReAdapt")) {
		AdaptAdjustmentItem();
	}


	ImGui::Checkbox("isMove", &isMove_);

	if(hitWaterSurface_) {
		ImGui::Text("Hit");
	} else {
		ImGui::Text("not Hit");
	}
	ImGui::End();
}


#endif // _DEBUG

//////////////////////////////////////////////////////
//  衝突時処理
//////////////////////////////////////////////////////

void Player::OnCollision(Collider* other){

	if(GamePlayTimer::GetIsFinish()){ return; }

	//障害物に当たった場合
	if(other->GetObjectType() == (int)ObjectType::FISH){

		// fish型に変換
		Fish* pFish = dynamic_cast<Fish*>(other);
		bodyCount_;//この数を使う
		// 魚を食べられたとき
		if(bodyCount_ >= pFish->GetEatCount()){//chargePower_ / fishSizeDivision >= (float)pFish->GetFishSize()

			// チャージを行う
			if (isTutorial_) {
				if (followModels_.size() <= 2) {
					chargePower_ += pFish->GetEnergy();
					chargePower_ = std::clamp(chargePower_, 0.0f, 1.0f);
				}
			} else {
				chargePower_ += pFish->GetEnergy();
				chargePower_ = std::clamp(chargePower_, 0.0f, 1.0f);
			}
			// 一時加速する
			temporaryAcceleration_ += increaseVelocity_;
			temporaryAcceleration_ = std::clamp(temporaryAcceleration_, kMinAcceleration_, kMaxAcceleration_);
			//基礎速度の変動
			baseSpeed_ = std::clamp(baseSpeed_ + kAddSpeed_, kMinBaseSpeed_, kMaxBaseSpeed_);
			// ボディーとスピードが最大だったらカットインフラグをオンにする
			if(baseSpeed_ + temporaryAcceleration_ >= 150.0f && bodyCount_ >= kMaxBodyCount_){
				if(!isCutIn_ && !autoFlying_){
					isCutIn_ = true;
					gamePlayTimer_->AddTime(5.0f);
					AudioPlayer::SinglShotPlay("CutIn1.mp3", 0.15f);
					AudioPlayer::SinglShotPlay("CutIn2.mp3", 0.15f);
				}
			}

			AudioPlayer::SinglShotPlay("eat.mp3", 0.15f);
			//AudioPlayer::SinglShotPlay("eatAccel.wav", 0.5f);
			AudioPlayer::SinglShotPlay("AddSpeed.mp3", 0.1f);
			float scale = (float)pFish->GetFishSize() + 2.0f;
			AnimetionEffectManager::AddListEffect("./Game/Resources/Model/EatEffect/", "EatEffect.gltf",
				nullptr, false, Vector3(scale, scale, scale), Quaternion(), transform_->GetTranslation());

			AnimetionEffectManager::AddListEffect("./Game/Resources/Model/AddSpeedEffect/", "AddSpeedEffect.gltf",
				transform_.get(), false, Vector3(1, 1, 1), Quaternion(), Vector3());


		} else{// 食べられなかったとき

			if(!isDiving_){

				// チャージが減る
				//chargePower_ -= 0.05f;
				//chargePower_ = std::clamp(chargePower_, 0.0f, 1.0f);

				float scale = (float)pFish->GetFishSize() + 1.0f;
				AnimetionEffectManager::AddListEffect("./Game/Resources/Model/FishDestroy/", "FishDestroy.gltf",
					nullptr, false, Vector3(scale, scale, scale), Quaternion(), GetWorldTranslation());
				SpeedDown();
				Camera::ShakeStart(cameraShakeTime_, cameraShakeRadius_);
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
					// dropSpeedが早ければ早い程早くなる
					curMaxSpeed = std::abs(dropSpeed_);
					curMaxSpeed = std::clamp(curMaxSpeed, 1.0f, kBirdJumpMaxRaito_ / 2);//通常時の限界は最高速度の半分
					//birdJumpRaito_ = plusSpeed / birdJumpRaito_;
					pressTime_ = 1.0f;
					birdJumpNum_++;
					isFalling_ = false;
					isCloseWing_ = false;

					if(birdJumpNum_ >= 3) {
						maxSpeedTimeCount = kMaxSpeedTime;
						curMaxSpeed = kBirdJumpMaxRaito_;
						birdJumpNum_ = 0;
						isHighSpeedMove = true;
						AudioPlayer::SinglShotPlay("AddSpeed.mp3", 0.5f);
					} else {
						maxSpeedTimeCount = kMaxSpeedTime / 2;
						//birdJumpRaito_ = 1.0f;
					}

					AudioPlayer::SinglShotPlay("BirdJump_3.mp3", 0.4f);
					AnimetionEffectManager::AddListEffect("./Game/Resources/Model/BirdJumpEffect/", "BirdJumpEffect.gltf", transform_.get(), false,
						Vector3(1.0f, 1.0f, 1.0f), Quaternion(), transform_->GetTranslation());
				}
			} else{
				// 正面衝突の場合
				isFacedBird_ = true;
				isCloseWing_ = true;
				SpeedDown();
				// 一時減速する
				temporaryAcceleration_ += decreaseVelocity_ * 2;//多めに減速
				temporaryAcceleration_ = std::clamp(temporaryAcceleration_, kMinAcceleration_, kMaxAcceleration_);
				pressTime_ = birdHitAngle;
				Camera::ShakeStart(cameraShakeTime_, cameraShakeRadius_);
			}

		} else{

			if(collisionAllowableTime_ <= 0.0f){

				if(transform_->GetTranslation().x > other->GetWorldTranslation().x){

					if(!isFacedBird_) {
						// dropSpeedが早ければ早い程早くなる
						curMaxSpeed = std::abs(dropSpeed_);
						curMaxSpeed = std::clamp(curMaxSpeed, 1.0f, kBirdJumpMaxRaito_ / 2);//通常時の限界は最高速度の半分
						//birdJumpRaito_ = plusSpeed / birdJumpRaito_;
						pressTime_ = 1.0f;
						birdJumpNum_++;
						isFalling_ = false;
						isCloseWing_ = false;

						if(birdJumpNum_ >= 3) {
							maxSpeedTimeCount = kMaxSpeedTime;
							curMaxSpeed = kBirdJumpMaxRaito_;
							birdJumpNum_ = 0;
							isHighSpeedMove = true;
							AudioPlayer::SinglShotPlay("AddSpeed.mp3", 0.5f);
						} else {
							maxSpeedTimeCount = kMaxSpeedTime / 2;
							//birdJumpRaito_ = 1.0f;
						}

						AudioPlayer::SinglShotPlay("BirdJump_3.mp3", 0.4f);
						AnimetionEffectManager::AddListEffect("./Game/Resources/Model/BirdJumpEffect/", "BirdJumpEffect.gltf", transform_.get(), false,
							Vector3(1.0f, 1.0f, 1.0f), Quaternion(), transform_->GetTranslation());
					}

				} else{

					// 一時減速する
					temporaryAcceleration_ += decreaseVelocity_ * 2;//多めに減速
					temporaryAcceleration_ = std::clamp(temporaryAcceleration_, kMinAcceleration_, kMaxAcceleration_);
					isFacedBird_ = true;
					isCloseWing_ = true;
					SpeedDown();
					pressTime_ = birdHitAngle;
					Camera::ShakeStart(cameraShakeTime_, cameraShakeRadius_);
				}
			}
		}
	}

	if(!isCutIn_ && !autoFlying_){
		if(other->GetObjectType() == (int)ObjectType::DRIFTWOOD) {
			pressTime_ *= (-1.0f) * reflection_;
			dontInputPressTime_ = dontInputTime_;
			AnimetionEffectManager::AddListEffect("./Game/Resources/Model/DriftWoodDestroy/", "DriftWoodDestroy.gltf", nullptr, true,
				Vector3(1, 1, 1), Quaternion(), other->GetWorldTranslation());
			SpeedDown();
			Camera::ShakeStart(cameraShakeTime_, cameraShakeRadius_);

		} else if(other->GetObjectType() == (int)ObjectType::ROCK) {
			SpeedDown();
			Camera::ShakeStart(cameraShakeTime_, cameraShakeRadius_);
		}
	}
}



// Max時のカットイン演出
void Player::DrawCutIn(){

	if(isCutIn_){

		// 媒介変数計算
		float t = std::clamp((cutInTime_ - kCutInTime_ * 0.8f) / (kCutInTime_ - kCutInTime_ * 0.8f), 0.0f, 1.0f);
		float t2 = (std::clamp(cutInTime_, 0.0f, kCutInTime_ * 0.3f)) / (kCutInTime_ * 0.3f);

		// 時間の更新
		cutInTime_ -= GameTimer::DeltaTime();

		// 拡縮と回転
		float ease = EaseOutBack(((1.0f - t)));
		ease = ease * ease * ease;
		cutInSprite_[0]->SetScale({ 1.0f,ease });
		cutInSprite_[0]->SetRotate(-6.28f + 6.28f * EaseOutExpo(((1.0f - t))));
		cutInSprite_[1]->SetColor({ 0.0f,0.0f,0.0f,ease * 0.9f });
		if(t2 < 1.0f){
			float ease2 = EaseOutExpo(1.0f - t2);
			cutInSprite_[0]->SetScale({ 1.0f,1.0f - ease2 });
			cutInSprite_[1]->SetColor({ 0.0f,0.0f,0.0f,(1.0f - ease2) * 0.9f });
		}

		// 描画
		cutInSprite_[1]->Draw();
		cutInSprite_[0]->Draw();


		// カットイン終了時処理
		if(cutInTime_ <= 0.0f){
			isCutIn_ = false;
			cutInTime_ = kCutInTime_;
		}

		if(cutInTime_ < kCutInTime_ * 0.2f){
			autoFlying_ = true;
		}
	}
}



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
	AudioPlayer::SinglShotPlay("confusion.mp3", 0.3f);
	AudioPlayer::SinglShotPlay("Damage.mp3", 2.5f);

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
				if(!autoFlying_){
					pressTime_ -= addPressTime_ * GameTimer::TimeRate();
				} else{
					pressTime_ += addPressTime_ * GameTimer::TimeRate();
				}
			}
			seCoolTime -= GameTimer::DeltaTime();
			if(seCoolTime <= 0 && Input::IsTriggerKey(DIK_SPACE)) {
				AudioPlayer::SinglShotPlay("MoveChangeUp.mp3", 0.1f);
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

	birdJumpRaito_ = 1.0f;
	maxSpeedTimeCount = 0;
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
		seCoolTime -= GameTimer::DeltaTime();


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
		if(wingOpen != isCloseWing_ && !isCloseWing_) {
			AudioPlayer::SinglShotPlay("WingOpen.mp3", 0.14f);
			seCoolTime = 0.25f;
		}
		wingOpen = isCloseWing_;

		////////////////////////////////////////////////////////////////////////////////

		// 下降ベクトルを格納する変数
		//Vector3 dropVec{};
		dropVec = { 0,0,0 };
		if(!isCloseWing_){//////// 翼を広げている際 ////////

			// 下降ベクトル
			dropSpeed_ = 0.0f;
			//dropSpeed_ += gravity_* descentRatio * GameTimer::DeltaTime();

			if(wingAnimatinoKeyFrame_ < 1.0f) {
				wingAnimatinoKeyFrame_ += GameTimer::DeltaTime() * 4;
			}

		} else{//////// 翼を閉じている際 ////////

			pressTime_ *= 0.3f * GameTimer::TimeRate();
			dropSpeed_ += gravity_ * GameTimer::DeltaTime();
			dropVec = Vector3(0.0f, dropSpeed_, 0.0f) * GameTimer::TimeRate();

			if(wingAnimatinoKeyFrame_ > 0.0f) {
				wingAnimatinoKeyFrame_ -= GameTimer::DeltaTime() * 2;
			}

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
			birdJumpNum_ = 0;//鳥踏んだカウントをリセット
			isHighSpeedMove = false;//ハイスピードの状態か否か
		}
	}

	animetor_->SetColor(birdJumpNum_);

	timer_.Measurement(transform_->GetTranslation().y);

}



void Player::UpdateTransform(){
	// 角度を加算
	pressTime_ = std::clamp(pressTime_, -1.0f, 1.0f);
	currentAngle_ = kMaxAngle_ * pressTime_;
	LookAtDirection(currentAngle_);

	// 移動量を加算
	velocity_ = Vector3(1.0f, 0.0f, 0.0f) * MakeRotateZMatrix(currentAngle_);
	// 鳥でのジャンプの加速を考慮する(birdJumpRaito_は基本1.0f)
	velocity_ *= GetMoveSpeed() * birdJumpRaito_ * std::fabsf(GameTimer::DeltaTime());
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
