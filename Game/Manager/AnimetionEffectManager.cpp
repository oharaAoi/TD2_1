#include "AnimetionEffectManager.h"

AnimetionEffectManager::AnimetionEffectManager() {
}

AnimetionEffectManager::~AnimetionEffectManager() {
}

void AnimetionEffectManager::Init() {
}

void AnimetionEffectManager::Update() {
	for (std::list<std::unique_ptr<BaseGameObject>>::iterator it = effectList_.begin(); it != effectList_.end();) {

	}
}

void AnimetionEffectManager::Draw() const {
}
