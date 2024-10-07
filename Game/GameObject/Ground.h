#pragma once
#include "Engine/GameObject/BaseGameObject.h"

/// <summary>
/// 水中の中の地面
/// </summary>
class Ground :
	public BaseGameObject {
public:

	Ground();
	~Ground();

	void Init() override;
	void Update() override;
	void Draw() const override;

	void Move();

#ifdef _DEBUG
	void Debug_Gui();
#endif // _DEBUG

	void SetPlayerVelocityX(const float& velocityX) { playerVelocityX_ = velocityX; }

private:

	float playerVelocityX_;

};

