#pragma once
#include "Engine/GameObject/BaseGameObject.h"

class WorldWall
	: public BaseGameObject {
public:

	WorldWall();
	~WorldWall();

	void Init() override;
	void Update() override;
	void Draw() const override;

private:



};

