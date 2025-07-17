#include "GuideUI.h"
#include "Game/Information/PlayConfig.h"

GuideUI::GuideUI() {}

GuideUI::~GuideUI() {
}

void GuideUI::Init() {
	// 編集項目を設定
	adjustmentItem_ = AdjustmentItem::GetInstance();
	groupName_ = "GuideUI";

	// 共通
	uiMap_.emplace("push_space", Engine::CreateSprite("UI_space.png"));
	uiMap_.emplace("game_start", Engine::CreateSprite("UI_game.png"));
	uiMap_.emplace("tutorial_start", Engine::CreateSprite("UI_tutorial.png"));
	// Titile
	uiMap_.emplace("go_title", Engine::CreateSprite("UI_Titlle.png"));
	// resutl
	uiMap_.emplace("arrow", Engine::CreateSprite("UI_arrow.png"));

	// ENver
	uiMap_EN_.emplace("push_space", Engine::CreateSprite("UI_space.png"));
	uiMap_EN_.emplace("game_start", Engine::CreateSprite("UI_game_EN.png"));
	uiMap_EN_.emplace("tutorial_start", Engine::CreateSprite("UI_tutorial_EN.png"));
	// Titile
	uiMap_EN_.emplace("go_title", Engine::CreateSprite("UI_Titlle_EN.png"));
	// resutl
	uiMap_EN_.emplace("arrow", Engine::CreateSprite("UI_arrow.png"));

	adjustmentItem_->AddItem(groupName_, "title_pushSpace", uiMap_["push_space"]->GetCenterPos());
	adjustmentItem_->AddItem(groupName_, "title_gameStart", uiMap_["game_start"]->GetCenterPos());
	adjustmentItem_->AddItem(groupName_, "title_tutorialStart", uiMap_["tutorial_start"]->GetCenterPos());
	adjustmentItem_->AddItem(groupName_, "Result_pushSpace", uiMap_["push_space"]->GetCenterPos());
	adjustmentItem_->AddItem(groupName_, "Result_goTitle", uiMap_["go_title"]->GetCenterPos());
	adjustmentItem_->AddItem(groupName_, "Title_Arrow", uiMap_["arrow"]->GetCenterPos());

	scale_ = { 0.5f, 0.5f };
}

void GuideUI::Update() {
	auto& map = (PlayConfig::language == LanguageSetting::LANGUAGE_JP) ? uiMap_ : uiMap_EN_;
	if (PlayConfig::inputMode == InputMode::INPUTTYPE_KEYBOARD) {
		map["push_space"]->SetTexture("SpaceButton.png");
	} else {
		map["push_space"]->SetTexture("Abutton.png");
	}

	for (auto& ui : drawSpriteList_) {
		ui->Update();
	}
}

void GuideUI::Draw() const {
	for (auto& ui : drawSpriteList_) {
		ui->Draw();
	}
}

void GuideUI::AddDrawList(const std::string& addName) {
	drawSpriteList_.push_back(uiMap_[addName].get());
}

void GuideUI::ClearDrawList() {
	uiMap_.clear();
}

void GuideUI::SetTitle() {
	drawSpriteList_.clear();

	auto& map = (PlayConfig::language == LanguageSetting::LANGUAGE_JP) ? uiMap_ : uiMap_EN_;
	
	map["push_space"]->SetTextureCenterPos(adjustmentItem_->GetValue<Vector2>(groupName_, "title_pushSpace"));
	map["game_start"]->SetTextureCenterPos(adjustmentItem_->GetValue<Vector2>(groupName_, "title_gameStart"));
	map["tutorial_start"]->SetTextureCenterPos(adjustmentItem_->GetValue<Vector2>(groupName_, "title_tutorialStart"));

	map["push_space"]->SetScale(scale_);
	map["game_start"]->SetScale(scale_);
	map["tutorial_start"]->SetScale(scale_);
	map["arrow"]->SetScale(scale_);

	drawSpriteList_.push_back(map["push_space"].get());
	drawSpriteList_.push_back(map["game_start"].get());
	drawSpriteList_.push_back(map["tutorial_start"].get());
	drawSpriteList_.push_back(map["arrow"].get());
}

void GuideUI::SetResult() {
	drawSpriteList_.clear();
	auto& map = (PlayConfig::language == LanguageSetting::LANGUAGE_JP) ? uiMap_ : uiMap_EN_;
	
	map["push_space"]->SetTextureCenterPos(adjustmentItem_->GetValue<Vector2>(groupName_, "Result_pushSpace"));
	map["push_space"]->SetScale(scale_);

	drawSpriteList_.push_back(map["push_space"].get());
}

void GuideUI::SetScore() {
	auto& map = (PlayConfig::language == LanguageSetting::LANGUAGE_JP) ? uiMap_ : uiMap_EN_;
	map["go_title"]->SetTextureCenterPos(adjustmentItem_->GetValue<Vector2>(groupName_, "Result_goTitle"));
	map["go_title"]->SetScale(scale_); map["go_title"]->SetScale(scale_);
	drawSpriteList_.push_back(map["go_title"].get());
}

void GuideUI::SetArrow(bool isGame) {
	Vector2 pos;
	auto& map = (PlayConfig::language == LanguageSetting::LANGUAGE_JP) ? uiMap_ : uiMap_EN_;
	if (isGame) {
		Vector2 pos = adjustmentItem_->GetValue<Vector2>(groupName_, "Title_Arrow");
		pos.y = map["game_start"]->GetCenterPos().y;
		map["arrow"]->SetCenterPos(adjustmentItem_->GetValue<Vector2>(groupName_, "Title_Arrow"));
	} else {
		Vector2 pos = adjustmentItem_->GetValue<Vector2>(groupName_, "Title_Arrow");
		pos.y = map["tutorial_start"]->GetCenterPos().y;
		map["arrow"]->SetCenterPos(pos);
	}
}

void GuideUI::SetUIPos(const std::string& addName, const Vector2& pos) {
	auto& map = (PlayConfig::language == LanguageSetting::LANGUAGE_JP) ? uiMap_ : uiMap_EN_;
	map[addName]->SetCenterPos(pos);
}

#ifdef _DEBUG
void GuideUI::Debug_Gui() {
	auto& map = (PlayConfig::language == LanguageSetting::LANGUAGE_JP) ? uiMap_ : uiMap_EN_;
	if (ImGui::TreeNode("GuideUI")) {
		for (auto& ui : map) {
			Vector2 pos = ui.second->GetCenterPos();
			std::string name = ui.first;
			ImGui::DragFloat2(name.c_str(), &pos.x, 1.0f);
			ui.second->SetCenterPos(pos);
		}

		ImGui::DragFloat2("scale", &scale_.x, 0.1f);
		ImGui::TreePop();
	}
}
#endif // _DEBUG