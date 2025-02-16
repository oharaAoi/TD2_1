#include "TutorialUI.h"

TutorialUI::TutorialUI(){
}

TutorialUI::~TutorialUI(){
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialUI::Init(){

	// このスケールで全体のUIのスケールを統一する
	scaleUpStrength_ = 1.2f;
	scaleUp_ = { scaleUpStrength_, scaleUpStrength_, scaleUpStrength_ };

	// この位置を調整することで鳥を踏むUIを移動できる
	jumpTutorialOffsetPos_ = { 40.0f, 85.0f, 0.0f };

	offsetLnegth_ = 300.0f;	// この距離にするといい感じの距離になる
	adjust_ = AdjustmentItem::GetInstance();
	groupName_ = "TutorialUI";
	adjust_->AddItem(groupName_, "offsetPos", offsetPos_);
	adjust_->AddItem(groupName_, "interval", interval_);
	adjust_->AddItem(groupName_, "offsetLnegth", offsetLnegth_);

	AdaptAdjustment();

	tutorialUI_.clear();

	tutorialUI_.try_emplace("kari1", std::make_unique<BaseGameObject>());
	tutorialUI_["kari1"]->Init();
	tutorialUI_["kari1"]->SetObject("UI_Plane.obj");
	tutorialUI_["kari1"]->SetTexture("Tutorial_1.png");
	tutorialUI_["kari1"]->SetIsLighting(false);

	tutorialUI_.try_emplace("kari2", std::make_unique<BaseGameObject>());
	tutorialUI_["kari2"]->Init();
	tutorialUI_["kari2"]->SetObject("UI_Plane.obj");
	tutorialUI_["kari2"]->SetTexture("Tutorial_2.png");
	tutorialUI_["kari2"]->SetIsLighting(false);

	tutorialUI_.try_emplace("kari3", std::make_unique<BaseGameObject>());
	tutorialUI_["kari3"]->Init();
	tutorialUI_["kari3"]->SetObject("UI_Plane.obj");
	tutorialUI_["kari3"]->SetTexture("Tutorial_4.png");
	tutorialUI_["kari3"]->SetIsLighting(false);

	tutorialUI_.try_emplace("kari4", std::make_unique<BaseGameObject>());
	tutorialUI_["kari4"]->Init();
	tutorialUI_["kari4"]->SetObject("TutorialGuide.obj");
	//tutorialUI_["kari4"]->SetTexture("sky.png");
	tutorialUI_["kari4"]->SetIsLighting(false);

	tutorialUI_.try_emplace("start", std::make_unique<BaseGameObject>());
	tutorialUI_["start"]->Init();
	tutorialUI_["start"]->SetObject("UI_Plane.obj");
	//tutorialUI_["start"]->SetTexture("UI_none.png");
	tutorialUI_["start"]->SetIsLighting(false);
	tutorialUI_["start"]->SetColor({ 0.0, 0.0,0.0,0.0f });

	// これがjumpUIのオブジェクト
	jumpTutorialUI_ = std::make_unique<BaseGameObject>();
	jumpTutorialUI_->Init();
	jumpTutorialUI_->SetObject("UI_Plane.obj");
	jumpTutorialUI_->SetIsLighting(false);
	jumpTutorialUI_->SetTexture("Tutorial_3.png");
	jumpTutorialUI_->GetTransform()->SetQuaternion(Quaternion::AngleAxis(180.0f * toRadian, { 0.0f, 1.0f, 0.0f }));

	jumpTutorialPos_ = tutorialUI_["kari4"]->GetTransform()->GetTranslation();

	float index = 0;
	for(auto& ui : tutorialUI_){
		Vector3 pos = offsetPos_;
		pos.x += (interval_ * index) + offsetLnegth_;
		ui.second->GetTransform()->SetTranslaion(pos);
		ui.second->GetTransform()->SetQuaternion(Quaternion::AngleAxis(180.0f * toRadian, { 0.0f, 1.0f, 0.0f }));

		index++;
	}
	tutorialUI_["kari3"]->GetTransform()->SetTranslaion(tutorialUI_["kari3"]->GetTransform()->GetTranslation() + Vector3(100, 0, 0));


	/*=================================*/
	// テキストの初期化
	/*=================================*/
	tutorialText_ = Engine::CreateSprite("tutorialText.png");
	tutorialText_->SetCenterPos({ 640.0f, 555.0f });
	tutorialText_->SetAnchorPoint({ 0.5f, 0.5f });
	tutorialText_->SetRectRange({ 1280.0f, 60.0f });
	tutorialText_->SetTextureSize({ 1280.0f, 60.0f });
	tutorialText_->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });

	textBackSprite_ = Engine::CreateSprite("tutorialBackSprite.png");
	textBackSprite_->SetCenterPos({ 640.0f, 360.0f });
	textBackSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	textBackSprite_->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });

	spaceSprite_ = Engine::CreateSprite("SpaceButton.png");
	spaceSprite_->SetCenterPos({ 1000.0f, 630.0f });
	spaceSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	spaceSprite_->SetScale({ 0.5f, 0.5f });
	spaceSprite_->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });

	tutorialText_->Update();
	textBackSprite_->Update();

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialUI::Update(){

	scaleUp_ = { scaleUpStrength_, scaleUpStrength_, scaleUpStrength_ };

	jumpTutorialPos_ = tutorialUI_["kari4"]->GetTransform()->GetTranslation() + jumpTutorialOffsetPos_;
	jumpTutorialUI_->GetTransform()->SetTranslaion(jumpTutorialPos_);
	jumpTutorialUI_->GetTransform()->SetScale(scaleUp_);
	jumpTutorialUI_->Update();

	for(auto& ui : tutorialUI_){
		ui.second->GetTransform()->SetScale(scaleUp_);
		ui.second->Update();
	}

	tutorialText_->Update();
	spaceSprite_->Update();

	// テキストの更新
	UpdateTutorialText();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialUI::Draw() const{
	jumpTutorialUI_->Draw();
	for(const auto& ui : tutorialUI_){
		ui.second->Draw();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　編集
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialUI::LineUpUI(const Vector3& playerPos){
	// この処理はプレイヤーが
	if(!isLineUp_){
		float index = 0;
		for(auto& ui : tutorialUI_){
			Vector3 pos = offsetPos_;
			pos.x += (interval_ * index) + offsetLnegth_ + playerPos.x;
			ui.second->GetTransform()->SetTranslaion(pos);
			ui.second->GetTransform()->SetQuaternion(Quaternion::AngleAxis(180.0f * toRadian, { 0.0f, 1.0f, 0.0f }));

			index++;
		}

		isLineUp_ = true;
		tutorialUI_["kari3"]->GetTransform()->SetTranslaion(tutorialUI_["kari3"]->GetTransform()->GetTranslation() + Vector3(100, 0, 0));

	} else{
		return;
	}
	tutorialUI_["start"]->GetTransform()->SetQuaternion(Quaternion::AngleAxis((-180.0f * toRadian), Vector3{ 0,1,0 }));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　調整項目の適応
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialUI::AdaptAdjustment(){
	offsetPos_ = adjust_->GetValue<Vector3>(groupName_, "offsetPos");
	interval_ = adjust_->GetValue<float>(groupName_, "interval");
	offsetLnegth_ = adjust_->GetValue<float>(groupName_, "offsetLnegth");
}

Vector3 TutorialUI::GetSessionFishPos(){
	return tutorialUI_["kari3"]->GetTransform()->GetTranslation();
}

Vector3 TutorialUI::GetSessionBirdPos(){
	return tutorialUI_["kari4"]->GetTransform()->GetTranslation();
}

Vector3 TutorialUI::GetStartPos(){
	return tutorialUI_["start"]->GetTransform()->GetTranslation();
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　看板に近づいたら一時停止して説明を表示する
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialUI::UpdateTutorialText(){

	static float sensingDistance = 75.0f;
	static float distanceToUI[tutorialCount_] = { 0.0f, 0.0f, 0.0f };
	static const int32_t kTextPage[tutorialCount_] = { 1,4,3 };// テキストのページ数
	static const int32_t textOffset[tutorialCount_] = { 0,1,5 };// テキストのオフセット
	static int32_t textPage[tutorialCount_] = { 0,0,0 };// 現在のページ
	static int32_t currentTutorialIndex = -1;// 現在のチュートリアルのインデックス
	static bool isReacedEndPage = false;// ページが最後まで行ったかどうか
	static const float textAppearingTime = 0.5f;// テキストが表示されるまでの時間(フェード時間)
	static float textTimer = 0.0f;// テキスト表示用のタイマー
	static float tutorialTimer = 0.0f;// チュートリアル全体用のタイマー
	static const float kWatingTime = 0.2f;// テキストが表示されるまでの待ち時間
	static float waitTime = kWatingTime;// テキストが表示されるまでの待ち時間

	// 各看板との距離を計算して、一定の距離に近づいたらテキストを表示する
	for(int i = 0; i < 3; i++){

		distanceToUI[i] = (tutorialUI_["kari" + std::to_string(i + 1)]->GetTransform()->GetTranslation() - playerPos_).Length();
		if(distanceToUI[i] < sensingDistance){

			// 一度表示したらもう表示しない
			if(!isShownText_[i]){
				isTextShowing_ = true;
				isShownText_[i] = true;
				textPage[i] = 0;
				currentTutorialIndex = i;
				textTimer = 0.0f;
				tutorialText_->SetLeftTop({ 0.0f, 60.0f * (textOffset[currentTutorialIndex] + textPage[currentTutorialIndex]) });
			}
		}
	}


	// テキストの表示
	if(isTextShowing_){

		if(waitTime > 0.0f){
			waitTime -= GameTimer::DeltaTime();// 止まってから1秒待ってからテキストを表示する
			return;
		}

		// テキスト用のタイマーを更新
		if(!isReacedEndPage){
			textTimer += GameTimer::DeltaTime();// ページが残っている場合はタイマーを増やす
			tutorialTimer += GameTimer::DeltaTime();
		} else{
			textTimer -= GameTimer::DeltaTime();// ページが最後まで行ったらタイマーを減らす(薄くして消していくため)
			tutorialTimer -= GameTimer::DeltaTime();

			// 時間が0になったらチュートリアルテキストの終了
			if(tutorialTimer <= 0.0f){
				isTextShowing_ = false;
				isReacedEndPage = false;
				tutorialTimer = 0.0f;
				waitTime = kWatingTime;
			}
		}

		// アルファ値などに使用する媒介変数
		textTimer = std::clamp(textTimer, 0.0f, textAppearingTime);
		tutorialTimer = std::clamp(tutorialTimer, 0.0f, textAppearingTime);
		float t = textTimer / textAppearingTime;
		float t2 = tutorialTimer / textAppearingTime;

		// spaceのUIは0.5秒ごとに暗い、明るいを繰り返す
		float alpha;
		bool isFlashing = std::fmod(GameTimer::TotalTime(), 1.0f) < 0.5f;
		if(isFlashing){
			alpha = 0.5f;
			spaceSprite_->SetScale(Vector2(0.5f,0.5f) * 0.9f);
		} else{
			alpha = 1.0f;
			spaceSprite_->SetScale(Vector2(0.5f, 0.5f));
		}

		// テキストの表示
		tutorialText_->SetColor({ 1.0f, 1.0f, 1.0f, t });
		textBackSprite_->SetColor({ 1.0f, 1.0f, 1.0f, t2 });
		spaceSprite_->SetColor({ 1.0f, 1.0f, 1.0f, t2 * alpha });



		// ページの更新
		if(Input::IsTriggerKey(DIK_SPACE)){
			textPage[currentTutorialIndex]++;

			// ページが最後まで行ったか確認
			if(textPage[currentTutorialIndex] >= kTextPage[currentTutorialIndex]){
				isReacedEndPage = true;
			} else{
				textTimer = 0.0f;// まだページが残っているのでタイマーをリセット
				// テキストkの切り抜き範囲の更新
				tutorialText_->SetLeftTop({ 0.0f, 60.0f * (textOffset[currentTutorialIndex] + textPage[currentTutorialIndex]) });
			}
		}
	}
}

void TutorialUI::DrawTutorialText(){
	if(isTextShowing_){
		textBackSprite_->Draw();
		tutorialText_->Draw();
		spaceSprite_->Draw();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　編集
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void TutorialUI::Debug_Gui(){
	if(ImGui::TreeNode("TutorialUI")){
		ImGui::DragFloat3("jumpTutorialOffsetPos", &jumpTutorialOffsetPos_.x, 0.1f);
		ImGui::DragFloat3("offsetPos", &offsetPos_.x, 0.1f);
		ImGui::DragFloat("interval", &interval_, 1.0f);
		ImGui::DragFloat("scaleUpStrength", &scaleUpStrength_, 1.0f);

		jumpTutorialUI_->Debug_Gui();
		/*for (auto& ui : tutorialUI_) {
			Vector3 pos = offsetPos_;
			pos.x += (interval_ * index) + offsetLnegth_;
			ui.second->GetTransform()->SetTranslaion(pos);
			ui.second->GetTransform()->SetQuaternion(Quaternion::AngleAxis(180.0f * toRadian, { 0.0f, 1.0f, 0.0f }));

			index++;
		}*/
		ImGui::TreePop();
	}
}


#endif