#pragma once
#include <vector>
#include <memory>
#include "Engine/Engine.h"
#include "Engine/2d/Sprite.h"

/// <summary>
/// Mission表示用のクラス
/// </summary>
class MissionUI {
public:

	MissionUI();
	~MissionUI();

	void Init();
	void Update();
	void Draw() const;

	/// <summary>
	/// Missionを画面からfadeInさせる
	/// </summary>
	void MissionAppearance();

	/// <summary>
	/// Missionを画面からfadeOutさせる
	/// </summary>
	void MissionDisappear();

#ifdef _DEBUG
	void Debug_Gui();
#endif // _DEBUG

private:

	// missionの内容をまとめたリスト
	std::vector<std::unique_ptr<Sprite>> missionSpriteList_;

	// prameter
	Vector2 offsetTopPos_;
	Vector2 topStartPos_;
	Vector2 topEndPos_;

	float interval_;

	// time系
	float moveTimeCount_;
	float moveTime_;

	// flag
	bool isAppearance_;
	bool isMove_;
};

