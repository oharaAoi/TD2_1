#include "GuideUI.h"

GuideUI::GuideUI() {}

GuideUI::~GuideUI() {
}

void GuideUI::Init() {
	uiMap_.emplace("push_space", Engine::CreateSprite("kari.png"));
	uiMap_.emplace("game_start", Engine::CreateSprite("kari.png"));
	uiMap_.emplace("tutorial_start", Engine::CreateSprite("kari.png"));

	uiMap_.emplace("go_title", Engine::CreateSprite("kari.png"));
}

void GuideUI::Update() {
	for(auto& ui : )
}

void GuideUI::Draw() const {
}
