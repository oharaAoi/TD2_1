#include "Mountain.h"

Mountain::Mountain() {
}

Mountain::~Mountain() {
}

void Mountain::Init() {
	BaseGameObject::Init();
	SetObject("Mountain.obj");
	SetIsLighting(false);
	offset_ = { 0.0f, -11.8f, 20.0f };
	transform_->SetTranslaion(offset_);
}

void Mountain::Update() {
	BaseGameObject::Update();
}

void Mountain::Draw() const {
	BaseGameObject::Draw();
}
