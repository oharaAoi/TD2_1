#pragma once
#include "Game/GameObject/BasePlacementObject.h"
#include "Game/GameObject/Bird.h"

class BirdToge : 
	public BasePlacementObject {
public:

	BirdToge();
	~BirdToge();

	void Init() override;
	void Update() override;
	void Draw() const override;

	void OnCollision([[maybe_unused]] Collider* other)override;

	void ApplyLoadData(
		const Vector3& scale, const Quaternion& rotate,
		const Vector3& pos, const SubAttributeType& subType
	)override;

	void SetMoveDirection(const Vector3& direction) { moveDirection_ = direction; }
	void SetMoveRadius(float radius) { moveRadius_ = radius; }
	void SetIsMove(bool flag) { isMove_ = flag; }

	void IndividualFromCommon(const SubAttributeType& subType = SubAttributeType::NONE) override;

	void SetBird(Bird* _bird) { bird_ = _bird; }

	void ScaleChange(bool playerCloseWind);

private:

	void ScalingTimer();

private:

	Vector3 firstPos_;// 出現時の座標
	bool isMove_;// その場から動くかどうか
	Vector3 moveDirection_;// 動く場合の動く方向
	float moveRadius_;// 動く半径
	float time_;
	bool isHitPlayer_ = false;
	float wipeOutTime_ = 2.0f;

	Bird* bird_;
	// scalingに関する変数
	Vector3 birdScale_;
	float scalingTimer_;
	float scalingTime_;

	int scalingSign_;	// タイマーの符号

};

