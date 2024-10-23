#pragma once
#include <memory>
#include "Engine/Engine.h"
#include "Engine/2d/Sprite.h"

class PlayerControlUI {
public:

	PlayerControlUI();
	~PlayerControlUI();

	void Init();
	void Update(const Vector2& pos, bool isGliding);
	void Draw() const;

#ifdef _DEBUG
	void Debug_Gui();
#endif // _DEBUG

private:

	std::unique_ptr<Sprite> sprite_ui_;
	Vector2 offset_;
	Vector2 scale_;
};

