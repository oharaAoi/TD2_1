#pragma once
#include <vector>
#include <memory>
#include "Engine/Engine.h"
#include "Engine/2d/Sprite.h"
#include "Engine/Math/Easing.h"

enum class SpeedMission : uint32_t {
	Mission_100 = 100,
	Mission_150 = 150,
	Mission_Finish,
};

enum class HeightMission : uint32_t {
	Mission_300 = 300,
	Mission_500 = 500,
	Mission_Finish = 99999,
};

/// <summary>
/// Mission表示用のクラス
/// </summary>
class MissionUI {
public:

	MissionUI();
	~MissionUI();

	void Init();
	void Update(float playerSpeed, float playerPosY);
	void Draw() const;

	/// <summary>
	/// Missionを画面からfadeInさせる
	/// </summary>
	void SpeedMissionAppearance();
	void HeightMissionAppearance();

	/// <summary>
	/// Missionを画面からfadeOutさせる
	/// </summary>
	void SpeedMissionDisappear();
	void HeightMissionDisappear();

	/// <summary>
	/// 速さのCheckを入れる
	/// </summary>
	void SpeedCheck();

	/// <summary>
	/// 高さのCheckを入れる
	/// </summary>
	void HeightCheck();

	/// <summary>
	/// Missionを入れ替える
	/// </summary>
	void SpeedMissionChange();

	/// <summary>
	/// Missionを入れ替える
	/// </summary>
	void HeightMissionChange();

#ifdef _DEBUG
	void Debug_Gui();
#endif // _DEBUG

private:

	std::unique_ptr<Sprite> speedMission_;
	std::unique_ptr<Sprite> heightMission_;

	std::unique_ptr<Sprite> speedCheck_;
	std::unique_ptr<Sprite> heightCheck_;

	SpeedMission nowSpeedMission_;
	HeightMission nowHeightMission_;

	// prameter
	Vector2 offsetTopPos_;
	Vector2 topStartPos_;
	Vector2 topEndPos_;

	float interval_;

	// time系
	float moveTimeCount_;
	float moveTime_;

	// 出現の処理に関する
	float appearanceSpeedCount_;
	float appearanceHeightCount_;
	float appearanceTime_;

	// 消える処理に関する
	float disappearSpeedCount_;
	float disappearHeightCount_;
	float disappearTime_;

	// 完了に関する
	float speedCheckCount_;
	float heightCheckCount_;
	float checkAppearanceTime_;

	float speedCheckWaitTime_;
	float heightCheckWaitTime_;
	float waitTime_;

	// flag
	bool isSpeesAppearance_;
	bool isHeightAppearance_;

	bool isSpeesAppearanceFinish_;
	bool isHeightAppearanceFinish_;

	bool isSpeedCheck_;
	bool isHeightCheck_;
};

