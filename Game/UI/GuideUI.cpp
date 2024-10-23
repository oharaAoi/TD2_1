#include "GuideUI.h"

GuideUI::GuideUI() {}

GuideUI::~GuideUI() {
}

void GuideUI::Init() {
	uiMap_.emplace("push_space", Engine::CreateSprite("kari.png"));
	uiMap_.emplace("game_start", Engine::CreateSprite("kari.png"));
	uiMap_.emplace("tutorial_start", Engine::CreateSprite("kari.png"));

	uiMap_.emplace("go_title", Engine::CreateSprite("kari.png"));

	adjustmentItem_ = AdjustmentItem::GetInstance();
	groupName_ = "GuideUI";

	adjustmentItem_->AddItem(groupName_, "title_pushSpace", uiMap_["push_space"]->GetCenterPos());
	adjustmentItem_->AddItem(groupName_, "title_gameStart", uiMap_["game_start"]->GetCenterPos());
	adjustmentItem_->AddItem(groupName_, "title_tutorialStart", uiMap_["tutorial_start"]->GetCenterPos());
	adjustmentItem_->AddItem(groupName_, "Result_pushSpace", uiMap_["push_space"]->GetCenterPos());
	adjustmentItem_->AddItem(groupName_, "Result_goTitle", uiMap_["go_title"]->GetCenterPos());

}

void GuideUI::Update() {
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
	
	uiMap_["push_space"]->SetTextureCenterPos(adjustmentItem_->GetValue<Vector2>(groupName_, "title_pushSpace"));
	uiMap_["game_start"]->SetTextureCenterPos(adjustmentItem_->GetValue<Vector2>(groupName_, "title_gameStart"));
	uiMap_["tutorial_start"]->SetTextureCenterPos(adjustmentItem_->GetValue<Vector2>(groupName_, "title_tutorialStart"));

	drawSpriteList_.push_back(uiMap_["push_space"].get());
	drawSpriteList_.push_back(uiMap_["game_start"].get());
	drawSpriteList_.push_back(uiMap_["tutorial_start"].get());
}

void GuideUI::SetResult() {
	drawSpriteList_.clear();
	
	uiMap_["push_space"]->SetTextureCenterPos(adjustmentItem_->GetValue<Vector2>(groupName_, "title_pushSpace"));
	uiMap_["go_title"]->SetTextureCenterPos(adjustmentItem_->GetValue<Vector2>(groupName_, "go_title"));

	drawSpriteList_.push_back(uiMap_["push_space"].get());
	drawSpriteList_.push_back(uiMap_["go_title"].get());
}

void GuideUI::SetUIPos(const std::string& addName, const Vector2& pos) {
	uiMap_[addName]->SetCenterPos(pos);
}


#ifdef _DEBUG
void GuideUI::Debug_Gui() {
	if (ImGui::TreeNode("GuideUI")) {
		for (auto& ui : uiMap_) {
			Vector2 pos = ui.second->GetCenterPos();
			std::string name = ui.first;
			ImGui::DragFloat2(name.c_str(), &pos.x, 1.0f);
			ui.second->SetCenterPos(pos);
		}
		ImGui::TreePop();
	}
}
#endif // _DEBUG