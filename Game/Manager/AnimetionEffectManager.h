#pragma once
#include <list>
#include <vector>
#include "Engine/GameObject/BaseGameObject.h"

class AnimetionEffectManager {
public:

	AnimetionEffectManager();
	~AnimetionEffectManager();

	void Init();
	void Update();
	void Draw() const;

private:

	std::list<std::unique_ptr<BaseGameObject>> effectList_;

};

