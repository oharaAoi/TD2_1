#include "Mountain.h"

Mountain::Mountain() {
}

Mountain::~Mountain() {
}

void Mountain::Init() {
	BaseGameObject::Init();
	SetObject("Mountain.obj");
	SetIsLighting(false);
	transform_->SetTranslaion(offset_);
}

void Mountain::Update() {
	transform_->SetTranslationY(0.6f);
	transform_->SetTranslationZ(-2.6f);
	BaseGameObject::Update();
}

void Mountain::Draw() const {
	BaseGameObject::Draw();
}
