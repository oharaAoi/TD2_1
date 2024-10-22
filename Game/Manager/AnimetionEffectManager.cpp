#include "AnimetionEffectManager.h"

AnimetionEffectManager::~AnimetionEffectManager() {}

AnimetionEffectManager* AnimetionEffectManager::GetInstance() {
	static AnimetionEffectManager instance;
	return &instance;
}

void AnimetionEffectManager::Finalize() {
	effectList_.clear();
}


void AnimetionEffectManager::Init() {
}

void AnimetionEffectManager::Update() {
	for (std::list<EffectData>::iterator it = effectList_.begin(); it != effectList_.end();) {
		if ((*it).effect->GetIsAnimationFinish()) {
			it = effectList_.erase(it);
			continue;
		}
		
		if ((*it).pTranslation != nullptr) {
			(*it).effect->GetTransform()->SetTranslaion(*(*it).pTranslation);
		}

		if ((*it).pQuaternion != nullptr) {
			(*it).effect->GetTransform()->SetQuaternion(*(*it).pQuaternion);
		}

		(*it).effect->Update();
		++it;
	}
}

void AnimetionEffectManager::Draw() const {
	for (std::list<EffectData>::const_iterator it = effectList_.begin(); it != effectList_.end();) {
		(*it).effect->Draw();
		++it;
	}
}

void AnimetionEffectManager::AddListEffect(const std::string& directoryPath, const std::string& filePath, 
										   const WorldTransform* parentWorldTransform, bool isParent, bool isLighting) {

	auto& newObj = GetInstance()->effectList_.emplace_back();
	newObj.effect = std::make_unique<BaseGameObject>();
	newObj.effect->Init();
	newObj.effect->SetObject(filePath);
	newObj.effect->SetAnimater(directoryPath, filePath, true);
	newObj.effect->SetIsLighting(isLighting);
	if (isParent) {
		newObj.pTranslation =  &parentWorldTransform->GetTranslation();
		newObj.pQuaternion = &parentWorldTransform->GetQuaternion();
	} else {
		newObj.pTranslation = nullptr;
		newObj.pQuaternion = nullptr;
		newObj.effect->GetTransform()->SetTranslaion(parentWorldTransform->GetTranslation());
		newObj.effect->GetTransform()->SetQuaternion(parentWorldTransform->GetQuaternion() * newObj.effect->GetTransform()->GetQuaternion());
	}
}

