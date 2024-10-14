#pragma once
#include <map>
#include "Engine.h"
#include "Engine/Assets/Skeleton.h"
#include "Engine/Assets/Skinning.h"

class PlayerAnimator {
public:

	PlayerAnimator();
	~PlayerAnimator();

	void Init();
	void Update();
	void LoadAnimation(Model* model);

	Skinning* GetSkinning() { return skinning_.get(); }

private:

	std::unique_ptr<Skeleton> skeleton_ = nullptr;
	std::unique_ptr<Skinning> skinning_ = nullptr;

	std::map<std::string, Skeleton::Joint*> joints_;

};

