#include "WorldWall.h"

WorldWall::WorldWall() {
}

WorldWall::~WorldWall() {
}

void WorldWall::Init() {
	BaseGameObject::Init();
	SetObject("WorldWall.obj");
	transform_->SetScale({ 1.0f, 1.0f, 1.0f });
	transform_->SetTranslaion({ 0.0f, -21.0f,31.0f });
}

void WorldWall::Update() {
	BaseGameObject::Update();
}

void WorldWall::Draw() const {
	BaseGameObject::Draw();
}