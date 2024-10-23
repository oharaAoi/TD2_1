#include "FlyingGaugeUI.h"

FlyingGaugeUI::FlyingGaugeUI(){}

FlyingGaugeUI::~FlyingGaugeUI(){}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void FlyingGaugeUI::Init(){

	icon_ = Engine::CreateSprite("RankIcon.png");
	icon_->SetScale({ 0.7f,0.7f });

	icon_maxHeight_ = Engine::CreateSprite("RankIcon_max.png");
	icon_maxHeight_->SetScale({ 0.7f,0.7f });

	tower_ = Engine::CreateSprite("tower.png");
	tower_->SetCenterPos(skytreeBasePos_);


	adjustmentItem_ = AdjustmentItem::GetInstance();
	groupName_ = "FlyingGaugeUI";

	// 登録

	// 適応
	AdaptAdjustmentItem();

	rankLenghtMin_ = 0.0f;
	rankLenghtMax_ = 100.0f;

	maxLenghtRaito_ = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void FlyingGaugeUI::Update(float currentLength){
	CalculationRaito(currentLength);

	// スカイツリーの高さを更新する
	height_t = std::clamp((currentLength - halfWindowHeight_) / skytreeHeight_, 0.0f, 1000.0f);
	halfHeight_t = std::clamp(currentLength / halfWindowHeight_, 0.0f, 1.0f);

	ImGui::Begin("t");
	ImGui::Text("max %f", maxHeight_t);
	ImGui::Text("cur %f", height_t);
	ImGui::End();

	tower_->SetCenterPos(skytreeBasePos_ + Vector2(0.0f, tower_->GetTextureSize().y * height_t));

	icon_->SetCenterPos(iconBasePos_ + Vector2(0.0f, -kWindowHeight_ * 0.5f * halfHeight_t));

	// 最高高度更新
	if(maxHeight_t <= height_t){
		maxHeight_t = height_t;
		icon_maxHeight_->SetCenterPos(icon_->GetCenterPos());
	} else{
		icon_maxHeight_->SetCenterPos(icon_->GetCenterPos() + Vector2(0.0f, -tower_->GetTextureSize().y * (1.0f - height_t)));
	}



	tower_->Update();
	icon_->Update();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void FlyingGaugeUI::Draw(float alpha){
	tower_->SetColor({ 1.0f,1.0f,1.0f,alpha });
	tower_->Draw();
	icon_maxHeight_->SetColor({ 1.0f,1.0f,1.0f,alpha });
	icon_maxHeight_->Draw();
	icon_->SetColor({ 1.0f,1.0f,1.0f,alpha });
	icon_->Draw();

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　現在の割合を出す
//////////////////////////////////////////////////////////////////////////////////////////////////

void FlyingGaugeUI::CalculationRaito(float currentLength){
	lengthRaito_ = currentLength / rankLenghtMax_;

	lengthRaito_ = std::clamp(lengthRaito_, 0.0f, 1.0f);

	//Vector2 st = outside_->GetCenterPos() - (outside_->GetTextureSize() * 0.5f);
	//Vector2 end = outside_->GetCenterPos() + (outside_->GetTextureSize() * 0.5f);
	//st.y = outside_->GetCenterPos().y - 30.0f;
	//end.y = outside_->GetCenterPos().y - 30.0f;
	//Vector2 pos = Vector2::Lerp(st, end, lengthRaito_);
	//icon_->SetCenterPos(pos);

	//if(nowMaxRank_ != FlyingRank::Rank_Master) {
	//	if(maxLenghtRaito_ < lengthRaito_) {
	//		bar_->SetUvDrawRange({ (1.0f * lengthRaito_) ,1.0f });
	//		maxLenghtRaito_ = lengthRaito_;
	//	}

	//	if(lengthRaito_ >= 1.0f) {
	//		RankUp();
	//	}
	//}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　ランクの昇格を行う
//////////////////////////////////////////////////////////////////////////////////////////////////

void FlyingGaugeUI::RankUp(){
	maxLenghtRaito_ = 0.0f;
	/*switch(nowMaxRank_) {
	case FlyingRank::Rank_First:
		nowMaxRank_ = FlyingRank::Rank_B;
		rankLenghtMin_ = 101.0f;
		rankLenghtMax_ = 200.0f;
		rank_->SetLeftTop({ 0.0f, 0.0f });
		break;
	case FlyingRank::Rank_B:
		nowMaxRank_ = FlyingRank::Rank_A;
		rankLenghtMin_ = 501.0f;
		rankLenghtMax_ = 1000.0f;
		rank_->SetLeftTop({ 48.0f, 0.0f });
		break;
	case FlyingRank::Rank_A:
		nowMaxRank_ = FlyingRank::Rank_S;
		rankLenghtMin_ = 0.0f;
		rankLenghtMax_ = 1500.0f;
		rank_->SetLeftTop({ 96.0f, 0.0f });
		break;
	case FlyingRank::Rank_S:
		nowMaxRank_ = FlyingRank::Rank_Master;
		rank_->SetTexture("kari_UI_Rank_master.png");
		break;
	case FlyingRank::Rank_Master:

		break;
	default:
		break;
	}*/
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　調整項目の適応
//////////////////////////////////////////////////////////////////////////////////////////////////

void FlyingGaugeUI::AdaptAdjustmentItem(){

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Debug表示
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void FlyingGaugeUI::Debug_Gui(){

	if(ImGui::TreeNode("FlyingTGaugeUI")) {

		// 再適応
		if(ImGui::Button("ReAdapt")) {
			AdaptAdjustmentItem();
		}

		ImGui::TreePop();
	}
}
#endif // _DEBUG
