#pragma once
#include <vector>
#include <memory>
#include "Engine/GameObject/BaseGameObject.h"

/// <summary>
/// Playerの羽根
/// </summary>
class PlayerWings {
public:

	PlayerWings();
	~PlayerWings();

	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="parentPos">親となる位置</param>
	/// <param name="rotate">回転</param>
	/// <param name="isGliding">滑空しているか</param>
	/// <param name="time">: 羽根を開く時間</param>
	void Update(const Vector3& parentPos, float time);
	void Draw() const;

#ifdef _DEBUG
	void Debug_Gui();
#endif

	void SetWindOpenTime(float time) { windOpenTime_ = time; }

	void NotFlying();

private:

	std::unique_ptr<BaseGameObject> wings_[2];

	float offsetZ_ = 2.0f;

	Quaternion moveQuaternion_;

	float time_;
	
	float windOpenTime_;
	float windOpenTimeLimit_;
	float openAngle_ = 60.0f;
	Vector3 rotateAxis_;
};

